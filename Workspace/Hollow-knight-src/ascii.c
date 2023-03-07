
#define GPIO_E 0x40021000
	#define GPIO_E_MODER ((volatile unsigned int *)(GPIO_E))
	#define GPIO_E_OTYPER ((volatile unsigned short *)(GPIO_E + 0x4))
	#define GPIO_E_SPEEDR ((volatile unsigned int *)(GPIO_E + 0x8))
	#define GPIO_E_PUPDR ((volatile unsigned int *)(GPIO_E + 0xC))
	#define GPIO_E_IDR_LOW ((volatile unsigned char *)(GPIO_E + 0x10))
	#define GPIO_E_IDR_HIGH ((volatile unsigned char *)(GPIO_E + 0x11))
	#define GPIO_E_ODR_LOW ((volatile unsigned char *)(GPIO_E + 0x14))
	#define GPIO_E_ODR_HIGH ((volatile unsigned char *)(GPIO_E + 0x15))	
	
// Bitar i styrregistret ASCII
	#define	B_E			0x40
	#define B_SELECT 	4
	#define	B_RW		2
	#define	B_RS		1
// ---- Ascii ----
void ascii_ctrl_bit_set (unsigned char x) {
	*GPIO_E_ODR_LOW |= x;
	*GPIO_E_ODR_LOW |= B_SELECT;
}
void ascii_ctrl_bit_clear (unsigned char x) {
	*GPIO_E_ODR_LOW &= ~x;
	*GPIO_E_ODR_LOW |= B_SELECT;
	
}

void ascii_write_cmd(unsigned char command) {
	ascii_ctrl_bit_clear(B_RS);
	ascii_ctrl_bit_clear(B_RW);
	
	ascii_write_controller(command);
	
}
void ascii_write_data(unsigned char data) {
	ascii_ctrl_bit_set(B_RS);
	ascii_ctrl_bit_clear(B_RW);
	
	ascii_write_controller(data);
	
}
	
void ascii_write_controller(unsigned char byte) {
	//delay_delay250ns
	ascii_ctrl_bit_set(B_E);
	*GPIO_E_ODR_HIGH = byte;
	delay_250ns();
	ascii_ctrl_bit_clear(B_E);
	
	
}

unsigned char ascii_read_controller(void) {
	*GPIO_E_MODER = 0x00005555;
	ascii_ctrl_bit_set(B_E);
	//delay_250ns();
	//delay_250ns();
	unsigned char rw = *GPIO_E_IDR_HIGH;
	ascii_ctrl_bit_clear(B_E);
	*GPIO_E_MODER = 0x55555555;
	return rw;
	
	
}

unsigned char ascii_read_status(void) {
	*GPIO_E_MODER = 0x00005555;
	ascii_ctrl_bit_clear(B_RS);
	ascii_ctrl_bit_set(B_RW);
	unsigned char rw = ascii_read_controller();
	*GPIO_E_MODER = 0x55555555;
	return rw;
}

unsigned char ascii_read_data(void) {
	*GPIO_E_MODER &= 0x0000FFFF;
	ascii_ctrl_bit_set(B_RS);
	ascii_ctrl_bit_set(B_RW);
	unsigned char rw = ascii_read_controller();
	*GPIO_E_MODER |= 0x55550000;
	return rw;
}


void ascii_init(void) {
	ascii_ctrl_bit_clear(B_RS);
	ascii_ctrl_bit_clear(B_RW);
	
	ascii_write_cmd(0b00111000); // 2 rader, 5 x 8.
	
	ascii_write_cmd(0b00001110); // Display på, markör på, blinkande markör av.
	
	ascii_write_cmd(0b00000110); // Increment, skift av.
	
}



void ascii_gotoxy(int row, int col) {
	int address = row -1;
	if (col == 2) {
		address = address + 0x40;
	}
	ascii_write_cmd(0x80 | address);
}

void ascii_write_char(unsigned char c) {
	ascii_ctrl_bit_clear(B_RS);
	ascii_ctrl_bit_set(B_RW);
	while ((ascii_read_status() & 0x80) == 0x80) {
		
	}
	delay_mikro(4);
	ascii_write_data(c);
	delay_mikro(1);
	
}

void ascii_clear_display( void ) {
	while ((ascii_read_status() & 0x80) == 0x80) {
		
	}
	delay_mikro(4);
	ascii_write_cmd(1);
	delay_mikro(1);
}

void intToString(int n, char *str) {
	int temp = n;
	int digits = 0;
	while (temp) {
		temp /= 10;
		digits++;
	}
	
	for (int i = digits -1; i >= 0; i--) {
		str[i] = '0' + (n % 10);
		n /= 10;
	}
	str[digits] = '\0';
	
}

void ascii_write_string(char *str) {
	while (*str) {
		ascii_write_char(*str++);
	}
}
