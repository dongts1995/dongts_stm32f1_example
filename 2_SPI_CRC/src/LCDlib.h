#ifndef __LCDLIB_H
#define __LCDLIB_H

#include "stm32f10x.h"


/*		Define Constant, Command Code
*/
/* LCD's pins Definition*/
#define LCD_A0_1 GPIO_SetBits(GPIOB, GPIO_Pin_5);
#define LCD_A0_0 GPIO_ResetBits(GPIOB, GPIO_Pin_5);
#define LCD_A0          P5.1	//B5
#define LCD_CS          P4.1	//B6
#define LCD_RST         P5.0 	//B7


#define LCD_WIDTH			128
#define LCD_HEIGHT		    8
#define DEPTH_SEGMENTS	    4
#define	SET_BIAS		    0xA3 			//0xA3 or 0xA2
#define DISPLAY_ON		    0xAF
#define DISPLAY_OFF		    0xAE
#define POWER_CONTROL	    0x2F
#define LCD_RESET			0xE2

#define DISPLAY_ALL_POINT_ON		0xA5
#define DISPLAY_ALL_POINT_OFF		0xA4

#define DISPLAY_REVERSE	            0xA7		//reverse all points of Display panel (black->white and vice versa)
#define DISPLAY_NORMAL	            0xA6		
#define ADC_NORMAL			        0xA0
#define ADC_REVERSE			        0xA1
#define VERTICAL_DIRECTION_NORMAL	0xC0
#define VERTICAL_DIRECTION_REVERSE	0xC8

#define REGULATION_RESISTOR_RATIO   0x24    // 0x24 or 0x20
#define ELECTRONIC_VOLUME_MODE		0x81

#define _CONTRAST_LEVEL_1			0x0A    //  0x0A or 0x18
#define _CONTRAST_LEVEL_2			0x1C
#define _CONTRAST_LEVEL_3			0x20
#define _CONTRAST_LEVEL_4			0x2D
#define _CONTRAST_LEVEL_5			0x30

/* Select typography want to display on LCD
*/
#define TEXT_NORMAL                0x00
#define TEXT_UNDERLINE             0x01
#define TEXT_REVERSE               0x02
#define TEXT_UPPER_REVERSE         0x03
#define TEXT_LOWER_REVERSE         0x04 
#define TEXT_REVERSE_FOOTER        0x05 
#define TEXT_REVERSE_16x12         0x06 


/**************************************************************/
/*  Writing a data byte or a command to LCD  */

void LCD_write(uint8_t isData, uint8_t byte);

/*		Initializing LCD
*/
void LCD_init(void);
void LCD_Power_saver();
void LCD_Normal_Mode();

/* Adjusting LCD contrast   
*/
void LCD_contrast_set(uint8_t contrastLevel);

/* Jumping to [x;y] segment on LCD panel; x is column and y is page coordinate
*/
void LCD_gotoxy(uint8_t x, uint8_t y);

/* Clearing LCD and return cursor to [0;0]
*/
void LCD_clear(void);

/* Putting a char to LCD
*/
void LCD_putc(uint8_t c);

/* Putting a char with typography to LCD   tanhhc: ham nay minh tu them vao
*/
void LCD_putc_with_style (uint8_t text_style, uint8_t c);

/* Putting a reversed color char to LCD.
Serve for displaying Pointer of List Box
*/
void LCD_putc_reverse(uint8_t c);

/*		Putting a reversed color string to LCD.
Serve for displaying Pointer of List Box
*/
void LCD_puts_reverse(uint8_t *str);

/*		Putting upper half part of a reversed color char to LCD
Serve for displaying Main Title of List Box
*/
void LCD_putc_upp_reverse(uint8_t c);

/*		Putting lower half part of a reversed color char to LCD
Serve for displaying Main Title of List Box
*/
void LCD_putc_lower_reverse(uint8_t c);

/*		Putting upper half part of a reversed color string to LCD
Serve for displaying Main Title of List Box
*/
void LCD_puts_upp_reverse(uint8_t *str);

/*		Putting lower half part of a reversed color string to LCD
Serve for displaying Main Title of List Box
*/
void LCD_puts_lower_reverse(uint8_t *str);

/*		Putting a 12x16 font char to [x,y] point on LCD
*/
void LCD_putc12x16(uint8_t x, uint8_t y, uint8_t c);
void LCD_putc12x16_reverse(uint8_t x, uint8_t y, uint8_t c);

/* LCD_puts: Put full string to LCD
LCD_puts_with_len: Put start of string and specific length
LCD_puts_sub:   Put a string with start index and end index
*/
void LCD_puts(uint8_t *str);
void LCD_puts_with_len(uint8_t *str, uint8_t len);
void LCD_puts_sub(uint8_t *str, uint8_t startPoint, uint8_t endPoint);

/*		Putting a 12x16 string to [x, y] point on LCD
*/
void LCD_puts12x16(uint8_t x, uint8_t y, uint8_t *str);
void LCD_puts12x16_reverse(uint8_t x, uint8_t y, uint8_t *str);

/*		Putting an interger number to LCD. 
Limit to 9 digits because of range of integer number type
*/
void LCD_putNumber(uint32_t num);
void LCD_putNumber_reverse(uint32_t num);

/*		Putting a 12x16 interger number to [x, y] point on LCD. 
Limit to 9 digits because of range of integer number type
*/
void LCD_put12x16Number(uint8_t x, uint8_t y, uint32_t num);
void LCD_put12x16Number_reverse(uint8_t x, uint8_t y, uint32_t num);

/*		Putting an Icon to [x,y] point on LCD
*/
void LCD_putIcon(uint8_t x, uint8_t y, uint8_t icon);

/*		Scrolling LCD
*/
void LCD_scroll();

/*		Serving for confirm Box displaying
*/
void LCD_putc_reverse_with_footer(uint8_t c);
void LCD_puts_reverse_with_footer(uint8_t *str);

/* Put a char with underline to LCD
*/
void LCD_putc_underLine(uint8_t c);
void LCD_puts_underLine(uint8_t *str);

/*		Reversing LCD content if toRev # 0
*/
void LCD_reverse(uint8_t toRev);


#endif