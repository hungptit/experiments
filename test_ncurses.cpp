#include <ncurses.h>
#include <string>

namespace console {
    constexpr char BACKSPACE = 127;
    constexpr char CTRL_D = 4;
    constexpr char ENTER = 10;
} // namespace console

int main() {
    initscr(); /* Start curses mode 		  */
    refresh(); /* Print it on to the real screen */
    std::string buffer;
    bool isok = true;
    while (isok) {
        noecho();				// Do not echo.
        char key = getch();
        mvprintw(1, 1, "Enter: %d\n", key);
        switch (key) {
        case console::BACKSPACE:
            if (!buffer.empty()) {
                buffer.pop_back();
            }
            break;
        case console::ENTER:
            break;
        case console::CTRL_D:
            isok = false;
            break;
        default:
            buffer.push_back(key);
        }
		mvprintw(2, 1, "%s", buffer.data()); /* Print Hello World		  */
    }
    endwin(); /* End curses mode		  */

    return 0;
}
