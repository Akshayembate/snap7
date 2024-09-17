#include <iostream>
#include <Windows.h>
#include "snap7.h"
#include <chrono>
#include <iomanip>

using namespace std;

TS7Client* Client;
int Rack = 0;
int Slot = 1;
int DB_NUMBER = 32;
const char* Address = "192.168.1.5";

// Define buffer size optimized for faster operation
const int BufferSize = 64; // Reduce buffer size for faster transfers
byte MyDB35[BufferSize]; // Buffer to hold data

int readStart = 256;
int writeStart = 0;
int stringLength = 32; // The length of the string to be written/read, optimized for small size

// Function to connect to the PLC
void plc_Connect() {
    Client = new TS7Client;
    int result = Client->ConnectTo(Address, Rack, Slot);

    if (result == 0) {
        cout << "Connected to PLC." << endl;
    } else {
        cout << "Failed to connect to PLC. Error: " << CliErrorText(result) << endl;
        delete Client;
        exit(1); // Exit if connection fails
    }
}

// Function to disconnect from the PLC
void plc_Disconnect() {
    Client->Disconnect();
    delete Client;
    cout << "Disconnected from PLC." << endl;
}

// Function to write data to the PLC using memory operations
void writeDataToPLC(const char* dataToWrite, int length) {
    memcpy(MyDB35, dataToWrite, length);  // Move data directly into buffer
    int result = Client->DBWrite(DB_NUMBER, writeStart, length, MyDB35);  // Write only necessary bytes

    if (result != 0) {
        cout << "Failed to write data to PLC. Error: " << CliErrorText(result) << endl;
    }
}

// Function to read data from the PLC using memory operations
void readDataFromPLC(int length) {
    int result = Client->DBRead(DB_NUMBER, readStart, length, MyDB35);  // Read only the necessary bytes

    if (result == 0) {
        cout << "Successfully read data from PLC. Data: ";
        for (int i = 0; i < length; i++) {
            cout << hex << setfill('0') << setw(2) << (int)MyDB35[i] << " ";
        }
        cout << endl;
    } else {
        cout << "Failed to read data from PLC. Error: " << CliErrorText(result) << endl;
    }
}

int main() {
    // Connect to the PLC
    plc_Connect();

    // Main loop for reading and writing without threads
    const char* data = "OptimizedDataFlow";
    
    while (true) {
        // Start timing for one read or write cycle
        auto start = std::chrono::high_resolution_clock::now();

        // Write data to the PLC
        writeDataToPLC(data, stringLength);

        // Read data from the PLC
        readDataFromPLC(stringLength);

        // End timing and calculate the cycle time
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> cycleTime = end - start; // Measure in milliseconds

        // Calculate and print the update frequency
        if (cycleTime.count() > 0) {
            double frequency = 1000.0 / cycleTime.count(); // Frequency in Hz (1 / cycle time in seconds)
            cout << "Cycle time: " << cycleTime.count() << " ms, Frequency: " << frequency << " Hz" << endl;
        }

        // Optional: reduce delay (or remove this completely)
        // Avoid this_thread::sleep_for(chrono::milliseconds(1)); to maximize the loop speed
    }

    // Disconnect from the PLC
    plc_Disconnect();

    return 0;
}
