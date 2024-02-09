#ifndef MY_IO_H
#define MY_IO_H

#include "structs.h"
#include "macros.h"

int textInput(char *txtPath, int *bufferSize, Matrix *M, int *seqCount, Sequence **Seqs);

int randomInput(int *bufferSize, Matrix *M, int *seqCount, Sequence **Seqs);

int saveResult(int maxPoint, Sequence Result, Coordinate *Coordinates, double time);

#endif