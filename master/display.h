#ifndef DISPLAY_H
#define DISPLAY_H
#include <inttypes.h>

//------------------------------------------------------------------------------------
//registers to set for window usage
#define WINDOW_ENABLE			0b11111001		//(cmd 34)
#define WINDOW_DISABLE			0b11111000 		//(cmd 34)

#define WINDOW_STARTCOL			0b11110100 		//dualcommand 2nd is coordinates (cmd30)
#define WINDOW_STARTPAGE		0b11110101		//dualcommand 2nd is coordinates (cmd31)
#define WINDOW_ENDCOL			0b11110110 		//dualcommand 2nd is coordinates (cmd32)
#define WINDOW_ENDPAGE			0b11110111		//dualcommand 2nd is coordinates (cmd33)
//------------------------------------------------------------------------------------
//registers to set at startup
#define DISPLAY_ENABLE			0b10101111		//enable display
#define SET_PAGE_ADDRESS 		0b01100000		//set ddram pageaddress for r/w access
#define SET_COLADR_LSB_0 		0b00000000		//select ddram column addr for r/wn access
#define SET_COLADR_MSB_0 		0b00010000		//select ddram column addr for r/wn access
#define WINDOW_SETOPTIONS_RAM	0b10001011		//ram access control (cmd13)
#define SET_RAM_ADR_CTRL 		0b10001001		//registers for ram access control

#define SET_SCRL_LINE_LSB_0 	0b01000000		//set scroll line number
#define SET_SCRL_LINE_MSB_0 	0b01010000		//set scroll line number
#define SET_SCRL_LINE_MSB_1 	0b01011111		//set scroll line number

#define SET_PANEL_LOADING  		0b00101011		//set according to panel capacitance
#define SET_DISPLAY_BIAS_RATIO 	0b11101011

#define VBIAS_VALUE 			0b01011111
#define SET_VBIAS_POTI 			0b10000001

#define WINDOW_SETOPTIONS_LCD	0b11000110		//lcd mapping control (cmd19)
#define SET_LCD_MAP_CTRL 		0b11000110		//set lcd mapping ctrl mirrored or not

#define SET_COM_END 			0b11110001
#define SET_COM_VALUE 			0b01100111
//------------------------------------------------------------------------------------
//extra Display Options
#define DISPLAY_FORCE_ON		0b10100100		//turn on all display pixel, doesnt change ram
#define DISPLAY_INVERSE			0b10100110		//inverse display colors, doesnt change ram
//------------------------------------------------------------------------------------

//Initialize ports used for serial connection with the display.
void displayInitPorts(void);

//Initialize display itself.
void displayInit(void);

/**sendByte: Send singular byte to the display.
 *uint8_t byte: Byte to send via serial interface.
 *uint8_t cd: 0: sent byte is a command, 1: sent byte is data.
 */
void sendbyte(uint8_t byte,uint8_t cd);
/**sendWindow: Send an entire block of data to the display.
 * uint8_t x,y: Coordinates of the left upper corner of the block.
 * uint8_t lx,ly: Size of the block.
 * unit8_t *data: Pointer to the data in memory.
 */
void sendWindow(uint8_t x,uint8_t y,uint8_t lx, uint8_t ly, const uint8_t *data);

//Clear the display.
void displayClear(void);

/**Invert display "color".
 * uint8_t option: 1: invert "color", 0: no inversion.
 */
void displayInverse(uint8_t option);


#endif
