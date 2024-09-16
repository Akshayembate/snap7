#include <iostream>
#include <Windows.h>
#include "snap7.h"
#include "s7.h"
//#include "KeyboardInput.h"
#include "ReadSnap.h"
#include "WriteSnap.h"
#include <chrono>
//#include <future>
#include <string>

using namespace std;
TS7Client* Client;

int Rack = 0;
int Slot = 1;

int DB_NUMBER = 2; // Database number
const char* Address = "192.168.1.5";

const int IntArraySize = 20;
const int RealArraySize = 20;
const int BufferSize = (IntArraySize * 2) + (RealArraySize * 4) + 512; // Each int is 2 bytes, each real is 4 bytes
byte MyDB35[BufferSize]; // Buffer to hold data

int readStart = 120;
int readSize = BufferSize;

int writeStart = 0;
int writeSize = BufferSize;

int writeIntValues[IntArraySize];
float writeRealValues[RealArraySize]; // No initial values here to avoid overwriting


// code for checking the operation completed status
bool isOperationCompleted(TS7Client* client)
{
    int result;
    return client->CheckAsCompletion(&result) == 0 && result == 0;
}

void plc_Connect()
{
    // Connect to S7 PLC
    Client = new TS7Client;
    int result = Client->ConnectTo(Address, Rack, Slot);

    if (result == 0)
    {
        cout << "Connected to PLC." << endl;
    }
    else
    {
        cout << "Failed to connect to PLC. Error: " << CliErrorText(result) << endl;
        delete Client;
        exit(1); // Exit if connection fails
    }
}

void plc_Disconnect()
{
    // Disconnect from S7 PLC
    Client->Disconnect();
    delete Client;
    cout << "Disconnected from PLC." << endl;
}

// async read 
//std::future<int> asyncRead (TS7Client* Client)
//{
//    return std::async(std::launch::async, [&]()
//    {
        // performing the read operation
//        return  readValuesFromPLC_2(Client, MyDB35, IntArraySize, RealArraySize, readStart, readSize, DB_NUMBER);
//    })
//}

int main()
{
    // Connect to the PLC
    plc_Connect();

 bool readInProgress = false;
    bool writeInProgress = false;

    while (true)
    {
        
        // Start timing for one read or write cycle
        auto start = std::chrono::high_resolution_clock::now();

        // writing the string to plc
        const char* writeToPlc = "123444:12344555:5:7777:123444:15375824854:409473047047:4u03840404040:2194719471047104710471:1561036501561051650goeg:";

        std::string readStringFromPlc = S7_GetStringAt(MyDB35, BufferSize);; // Allocate 33 bytes to hold 32 characters plus a null terminator
        S7_SetStringAt(MyDB35, 496, BufferSize+32, writeToPlc);


        // Read values from DB
        //if (!readInProgress )  // Start read only if no other operation is in progress
        //{
            //cout << 'started read\n';
        //    bool readDone = readValuesFromPLC_2(Client, MyDB35, IntArraySize, RealArraySize, readStart, readSize, DB_NUMBER);
        //    if (readDone)
        //    {
               // cout << " read started\n";
        //        readInProgress = true;  // Set readInProgress to true after starting the read
        //    }
        //    else
        //    {
                //cout << " failed to do the read\n";
        //    }
        //}

        // Check if read operation is complete
        //if (readInProgress )
        //{
            // Mark the read operation as complete

            // Start write operation after read completes
        //    if (!writeInProgress)
        //    {
                //cout << "started write\n";
        //        bool writeDone = writeValuesToPLC_2(Client, MyDB35, writeIntValues, writeRealValues, IntArraySize, RealArraySize, writeStart, writeSize, DB_NUMBER);
        //        if (writeDone)
        //        {
                   // cout << " write operatin initialted\n";
        //            writeInProgress = true; // Set writeInProgress to true after starting the write
        //        }
                //else
                   // cout << " could not able to write\n";
        //    }
        //     readInProgress = false;
        //}

        // Check if write operation is complete
        //if (writeInProgress )
        //{
             // Mark the write operation as complete
            //cout << " write operation is done\n";
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> cycleTime = end - start;

            // Calculate and print the update frequency
            double frequency = 1.0 / cycleTime.count();
            cout << "Cycle time: " << cycleTime.count() << " seconds, Frequency: " << frequency << " Hz" << endl;
            writeInProgress = false;
        
        //Sleep(100); // Sleep for 100 ms
        // Sleep briefly to avoid hogging the CPU
       // std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    // Disconnect from the PLC
    plc_Disconnect();

    return 0;
}
