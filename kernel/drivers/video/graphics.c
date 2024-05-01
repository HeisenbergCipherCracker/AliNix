#include <alinix/paging.h>
#include <alinix/mm.h>
#include <alinix/kernel.h>


void paint_desktop() {
    draw_rect(0, 0, 1024, 768, 0xCE2C2C);
    paint_windows();
    paint_mouse();
}

void paint_mouse() {
    draw_rect(get_mouse_info()->x, get_mouse_info()->y, 10, 10, 0xFFFFFF);
}