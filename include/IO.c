#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "IO.h"

void empty_stdin (void){
    ungetc('t', stdin);
    int c = getchar();

    while (c != '\n' && c != EOF)
        c = getchar();
}

int isValidTokenChar(char c){
    return ((c >= 48 AND c <= 57) OR (c >= 65 AND c <= 90));
}

int exists(const char *fname)
{
    FILE *file;
    if ((file = fopen(fname, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

int textInput(char* txtPath, int* bufferSize, TokenMatrix* M, int* seqCount, Sequence** Seqs){
    
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

    // Read TokenMatrix Size
    if(fgets(line, 100, fptr)){
        if(sscanf(line, "%d %d", &((*M).width), &((*M).height)) != 2){
            goto matrixSizeFail;
        }
        else{
            if ((*M).width < 0 OR ((*M).height) < 0){goto matrixSizeFail;}
        }
    }
    else{
        matrixSizeFail:
        printf("TokenMatrix size invalid\n");
        goto txtFail;
    }
    CreateTokenMatrix(M);
    if (M->height == 0 OR M->width == 0){goto matrixSkip;}

    // Read TokenMatrix
    int i, j;
    char decoy;

    traversal(i, 1, (*M).height){
        if(fgets(line, 100, fptr)){
            if (line[3*(*M).width] != '\0' AND line[3*(*M).width] != '\n' AND line[3*(*M).width] != '\r') {goto matrixFail;}
            traversal(j, 1, (*M).width){
                if(sscanf((line + 3*(j - 1)), "%c%c%c", &(ACCESS((*M), j, i).first), &(ACCESS((*M), j, i).second), &decoy) != 3){
                    goto matrixFail;
                }
                if (decoy != ' ' AND decoy != '\n' AND decoy != '\r'){goto matrixFail;}
                if (NOT (isValidTokenChar(ACCESS((*M), j, i).first) AND isValidTokenChar(ACCESS((*M), j, i).second))) {goto matrixFail;}
            }
        }
        else{
            matrixFail:
            printf("TokenMatrix Invalid\n");
            goto txtFail;
        }
    }

    matrixSkip:

    // Read Sequence Amount
    if(fgets(line, 100, fptr)){
        if (sscanf(line, "%d", seqCount) != 1){
            goto seqAmountFail;
        }
        else{
            if (*seqCount < 0){goto seqAmountFail;}
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

    if (*seqCount == 0) {goto seqSkip;}

    traversal(j, 0, *seqCount - 1){
        currentSeq = &((*Seqs)[j]);
        CreateSequence(currentSeq, 33);

        // Read sequence of tokens
        if(fgets(line, 100, fptr)){
            i = 0;
            while(line[3*i] != '\0' AND line[3*i] != '\n' AND line[3*i] != '\r'){
                if (sscanf((line + 3*i), "%c%c%c", &(((currentSeq->buffer)[currentSeq->size]).first), &(((currentSeq->buffer)[currentSeq->size]).second), &decoy) != 3){
                    goto seqInputFail;
                }
                if(decoy != ' ' AND decoy != '\n' AND decoy != '\r') {goto seqInputFail;}
                (currentSeq->size)++;
                i++;
            }
            if (currentSeq->size < 2) {
                printf("Sequence Too Short\n");
                goto seqInputFail;
            }
            if (j > 0){
                traversal(i, 0, j - 1){
                    if (isSeqEq((*Seqs)[i], (*Seqs)[j])){
                        printf("Sequence has to be unique\n");
                        goto seqInputFail;
                    }
                }
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

    seqSkip:

    if(fgets(line, 100, fptr)){
        printf("Extra line detected in txt file\n");
        goto txtFail;
    }

    return 0;

    txtFail:
    printf("Failed to read input file\nProcess terminated\n");
    return 1;
}

int randomInput(int* bufferSize, TokenMatrix* M, int* seqCount, Sequence** Seqs){
    int TokenCount;
    char decoy[100];

    printf("---------------Generasi permainan secara Random---------------\n\n");

    int invalid;

    printf("Masukkan jumlah token unik : ");
    invalid = (scanf("%d", &TokenCount) != 1);
    if (NOT invalid) {invalid = (TokenCount <= 0);}

    while(invalid){
        printf("Jumlah token unik seharusnya integer positif\n");
        printf("Masukkan jumlah token unik : ");
        empty_stdin();
        invalid = (scanf("%d", &TokenCount) != 1);
        if (NOT invalid) {invalid = (TokenCount <= 0);}
    }
    
    empty_stdin();
    repeatToken:

    Token* Tokens = (Token*) malloc (sizeof(Token) * TokenCount);

    printf("Masukkan Token-Token dengan dipisah spasi : \n");
    int i, j;
    traversal(i, 0, TokenCount - 1){
        if (scanf("%c%c%c", &(Tokens[i].first), &(Tokens[i].second), decoy) != 3){
            printf("Masukkan token tidak valid\n");
            goto repeatToken;
        }
        if (*decoy != ' ' AND *decoy != '\n' AND *decoy != '\r'){
            printf("Masukkan token tidak valid\n");
            goto tokenInputFail;
        }
        if (NOT (isValidTokenChar(Tokens[i].first) AND isValidTokenChar(Tokens[i].second))){
            printf("Token hanya boleh terdiri dari huruf kapital dan angka 0-9\n");
            goto repeatToken;
        }
        if (i > 0){
            traversal(j, 0, (i - 1)){
                if (isTokenEqual(Tokens[i], Tokens[j])){
                    printf("Masukkan token harus unik\n");
                    tokenInputFail:
                    if(*decoy != '\n') {empty_stdin();}
                    goto repeatToken;
                }
            }
        }
    }
    if (*decoy != '\n' AND *decoy != '\r'){
        printf("Masukkan token tidak valid (terlalu banyak karakter)\n");
        goto tokenInputFail;
    }

    printf("Masukkan ukuran buffer : ");
    invalid = (scanf("%d", bufferSize) != 1);
    if (NOT invalid) {invalid = (*bufferSize < 0);}

    while (invalid) {
        printf("Ukuran buffer seharusnya integer non negatif\n");
        printf("Masukkan ukuran buffer : ");
        empty_stdin();
        invalid = (scanf("%d", bufferSize) != 1);
        if (NOT invalid) {invalid = (*bufferSize < 0);}
    }

    printf("Masukkan ukuran matriks (Kolom Baris) : ");
    empty_stdin();
    invalid = (scanf("%d %d", &((*M).width), &((*M).height)) != 2);
    if (NOT invalid) {invalid = ((*M).width < 0 || (*M).height < 0);}

    while (invalid) {
        printf("Ukuran matriks seharusnya 2 integer non negatif\n");
        printf("Masukkan ukuran matriks (Kolom Baris) : ");
        empty_stdin();
        invalid = (scanf("%d %d", &((*M).width), &((*M).height)) != 2);
        if (NOT invalid) {invalid = ((*M).width < 0 || (*M).height < 0);}
    }

    printf("Masukkan jumlah sekuens : ");
    empty_stdin();
    invalid = (scanf("%d", seqCount) != 1);
    if (NOT invalid) {invalid = (*seqCount < 0);}

    while (invalid) {
        printf("Jumlah sekuens seharusnya integer non negatif\n");
        printf("Masukkan jumlah sekuens : ");
        empty_stdin();
        invalid = (scanf("%d", seqCount) != 1);
        if (NOT invalid) {invalid = (*seqCount < 0);}
    }

    int seqSize;

    printf("Masukkan panjang maksimal sekuens : ");
    empty_stdin();
    invalid = (scanf("%d", &seqSize) != 1);
    if (NOT invalid) {invalid = (seqSize < 2);}

    while (invalid) {
        printf("Ukuran buffer seharusnya integer > 1\n");
        printf("Masukkan panjang maksimal sekuens : ");
        empty_stdin();
        invalid = (scanf("%d", &seqSize) != 1);
        if (NOT invalid) {invalid = (seqSize < 2);}
    }
    empty_stdin();

    int PossibleSequences = 0;
    traversal(i, 2, seqSize){
        PossibleSequences += (int)pow((double)TokenCount, (double)i);
    }

    if (*seqCount > PossibleSequences){
        printf("Dengan %d token unik tidak memungkinkan dibuat %d sekuens unik\n", TokenCount, *seqCount);
        goto randomFail;
    }

    // Random Generation
    time_t t;
    srand((unsigned) time(&t));
    CreateTokenMatrix(M);
    if (M->height > 0 AND M->width > 0){
        traversal(i, 1, M->height){
            traversal(j, 1, M->width){
                ACCESS((*M), j, i) = Tokens[rand() % TokenCount];
            }
        }
    }

    *Seqs = (Sequence*) malloc (sizeof(Sequence) * *seqCount);
    if (*seqCount > 0){
        traversal(i, 0, *seqCount - 1){
            repeatSeqGeneration:
            CreateSequence(&((*Seqs)[i]), 33);
            int seqLength = (rand() % (seqSize - 1)) + 2;
            (*Seqs)[i].size = seqLength;
            (*Seqs)[i].reward = (rand() % 600) - 200;
            traversal(j, 0, seqLength - 1){
                ((*Seqs)[i].buffer)[j] = Tokens[rand() % TokenCount];
            }
            if (i > 0){
                traversal(j, 0, i - 1){
                    if (isSeqEq((*Seqs)[i], (*Seqs)[j])){
                        goto repeatSeqGeneration;
                    }
                }
            }
        }
    }

    printf("\n---------------Hasil Generasi Random---------------\n");

    printf("\n---------------Matriks---------------\n");
    printTokenMatrix(*M);

    printf("\n---------------Sekuens---------------\n");
    if (*seqCount > 0){
        traversal(j, 0, *seqCount - 1){
            printSequence((*Seqs)[j]);
            printf("Bobot : %d", (*Seqs)[j].reward);
            printf("\n");
        }
    }
    return 0;

    randomFail:
    printf("Input data random gagal\nProses Terminasi\n");
    return 1;
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