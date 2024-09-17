#include <iostream>
#include <Windows.h>
#include "snap7.h"
#include "s7.h"
#include <chrono>
#include <string>
#include <iomanip> // For printing byte data

using namespace std;

TS7Client* Client;
int Rack = 0;
int Slot = 1;
int DB_NUMBER = 32; // Ensure this matches your DB number
const char* Address = "192.168.1.5";

// Define buffer size
const int BufferSize = 128;
byte MyDB35[BufferSize]; // Buffer to hold data

int readStart = 256; // Ensure this offset is valid within the PLC's DB
int writeStart = 0;
int stringLength = 100;

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

// Function to write a string to the PLC
void writeStringToPLC(const char* strToWrite) {
    // Ensure that the string fits into the specified buffer size
    S7_SetStringAt(MyDB35, writeStart, stringLength, strToWrite);
    int result = Client->DBWrite(DB_NUMBER, writeStart, BufferSize, MyDB35);

    if (result == 0) {
        cout << "Successfully wrote string to PLC: " << strToWrite << endl;
    } else {
        cout << "Failed to write string to PLC. Error: " << CliErrorText(result) << endl;
    }
}

// Function to read a string from the PLC
void readStringFromPLC() {
    int result = Client->DBRead(DB_NUMBER, readStart, BufferSize, MyDB35);
    //std::cout << "Result: " << result << "\n";

    if (result == 0) {
        cout << "Raw byte data from PLC: ";
        for (int i = 0; i < BufferSize; i++) {
            cout << hex << setfill('0') << setw(2) << (int)MyDB35[i] << " ";
        }
        cout << endl;

        // Try to extract the string from the buffer
        string readString = S7_GetStringAt(MyDB35, readStart);
        cout << "Successfully read string from PLC: " << readString << endl;
    } else {
        cout << "Failed to read string from PLC. Error: " << CliErrorText(result) << endl;
    }
}

int main() {
    // Connect to the PLC
    plc_Connect();

    while (true) {
        // Start timing for one read or write cycle
        auto start = std::chrono::high_resolution_clock::now();

        // Write a string to the PLC
        writeStringToPLC("SecondStringToWriteHere");

        // Read a string from the PLC
        readStringFromPLC();

        // End timing and calculate the cycle time
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> cycleTime = end - start; // Measure in milliseconds

        // Calculate and print the update frequency
        if (cycleTime.count() > 0) {
            double frequency = 1000.0 / cycleTime.count(); // Frequency in Hz (1 / cycle time in seconds)
            cout << "Cycle time: " << cycleTime.count() << " ms, Frequency: " << frequency << " Hz" << endl;
        }
    }

    // Disconnect from the PLC
    plc_Disconnect();

    return 0;
}
