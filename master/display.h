#ifndef DISPLAY_H
#define DISPLAY_H
#include <inttypes.h>

//set registers for window option 2
#define WINDOW_SETOPTIONS_RAM	0b10001011		//ram access control (cmd13)
#define WINDOW_SETOPTIONS_LCD	0b11000110		//lcd mapping control (cmd19)

#define WINDOW_ENABLE			0b11111001		//(cmd 34)
#define WINDOW_DISABLE			0b11111000 		//(cmd 34)

#define WINDOW_STARTCOL			0b11110100 		//dualcommand 2nd is coordinates (cmd30)
#define WINDOW_STARTPAGE		0b11110101		//dualcommand 2nd is coordinates (cmd31)
#define WINDOW_ENDCOL			0b11110110 		//dualcommand 2nd is coordinates (cmd32)
#define WINDOW_ENDPAGE			0b11110111		//dualcommand 2nd is coordinates (cmd33)


void initWindow(void);
void sendWindow(uint8_t x1,uint8_t y1,uint8_t x2, uint8_t y2, const uint8_t *data);

void displayInit(void);
void clear(void);
void sendbyte(uint8_t byte,uint8_t cd);
void page(uint8_t x,uint8_t y,uint8_t h);
void sendChar(uint8_t x,uint8_t y,uint8_t lx, uint8_t ly, const uint8_t Bstabe);

#endif
