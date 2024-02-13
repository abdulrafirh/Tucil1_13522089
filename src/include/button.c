#include "button.h"

void init_button(Button* button, Rectangle rect, Color color){
    button->rect = rect;
    button->color = color;
    button->isClicked = 0;
}

int is_mouse_over_button(Button button){
    return CheckCollisionPointRec(GetMousePosition(), button.rect);
}

void drawButton(Button button){
    DrawRectangleRec(button.rect, button.color);
}

void addButtonText(Button button, char* Text, Font font){
    DrawTextEx(font, Text, (Vector2){button.rect.x + button.rect.width/2 - (MeasureText(Text, 24)/(float)2), button.rect.y + button.rect.height/2 - 32/2}, 32, 1, BLACK);
}

void highlightButton(Button button){
    DrawRectangleLinesEx(button.rect, 1.5, (Color){147, 32, 32, 255});
}

void hoverCheckButton(Button button){
    if(is_mouse_over_button(button)){highlightButton(button);}
}

void clickCheckButton(Button* button){
    if(is_mouse_over_button(*button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        button->isClicked = 1;
    }
    else{
        button->isClicked = 0;
    }
}