#ifndef MY_BUTTON_H
#define MY_BUTTON_H

#include "raylib.h"

typedef struct button{
    Rectangle rect;
    Color color;
    int isClicked;
} Button;

void init_button(Button *button, Rectangle rect, Color color);

int is_mouse_over_button(Button button);

void drawButton(Button button);

void addButtonText(Button button, char *Text, Font font);

void highlightButton(Button button);

void hoverCheckButton(Button button);

void clickCheckButton(Button *button);


#endif