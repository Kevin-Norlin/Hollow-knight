// Drivrutiner för grafisk display
void graphic_initialize(void);
void graphic_clear_screen(void);
void graphic_pixel_set(int x, int y);
void graphic_pixel_clear(int x, int y);

// ---- DISPLAY ----

__attribute__((naked))
void graphic_initialize(void) {
	__asm volatile (" .HWORD 0xDFF0\n");
	__asm volatile (" BX LR\n");
}

__attribute__((naked))
void graphic_clear_screen(void) {
	__asm volatile (" .HWORD 0xDFF1\n");
	__asm volatile (" BX LR\n");
}

__attribute__((naked))
void graphic_pixel_set(int x, int y) {
	__asm volatile (" .HWORD 0xDFF2\n");
	__asm volatile (" BX LR\n");
}
__attribute__((naked))
void graphic_pixel_clear(int x, int y) {
	__asm volatile (" .HWORD 0xDFF3\n");
	__asm volatile (" BX LR\n");
}
