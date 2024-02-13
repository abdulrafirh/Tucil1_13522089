#ifndef TEXT_BOX_H
#define TEXT_BOX_H

#include "raygui.h"

typedef struct text_box{
    Rectangle rect;
    Color color;
    char* content;
    int capacity;
    int currLength;
    int (*constraint_f)(char);
    int isActive;
} TextBox;

int isInRange(int x, int a, int b);

int isKeyboardKey(char c);

int isTokenChar(char c);

int isNumber(char c);

void init_text_box(TextBox *textBox, Rectangle rect, Color color, int capacity, int (*constraint_f)(char));

int is_mouse_over_text_box(TextBox textBox);

void drawTextBox(TextBox textBox);

void hoverCheckTextBox(TextBox textBox);

void clickCheckTextBox(TextBox *textBox);

void activeCheckTextBox(TextBox *textBox);

void allCheckTextBox(TextBox *textBox);

#endif