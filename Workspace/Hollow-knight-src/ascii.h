#ifndef	ASCII_H
#define	ASCII_H

// Ascii


void ascii_ctrl_bit_set (unsigned char x);
void ascii_ctrl_bit_clear (unsigned char x);
void ascii_write_cmd(unsigned char command);
void ascii_write_data(unsigned char data);
unsigned char ascii_read_status(void);
unsigned char ascii_read_data(void);
void ascii_write_controller(unsigned char byte);
unsigned char ascii_read_controller(void);
void ascii_init(void);
void app_init(void);
void ascii_write_string(char *str);
void intToString(int n, char *str);

#endif