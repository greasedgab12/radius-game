#ifndef DISPLAY_H
#define DISPLAY_H


void initWindow(void);
void sendWindow(uint8_t x1,uint8_t y1,uint8_t x2, uint8_t y2, const uint8_t *data);

void displayInit(void);
void clear(void);
void sendbyte(uint8_t byte,uint8_t cd);
void page(uint8_t x,uint8_t y,uint8_t h);

#endif
