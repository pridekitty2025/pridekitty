/*
 * Developed by:            K. Lange
 *                          http://github.com/klange/nyancat
 *
 * Edited by:               C. Mion
 *                          https://github.com/ChrisMion2022/Pridekitty
 *
 * 40-column support by:    Peter Hazenberg
 *                          http://github.com/Peetz0r/pridekitty
 *                          http://peter.haas-en-berg.nl
 *
 * Build tools unified by:  Aaron Peschel
 *                          https://github.com/apeschel
 *
 * This is a standalone terminal application which renders the
 * classic Nyan Cat animation to your terminal.
 *
 * For more information, please see:
 *     https://github.com/ChrisMion2022/Pridekitty
 */

#define _XOPEN_SOURCE 700
#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/ioctl.h>
#include <termios.h>

/*
 * The animation frames are stored separately in
 * this header so they don't clutter the core source
 */
#include "animation.c"

const char * colors[256] = {NULL};
const char * output = "  ";
int show_counter = 1;
unsigned int frame_count = 0;
int clear_screen = 1;
int set_title = 1;

int min_row = -1;
int max_row = -1;
int min_col = -1;
int max_col = -1;

int terminal_width = 80;
int terminal_height = 24;
char using_automatic_width = 0;
char using_automatic_height = 0;

void finish() {
    if (clear_screen) {
        printf("\033[?25h\033[0m\033[H\033[2J");
    } else {
        printf("\033[0m\n");
    }
    exit(0);
}

void SIGINT_handler(int sig){
    (void)sig;
    finish();
}

void SIGPIPE_handler(int sig) {
    (void)sig;
    finish();
}

void SIGWINCH_handler(int sig) {
    (void)sig;
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    terminal_width = w.ws_col;
    terminal_height = w.ws_row;

    if (using_automatic_width) {
        min_col = (FRAME_WIDTH - terminal_width/2) / 2;
        max_col = (FRAME_WIDTH + terminal_width/2) / 2;
    }

    if (using_automatic_height) {
        min_row = (FRAME_HEIGHT - (terminal_height-1)) / 2;
        max_row = (FRAME_HEIGHT + (terminal_height-1)) / 2;
    }

    signal(SIGWINCH, SIGWINCH_handler);
}

void newline(int n) {
    for (int i = 0; i < n; ++i) {
        putc('\n', stdout);
    }
}

int digits(int val) {
    int d = 1, c;
    if (val >= 0) for (c = 10; c <= val; c *= 10) d++;
    else for (c = -10 ; c >= val; c *= 10) d++;
    return (c < 0) ? ++d : d;
}

void usage(char * argv[]) {
    printf(
        "Terminal pridekitty\n"
        "\n"
        "usage: %s [-hin] [-f \033[3mframes\033[0m]\n"
        "\n"
        " -n --no-counter \033[3mDo not display the timer\033[0m\n"
        " -s --no-title   \033[3mDo not set the titlebar text\033[0m\n"
        " -e --no-clear   \033[3mDo not clear the display between frames\033[0m\n"
        " -d --delay      \033[3mDelay between frames (10-1000ms)\033[0m\n"
        " -f --frames     \033[3mNumber of frames to display\033[0m\n"
        " -r --min-rows   \033[3mCrop from top\033[0m\n"
        " -R --max-rows   \033[3mCrop from bottom\033[0m\n"
        " -c --min-cols   \033[3mCrop from left\033[0m\n"
        " -C --max-cols   \033[3mCrop from right\033[0m\n"
        " -W --width      \033[3mSet output width\033[0m\n"
        " -H --height     \033[3mSet output height\033[0m\n"
        " -h --help       \033[3mShow this help\033[0m\n",
        argv[0]);
}

