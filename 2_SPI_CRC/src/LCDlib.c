#include "LCDlib.h"

#include "stm32f10x.h"


void LCD_write(uint8_t isData, uint8_t byte)
{
    if (isData)
    LCD_A0_1
    else
    LCD_A0_0
    Sync_Send_Byte(byte);
}

uint8_t LCD_ST = 2;
void LCD_init(void)
{
  LCD_RST = LOW;
  delay_ms(5);   // equal to 1ms
  LCD_RST = HIGH;
  delay_ms(5);   // equal to 1ms
  LCD_write(0, LCD_RESET);
  delay_ms(5);   // equal to 1ms
  LCD_write(0, SET_BIAS);
  LCD_write(0, POWER_CONTROL);
  delay_ms(250);   // equal to 50ms 
  LCD_write(0, REGULATION_RESISTOR_RATIO);
  LCD_write(0, DISPLAY_ON);
  LCD_write(0, ADC_REVERSE);
  LCD_write(0, VERTICAL_DIRECTION_REVERSE);
  LCD_contrast_set(screen_Contrast);
  Set_LCD_BackLight_Level(screen_BackLight);
  LCD_clear();
  LCD_ST = 1;

}

void LCD_Power_saver() {  // tanhhc:  chua test func nay
  if (LCD_ST != 0) {
    LCD_write(0, DISPLAY_OFF);
    LCD_write(0, DISPLAY_ALL_POINT_ON);

    LCD_CS = 0;
    LCD_A0 = 1;
//    LCD_SCL = 1;          tanhhc:  Luu y cho nay chuyen thanh disable module SPI, vi qua Renesas minh dung SPI de giao tiep
//    LCD_SDI = 1;

    LCD_ST = 0;
  }
}

void LCD_Normal_Mode() {        // tanhhc: chua test func nay, tai sao lai khong reset LCD, set bias, resistor ratio nua, ... nhu ham LCD_init?
  if (LCD_ST != 1) {
    LCD_write(0, DISPLAY_ALL_POINT_OFF);
    LCD_write(0, DISPLAY_ON);
    LCD_write(0, POWER_CONTROL);
    LCD_write(0, ADC_REVERSE);
    LCD_write(0, VERTICAL_DIRECTION_REVERSE);
    LCD_contrast_set(screen_Contrast);
    Set_LCD_BackLight_Level(screen_BackLight);
    LCD_clear();
    LCD_ST = 1;
  }
}  

void LCD_contrast_set(uint8_t contrastLevel) {
  LCD_write(0, ELECTRONIC_VOLUME_MODE);
  switch (contrastLevel) {
  case CONTRAST_LEVEL_1:
    LCD_write(0, _CONTRAST_LEVEL_1);
    break;
  case CONTRAST_LEVEL_2:
    LCD_write(0, _CONTRAST_LEVEL_2);
    break;
  case CONTRAST_LEVEL_3:
    LCD_write(0, _CONTRAST_LEVEL_3);
    break;
  case CONTRAST_LEVEL_4:
    LCD_write(0, _CONTRAST_LEVEL_4);
    break;
  case CONTRAST_LEVEL_5:
    LCD_write(0, _CONTRAST_LEVEL_5);
    break;
  default:
    LCD_write(0, _CONTRAST_LEVEL_2);
  }
}

void LCD_gotoxy(uint8_t x, uint8_t y)
{
    LCD_write(0, 0xB0 | (y));
    LCD_write(0, x & 0x0F);
    LCD_write(0, 0x10 | ((x >> 4) & 0x0F));
}

void LCD_clear(void)
{
    uint8_t i, j;
    for (i = 0; i < 8; i++)
    {
        LCD_gotoxy(0, i);
        for (j = 0; j < 132; j++)
        {
            LCD_write(1, 0);
        }
    }
    LCD_gotoxy(0,0);
}

void LCD_putc(uint8_t c)
{
    if ((c >= 0x20) && (c <= 0x7F))
    {
        c = c - 32U;
        LCD_A0 = 1;        
        Sync_Send_Array(TEXT_NORMAL, font6x8[c], 6);            // truyen them doi so de xac dinh typography muon hien thi
    }
}

void LCD_putc_with_style (uint8_t text_style, uint8_t c)  //tanhhc:    ham nay minh tu them vao
{
    if ((c >= 0x20) && (c <= 0x7F))
    {
      c = c - 32U;
      LCD_A0 = 1;        
      Sync_Send_Array(text_style, font6x8[c], 6);            // truyen them doi so de xac dinh typography muon hien thi
    }
}  

