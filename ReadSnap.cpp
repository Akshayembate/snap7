#include <iostream>
#include "ReadSnap.h"
#include "s7.h"

void readValuesFromPLC(TS7Client* Client, byte* MyDB35, int IntArraySize, int RealArraySize, int readStart, int readSize, int DB_NUMBER)
{
    // Read array of ints and reals from DB
    int result = Client->DBRead(DB_NUMBER, readStart, readSize, MyDB35);
    if (result == 0)
    {
        std::cout << "Integer values:" << std::endl;
        for (int i = 0; i < IntArraySize; ++i)
        {
            int value = S7_GetIntAt(MyDB35, i * 2);
            std::cout << "Value[" << i << "] = " << value << std::endl;
        }

        std::cout << "Real values:" << std::endl;
        for (int i = 0; i < RealArraySize; ++i)
        {
            float value = S7_GetRealAt(MyDB35, (IntArraySize * 2) + (i * 4));
            std::cout << "RealValue[" << i << "] = " << value << std::endl;
        }
    }
    else
    {
        std::cout << "Failed to read from PLC. Error: " << CliErrorText(result) << std::endl;
    }
}
