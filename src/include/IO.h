#ifndef MY_IO_H
#define MY_IO_H

#include "structs.h"
#include "macros.h"

void empty_stdin(void);

int isValidTokenChar(char c);

int textInput(char *txtPath, int *bufferSize, TokenMatrix *M, int *seqCount, Sequence **Seqs);

int randomInput(int *bufferSize, TokenMatrix *M, int *seqCount, Sequence **Seqs);

int saveResult(int maxPoint, Sequence Result, Coordinate *Coordinates, double time);

#endif