#include "sprite.h"
#include <stdlib.h>


Sprite newSprite(uint8_t x, uint8_t y, uint8_t *data){

    Sprite self = (Sprite)malloc(sizeof(struct Sprite_Struct));

    self->x = x;
    self->y = y;

    self->setData = &setSpriteData;
    self->setData(self, data);
    self->setXY = &setSpriteXY;
    return self;
}

void setSpriteXY(Sprite self, uint8_t x, uint8_t y){
    self->x = x;
    self->y = y;
}

void setSpriteData(Sprite self, uint8_t *data){
    self->lx = data[0];
    self->ly = data[1]/data[0];
    self->data= data;
}

Blank newBlank(Sprite instance){
    Blank self = (Blank)malloc(sizeof(struct Blank_Struct));

    self->x = instance->x;
    self->y = instance->y;
    self->lx = instance->lx;
    self->ly = instance->ly;
    return self;
}
