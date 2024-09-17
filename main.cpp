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
const int BufferSize = 64;
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

// Optimized function to perform a multi-variable read and write
void multiVarReadWrite(const char* dataToWrite, int length) {
    TS7DataItem items[2];

    // Set up the item for reading
    items[0].Area = S7AreaDB;
    items[0].WordLen = S7WLByte;
    items[0].Result = 0;
    items[0].DBNumber = DB_NUMBER;
    items[0].Start = readStart;
    items[0].Amount = length;
    items[0].pdata = MyDB35;

    // Set up the item for writing
    items[1].Area = S7AreaDB;
    items[1].WordLen = S7WLByte;
    items[1].Result = 0;
    items[1].DBNumber = DB_NUMBER;
    items[1].Start = writeStart;
    items[1].Amount = length;
    items[1].pdata = (void*)dataToWrite;

    // Perform the multi-variable write and read in a single communication cycle
    int result = Client->WriteMultiVars(&items[1], 1);
    if (result != 0) {
        cout << "WriteMultiVars failed. Error: " << CliErrorText(result) << endl;
        return;
    }

    result = Client->ReadMultiVars(&items[0], 1);
    if (result != 0) {
        cout << "ReadMultiVars failed. Error: " << CliErrorText(result) << endl;
        return;
    }
}

int main() {
    // Connect to the PLC
    plc_Connect();

    // Main loop for multi-variable reading and writing
    const char* data = "OptimizedDataFlow";
    
    while (true) {
        // Start timing
        auto start = std::chrono::high_resolution_clock::now();

        // Perform the optimized multi-variable read and write
        multiVarReadWrite(data, stringLength);

        // End timing and calculate the cycle time
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> cycleTime = end - start;

        // Calculate and print the update frequency
        if (cycleTime.count() > 0) {
            double frequency = 1000.0 / cycleTime.count(); // Frequency in Hz
            cout << "Cycle time: " << cycleTime.count() << " ms, Frequency: " << frequency << " Hz" << endl;
        }

        // Optional: Add a short sleep to reduce CPU load (adjust or remove as needed)
        Sleep(1);
    }

    // Disconnect from the PLC
    plc_Disconnect();

    return 0;
}
