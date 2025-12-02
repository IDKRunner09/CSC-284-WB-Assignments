#include <curses.h>
#include <vector>
#include <string>
#include <cstring>

int main() {
    std::vector<std::string> classTypes = { "General", "Sports", "Programming", "Music", "Random" };

    initscr();
    curs_set(0);
    cbreak();
    noecho();
    keypad(stdscr, true);

    // ================= MAIN WINDOW =================
    int height, width;
    getmaxyx(stdscr, height, width);

    WINDOW* mainWin = newwin(height, width, 0, 0);
    box(mainWin, 0, 0);
    mvwprintw(mainWin, 2, width / 2 - 12, "Advanced C++ Chat Client");
    mvwhline(mainWin, 3, 1, ACS_HLINE, width - 2);
    mvwvline(mainWin, 4, width - (width / 4) + 1, ACS_VLINE, height - 11);
    wrefresh(mainWin);

    // ================= CHAT WINDOW =================
    int chatH = height - 11, chatW = width - (width / 4);
    int chatStartY = 4, chatStartX = 1;

    WINDOW* chatWin = newwin(chatH, chatW, chatStartY, chatStartX);
    box(chatWin, 0, 0);
    mvwprintw(chatWin, 1, 1, "Welcome to chat:");
    scrollok(chatWin, TRUE);
    idlok(chatWin, TRUE);
    wsetscrreg(chatWin, 1, chatH - 2);
    wrefresh(chatWin);

    // ================= LIST WINDOW =================
    int listH = chatH, listW = width - chatW - 3;
    int listStartY = chatStartY, listStartX = chatW + 2;

    WINDOW* listWin = newwin(listH, listW, listStartY, listStartX);
    box(listWin, 0, 0);
    for (int i = 0; i < classTypes.size(); i++) {
        mvwprintw(listWin, i + 1, 2, "%s", classTypes[i].c_str());
    }
    keypad(listWin, true);
    wrefresh(listWin);

    // ================= INPUT WINDOW =================
    int inputH = height - chatH - 5, inputW = width - 2;
    int inputStartY = chatH + 4, inputStartX = chatStartX;

    WINDOW* inputBarWin = newwin(inputH, inputW, inputStartY, inputStartX);
    box(inputBarWin, 0, 0);
    mvwprintw(inputBarWin, 2, 1, "> ");
    keypad(inputBarWin, true);
    wrefresh(inputBarWin);

    // ================= STATE =================
    bool running = true;
    bool inputMode = true;
    int selectedItem = 0;

    int pos = 2;
    char input_buffer[250]{};

    // ================= MAIN LOOP =================
    while (running) {
        // ACTIVE WINDOW INPUT
        WINDOW* activeWin = inputMode ? inputBarWin : listWin;
        int ch = wgetch(activeWin);

        if (ch == KEY_RIGHT) inputMode = false;
        if (ch == KEY_LEFT)  inputMode = true;

        // ================= INPUT MODE =================
        if (inputMode) {
            switch (ch) {
            case 10: // enter
                input_buffer[pos - 2] = '\0';

                wmove(chatWin, getcury(chatWin) + 1, 1);
                wprintw(chatWin, "USER: %s", input_buffer);

                box(chatWin, 0, 0);
                wrefresh(chatWin);

                pos = 2;
                memset(input_buffer, 0, sizeof(input_buffer));

                werase(inputBarWin);
                box(inputBarWin, 0, 0);
                mvwprintw(inputBarWin, 2, 1, "> ");
                wrefresh(inputBarWin);
                break;

            case 27: // ESC
                running = false;
                break;

            case KEY_BACKSPACE:
            case 127:
                if (pos > 2) {
                    pos--;
                    input_buffer[pos - 2] = '\0';
                    mvwaddch(inputBarWin, 2, pos, ' ');
                    wmove(inputBarWin, 2, pos);
                    wrefresh(inputBarWin);
                }
                break;

            default:
                if (pos < 248 && ch >= 32 && ch <= 126) {
                    input_buffer[pos - 2] = ch;
                    mvwaddch(inputBarWin, 2, pos, ch);
                    wrefresh(inputBarWin);
                    pos++;
                }
                break;
            }
        }

		//selecting list item
        else {
            switch (ch) {
            case KEY_UP:
                selectedItem--;
                if (selectedItem < 0)
                    selectedItem = classTypes.size() - 1;
                break;

            case KEY_DOWN:
                selectedItem++;
                if (selectedItem >= classTypes.size())
                    selectedItem = 0;
                break;

            
            case 10: //enter
                wmove(chatWin, getcury(chatWin) + 1, 1);
                wprintw(chatWin, "USER SELECTED: %s", classTypes[selectedItem].c_str());
                box(chatWin, 0, 0);
                wrefresh(chatWin);
                inputMode = true;
                break;
            }

            werase(listWin);
            box(listWin, 0, 0);
            for (int i = 0; i < classTypes.size(); i++) {
                if (i == selectedItem)
                    wattron(listWin, A_REVERSE);

                mvwprintw(listWin, i + 1, 2, "%s", classTypes[i].c_str());

                if (i == selectedItem)
                    wattroff(listWin, A_REVERSE);
            }
            wrefresh(listWin);
        }
    }
	delwin(chatWin);
	delwin(listWin);
	delwin(inputBarWin);
    delwin(mainWin);  
    printf("Exiting chat client...");
    endwin();
    return 0;
}
