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

const int BufferSize = 64;
byte MyDB35[BufferSize]; // Buffer to hold data

int readStart = 256;
int writeStart = 0;
int stringLength = 32;

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

void plc_Disconnect() {
    Client->Disconnect();
    delete Client;
    cout << "Disconnected from PLC." << endl;
}

DWORD WINAPI asyncRead(LPVOID length) {
    TS7DataItem readItem;
    readItem.Area = S7AreaDB;
    readItem.WordLen = S7WLByte;
    readItem.Result = 0;
    readItem.DBNumber = DB_NUMBER;
    readItem.Start = readStart;
    readItem.Amount = *(int*)length;
    readItem.pdata = MyDB35;

    int result = Client->ReadMultiVars(&readItem, 1);
    if (result != 0) {
        cout << "ReadMultiVars failed. Error: " << CliErrorText(result) << endl;
    }
    return 0;
}

DWORD WINAPI asyncWrite(LPVOID param) {
    const char* dataToWrite = (const char*)param;
    TS7DataItem writeItem;
    writeItem.Area = S7AreaDB;
    writeItem.WordLen = S7WLByte;
    writeItem.Result = 0;
    writeItem.DBNumber = DB_NUMBER;
    writeItem.Start = writeStart;
    writeItem.Amount = stringLength;
    writeItem.pdata = (void*)dataToWrite;

    int result = Client->WriteMultiVars(&writeItem, 1);
    if (result != 0) {
        cout << "WriteMultiVars failed. Error: " << CliErrorText(result) << endl;
    }
    return 0;
}

int main() {
    plc_Connect();
    const char* data = "OptimizedDataFlow";

    while (true) {
        auto start = std::chrono::high_resolution_clock::now();

        // Create threads using Windows-specific CreateThread
        HANDLE hReadThread = CreateThread(NULL, 0, asyncRead, &stringLength, 0, NULL);
        HANDLE hWriteThread = CreateThread(NULL, 0, asyncWrite, (LPVOID)data, 0, NULL);

        // Wait for both threads to complete
        WaitForSingleObject(hReadThread, INFINITE);
        WaitForSingleObject(hWriteThread, INFINITE);

        CloseHandle(hReadThread);
        CloseHandle(hWriteThread);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> cycleTime = end - start;

        if (cycleTime.count() > 0) {
            double frequency = 1000.0 / cycleTime.count();
            cout << "Cycle time: " << cycleTime.count() << " ms, Frequency: " << frequency << " Hz" << endl;
        }

        Sleep(1);
    }

    plc_Disconnect();
    return 0;
}
