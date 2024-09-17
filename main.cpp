#include <iostream>
#include <Windows.h>
#include "snap7.h"
#include <chrono>
#include <thread> // For threading

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

// Asynchronous read function
void asyncRead(int length) {
    TS7DataItem readItem;
    readItem.Area = S7AreaDB;
    readItem.WordLen = S7WLByte;
    readItem.Result = 0;
    readItem.DBNumber = DB_NUMBER;
    readItem.Start = readStart;
    readItem.Amount = length;
    readItem.pdata = MyDB35;

    int result = Client->ReadMultiVars(&readItem, 1);
    if (result != 0) {
        cout << "ReadMultiVars failed. Error: " << CliErrorText(result) << endl;
    }
}

// Asynchronous write function
void asyncWrite(const char* dataToWrite, int length) {
    TS7DataItem writeItem;
    writeItem.Area = S7AreaDB;
    writeItem.WordLen = S7WLByte;
    writeItem.Result = 0;
    writeItem.DBNumber = DB_NUMBER;
    writeItem.Start = writeStart;
    writeItem.Amount = length;
    writeItem.pdata = (void*)dataToWrite;

    int result = Client->WriteMultiVars(&writeItem, 1);
    if (result != 0) {
        cout << "WriteMultiVars failed. Error: " << CliErrorText(result) << endl;
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

        // Perform asynchronous read and write in parallel
        std::thread readThread(asyncRead, stringLength);
        std::thread writeThread(asyncWrite, data, stringLength);

        // Wait for both threads to complete
        readThread.join();
        writeThread.join();

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
