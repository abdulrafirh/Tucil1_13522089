#ifndef MY_MACROS_H
#define MY_MACROS_H

#define ACCESS(M, col, row) M.buffer[(row - 1)*(M.width) + (col - 1)]
#define INDEX(col, row) (row - 1)*(M.width) + (col - 1)
#define traversal(i, start, end) for(i = start; ((end >= start) && (i <= end)) || ((end < start) && (i >= end)); i += (end >= start) ? 1 : -1)
#define OR ||
#define AND &&
#define NOT !

#endif