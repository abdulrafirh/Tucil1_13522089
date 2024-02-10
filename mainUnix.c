#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "macros.h"
#include "structs.h"
#include "IO.h"

void findOptimumSequence(Matrix M, Token* Buffer, Sequence* Seqs, Coordinate* visited, int SeqCount, int bufferSize, int horizontal, int currentLength, Coordinate* CurrentOptimumCoords, int* CurrentMax, int* maxBufferSize){
    if (SeqCount == 0) {return;}
    int currentPoint = BufferPoint(Buffer, Seqs, SeqCount, currentLength);
    
    if (currentPoint > *CurrentMax || (currentPoint >= *CurrentMax && currentLength < *maxBufferSize)){
        copyCoord(visited, CurrentOptimumCoords);
        // BufferPoint(Buffer, Seqs, SeqCount, currentLength);
        *CurrentMax = currentPoint;
        *maxBufferSize = currentLength;
    }

    if (currentLength == bufferSize){return;}

    int i;
    Coordinate currentCoord;
    if (currentLength > 0){
        currentCoord = visited[currentLength - 1];
    }
    else{
        currentCoord.x = 0;
        currentCoord.y = 1;
    }

    if (horizontal){
        traversal(i, 1, M.width){
            Coordinate nextCoord = {i, currentCoord.y};
            if (NOT HasCoord(visited, nextCoord)){
                visited[currentLength] = nextCoord;
                visited[currentLength + 1] = NullCoordinate;
                Buffer[currentLength] = ACCESS(M, nextCoord.x, nextCoord.y);
                Buffer[currentLength + 1] = NullToken;
                findOptimumSequence(M, Buffer, Seqs, visited, SeqCount, bufferSize, NOT horizontal, currentLength + 1, CurrentOptimumCoords, CurrentMax, maxBufferSize);
            }
        }
    }
    else{
        traversal(i, 1, M.height){
            Coordinate nextCoord = {currentCoord.x, i};
            if (NOT HasCoord(visited, nextCoord)){
                visited[currentLength] = nextCoord;
                visited[currentLength + 1] = NullCoordinate;
                Buffer[currentLength] = ACCESS(M, nextCoord.x, nextCoord.y);
                Buffer[currentLength + 1] = NullToken;
                findOptimumSequence(M, Buffer, Seqs, visited, SeqCount, bufferSize, NOT horizontal, currentLength + 1, CurrentOptimumCoords, CurrentMax, maxBufferSize);
            }
        }
    }
}

int main(int argc, char **argv){
    Matrix M;
    int bufferSize;
    int seqCount;
    Sequence* Seqs;
    int i;

    // textInput("tests/negativeSeq.txt", &bufferSize, &M, &seqCount, &Seqs);

    if (argc > 1){
        char TestFolder[100] = "tests/";
        strcat(TestFolder, argv[1]);

        if (textInput(TestFolder, &bufferSize, &M, &seqCount, &Seqs) != 0){
            goto inputFail;
        }
    }
    else{
        if (randomInput(&bufferSize, &M, &seqCount, &Seqs)){
            goto inputFail;
        }
    }

    Token* Buffer = (Token*) malloc (sizeof(Token)*bufferSize + 1);
    Coordinate* visited = (Coordinate*) malloc (sizeof(Coordinate)*bufferSize + 1);
    Coordinate* OptimumCoords = (Coordinate*) malloc (sizeof(Coordinate)*bufferSize + 1);
    Buffer[0] = NullToken;
    visited[0] = NullCoordinate;
    OptimumCoords[0] = NullCoordinate;
    int max = 0;
    int maxBufferSize = 0;

    struct timeval tv;
    struct timeval start_tv;
    double elapsed = 0.0;

    gettimeofday(&start_tv, NULL);

    findOptimumSequence(M, Buffer, Seqs, visited, seqCount, bufferSize, 1, 0, OptimumCoords, &max, &maxBufferSize);

    gettimeofday(&tv, NULL);
    elapsed = (tv.tv_sec - start_tv.tv_sec) + (tv.tv_usec - start_tv.tv_usec) / 1000.0;

    Sequence TumbalPrint;
    TumbalPrint.reward = 0;
    TumbalPrint.size = maxBufferSize;
    TumbalPrint.buffer = (Token*) malloc (sizeof(Token) * maxBufferSize + 1);
    CoordsToTokens(M, OptimumCoords, TumbalPrint.buffer);

    printf("\n-------------Output------------\n");
    printf("%d\n", max);
    printSequence(TumbalPrint);
    printf("\n");

    i = 0;

    while(NOT isCoordinateEqual(OptimumCoords[i], NullCoordinate)){
        printCoord(OptimumCoords[i]);
        printf("\n");
        i++;
    }

    printf("\n%f ms\n", elapsed);

    char response;
    char decoy;
    repeatyn:
    printf("\nApakah ingin menyimpan solusi? (y/n)\n>> ");
    scanf("%c", &response);
    if (response == 'y'){
        saveResult(max, TumbalPrint, OptimumCoords, elapsed);
    }
    else if(response == 'n'){
        return 0;
    }
    else{
        scanf("%c", &decoy);
        goto repeatyn;
    }
    return 0;

    inputFail:
    return 1;
}