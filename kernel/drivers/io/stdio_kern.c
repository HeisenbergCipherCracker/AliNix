#include <alinix/stdio.h>
#include <alinix/kernel.h>

#include <alinix/string.h>
vga_entry_t* terminal = (vga_entry_t*)0xB8000;
uint8_t color = 0;
uint8_t background = 0;
uint8_t foreground = 0;
uint8_t x, y;

//store history and future for scrolling support
vga_entry_t* history;
uint32_t history_index = 0;

vga_entry_t* future;
uint32_t future_index = 0;
void terminal_scroll(){
	//don't allow scroll if the buffer can't hold any more data
	if(history_index + 80 >= 32000){
		return;
	}

	for(int i = 0; i < 80; i++){
		history[history_index] = terminal[i];
		history_index++;
		terminal[i] = terminal[i + 80];
	}

	for(uint16_t i = 80; i < 80*24; i++){
		terminal[i] = terminal[i + 80];
	}

	//fill bottom row with whitespace if future doesn't contain enough data
	if(future_index < 80) {
		for(uint16_t i = 80*24; i < 80*25; i++){
			terminal[i] = terminal_make_vga_entry(' ');
		}
		y--;
		x = 0;
	}else {
		for(uint16_t i = 80*25 - 1; i >= 80*24; i--){
			future_index--;
			terminal[i] = future[future_index];
		}
	}
	if(future_index < 80) {
		terminal_set_cursor_position(x, y);
	}
}


void terminal_writeline(char* string){
	terminal_reset_scroll();
	uint16_t length = strlen(string);
	for(uint16_t i = 0; i < length; i++){
		terminal_write_next_entry(terminal_make_vga_entry(string[i]));
	}
}

vga_entry_t terminal_make_vga_entry(char c){
	vga_entry_t ret;
	ret.character = c;
	ret.color = color;
	return ret;
}

// int fprintf(FILE * file, const char *format, ...)
// {
// 	va_list args;
// 	int length;

// 	va_start(args, format);
// 	length = vfprintf(file, format, args);
// 	va_end(args);

// 	return (length);
// }