int main(int argc, char ** argv) {
    char *term = NULL;
    unsigned int k;
    int ttype;
    int delay_ms = 90;
    struct winsize w;

    static struct option long_opts[] = {
        {"help",       no_argument,       0, 'h'},
        {"no-counter", no_argument,       0, 'n'},
        {"no-title",   no_argument,       0, 's'},
        {"no-clear",   no_argument,       0, 'e'},
        {"delay",      required_argument, 0, 'd'},
        {"frames",     required_argument, 0, 'f'},
        {"min-rows",   required_argument, 0, 'r'},
        {"max-rows",   required_argument, 0, 'R'},
        {"min-cols",   required_argument, 0, 'c'},
        {"max-cols",   required_argument, 0, 'C'},
        {"width",      required_argument, 0, 'W'},
        {"height",     required_argument, 0, 'H'},
        {0,0,0,0}
    };

    int index, c;
    while ((c = getopt_long(argc, argv, "eshned:f:r:R:c:C:W:H:", long_opts, &index)) != -1) {
        switch (c) {
            case 'e':
                clear_screen = 0;
                break;
            case 's':
                set_title = 0;
                break;
            case 'h':
                usage(argv);
                exit(0);
            case 'n':
                show_counter = 0;
                break;
            case 'd':
                if (10 <= atoi(optarg) && atoi(optarg) <= 1000)
                    delay_ms = atoi(optarg);
                break;
            case 'f':
                frame_count = atoi(optarg);
                break;
            case 'r':
                min_row = atoi(optarg);
                break;
            case 'R':
                max_row = atoi(optarg);
                break;
            case 'c':
                min_col = atoi(optarg);
                break;
            case 'C':
                max_col = atoi(optarg);
                break;
            case 'W':
                min_col = (FRAME_WIDTH - atoi(optarg)) / 2;
                max_col = (FRAME_WIDTH + atoi(optarg)) / 2;
                break;
            case 'H':
                min_row = (FRAME_HEIGHT - atoi(optarg)) / 2;
                max_row = (FRAME_HEIGHT + atoi(optarg)) / 2;
                break;
            default:
                break;
        }
    }

    term = getenv("TERM");
    ioctl(0, TIOCGWINSZ, &w);
    terminal_width = w.ws_col;
    terminal_height = w.ws_row;

    ttype = 2;
    if (term) {
        for (k = 0; k < strlen(term); ++k)
            term[k] = tolower(term[k]);

        if (strstr(term, "xterm")) ttype = 1;
        else if (strstr(term, "linux")) ttype = 3;
        else if (strstr(term, "vtnt")) ttype = 5;
        else if (strstr(term, "cygwin")) ttype = 5;
        else if (strstr(term, "vt220")) ttype = 6;
        else if (strstr(term, "fallback")) ttype = 4;
        else if (strstr(term, "rxvt")) ttype = 3;
        else if (strstr(term, "vt100") && terminal_width == 40) ttype = 7;
        else if (!strncmp(term, "st", 2)) ttype = 1;
    }

    int always_escape = 0;
    signal(SIGINT, SIGINT_handler);
    signal(SIGPIPE, SIGPIPE_handler);
    signal(SIGWINCH, SIGWINCH_handler);

    switch (ttype) {
        case 1:
            colors[',']  = "\033[48;5;17m";
            colors['.']  = "\033[48;5;231m";
            colors['\''] = "\033[48;5;16m";
            colors['@']  = "\033[48;5;230m";
            colors['$']  = "\033[48;5;45m";
            colors['-']  = "\033[48;5;162m";
            colors['>']  = "\033[48;5;196m";
            colors['&']  = "\033[48;5;214m";
            colors['+']  = "\033[48;5;226m";
            colors['#']  = "\033[48;5;118m";
            colors['=']  = "\033[48;5;19m";
            colors[';']  = "\033[48;5;55m";
            colors['*']  = "\033[48;5;235m";
            colors['%']  = "\033[48;5;175m";
            break;
        // ... (keep existing color definitions for other cases)
    }

    if (min_col == max_col) {
        min_col = (FRAME_WIDTH - terminal_width/2) / 2;
        max_col = (FRAME_WIDTH + terminal_width/2) / 2;
        using_automatic_width = 1;
    }

    if (min_row == max_row) {
        min_row = (FRAME_HEIGHT - (terminal_height-1)) / 2;
        max_row = (FRAME_HEIGHT + (terminal_height-1)) / 2;
        using_automatic_height = 1;
    }

    if (set_title) {
        printf("\033]1;Pridekitty\007");
        printf("\033]2;Pridekitty\007");
    }

    if (clear_screen) {
        printf("\033[H\033[2J\033[?25l");
    } else {
        printf("\033[s");
    }

    time_t start, current;
    time(&start);
    size_t i = 0;
    unsigned int f = 0;
    char last = 0;
    int y, x;

    while (1) {
        if (clear_screen) printf("\033[H");
        else printf("\033[u");

        for (y = min_row; y < max_row; ++y) {
            for (x = min_col; x < max_col; ++x) {
                char color;
                if (y > 23 && y < 43 && x < 0) {
                    const char *rainbow = ",,>>&&&+++###==;;;,,";
                    int mod_x = ((-x+2) % 16) / 8;
                    if ((i / 2) % 2) mod_x = 1 - mod_x;
                    color = rainbow[mod_x + y-23];
                    if (color == 0) color = ',';
                } else if (x < 0 || y < 0 || y >= FRAME_HEIGHT || x >= FRAME_WIDTH) {
                    color = ',';
                } else {
                    color = frames[i][y][x];
                }

                if (always_escape) {
                    printf("%s", colors[(int)color]);
                } else {
                    if (color != last && colors[(int)color]) {
                        last = color;
                        printf("%s%s", colors[(int)color], output);
                    } else {
                        printf("%s", output);
                    }
                }
            }
            newline(1);
        }

        if (show_counter) {
            time(&current);
            double diff = difftime(current, start);
            int nLen = digits((int)diff);
            int width = (terminal_width - 29 - nLen) / 2;
            while (width-- > 0) printf(" ");
            printf("\033[1;37mNyaning for %0.0f seconds!\033[J\033[0m", diff);
        }

        last = 0;
        if (frame_count && ++f == frame_count) finish();
        if (!frames[++i]) i = 0;
        usleep(1000 * delay_ms);
    }
    return 0;
}
