#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "raylib.h"
#include "button.h"
#include "text_box.h"
#include "macros.h"
#include "structs.h"
#include "IO.h"

void findOptimumSequence(TokenMatrix M, Token* Buffer, Sequence* Seqs, Coordinate* visited, int SeqCount, int bufferSize, int horizontal, int currentLength, Coordinate* CurrentOptimumCoords, int* CurrentMax, int* maxBufferSize){
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

void calculateResultSpace(TokenMatrix M, Sequence* Seqs, int SeqCount, Rectangle* Cells, Rectangle* SeqsBackground){

    int Mwidth = 600;
    int Mheight = 600;

    double cellWidth = (double)5.0*Mwidth/((double)6*M.width + (double)1);
    double cellHeight = (double)5.0*Mheight/((double)6*M.height + (double)1);

    int i, j;

    traversal(i, 0, M.width - 1){
        traversal(j, 0, M.height - 1){
            (Cells)[j*M.width + i] = (Rectangle){0.2*cellWidth*(i + 1) + cellWidth*i, 0.2*cellHeight*(j + 1) + cellHeight*j + 75, cellWidth, cellHeight};
        }
    }

    traversal(i, 0, SeqCount - 1){
        (SeqsBackground)[i] = (Rectangle){600, 135 + 45*i + 7.5*i, 600, 45};
    }
}

void drawResultSpace(TokenMatrix M, Sequence* Seqs, int SeqCount, Rectangle* Cells, Rectangle* SeqsBackground){

    int i, j;
    char currentToken[3];
    currentToken[2] = '\0';
    Rectangle currentRect;

    traversal(i, 0, M.width - 1){
        traversal(j, 0, M.height - 1){
            currentRect = Cells[j*M.width + i];
            DrawRectangleRec(currentRect, DARKGREEN);
            currentToken[0] = ACCESS(M, i + 1, j + 1).first;
            currentToken[1] = ACCESS(M, i + 1, j + 1).second;
            DrawText(currentToken, currentRect.x + currentRect.width*0.5 - 0.5*MeasureText(currentToken, currentRect.height*0.5), currentRect.y + currentRect.height*0.5 - currentRect.height*0.5*0.5, currentRect.height*0.5, BLACK);
        }
    }

    char currentSeq[31];
    char rewardText[5];
    traversal(i, 0, SeqCount - 1){
        DrawRectangleRec(SeqsBackground[i], DARKGREEN);
        traversal(j, 0, Seqs[i].size - 1){
            currentSeq[3*j] = Seqs[i].buffer[j].first;
            currentSeq[3*j + 1] = Seqs[i].buffer[j].second;
            currentSeq[3*j + 2] = ' ';
        }
        currentSeq[3*j] = '\0';
        DrawText(currentSeq, 610, SeqsBackground[i].y + SeqsBackground[i].height*0.5 - 0.5*20, 20, BLACK);
        sprintf(rewardText, "%d", Seqs[i].reward);
        DrawText(rewardText, 1190 - MeasureText(rewardText, 20), SeqsBackground[i].y + SeqsBackground[i].height*0.5 - 0.5*20, 20, BLACK);
    }

    Rectangle LastText = (Rectangle){600, 555, 600, 45};
    DrawRectangleRec(LastText, MAROON);
    DrawText("Buffer", 610, 555 + 45*0.5 - 20*0.5, 20, GREEN);

    Rectangle BufferBackground = (Rectangle){600, 612.5, 600, 45};
    DrawRectangleRec(BufferBackground, DARKGREEN);
}

void showSol(TokenMatrix M, Sequence* Seqs, Coordinate* OptimumCoords, int MaxPoint, int usedBuffer, Rectangle* Cells, Rectangle* SeqsBackground, int iteration, char* currentBuffer, int* filledBuffer, Sound* nani, double elapsed){
    int i;
    int top = ((iteration) > (usedBuffer - 1)) ? (usedBuffer - 1) : (iteration);

    char currentToken[3];
    currentToken[2] = '\0';

    traversal(i, 0, top){

        int currX = OptimumCoords[i].x;
        int currY = OptimumCoords[i].y;

        Rectangle currentRect = Cells[(currY-1)*M.width + (currX - 1)];

        DrawRectangleLinesEx(currentRect, 3, GREEN);

        currentToken[0] = ACCESS(M, currX, currY).first;
        currentToken[1] = ACCESS(M, currX, currY).second;
        DrawText(currentToken, currentRect.x + currentRect.width*0.5 - 0.5*MeasureText(currentToken, currentRect.height*0.5), currentRect.y + currentRect.height*0.5 - currentRect.height*0.5*0.5, currentRect.height*0.5, RED);

        if (i == top AND top <= usedBuffer - 1 AND i >= (*filledBuffer)){
            currentBuffer[3*(*filledBuffer)] = currentToken[0];
            currentBuffer[3*(*filledBuffer) + 1] = currentToken[1];
            currentBuffer[3*(*filledBuffer) + 2] = ' ';
            currentBuffer[3*(*filledBuffer) + 3] = '\0';
            *filledBuffer += 1;
            PlaySound(*nani);
        }

        DrawText(currentBuffer, 610, 612.5 + 45*0.5 - 20*0.5, 20, BLACK);
    }

    if (iteration >= usedBuffer){
        char pointText[20];
        sprintf(pointText, "Points : %d", MaxPoint);
        DrawText(pointText, 1190 - MeasureText(pointText, 20), 555 + 45*0.5 - 20*0.5, 20, GREEN);

        char resultText[50];
        sprintf(resultText, "Solution Found in %lf ms", elapsed);
        DrawText(resultText, 610, 95, 20, GREEN);
    }
}

int main(){

    TokenMatrix M;
    int bufferSize;
    int seqCount;
    Sequence* Seqs;
    int tokenCount;
    Token* Tokens;
    int seqSize;

    Token* Buffer;
    Coordinate* visited;
    Coordinate* OptimumCoords;
    int max = 0;
    int maxBufferSize = 0;

    Rectangle* Cells;
    Rectangle* SeqsBackground;

    char currentBuffer[50];
    int filledBuffer = 0;

    double elapsed;

    int InMainMenu = 1;
    int IntxtInput = 0;
    int InRandomInput = 0;
    int renderResult = 0;
    int showingSolution = 0;
    int InOutputTxt = 0;
    int wrongFile = 0;
    int wrongRandom = 0;
    int fileSaved = 0;

    Button txtInputButton = {0};
    Button randomInputButton = {0};
    Button submitTxt = {0};
    Button showSolutionButton = {0};
    Button outputToTxt = {0};

    TextBox txtPathInput = {0};

    // RandomInput Page
    Button TokenCountRect, TokenRect, BufferSizeRect, MatrixColumnRect, MatrixRowRect, SeqCountRect, MaxSeqLengthRect;
    Button submitRandom;
    TextBox TokenCountBox, TokenBox, BufferSizeBox, MatrixColumnBox, MatrixRowBox, SeqCountBox, MaxSeqLengthBox;
    char warningText[100];

    // Output To File
    TextBox txtOutputName = {0};
    Button submitTxtOutput = {0};

    // Init
    // --------------------------------------------------------

    // Window
    const int screenWidth = 1200;
    const int screenHeight = 675;
    InitWindow(screenWidth, screenHeight, "SADAP.EXE");

    Image icon;
    icon = LoadImage("resources/icon.png");
    SetWindowIcon(icon);

    // External Files

    Font Roboto = LoadFontEx("font/Roboto-Regular.ttf", 24, 0, 0);
    Texture2D Backgrounds[3];

    Backgrounds[0] = LoadTexture("resources/cyberpunk_street_background.png");
    Backgrounds[1] = LoadTexture("resources/cyberpunk_street_midground.png");
    Backgrounds[2] = LoadTexture("resources/cyberpunk_street_foreground.png");

    float scrollingBack = 0.0f;
    float scrollingMid = 0.0f;
    float scrollingFore = 0.0f;

    InitAudioDevice();
    Music music = LoadMusicStream("resources/music.mp3");
    SetMusicVolume(music, 0.2);
    PlayMusicStream(music);

    Sound nani = LoadSound("resources/nani.wav");
    SetSoundVolume(nani, 0.45);

    // Main Menu
    Texture2D TitleText;
    TitleText = LoadTexture("resources/SADAP-TEXT.png");
    
    init_button(&txtInputButton, (Rectangle){screenWidth/4, screenHeight*175/(float)450, screenWidth/2, screenHeight/9}, GRAY);
    init_button(&randomInputButton, (Rectangle){screenWidth/4, screenHeight*250/(float)450, screenWidth/2, screenHeight/9}, GRAY);

    // Txt input

    init_text_box(&txtPathInput, (Rectangle){screenWidth*0.075, screenHeight*1/3, screenWidth*0.775, 48}, WHITE, 100, isKeyboardKey);

    Texture2D txtPromptText, submitTextHigh, submitTextLow;
    txtPromptText = LoadTexture("resources/txt_input_text.png");
    submitTextHigh = LoadTexture("resources/submit_button_high.png");
    submitTextLow = LoadTexture("resources/submit_button_low.png");

    Rectangle txtBackground;
    txtBackground = (Rectangle){screenWidth*0.8625, screenHeight*1/3, screenWidth*0.075, 48};

    // Txt Output
    Texture2D txtOutputPrompt;
    txtOutputPrompt = LoadTexture("resources/txt_output_text.png");

    init_button(&submitTxt, (Rectangle){screenWidth*0.5 - submitTextHigh.width*0.5, screenHeight*0.6, submitTextHigh.width, submitTextHigh.height}, WHITE);

    // Random Input
    init_button(&TokenCountRect, (Rectangle){screenWidth*0.0375, 0.05625*screenHeight, screenWidth*0.25, screenHeight/9}, DARKGREEN);
    init_button(&TokenRect, (Rectangle){screenWidth*0.0375, 0.05625*screenHeight + 90, screenWidth*0.25, screenHeight/9}, DARKGREEN);
    init_button(&BufferSizeRect, (Rectangle){screenWidth*0.0375, 0.05625*screenHeight + 180, screenWidth*0.25, screenHeight/9}, DARKGREEN);
    init_button(&MatrixColumnRect, (Rectangle){screenWidth*0.0375, 0.05625*screenHeight + 270, screenWidth*0.25, screenHeight/9}, DARKGREEN);
    init_button(&MatrixRowRect, (Rectangle){screenWidth*0.5375, 0.05625*screenHeight + 270, screenWidth*0.25, screenHeight/9}, DARKGREEN);
    init_button(&SeqCountRect, (Rectangle){screenWidth*0.0375, 0.05625*screenHeight + 360, screenWidth*0.25, screenHeight/9}, DARKGREEN);
    init_button(&MaxSeqLengthRect, (Rectangle){screenWidth*0.5375, 0.05625*screenHeight + 360, screenWidth*0.25, screenHeight/9}, DARKGREEN);

    init_text_box(&TokenCountBox, (Rectangle){screenWidth*0.30625, screenHeight*0.05625, 0.65*screenWidth, screenHeight/9}, WHITE, 30, isNumber);
    init_text_box(&TokenBox, (Rectangle){screenWidth*0.30625, screenHeight*0.05625 + 90, 0.65*screenWidth, screenHeight/9}, WHITE, 60, isTokenChar);
    init_text_box(&BufferSizeBox, (Rectangle){screenWidth*0.30625, screenHeight*0.05625 + 180, 0.65*screenWidth, screenHeight/9}, WHITE, 30, isNumber);
    init_text_box(&MatrixColumnBox, (Rectangle){screenWidth*0.30625, screenHeight*0.05625 + 270, 0.11*screenWidth, screenHeight/9}, WHITE, 30, isNumber);
    init_text_box(&MatrixRowBox, (Rectangle){screenWidth*0.8, screenHeight*0.05625 + 270, 0.11*screenWidth, screenHeight/9}, WHITE, 30, isNumber);
    init_text_box(&SeqCountBox, (Rectangle){screenWidth*0.30625, screenHeight*0.05625 + 360, 0.11*screenWidth, screenHeight/9}, WHITE, 30, isNumber);
    init_text_box(&MaxSeqLengthBox, (Rectangle){screenWidth*0.8, screenHeight*0.05625 + 360, 0.11*screenWidth, screenHeight/9}, WHITE, 30, isNumber);

    init_button(&submitRandom, (Rectangle){0.65*screenWidth, 0.822*screenHeight, 0.275*screenWidth, screenHeight/9}, LIGHTGRAY);

    // Result
    init_button(&showSolutionButton, (Rectangle){0.375*screenWidth, 5, screenWidth*0.25, screenHeight*0.05}, LIGHTGRAY);
    init_button(&outputToTxt, (Rectangle){0.7375*screenWidth, 5, screenWidth*0.25, screenHeight*0.05}, LIGHTGRAY);

    // Output Window
    init_button(&submitTxtOutput, (Rectangle){screenWidth*0.5 - submitTextHigh.width*0.5, screenHeight*0.6, submitTextHigh.width, submitTextHigh.height}, LIGHTGRAY);
    init_text_box(&txtOutputName, (Rectangle){screenWidth*0.075, screenHeight*1/3, screenWidth*0.775, 48}, WHITE, 100, isKeyboardKey);

    int solutionStartTime = 0;

    SetTargetFPS(60); // 60 FPS

    // --------------------------------------------------------

    // Main Loop
    // --------------------------------------------------------

    while(!WindowShouldClose()){

        // Update
        // --------------------------------------------------------
        UpdateMusicStream(music);

        // Background
        scrollingBack -= 0.1f;
        scrollingMid -= 0.5f;
        scrollingFore -= 1.0f;

        if (scrollingBack <= -Backgrounds[0].width*3.2) scrollingBack = 0;
        if (scrollingMid <= -Backgrounds[1].width*3.2) scrollingMid = 0;
        if (scrollingFore <= -Backgrounds[2].width*3.2) scrollingFore = 0;

        // In Main Menu
        if (InMainMenu){
            clickCheckButton(&txtInputButton);
            clickCheckButton(&randomInputButton);

            if(txtInputButton.isClicked){
                InMainMenu = 0;
                IntxtInput = 1;
            }
            else if(randomInputButton.isClicked){
                InMainMenu = 0;
                InRandomInput = 1;
            }
        }
        // In Txt Input Window
        else if (IntxtInput){
            clickCheckButton(&submitTxt);
            allCheckTextBox(&txtPathInput);
            char TestFolder[100] = "tests/";
            strcat(TestFolder, txtPathInput.content);
            strcat(TestFolder, ".txt");
            if (submitTxt.isClicked){
                if (NOT FileExists(TestFolder) AND NOT wrongFile){
                    wrongFile = 1;
                }
                else
                {
                    if (textInput(TestFolder, &bufferSize, &M, &seqCount, &Seqs) != 0){
                        goto inputFail;
                    }
                    IntxtInput = 0;
                    renderResult = 1;
                    Cells = (Rectangle*) malloc (sizeof(Rectangle) * M.width * M.height);
                    SeqsBackground = (Rectangle*) malloc (sizeof(Rectangle) * seqCount);
                    calculateResultSpace(M, Seqs, seqCount, Cells, SeqsBackground);
                }
            }
        }
        else if(InRandomInput){
            allCheckTextBox(&TokenCountBox);
            allCheckTextBox(&TokenBox);
            allCheckTextBox(&BufferSizeBox);
            allCheckTextBox(&MatrixColumnBox);
            allCheckTextBox(&MatrixRowBox);
            allCheckTextBox(&SeqCountBox);
            allCheckTextBox(&MaxSeqLengthBox);
            clickCheckButton(&submitRandom);

            if (submitRandom.isClicked){
                int invalid;
                char decoy[100];

                invalid = (sscanf(TokenCountBox.content, "%d", &tokenCount) != 1);
                if (invalid){
                    wrongRandom = 1;
                    strcpy(warningText, "Token count missing!");
                    goto tryRandomInputAgain;
                }

                Tokens = (Token*) malloc (sizeof(Token)*tokenCount);

                int i, j;
                if (TokenBox.currLength != tokenCount*3 - 1){
                    strcpy(warningText, "Token Invalid, check for trailing whitespace");
                    wrongRandom = 1;
                    goto tokenRandomInvalid;
                }
                traversal(i, 0, tokenCount - 1){
                    sscanf((TokenBox.content + 3*i), "%c%c%c", &(Tokens[i].first), &(Tokens[i].second), decoy);
                    if (*decoy != ' '){
                        wrongRandom = 1;
                        strcpy(warningText, "Token Invalid");
                        goto tokenRandomInvalid;
                    }
                    if (NOT (isValidTokenChar(Tokens[i].first) AND isValidTokenChar(Tokens[i].second))){
                        wrongRandom = 1;
                        strcpy(warningText, "Token Invalid");
                        goto tokenRandomInvalid;
                    }
                    if (i > 0){
                        traversal(j, 0, (i - 1)){
                            if (isTokenEqual(Tokens[i], Tokens[j])){
                                wrongRandom = 1;
                                strcpy(warningText, "Token has to be unique!");
                                tokenRandomInvalid:
                                goto tryRandomInputAgain;
                            }
                        }
                    }
                }

                invalid = (sscanf(BufferSizeBox.content, "%d", &bufferSize) != 1);
                if (invalid){
                    wrongRandom = 1;
                    strcpy(warningText, "Buffer Size Missing");
                    goto tryRandomInputAgain;
                }

                invalid = (sscanf(MatrixColumnBox.content, "%d", &(M.width)) != 1);
                if (invalid){
                    wrongRandom = 1;
                    strcpy(warningText, "Matrix Column Missing");
                    goto tryRandomInputAgain;
                }

                invalid = (sscanf(MatrixRowBox.content, "%d", &(M.height)) != 1);
                if (invalid){
                    wrongRandom = 1;
                    strcpy(warningText, "Buffer Row Missing");
                    goto tryRandomInputAgain;
                }

                invalid = (sscanf(SeqCountBox.content, "%d", &seqCount) != 1);
                if (invalid){
                    wrongRandom = 1;
                    strcpy(warningText, "Sequence Count Missing");
                    goto tryRandomInputAgain;
                }

                invalid = (sscanf(MaxSeqLengthBox.content, "%d", &seqSize) != 1);
                if (invalid){
                    wrongRandom = 1;
                    strcpy(warningText, "Max Sequence Length Missing");
                    goto tryRandomInputAgain;
                }

                int PossibleSequences = 0;
                traversal(i, 2, seqSize){
                    PossibleSequences += (int)pow((double)tokenCount, (double)i);
                }

                if (seqCount > PossibleSequences){
                    strcpy(warningText, "Not enough tokens and max sequence length\nto make the given amount of unique sequence");
                    wrongRandom = 1;
                    goto tryRandomInputAgain;
                }

                time_t t;
                srand((unsigned) time(&t));
                CreateTokenMatrix(&M);
                if (M.height > 0 AND M.width > 0){
                    traversal(i, 1, M.height){
                        traversal(j, 1, M.width){
                            ACCESS(M, j, i) = Tokens[rand() % tokenCount];
                        }
                    }
                }

                Seqs = (Sequence*) malloc (sizeof(Sequence) * seqCount);
                if (seqCount > 0){
                    traversal(i, 0, seqCount - 1){
                        repeatSeqGeneration:
                        CreateSequence(&(Seqs[i]), 33);
                        int seqLength = (rand() % (seqSize - 1)) + 2;
                        (Seqs)[i].size = seqLength;
                        (Seqs)[i].reward = (rand() % 600) - 200;
                        traversal(j, 0, seqLength - 1){
                            ((Seqs)[i].buffer)[j] = Tokens[rand() % tokenCount];
                        }
                        if (i > 0){
                            traversal(j, 0, i - 1){
                                if (isSeqEq((Seqs)[i], (Seqs)[j])){
                                    goto repeatSeqGeneration;
                                }
                            }
                        }
                    }
                }
                InRandomInput = 0;
                renderResult = 1;
                Cells = (Rectangle*) malloc (sizeof(Rectangle) * M.width * M.height);
                SeqsBackground = (Rectangle*) malloc (sizeof(Rectangle) * seqCount);
                calculateResultSpace(M, Seqs, seqCount, Cells, SeqsBackground);
            }
            tryRandomInputAgain:
        }
        else if (renderResult){
            clickCheckButton(&showSolutionButton);
            if (showSolutionButton.isClicked AND NOT showingSolution){
                Buffer = (Token*) malloc (sizeof(Token)*bufferSize + 1);
                visited = (Coordinate*) malloc (sizeof(Coordinate)*bufferSize + 1);
                OptimumCoords = (Coordinate*) malloc (sizeof(Coordinate)*bufferSize + 1);
                Buffer[0] = NullToken;
                visited[0] = NullCoordinate;
                OptimumCoords[0] = NullCoordinate;
                filledBuffer = 0;
                elapsed = GetTime();
                findOptimumSequence(M, Buffer, Seqs, visited, seqCount, bufferSize, 1, 0, OptimumCoords, &max, &maxBufferSize);
                elapsed = (GetTime() - elapsed)*1000;
                showingSolution = 1;
                StopMusicStream(music);
                solutionStartTime = (int)GetTime();
            }
            if (showingSolution){
                clickCheckButton(&outputToTxt);
            }
            if(outputToTxt.isClicked){
                renderResult = 0;
                InOutputTxt = 1;
            }
        }
        else if (InOutputTxt){
            clickCheckButton(&submitTxtOutput);
            allCheckTextBox(&txtOutputName);
            char ResultFolder[100] = "results/";
            strcat(ResultFolder, txtOutputName.content);
            strcat(ResultFolder, ".txt");
            if (submitTxtOutput.isClicked AND NOT fileSaved){

                Sequence TumbalPrint;
                TumbalPrint.reward = 0;
                TumbalPrint.size = maxBufferSize;
                TumbalPrint.buffer = (Token*) malloc (sizeof(Token) * maxBufferSize + 1);
                CoordsToTokens(M, OptimumCoords, TumbalPrint.buffer);

                FILE* fptr;
                fptr = fopen(ResultFolder, "w");

                fprintf(fptr, "%d\n", max);

                int i;
                int bufferLength = TumbalPrint.size;

                traversal(i, 0, bufferLength - 1){
                    fprintf(fptr, "%c%c", TumbalPrint.buffer[i].first, TumbalPrint.buffer[i].second);
                    if (i < bufferLength - 1) {fprintf(fptr, " ");}
                }
                fprintf(fptr, "\n");

                traversal(i, 0, bufferLength - 1){
                    fprintf(fptr, "%d,%d\n", OptimumCoords[i].x, OptimumCoords[i].y);
                }
                fprintf(fptr, "\n");

                fprintf(fptr, "%f ms", elapsed);
                fclose(fptr);
                fileSaved = 1;
            }
        }

        // Draw
        // --------------------------------------------------------
        BeginDrawing();

        // Background
        ClearBackground(GetColor(0x052c46ff));

        // Draw background image twice
        DrawTextureEx(Backgrounds[0], (Vector2){ scrollingBack, 20 }, 0.0f, 3.5f, WHITE);
        DrawTextureEx(Backgrounds[0], (Vector2){ Backgrounds[0].width*3.2 + scrollingBack, 20 }, 0.0f, 3.5f, WHITE);

        // Draw midground image twice
        DrawTextureEx(Backgrounds[1], (Vector2){ scrollingMid, 20 }, 0.0f, 3.5f, WHITE);
        DrawTextureEx(Backgrounds[1], (Vector2){ Backgrounds[1].width*3.2 + scrollingMid, 20 }, 0.0f, 3.5f, WHITE);

        // Draw foreground image twice
        DrawTextureEx(Backgrounds[2], (Vector2){ scrollingFore, 70 }, 0.0f, 3.5f, WHITE);
        DrawTextureEx(Backgrounds[2], (Vector2){ Backgrounds[2].width*3.2 + scrollingFore, 70 }, 0.0f, 3.5f, WHITE);

        if (InMainMenu){

            DrawTextureEx(TitleText, (Vector2){0.25*screenWidth, 0.12*screenHeight}, 0.0f, 1.0f, WHITE);

            drawButton(txtInputButton);
            drawButton(randomInputButton);
            addButtonText(txtInputButton, "Input from txt file", Roboto);
            addButtonText(randomInputButton, "Randomly Generate", Roboto);

            hoverCheckButton(txtInputButton);
            hoverCheckButton(randomInputButton);
        }
        else if(IntxtInput){
            DrawTextureEx(txtPromptText, (Vector2){0.5*screenWidth - 0.5*0.75*(txtPromptText.width), 0.2*screenHeight}, 0.0f, 0.75f, WHITE);
            drawTextBox(txtPathInput);
            DrawRectangleRec(txtBackground, WHITE);
            DrawRectangleLinesEx(txtBackground, 2, LIGHTGRAY);
            DrawText(".txt", txtBackground.x + txtBackground.width*0.5 - 0.5*MeasureText(".txt", 32), txtBackground.y + 8, 32, MAROON);
            if (is_mouse_over_button(submitTxt)){
                DrawTextureEx(submitTextHigh, (Vector2){0.5*screenWidth - 0.5*submitTextHigh.width, 0.6*screenHeight}, 0.0f, 1.0f, WHITE);
                DrawRectangleLinesEx(submitTxt.rect, 1.0f, MAROON);
            }
            else{
                DrawTextureEx(submitTextLow, (Vector2){0.5*screenWidth - 0.5*submitTextHigh.width, 0.6*screenHeight}, 0.0f, 1.0f, WHITE);
            }

            if (wrongFile){
                DrawText("File tersebut tidak valid", screenWidth*0.5 - 0.5*MeasureText("File tersebut tidak valid", 32), screenHeight*0.5 - 16, 32, GREEN);
            }
        }
        else if(InRandomInput){
            drawButton(TokenCountRect);
            drawButton(TokenRect);
            drawButton(BufferSizeRect);
            drawButton(MatrixColumnRect);
            drawButton(MatrixRowRect);
            drawButton(SeqCountRect);
            drawButton(MaxSeqLengthRect);
            addButtonText(TokenCountRect, "Number of Token :", Roboto);
            addButtonText(TokenRect, "Tokens :", Roboto);
            addButtonText(BufferSizeRect, "Buffer Size :", Roboto);
            addButtonText(MatrixColumnRect, "Matrix Column :", Roboto);
            addButtonText(MatrixRowRect, "Matrix Row :", Roboto);
            addButtonText(SeqCountRect, "Sequence Count :", Roboto);
            addButtonText(MaxSeqLengthRect, "Max Sequence Length :", Roboto);
            drawTextBox(TokenCountBox);
            drawTextBox(TokenBox);
            drawTextBox(BufferSizeBox);
            drawTextBox(MatrixColumnBox);
            drawTextBox(MatrixRowBox);
            drawTextBox(SeqCountBox);
            drawTextBox(MaxSeqLengthBox);

            drawButton(submitRandom);
            addButtonText(submitRandom, "Submit", Roboto);
            hoverCheckButton(submitRandom);

            if (wrongRandom){
                DrawText(warningText, screenWidth*0.0375, 0.9*screenHeight - 26*0.5, 26, GREEN);
            }
        }
        else if(renderResult){
            drawResultSpace(M, Seqs, seqCount, Cells, SeqsBackground);
            drawButton(showSolutionButton);
            addButtonText(showSolutionButton, "Show Solution", Roboto);
            hoverCheckButton(showSolutionButton);
            if (showingSolution){
                int iter = ((int)GetTime() - solutionStartTime);
                showSol(M, Seqs, OptimumCoords, max, maxBufferSize, Cells, SeqsBackground, iter, currentBuffer, &filledBuffer, &nani, elapsed);
                drawButton(outputToTxt);
                addButtonText(outputToTxt, "Output To Txt File", Roboto);
                hoverCheckButton(outputToTxt);
            }
        }
        else if(InOutputTxt){
            DrawTextureEx(txtOutputPrompt, (Vector2){0.5*screenWidth - 0.5*0.75*(txtPromptText.width), 0.2*screenHeight}, 0.0f, 0.75f, WHITE);
            drawTextBox(txtOutputName);
            DrawRectangleRec(txtBackground, WHITE);
            DrawRectangleLinesEx(txtBackground, 2, LIGHTGRAY);
            DrawText(".txt", txtBackground.x + txtBackground.width*0.5 - 0.5*MeasureText(".txt", 32), txtBackground.y + 8, 32, MAROON);
            if (is_mouse_over_button(submitTxtOutput)){
                DrawTextureEx(submitTextHigh, (Vector2){0.5*screenWidth - 0.5*submitTextHigh.width, 0.6*screenHeight}, 0.0f, 1.0f, WHITE);
                DrawRectangleLinesEx(submitTxtOutput.rect, 1.0f, MAROON);
            }
            else{
                DrawTextureEx(submitTextLow, (Vector2){0.5*screenWidth - 0.5*submitTextHigh.width, 0.6*screenHeight}, 0.0f, 1.0f, WHITE);
            }
            if(fileSaved){
                DrawText("File has been successfully saved", screenWidth/2 - 0.5*MeasureText("File has been successfully saved", 30), 0.1*screenHeight, 30, GREEN);
            }
        }

        DrawFPS(20, 20);
        EndDrawing();
    }

    // --------------------------------------------------------

    // Deinitialization
    // --------------------------------------------------------

    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();

    // --------------------------------------------------------

    return 0;

    inputFail:
    printf("fail\n");
    return 1;
}