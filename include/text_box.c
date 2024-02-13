#include <stdlib.h>
#include "macros.h"
#include "text_box.h"

int isInRange(int x, int a, int b){
    return ((x >= a) AND (x <= b));
}

int isKeyboardKey(char c){
    return isInRange((int)c, 32, 126);
}

int isTokenChar(char c){
    return (isInRange((int)c, 48, 57) OR isInRange((int)c, 65, 90) OR (int)c == 32);
}

int isNumber(char c){
    return (isInRange((int)c, 48, 57));
}

void init_text_box(TextBox* textBox, Rectangle rect, Color color, int capacity, int (*constraint_f)(char)){
    textBox->rect = rect;
    textBox->color = color;
    textBox->capacity = capacity;
    textBox->content = (char*) malloc (sizeof(char) * (capacity + 1));
    textBox->currLength = 0;
    textBox->constraint_f = constraint_f;
    textBox->isActive = 0;

    (textBox->content)[0] = '\0';
}

int is_mouse_over_text_box(TextBox textBox){
    return CheckCollisionPointRec(GetMousePosition(), textBox.rect);
}

void drawTextBox(TextBox textBox){
    DrawRectangleRec(textBox.rect, textBox.color);
    DrawText(textBox.content, (int)textBox.rect.x + 5, (int)textBox.rect.y + 8, 40, MAROON);
    if (textBox.isActive){
        if ((int)(2*GetTime()) % 2)
            {DrawText("_", (int)textBox.rect.x + 8 + MeasureText(textBox.content, 40), (int)textBox.rect.y + 12, 40, MAROON);}
        DrawRectangleLinesEx(textBox.rect, 2, RED);
    }
    else{
        DrawRectangleLinesEx(textBox.rect, 2, LIGHTGRAY);
    }
}

void hoverCheckTextBox(TextBox textBox){
    if (is_mouse_over_text_box(textBox)) {SetMouseCursor(MOUSE_CURSOR_IBEAM);}
}

void clickCheckTextBox(TextBox* textBox){
    if (is_mouse_over_text_box(*textBox) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        textBox->isActive = 1;
    }
    else if(!is_mouse_over_text_box(*textBox) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        textBox->isActive = 0;
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}

void activeCheckTextBox(TextBox* textBox){
    int key = GetCharPressed();

    while (key > 0)
    {
        if ((textBox->constraint_f)(key) && (textBox->currLength < textBox->capacity))
        {
            textBox->content[textBox->currLength] = (char)key;
            textBox->content[textBox->currLength + 1] = '\0';
            (textBox->currLength)++;
        }
        key = GetCharPressed(); 
    }

    if (IsKeyDown(KEY_BACKSPACE) AND textBox->currLength > 0){
        (textBox->currLength)--;
        textBox->content[textBox->currLength] = '\0';
    }
}

void allCheckTextBox(TextBox* textBox){
    hoverCheckTextBox(*textBox);
    clickCheckTextBox(textBox);
    if (textBox->isActive){activeCheckTextBox(textBox);}
}