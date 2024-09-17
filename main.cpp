#include <iostream>
#include <Windows.h>
#include "snap7.h"
#include <chrono>

using namespace std;

TS7Client* Client;
int Rack = 0;
int Slot = 1;
int DB_NUMBER = 32;
const char* Address = "192.168.1.5";

// Define buffer size
const int BufferSize = 128;
byte MyDB35[BufferSize]; // Buffer to hold data

int readStart = 256;
int writeStart = 0;
int stringLength = 32;

// Function to connect to the PLC
void plc_Connect() {
    Client = new TS7Client;
    int result = Client->ConnectTo(Address, Rack, Slot);

    if (result == 0) {
        cout << "Connected to PLC." << endl;
    } else {
        cout << "Failed to connect to PLC. Error: " << CliErrorText(result) << endl;
        delete Client;
        exit(1);
    }
}

// Function to disconnect from the PLC
void plc_Disconnect() {
    Client->Disconnect();
    delete Client;
    cout << "Disconnected from PLC." << endl;
}

// Optimized function to read and write in a single batch
void readWriteData(const char* dataToWrite, int length) {
    // Write data
    memcpy(MyDB35, dataToWrite, length);  // Move data into buffer
    int writeResult = Client->DBWrite(DB_NUMBER, writeStart, length, MyDB35);

    // Read data
    int readResult = Client->DBRead(DB_NUMBER, readStart, length, MyDB35);

    if (writeResult == 0 && readResult == 0) {
        // Optional: Process the read data if necessary
        // cout << "Successfully wrote and read data from PLC." << endl;
    } else {
        if (writeResult != 0) {
            cout << "Failed to write data to PLC. Error: " << CliErrorText(writeResult) << endl;
        }
        if (readResult != 0) {
            cout << "Failed to read data from PLC. Error: " << CliErrorText(readResult) << endl;
        }
    }
}

int main() {
    // Connect to the PLC
    plc_Connect();

    // Main loop for reading and writing
    const char* data = "OptimizedDataFlow";
    
    while (true) {
        // Start timing
        auto start = std::chrono::high_resolution_clock::now();

        // Perform combined read and write operation
        readWriteData(data, stringLength);

        // End timing and calculate the cycle time
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> cycleTime = end - start;

        // Calculate and print the update frequency
        if (cycleTime.count() > 0) {
            double frequency = 1000.0 / cycleTime.count(); // Frequency in Hz
            cout << "Cycle time: " << cycleTime.count() << " ms, Frequency: " << frequency << " Hz" << endl;
        }

        // Optional: Add a short sleep or remove entirely to allow faster operation
        // this_thread::sleep_for(chrono::milliseconds(1));  // Adjust based on performance
    }

    // Disconnect from the PLC
    plc_Disconnect();

    return 0;
}
