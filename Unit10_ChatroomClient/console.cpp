#include <string>
#include <vector>
#include <curses.h>

int main() {
	std::vector<std::string> list = { "General", "Sports", "Programming", "Music", "Random" };
	int size = list.size();
	int height, width;
	initscr();
	cbreak();
	keypad(stdscr, true);
	curs_set(0);

	getmaxyx(stdscr, height, width);
	WINDOW* main_win, * subject_win, * input_win, * chat_win;

	main_win = newwin(height - 1, width - 1, 1, 1);

	//horizontal line below title
	mvwhline(main_win, 3, 1, ACS_HLINE, width - 2);
	//vertical line between subject and chat
	mvwvline(main_win, 4, 3 * (width / 4), ACS_VLINE, height - 10);
	mvwprintw(main_win, 2, (width / 2) - 8, " Chat Room Client ");

	//chat window
	chat_win = derwin(main_win, height - 10, (3 * (width / 4)) - 1, 4, 1);
	scrollok(chat_win, true);
	mvwprintw(chat_win, 1, 1, "Welcome to the Chat Room!\n");
	mvwprintw(chat_win, 2, 1, "This is a placeholder message\n");
	mvwprintw(chat_win, 3, 1, "More messages will appear here...\n");
	//subject window
	subject_win = derwin(main_win, height - 10, (width / 4) - 3, 4, (3 * (width / 4)) + 1);
	//input window
	input_win = derwin(main_win, 4, width - 3, getmaxy(chat_win) + 4, 1);

	//populate subject window with list of subjects
	for (int i = 0; i < size; ++i) {
		mvwprintw(subject_win, 1 + i, 2, list[i].c_str());
	}

	mvwprintw(input_win, 1, 1, "> ");
	//turning windows to box to give a frame
	box(subject_win, 0, 0);
	box(chat_win, 0, 0);
	box(input_win, 0, 0);
	box(main_win, 0, 0);
	//refreshing all windows
	refresh();
	wrefresh(subject_win);
	wrefresh(chat_win);
	wrefresh(input_win);
	wrefresh(main_win);

	bool repeat = true;
	int input_length = 0;
	//selector for subject window
	bool selecting = false;
	const int buffer_size = 250;
	char ch[buffer_size];
	memset(ch, 0, sizeof(ch));
	int pos = 3;
	int selector = 0;
	noecho();
	wmove(input_win, 1, pos);
	keypad(input_win, true);
	while (repeat) { //loops
		int c = wgetch(input_win);

		switch (c) {
		case 27: {
			repeat = false;
			break;
		}
		case KEY_RIGHT: { // to go to subject window
			if (!selecting) {
				selecting = true;
			}
			for (int i = 0; i < size; ++i) {
				//highlight first value of list by default
				if (i == 0) wattron(subject_win, A_REVERSE);
				mvwprintw(subject_win, 1 + i, 2, list[i].c_str());
				if (i == 0) wattroff(subject_win, A_REVERSE);
			}
			wrefresh(subject_win);
			wrefresh(input_win);
			break;
		}
		case KEY_LEFT: {
			if (selecting) selecting = false;
			for (int i = 0; i < size; i++) {
				mvwprintw(subject_win, 1 + i, 2, list[i].c_str());
			}
			wclear(input_win);
			mvwprintw(input_win, 1, 1, ">");
			box(input_win, 0, 0);
			wmove(input_win, 1, pos);
			echo();
			wrefresh(input_win);
			wrefresh(subject_win);
			break;
		}
		case KEY_UP: {
			if (!selecting) break;
			selector--;
			
			if (selector < 0) selector = size - 1;

			for (int i = 0; i < size; i++) {
				//turn on list list[i] highlight
				if (i == selector) wattron(subject_win, A_REVERSE);
				//print list
				mvwprintw(subject_win, 1 + i, 2, list[i].c_str());
				//turn off highlight
				if (i == selector) wattroff(subject_win, A_REVERSE);
			}
			wrefresh(subject_win);
			break;
		}
		case KEY_DOWN: {
			if (!selecting) break;
			selector++;
			if (selector > size-1) selector = 0;
			for (int i = 0; i < size; i++) {
				//turn on list list[i] highlight
				if (i == selector) wattron(subject_win, A_REVERSE);
				//print list
				mvwprintw(subject_win, 1 + i, 2, list[i].c_str());
				//turn off highlight
				if (i == selector) wattroff(subject_win, A_REVERSE);
			}
			wrefresh(subject_win);
			break;
		}
		case KEY_ENTER: 
		case 10: {
			if (selecting) {
				for (int i = 0; i < size; i++) {
					mvwprintw(subject_win, 1 + i, 2, list[i].c_str());
				}
				wclear(input_win);
				wmove(chat_win, getcury(chat_win), 1);
				wprintw(chat_win, "USER SELECTED: %s\n", list[selector].c_str());
				box(chat_win, 0, 0);
				selector = 0;
				wmove(input_win, 1, pos);
				box(input_win, 0, 0);
				mvwprintw(input_win, 1, 1, "> ");
				pos = 3;
				echo();
				wrefresh(subject_win);
				wrefresh(input_win);
				wrefresh(chat_win);
				selecting = false;
				break;
			}
			else {
				ch[input_length] = '\0';
				wmove(chat_win, getcury(chat_win), 1);
				wprintw(chat_win, "USER: %s\n", ch);
				box(chat_win, 0, 0);
				memset(ch, 0, sizeof(ch));
				ch[input_length] = '\0';
				werase(input_win);
				box(input_win, 0, 0);
				mvwprintw(input_win, 1, 1, "> ");
				input_length = 0;
				pos = 3;
				wrefresh(chat_win);
				wrefresh(input_win);
				break;
			}
			
		}
		case 8:
		case KEY_BACKSPACE:
		{
			if (pos > 3) {
				pos--;
				ch[input_length] = '\0';	
				input_length--;
				mvwaddch(input_win, 1, pos, ' ');
				box(input_win, 0, 0);
				wmove(input_win, 1, pos);
				wrefresh(input_win);
			}
			break;
		}
		default: {
			if (selecting) break;
			
			if (pos <= width-5 && c >= 32 && c <= 126) {
				if (input_length < buffer_size - 1) {
					ch[input_length++] = (char)c;
					mvwaddch(input_win, 1, pos, c);
					pos++;
				}
				break;
			}
		}
		}
		wrefresh(chat_win);
		wrefresh(input_win);
	}
	//deleting windows
	delwin(main_win);
	delwin(chat_win);
	delwin(subject_win);
	delwin(input_win);
	//end window
	endwin();
	return 0;
}
