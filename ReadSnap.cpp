#include <iostream>
#include "ReadSnap.h"
#include "s7.h"

bool readValuesFromPLC_2(TS7Client* Client, byte* MyDB35, int IntArraySize, int RealArraySize, int readStart, int readSize, int DB_NUMBER)
{
    // Read array of ints and reals from DB
    int result = Client->ReadArea(S7AreaDB, DB_NUMBER, readStart, readSize, S7WLByte, MyDB35);
    if (result == 0)
        return true;
    //{
       // std::cout << "Integer values:" << std::endl;
    //    for (int i = 0; i < IntArraySize; ++i)
    //    {
    //        int value = S7_GetIntAt(MyDB35, i * 2);
           // std::cout << "Value[" << i << "] = " << value << std::endl;
    //    }

        //std::cout << "Real values:" << std::endl;
    //    for (int i = 0; i < RealArraySize; ++i)
    //    {
    //        float value = S7_GetRealAt(MyDB35, (IntArraySize * 2) + (i * 4));
    //       // std::cout << "RealValue[" << i << "] = " << value << std::endl;
    //    }
    //    return true;
    //}
    //else
    //{
        //std::cout << "Failed to read from PLC. Error: " << CliErrorText(result) << std::endl;
    //}
}
