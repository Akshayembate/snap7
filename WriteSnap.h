#ifndef WRITESNAP_H
#define WRITESNAP_H

#include "snap7.h"

void writeValuesToPLC(TS7Client* Client, byte* MyDB35, int* writeIntValues, float* writeRealValues, int IntArraySize, int RealArraySize, int writeStart, int writeSize, int DB_NUMBER);

#endif // WRITESNAP_H
