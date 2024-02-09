#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

Token NullToken = {' ', ' '};
Coordinate NullCoordinate = {-1, -1};

int isTokenEqual(Token token1, Token token2){
    return (token1.first == token2.first && token1.second == token2.second);
}

int isCoordinateEqual(Coordinate coord1, Coordinate coord2){
    return (coord1.x == coord2.x AND coord1.y == coord2.y);
}

void printToken(Token token){
    printf("%c%c", token.first, token.second);
}

void printCoord(Coordinate Coord){
    printf("%d,%d", Coord.x, Coord.y);
}

void CreateMatrix(Matrix* M){
    M->buffer = (Token*) malloc (sizeof(Token) * M->width * M->height);
}

void CreateSequence(Sequence* S, int size){
    S->buffer = (Token*) malloc (sizeof(Token) * size);
    S->size = 0;
    S->reward = 0;
}

void printMatrix(Matrix M){
    int i, j;
    if (M.height > 0 AND M.width > 0){
        traversal(i, 1, M.height){
            traversal(j, 1, M.width){
                printToken(ACCESS(M, j, i));
                printf(" ");
            }
            printf("\n");
        }
    }
}

void printSequence(Sequence S){
    int i;
    if (S.size == 0) {return;}
    if (S.size == 1) {printToken(S.buffer[0]);}
    traversal(i, 0, S.size - 1){
        printToken(S.buffer[i]);
        printf(" ");
    }
}

int HasSequence(Token* Tokens, Sequence Seq, int TokenCount){
    int i = 0;
    int j;
    while(i + Seq.size - 1 < TokenCount){
        if (isTokenEqual(Tokens[i], Seq.buffer[0])){
            int has = 1;
            traversal(j, 1, Seq.size - 1){
                if (NOT isTokenEqual(Tokens[i + j], Seq.buffer[j])){
                    has = 0;
                }
            }
            if (has) {return 1;}
        }
        i++;
    }
    return 0;
}

int BufferPoint(Token* Tokens, Sequence* Seqs, int SeqCount, int TokenCount){
    int point = 0;
    int i;
    traversal(i, 0, SeqCount - 1){
        if (HasSequence(Tokens, Seqs[i], TokenCount)){
            point += Seqs[i].reward;
        }
    }
    return point;
}

void CoordsToTokens(Matrix M, Coordinate* Coords, Token* Tokens){
    int i = 0;
    while(NOT isCoordinateEqual(Coords[i], NullCoordinate)){
        Tokens[i] = ACCESS(M, Coords[i].x, Coords[i].y);
        i++;
    }
}

int HasCoord(Coordinate* Coords, Coordinate Coord){
    int i = 0;
    while(NOT isCoordinateEqual(Coords[i], NullCoordinate)){
        if (isCoordinateEqual(Coords[i], Coord)){
            return 1;
        }
        i++;
    }
    return 0;
}

void copyCoord(Coordinate* Source, Coordinate* Result){
    int i = 0;
    while(NOT isCoordinateEqual(Source[i], NullCoordinate)){
        Result[i] = Source[i];
        i++;
    }
    Result[i] = Source[i];
}