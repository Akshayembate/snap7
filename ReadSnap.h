#ifndef READSNAP_H
#define READSNAP_H

#include "snap7.h"

bool readValuesFromPLC_2(TS7Client* Client, byte* MyDB35, int IntArraySize, int RealArraySize, int readStart, int readSize, int DB_NUMBER);

#endif // READSNAP_H
