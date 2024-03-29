#ifndef MY_STRUCTS_H
#define MY_STRUCTS_H

#include "macros.h"

typedef struct SymbolToken{
    char first;
    char second;
} Token;

typedef struct matrix{
    int width;
    int height;
    Token* buffer;
} TokenMatrix;

typedef struct sequence{
    int size;
    int reward;
    Token* buffer;
} Sequence;

typedef struct coordinate{
    int x;
    int y;
} Coordinate;

extern Token NullToken;
extern Coordinate NullCoordinate;

int isTokenEqual(Token token1, Token token2);

int isCoordinateEqual(Coordinate coord1, Coordinate coord2);

void printToken(Token token);

void printCoord(Coordinate Coord);

void CreateTokenMatrix(TokenMatrix* M);

void CreateSequence(Sequence* S, int size);

void printTokenMatrix(TokenMatrix M);

void printSequence(Sequence S);

int HasSequence(Token *Tokens, Sequence Seq, int TokenCount);

int isSeqEq(Sequence Seq1, Sequence Seq2);

int BufferPoint(Token *Tokens, Sequence *Seqs, int SeqCount, int TokenCount);

void CoordsToTokens(TokenMatrix M, Coordinate *Coords, Token *Tokens);

int HasCoord(Coordinate *Coords, Coordinate Coord);

void copyCoord(Coordinate *Source, Coordinate *Result);

#endif
