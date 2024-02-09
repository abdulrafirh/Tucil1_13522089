#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "IO.h"

int textInput(char* txtPath, int* bufferSize, Matrix* M, int* seqCount, Sequence** Seqs){
    FILE* fptr = fopen(txtPath, "r");
    char line[100];

    // Read Buffer Size
    if(fgets(line, 100, fptr)){
        if (sscanf(line, "%d", bufferSize) != 1){
            goto bufferFail;
        }
    }
    else{
        bufferFail:
        printf("Buffer size invalid\n");
        goto txtFail;
    }

    // Read Matrix Size
    if(fgets(line, 100, fptr)){
        if(sscanf(line, "%d %d", &((*M).width), &((*M).height)) != 2){
            goto matrixSizeFail;
        }
    }
    else{
        matrixSizeFail:
        printf("Matrix size invalid\n");
        goto txtFail;
    }
    CreateMatrix(M);

    // Read Matrix
    int i, j;

    traversal(i, 1, (*M).height){
        if(fgets(line, 100, fptr)){
            traversal(j, 1, (*M).width){
                if(sscanf((line + 3*(j - 1)), "%c%c", &(ACCESS((*M), j, i).first), &(ACCESS((*M), j, i).second)) != 2){
                    goto matrixFail;
                }
            }
        }
        else{
            matrixFail:
            printf("Matrix Invalid\n");
            goto txtFail;
        }
    }

    // Read Sequence Amount
    if(fgets(line, 100, fptr)){
        if (sscanf(line, "%d", seqCount) != 1){
            goto seqAmountFail;
        }
    }
    else{
        seqAmountFail:
        printf("Sequence count invalid\n");
        goto txtFail;
    }

    // Read Sequences
    *Seqs = (Sequence*) malloc (sizeof(Sequence) * *seqCount);
    Sequence* currentSeq;

    traversal(j, 0, *seqCount - 1){
        currentSeq = &((*Seqs)[j]);
        CreateSequence(currentSeq, 33);

        // Read sequence of tokens
        if(fgets(line, 100, fptr)){
            i = 0;
            while(line[3*i] != '\0'){
                if (sscanf((line + 3*i), "%c%c", &(((currentSeq->buffer)[currentSeq->size]).first), &(((currentSeq->buffer)[currentSeq->size]).second)) != 2){
                    goto seqInputFail;
                }
                (currentSeq->size)++;
                i++;
            }
        }
        else{
            seqInputFail:
            printf("Sequence input invalid\n");
            goto txtFail;
        }

        // Read sequence reward value
        if(fgets(line, 100, fptr)){
            if (sscanf(line, "%d", &(currentSeq->reward)) != 1){
                goto seqRewardFail;
            }
        }
        else{
            seqRewardFail:
            printf("Sequence reward invalid\n");
            goto txtFail;
        }
    }
    return 0;

    txtFail:
    printf("Failed to read input file\nProcess terminated\n");
    return 1;
}

int randomInput(int* bufferSize, Matrix* M, int* seqCount, Sequence** Seqs){
    int TokenCount;
    char decoy[100];

    printf("---------------Generasi permainan secara Random---------------\n\n");
    printf("Masukkan jumlah token unik : ");
    scanf("%d", &TokenCount);
    scanf("%c", decoy);

    Token* Tokens = (Token*) malloc (sizeof(Token) * TokenCount);

    printf("Masukkan Token-Token dengan dipisah spasi : \n");
    int i, j;
    traversal(i, 0, TokenCount - 1){
        scanf("%c%c%c", &(Tokens[i].first), &(Tokens[i].second), decoy);
    }

    printf("Masukkan ukuran buffer : ");
    scanf("%d", bufferSize);

    printf("Masukkan ukuran matriks (Kolom Baris) : ");
    scanf("%d %d", &((*M).width), &((*M).height));

    printf("Masukkan jumlah sekuens : ");
    scanf("%d", seqCount);

    int seqSize;
    printf("Masukkan panjang maksimal sekuens : ");
    scanf("%d", &seqSize);

    // Random Generation
    time_t t;
    srand((unsigned) time(&t));
    CreateMatrix(M);
    traversal(i, 1, M->height){
        traversal(j, 1, M->width){
            ACCESS((*M), j, i) = Tokens[rand() % TokenCount];
        }
    }

    *Seqs = (Sequence*) malloc (sizeof(Sequence) * *seqCount);
    traversal(i, 0, *seqCount - 1){
        CreateSequence(&((*Seqs)[i]), 33);
        int seqLength = (rand() % (seqSize - 1)) + 2;
        (*Seqs)[i].size = seqLength;
        (*Seqs)[i].reward = (rand() % 200) - 100;
        traversal(j, 0, seqLength - 1){
            ((*Seqs)[i].buffer)[j] = Tokens[rand() % TokenCount];
        }
    }

    printf("\n---------------Hasil Generasi Random---------------\n");

    printf("\n---------------Matriks---------------\n");
    printMatrix(*M);

    printf("\n---------------Sekuens---------------\n");
    traversal(j, 0, *seqCount - 1){
        printSequence((*Seqs)[j]);
        printf("Bobot : %d", (*Seqs)[j].reward);
        printf("\n");
    }
    return 0;
}

int saveResult(int maxPoint, Sequence Result, Coordinate* Coordinates, double time){

    char Filename[100] = "results/";
    char temp[100];
    printf("Masukkan nama file output tanpa ekstensi .txt :\n>> ");
    scanf("%s", temp);
    strcat(Filename, temp);
    strcat(Filename, ".txt");

    FILE* fptr;
    fptr = fopen(Filename, "w");

    fprintf(fptr, "%d\n", maxPoint);

    int i;
    int bufferLength = Result.size;

    traversal(i, 0, bufferLength - 1){
        fprintf(fptr, "%c%c", Result.buffer[i].first, Result.buffer[i].second);
        if (i < bufferLength - 1) {fprintf(fptr, " ");}
    }
    fprintf(fptr, "\n");

    traversal(i, 0, bufferLength - 1){
        fprintf(fptr, "%d,%d\n", Coordinates[i].x, Coordinates[i].y);
    }
    fprintf(fptr, "\n");

    fprintf(fptr, "%f ms", time);

    return 0;
}