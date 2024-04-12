#include <new.h>
#include <log.h>
#include <gui/widgets/button.h>
#include <gui/widgets/window.h>
#include <gui/directgui.h>
#include <random.h>
#include <datetime.h>
#include <time.h>
#include <convert.h>
#include <string.h>
#include <gui/widgets/label.h>

#include "welcome.h"

using namespace LIBHeisenKernel;

const int fieldWidth = 10;
const int fieldHeight = 10;
const int numOfMines = 16;


static const char multi_line_string[] = 
"Heisen Os , licensed under MIT license, written by Heisenberg and intended to provide utility for personal use\n"
"\nIt is recommended to reboot your system after installing Os.\n"
"\nOr you can just continue using it.\n";



/**
 * @brief Static functions that will be implemented on the help GUI menu
 * @todo call SYSCALL_EXIT (important! otherwise you'll have a memory leak)
*/


int main(){
    GUI::SetDefaultFont();

    Window* mainWindow = new Window(600, 400, GUI::Width/2 - 300, GUI::Width/2 - 200);
    mainWindow->titleString = "Welcome to HeisenOs!";

    Control* control = new Control(600, 400, 0, 0);
    control->backColor = 0xFFBBBBBB;

    Button* helpMoreOptions = new Button("More Options");
    helpMoreOptions->width = 150 - 2;
    helpMoreOptions->height = 47;
    helpMoreOptions->x = 1;
    helpMoreOptions->y = 2;
    // Implement shutdown and reboot button here.
    Button* shutdownAndReboot = new Button("Reboot and shutdown (recommended)");
    shutdownAndReboot->width = 300 - 2;
    shutdownAndReboot->height = 49;
    shutdownAndReboot->y = 50;
    shutdownAndReboot->x = 1;
    shutdownAndReboot->MouseClick += rebootSystemAndSayGoodBye;

    Button* exit = new Button("Exit");
    exit->width = 150 - 2;
    exit->height = 47;
    exit->x = 1;
    exit->y = 100;
    exit->MouseClick += exitButtonCallback;

    Button* moreOptions = new Button("More Options");
    moreOptions->width = 150 - 2;
    moreOptions->height = 47;
    moreOptions->x = 1;
    moreOptions->y = 150;
    moreOptions->MouseClick += moreTipsHelpMenu;
    // helpMoreOptions->MouseClick += HelpMoreOptionsCallback;
}

static void exitButtonCallback(void *sender,MouseButtonArgs args){
    /**
     * @brief This should be the sys exit call after user click on the close button
    */
    DoSyscall(SYSCALL_EXIT);
}

static void rebootSystemAndSayGoodBye(void *sender,MouseButtonArgs args){
    /**
     * @brief This should be the sys reboot call after user click on the close button
    */
    DoSyscall(SYSCALL_REBOOT);
}


static void moreTipsHelpMenu(void *sender,MouseButtonArgs args){
    /**
     * @brief This should be the sys reboot call after user click on the close button
    */
    DoSyscall(SYSCALL_EXIT);
}