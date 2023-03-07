#define GPIO_D 0x40020C00
	#define GPIO_D_MODER ((volatile unsigned int *)(GPIO_D))
	#define GPIO_D_OTYPER ((volatile unsigned short *)(GPIO_D + 0x4))
	#define GPIO_D_SPEEDR ((volatile unsigned int *)(GPIO_D + 0x8))
	#define GPIO_D_PUPDR ((volatile unsigned int *)(GPIO_D + 0xC))
	#define GPIO_D_IDR_LOW ((volatile unsigned char *)(GPIO_D + 0x10))
	#define GPIO_D_IDR_HIGH ((volatile unsigned char *)(GPIO_D + 0x11))
	#define GPIO_D_ODR_LOW ((volatile unsigned char *)(GPIO_D + 0x14))
	#define GPIO_D_ODR_HIGH ((volatile unsigned char *)(GPIO_D + 0x15))
	
	
// ---- Keyb ----
unsigned char keyb(void) {
    char key[] = {1,2,3,0xA,4,5,6,0xB,7,8,9,0xC,0xE,0,0xF,0xD};
    for (int i = 1; i < 5; i++) {
        activateRow(i);
        int column = readColumn();
        if (column != 0) {
            activateRow(0);
            return key[4*(i - 1) + (column - 1)];
        }
        
        }
		
            activateRow(0);
            return 0xFF;
    
}
void activateRow(int row) {
    switch (row) {
        case 1:
            *GPIO_D_ODR_HIGH = 0x10;
            break;
            
        case 2:
            *GPIO_D_ODR_HIGH = 0x20;
            break;
            
        case 3:
            *GPIO_D_ODR_HIGH = 0x40;
            break;
            
        case 4:
            *GPIO_D_ODR_HIGH = 0x80;
            break;
            
        default: 
            *GPIO_D_ODR_HIGH = 0;
    }
}

int readColumn(void) {
    char c = *GPIO_D_IDR_HIGH;
    if (c & 8) {
		return 4;
	}
	if (c & 4) {
		return 3;
	}
	if (c & 2) {
		return 2;
	}
	if (c & 1) {
		return 1;
	}
	return 0;


}
