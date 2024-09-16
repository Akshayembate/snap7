#include <iostream>
#include <Windows.h>
#include "snap7.h"
#include "s7.h"
//#include "KeyboardInput.h"
#include "ReadSnap.h"
#include "WriteSnap.h"
#include <chrono>
#include <future>

using namespace std;
TS7Client* Client;

int Rack = 0;
int Slot = 1;

int DB_NUMBER = 2; // Database number
const char* Address = "192.168.1.5";

const int IntArraySize = 20;
const int RealArraySize = 20;
const int BufferSize = (IntArraySize * 2) + (RealArraySize * 4); // Each int is 2 bytes, each real is 4 bytes
byte MyDB35[BufferSize]; // Buffer to hold data

int readStart = 120;
int readSize = BufferSize;

int writeStart = 0;
int writeSize = BufferSize;

int writeIntValues[IntArraySize];
float writeRealValues[RealArraySize]; // No initial values here to avoid overwriting

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

// 

int main()
{
    // Connect to the PLC
    plc_Connect();

    while (true)
    {
        // start timing for one read or write cycle
        auto start = std::chrono::high_resolution_clock::now();

        // Handle keyboard input
        //handleKeyboardInput();
        
        // Read values from DB
        bool readDone = readValuesFromPLC_2(Client, MyDB35, IntArraySize, RealArraySize, readStart, readSize, DB_NUMBER);
        
        // Write values to DB
        if (readDone)
        {
        bool writedone = writeValuesToPLC_2(Client, MyDB35, writeIntValues, writeRealValues, IntArraySize, RealArraySize, writeStart, writeSize, DB_NUMBER);
        if (writedone)
        {
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> cycletime = end - start;

            // calculate and print the update freequency
            double freequency = 1.0 / cycletime.count();
            cout << "cycle time : " << cycletime.count() << "freequency : " << freequency << "\n";
        }
        }
        //Sleep(100); // Sleep for 100 ms
    }

    // Disconnect from the PLC
    plc_Disconnect();

    return 0;
}
