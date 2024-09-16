#include <iostream>
#include "WriteSnap.h"
#include "s7.h"

bool writeValuesToPLC_2(TS7Client* Client, byte* MyDB35, int* writeIntValues, float* writeRealValues, int IntArraySize, int RealArraySize, int writeStart, int writeSize, int DB_NUMBER)
{
    // Prepare buffer with array of ints and reals
    for (int i = 0; i < IntArraySize; ++i)
    {
        S7_SetIntAt(MyDB35, i * 2, writeIntValues[i]);
    }

    for (int i = 0; i < RealArraySize; ++i)
    {
        S7_SetRealAt(MyDB35, (IntArraySize * 2) + (i * 4), writeRealValues[i]);
    }

    // Write array of ints and reals to DB
    int result = Client->DBWrite(DB_NUMBER, writeStart, writeSize, MyDB35);
    if (result == 0)
    {
        std::cout << "Successfully wrote values to PLC." << std::endl;
        return true;
    }
    else
    {
        std::cout << "Failed to write to PLC. Error: " << CliErrorText(result) << std::endl;
    }
}
