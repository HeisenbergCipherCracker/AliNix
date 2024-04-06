#ifndef __HEISEN_KERNEL_HUMAN_USB_INTERFACE
#define __HEISEN_KERNEL_HUMAN_USB_INTERFACE

#include <types.h>
#include <keys.h>
#ifdef __cplusplus
using namespace LIBHeisenKernel;
#endif

#define USB_DEVICE_KEYBOARD 0x030101
#define USB_DEVICE_MOUSE 0x030102

#define USB_KEYBOARD_ZERO_10 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

dword_t usb_keyboard_layout[256] = {
0, 0, 0, 0,
KEY_A,  KEY_B,  KEY_C,  KEY_D,  KEY_E,  KEY_F,  KEY_G,  KEY_H,  KEY_I,  KEY_J,  KEY_K,  KEY_L,  KEY_M,  KEY_N,  KEY_O,  KEY_P,  KEY_Q,  KEY_R,  KEY_S,  KEY_T,  KEY_U,  KEY_V,  KEY_W,  KEY_X,  KEY_Y,  KEY_Z,
KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0,
KEY_ENTER, KEY_ESC, KEY_BACKSPACE, KEY_TAB, KEY_SPACE,
KEY_DASH, KEY_EQUAL, KEY_LEFT_BRACKET, KEY_RIGHT_BRACKET, KEY_BACKSLASH, 0, KEY_SEMICOLON, KEY_SINGLE_QUOTE, KEY_BACK_TICK, KEY_COMMA, KEY_DOT, KEY_FORWARD_SLASH,
KEY_CAPSLOCK, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,
KEY_PRINT_SCREEN, 0, 0, 0, KEY_HOME, KEY_PAGE_UP, KEY_DELETE, KEY_END, KEY_PAGE_DOWN, KEY_RIGHT, KEY_LEFT, KEY_DOWN, KEY_UP,
USB_KEYBOARD_ZERO_10, USB_KEYBOARD_ZERO_10, USB_KEYBOARD_ZERO_10, USB_KEYBOARD_ZERO_10, USB_KEYBOARD_ZERO_10, USB_KEYBOARD_ZERO_10, USB_KEYBOARD_ZERO_10, USB_KEYBOARD_ZERO_10, USB_KEYBOARD_ZERO_10, USB_KEYBOARD_ZERO_10, USB_KEYBOARD_ZERO_10, USB_KEYBOARD_ZERO_10, USB_KEYBOARD_ZERO_10, USB_KEYBOARD_ZERO_10, 0, 0, //142 zero bytes
KEY_LEFT_SHIFT, 0, 0, 0, KEY_RIGHT_SHIFT, 0, 0
};

struct usb_mouse_informations {
 byte_t controller_type;
 byte_t controller_number;
 byte_t port;
 byte_t address;
 byte_t device_speed;
 byte_t endpoint;
 byte_t endpoint_size;
 byte_t interrupt_time;
 dword_t transfer_descriptor_check;
}__attribute__((packed));
struct usb_mouse_informations usb_mouse[1];

struct usb_keyboard_informations {
 byte_t controller_type;
 byte_t controller_number;
 byte_t port;
 byte_t address;
 byte_t device_speed;
 byte_t endpoint;
 byte_t endpoint_size;
 byte_t interrupt_time;
 dword_t transfer_descriptor_check;
}__attribute__((packed));
struct usb_keyboard_informations usb_keyboard[1];

dword_t usb_mouse_data_memory = 0, usb_mouse_packet_received = 0;
dword_t usb_keyboard_data_memory = 0, usb_keyboard_packet_received = 0;
dword_t usb_keyboard_count = 0, usb_keyboard_code_of_pressed_key = 0;
byte_t usb_keyboard_code = 0;

dword_t usb_mouse_buttons_data_offset_byte = 0;
dword_t usb_mouse_buttons_data_offset_shift = 0;
dword_t usb_mouse_movement_x_data_offset_byte = 0;
dword_t usb_mouse_movement_x_data_offset_shift = 0;
dword_t usb_mouse_movement_x_data_length = 0;
dword_t usb_mouse_movement_y_data_offset_byte = 0;
dword_t usb_mouse_movement_y_data_offset_shift = 0;
dword_t usb_mouse_movement_y_data_length = 0;
dword_t usb_mouse_movement_wheel_data_offset_byte = 0;
dword_t usb_mouse_movement_wheel_data_offset_shift = 0;
dword_t usb_mouse_movement_wheel_data_length = 0;

void usb_mouse_read_hid_descriptor(byte_t controller_number, byte_t device_address, byte_t device_speed, byte_t interface, word_t length);
void usb_hid_device_set_protocol(byte_t controller_number, byte_t device_address, byte_t device_speed, byte_t interface, byte_t protocol);
void usb_hid_device_set_idle(byte_t controller_number, byte_t device_address, byte_t device_speed, byte_t interface);
void usb_keyboard_process_new_packet(void);
void usb_keyboard_process_no_new_packet(void);
void usb_mouse_process_received_data(void);


#endif // __HEISEN_KERNEL_HUMAN_USB_INTERFACE