#include <algorithm>
#include <ncurses.h>
#include <string>
#include <vector>

namespace console {
    constexpr int BACKSPACE = 127;
    constexpr int CTRL_D = 4;
    constexpr int ENTER = 10;
    constexpr int ESC = 27;
    constexpr int UP = 661;
    constexpr int DOWN = 651;

    class Screen {
      public:
        Screen() {
            initscr();
            refresh();
            noecho();
            nrows = LINES;
            ncols = COLS;
            number_of_output_rows = nrows - 3;
            separator = std::string(ncols, '-');
            init_data();
            update_data();
            print_input();
            print_output();
        }
        
        ~Screen() { endwin(); }

        void run() {
            bool isok = true;
            while (isok) {
                int key = getch();
                switch (key) {
                case console::BACKSPACE:
                    if (!pattern.empty()) {
                        pattern.pop_back();
                    }
                    break;
                case ENTER:
                    break;
                case ESC:
                    isok = false;
                    break;
                case UP:
                    break;
                case DOWN:
                    break;
                case CTRL_D:
                    isok = false;
                    break;
                default:
                    pattern.push_back(key);
                }

                update_data();
                print_input();
                print_output();
            }
        }

      private:
        void init_data() {
            for (auto idx = 0; idx < number_of_output_rows; ++idx) {
                original_data.push_back(std::to_string(idx).data());
            }            
        }
        
        void update_data() {
            data.clear();
            if (pattern.empty()) {
                std::copy(original_data.cbegin(), original_data.cend(), data.begin());
            } else {
                for (auto const &line : original_data) {
                    if (line.find(pattern) != std::string::npos) {
                        data.push_back(line);
                    }
                }                
            }

        }
        
        void print_output() {
            int idx = number_of_output_rows;
            for (auto const &aline : data) {
                mvprintw(idx, 0, "%s", aline.data());
                --idx;
            }
            move(nrows - 1, pattern.size() + 2);
        }
        
        void print_input() {
            std::string row(ncols, ' ');
            mvprintw(nrows - 2, 0, "%s", separator.data());
            for (size_t idx = 0; idx < pattern.size(); ++idx) {
                row[idx] = pattern[idx];
            }
            mvprintw(nrows - 1, 0, "> %s", row.data());
        }

      private:
        int nrows;
        int ncols;
        int number_of_output_rows;
        std::string pattern;
        std::string separator;
        std::vector<std::string> original_data;
        std::vector<std::string> data;
    };

} // namespace console

int main() {
    console::Screen scr;
    scr.run();
    return 0;
}