void LCD_puts(uint8_t * str)     // tanhhc: Luu y toan bo a Minh dung char * str, nhung minh lai dung uint8_t *. Sau nay can double check lai
{
    while (* str)
    LCD_putc(* str++);
}

void LCD_puts_with_len(uint8_t * str, uint8_t len) {    // tanhhc: Luu y toan bo a Minh dung char * str, nhung minh lai dung uint8_t *. Sau nay can double check lai
  uint8_t i = 0;
  while ( * (str + i) && i < len) {
    LCD_putc( * (str + i));
    i++;
  }
}

void LCD_puts_sub(uint8_t * str, uint8_t startPoint, uint8_t endPoint) {
  uint8_t i = startPoint;
  while ( * (str + i) && i <= endPoint) {
    LCD_putc( * (str + i));
    i++;
  }
}

void LCD_putc_underLine(uint8_t c)          //tanhhc:  Sau nay toi uu code lai sau (boi vi hien tai de cho giong ten ham trong source hien tai), 
{                                                 //                        replace toan bo cac ham underline/reverse thanh ham LCD_putc_with_style duy nhat
    LCD_putc_with_style(TEXT_UNDERLINE, c);
}

void LCD_puts_underLine(uint8_t * str)
{
  while ( * str)
  LCD_putc_underLine( * str++);
}

void LCD_putc_reverse(uint8_t c) 
{
  LCD_putc_with_style(TEXT_REVERSE, c);
}

void LCD_puts_reverse(uint8_t * str) 
{
  while ( * str)
    LCD_putc_reverse( * str++);
}

void LCD_putc_upp_reverse(uint8_t c) 
{
  LCD_putc_with_style(TEXT_UPPER_REVERSE, c);
}

void LCD_putc_lower_reverse(uint8_t c) 
{
  LCD_putc_with_style(TEXT_LOWER_REVERSE, c);
}

void LCD_puts_upp_reverse(uint8_t * str) 
{
  while ( * str)
  LCD_putc_upp_reverse( * str++);
}

void LCD_puts_lower_reverse(uint8_t * str) 
{
  while ( * str)
  LCD_putc_lower_reverse( * str++);
}

void LCD_putNumber(uint32_t num) 
{
  char str[10] = "";
  sprintf_vft(str, num);
  LCD_puts(str);
}

void LCD_putNumber_reverse(uint32_t num) 
{
  char str[10] = "";
  sprintf_vft(str, num);
  LCD_puts_reverse(str);
}

void LCD_scroll() 
{
  uint8_t i;
  for (i = 0; i < 64; i++) {
    LCD_write(0, 0x40 | (i));
    delay_ms(100);
  }
  LCD_write(0, 0x40);
}

void LCD_reverse(uint8_t toRev) 
{
  if (toRev) 
  LCD_write(0, DISPLAY_REVERSE);
  else 
  LCD_write(0, DISPLAY_NORMAL);
}

void LCD_putc12x16(uint8_t x, uint8_t y, uint8_t c) {
    uint8_t i;
    if ((x < LCD_WIDTH) && (y < LCD_HEIGHT))
        LCD_gotoxy(x, y);
    else
        return;
    if ((c >= 0x20) && (c <= 0x7F)) {
        c = c - 32U;
        LCD_A0 = 1;
        Sync_Send_Array(TEXT_NORMAL, font12x16[2 * c], 12);
        LCD_gotoxy(x, y + 1); // go to next page
        LCD_A0 = 1;
        Sync_Send_Array(TEXT_NORMAL, font12x16[2 * c + 1], 12);
    }
}

void LCD_putc12x16_reverse(uint8_t x, uint8_t y, uint8_t c) 
{
  uint8_t i;
  if ((x < LCD_WIDTH) && (y < LCD_HEIGHT))
    LCD_gotoxy(x, y);
  else 
  return;
  if ((c >= 0x20) && (c <= 0x7F)) 
  {
    c = c - 32U;
    LCD_A0 = 1;
    Sync_Send_Array(TEXT_REVERSE_16x12, font12x16[2*c], 12);
    LCD_gotoxy(x, y + 1); // go to next page
    LCD_A0 = 1;
    Sync_Send_Array(TEXT_REVERSE_16x12, font12x16[2*c + 1], 12);
  }
}

