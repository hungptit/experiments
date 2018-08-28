#include <ncurses.h>
#include <string>

int main() {
    initscr(); /* Start curses mode 		  */
    refresh(); /* Print it on to the real screen */
    std::string buffer;
    while (1) {
        char key = getch();
        if (key == 4)
            break;
        buffer.push_back(key);
        printw("%s", buffer.data()); /* Print Hello World		  */
    }
    endwin(); /* End curses mode		  */

    return 0;
}
