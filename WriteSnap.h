#ifndef WRITESNAP_H
#define WRITESNAP_H

#include "snap7.h"

bool writeValuesToPLC_2(TS7Client* Client, byte* MyDB35, int* writeIntValues, float* writeRealValues, int IntArraySize, int RealArraySize, int writeStart, int writeSize, int DB_NUMBER);

#endif // WRITESNAP_H