void LCD_puts12x16(uint8_t x, uint8_t y, uint8_t * str) 
{
  while (( * str > 0x20) && ( * str < 0x3b)) 
  {
    LCD_putc12x16(x, y, * str++); //x=100 > LCD_WIDTH, in order to put to current cursor
    x += 12;
  }
}

void LCD_puts12x16_reverse(uint8_t x, uint8_t y, uint8_t * str) 
{
  while (( * str > 0x20) && ( * str < 0x3b)) {
    LCD_putc12x16_reverse(x, y, * str++); //x=100 > LCD_WIDTH, in order to put to current cursor
    x += 12;
  }
}

void LCD_put12x16Number(uint8_t x, uint8_t y, uint32_t num) 
{
  char str[10] = "";
  sprintf_vft(str, num);
  LCD_puts12x16(x, y, str);
}

void LCD_put12x16Number_reverse(uint8_t x, uint8_t y, uint32_t num) 
{
  char str[10] = "";
  sprintf_vft(str, num);
  LCD_puts12x16_reverse(x, y, str);
}

void LCD_putc_reverse_with_footer(uint8_t c) 
{
  LCD_putc_with_style(TEXT_REVERSE_FOOTER, c);
}

void LCD_puts_reverse_with_footer(uint8_t * str) 
{
  while ( * str)
    LCD_putc_reverse_with_footer( * str++);
}

void LCD_putIcon(uint8_t x, uint8_t y, uint8_t icon)    // tanhhc:   Sau nay, Ham nay se doi ve dung Sync_Send_Array thay cho LCD_write
{
  uint8_t i, temp;
  LCD_gotoxy(x, y);
  if (icon < NUMBER_OF_24X16_ICON) 
  {
    for (i = 0; i < 24; i++)
      LCD_write(1, icons24x16[2 * icon][i]);
    LCD_gotoxy(x, y + 1); // go to next page
    for (i = 0; i < 24; i++)
      LCD_write(1, icons24x16[2 * icon + 1][i]);
    return;
  }
  if (icon < (NUMBER_OF_24X16_ICON + NUMBER_OF_8X8_ICON)) 
  {
    for (i = 0; i < 8; i++)
      LCD_write(1, icons8x8[icon - NUMBER_OF_24X16_ICON][i]);
    return;
  }
  if (icon < (NUMBER_OF_24X16_ICON + NUMBER_OF_8X8_ICON + NUMBER_OF_16X8_ICON)) 
  {
    for (i = 0; i < 16; i++)
      LCD_write(1, icons16x8[icon - (NUMBER_OF_24X16_ICON + NUMBER_OF_8X8_ICON)][i]);
    return;
  }
  if (icon < (NUMBER_OF_24X16_ICON + NUMBER_OF_8X8_ICON + NUMBER_OF_16X8_ICON + NUMBER_OF_20X8_ICON)) 
  {
    for (i = 0; i < 20; i++)
      LCD_write(1, icons20x8[icon - (NUMBER_OF_24X16_ICON + NUMBER_OF_8X8_ICON + NUMBER_OF_16X8_ICON)][i]);
    return;
  }
  if (icon < (NUMBER_OF_24X16_ICON + NUMBER_OF_8X8_ICON + NUMBER_OF_16X8_ICON + NUMBER_OF_20X8_ICON + NUMBER_OF_32X32_ICON)) 
  {
    //				for(unsigned char j=0; j<4; j++){
    //						for(i=0; i<32; i++)
    //						LCD_write(1, icons32x32[icon-
    //												(NUMBER_OF_24X16_ICON+NUMBER_OF_8X8_ICON+NUMBER_OF_16X8_ICON+NUMBER_OF_20X8_ICON)][32*j + i]);
    //						LCD_gotoxy(x, y+j+1);
    //				}
    uint8_t j;
    for (i = 0; i < 32; i++)
      for (j = 0; j < 4; j++) {
        LCD_gotoxy(x + i, y + j);
        temp = icons32x32[icon -
          (NUMBER_OF_24X16_ICON + NUMBER_OF_8X8_ICON + NUMBER_OF_16X8_ICON + NUMBER_OF_20X8_ICON)][i * 4 + j];
        //temp = ((temp<<4)&0xF0)|((temp>>4)&0x0F);
        temp = swap_byte(temp);
        LCD_write(1, temp);

      }
    return;
  }
}
