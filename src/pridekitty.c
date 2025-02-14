/*
 * Pridekitty - Terminal Animation Renderer
 * Original concept by K. Lange, modified for standalone use
 */

#define _XOPEN_SOURCE 700  /* POSIX 2008 for usleep */
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
#include <getopt.h>

#include "animation.c"

/* Global configuration */
const char * colors[256] = {NULL};
const char * output = "  ";
int show_counter = 1;
unsigned int frame_count = 0;
int clear_screen = 1;
int set_title = 1;

/* Display boundaries */
int min_row = -1;
int max_row = -1;
int min_col = -1;
int max_col = -1;

/* Terminal dimensions */
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

/* Signal handlers */
void SIGINT_handler(int sig) { (void)sig; finish(); }
void SIGPIPE_handler(int sig) { (void)sig; finish(); }

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

/* Helper functions */
void newline(int n) { for(int i=0; i<n; i++) putchar('\n'); }

int digits(int val) {
    int d = 1, c;
    if (val >= 0) for(c=10; c<=val; c*=10) d++;
    else for(c=-10; c>=val; c*=10) d++;
    return (c < 0) ? ++d : d;
}

void usage(char *argv[]) {
    printf("Usage: %s [OPTIONS]\n"
           "Options:\n"
           "  -n, --no-counter   Hide timer\n"
           "  -s, --no-title     Disable title setting\n"
           "  -e, --no-clear     Keep screen between frames\n"
           "  -d, --delay MS     Frame delay (10-1000ms)\n"
           "  -f, --frames N     Number of frames to show\n"
           "  -W, --width W      Output width\n"
           "  -H, --height H     Output height\n"
           "  -h, --help         Show this help\n",
           argv[0]);
}

int main(int argc, char **argv) {
    char *term = NULL;
    int c, opt_index = 0;
    unsigned int k;
    struct winsize w;
    int delay_ms = 90;
    int ttype = 2;
    int always_escape = 0;

    static struct option long_opts[] = {
        {"help",       no_argument,       NULL, 'h'},
        {"no-counter", no_argument,       NULL, 'n'},
        {"no-title",   no_argument,       NULL, 's'},
        {"no-clear",   no_argument,       NULL, 'e'},
        {"delay",      required_argument, NULL, 'd'},
        {"frames",     required_argument, NULL, 'f'},
        {"min-rows",   required_argument, NULL, 'r'},
        {"max-rows",   required_argument, NULL, 'R'},
        {"min-cols",   required_argument, NULL, 'c'},
        {"max-cols",   required_argument, NULL, 'C'},
        {"width",      required_argument, NULL, 'W'},
        {"height",     required_argument, NULL, 'H'},
        {NULL, 0, NULL, 0}
    };

    /* Parse command line options */
    while ((c = getopt_long(argc, argv, "eshned:f:r:R:c:C:W:H:", 
                           long_opts, &opt_index)) != -1) {
        switch(c) {
            case 'e': clear_screen = 0; break;
            case 's': set_title = 0; break;
            case 'h': usage(argv); exit(0);
            case 'n': show_counter = 0; break;
            case 'd': delay_ms = atoi(optarg); break;
            case 'f': frame_count = atoi(optarg); break;
            case 'W':
                min_col = (FRAME_WIDTH - atoi(optarg)) / 2;
                max_col = (FRAME_WIDTH + atoi(optarg)) / 2;
                break;
            case 'H':
                min_row = (FRAME_HEIGHT - atoi(optarg)) / 2;
                max_row = (FRAME_HEIGHT + atoi(optarg)) / 2;
                break;
            default: break;
        }
    }

    /* Terminal setup */
    ioctl(0, TIOCGWINSZ, &w);
    terminal_width = w.ws_col;
    terminal_height = w.ws_row;

    /* Terminal type detection */
    if ((term = getenv("TERM"))) {
        for(k=0; k<strlen(term); k++) term[k] = tolower(term[k]);
        if (strstr(term, "xterm")) ttype = 1;
        else if (strstr(term, "linux")) ttype = 3;
        else if (strstr(term, "vt220")) ttype = 6;
    }

    /* Color setup */
    switch(ttype) {
        case 1:  /* 256-color */
            colors[','] = "\033[48;5;17m"; 
            colors['.'] = "\033[48;5;231m";
            colors['\''] = "\033[48;5;16m";
            colors['@'] = "\033[48;5;230m";
            colors['$'] = "\033[48;5;45m";
            colors['-'] = "\033[48;5;162m";
            colors['>'] = "\033[48;5;196m";
            colors['&'] = "\033[48;5;214m";
            colors['+'] = "\033[48;5;226m";
            colors['#'] = "\033[48;5;118m";
            colors['='] = "\033[48;5;19m";
            colors[';'] = "\033[48;5;55m";
            colors['*'] = "\033[48;5;235m";
            colors['%'] = "\033[48;5;175m";
            break;
        case 6:  /* VT220 */
            output = "::";
            always_escape = 1;
            break;
    }

    /* Set display boundaries */
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

    /* Terminal initialization */
    if (set_title) printf("\033]1;Pridekitty\007");
    printf(clear_screen ? "\033[H\033[2J\033[?25l" : "\033[s");

    /* Animation loop */
    time_t start, current;
    time(&start);
    size_t frame_idx = 0;
    unsigned int frames_shown = 0;
    char last_color = 0;

    while(1) {
        printf(clear_screen ? "\033[H" : "\033[u");
        
        for(int y = min_row; y < max_row; y++) {
            for(int x = min_col; x < max_col; x++) {
                char color;
                if (y > 23 && y < 43 && x < 0) {
                    const char *rainbow = ",,>>&&&+++###==;;;,,";
                    int mod_x = ((-x+2) % 16) / 8;
                    if ((frame_idx / 2) % 2) mod_x = 1 - mod_x;
                    color = rainbow[mod_x + y-23];
                    color = color ? color : ',';
                } else if (x < 0 || y < 0 || y >= FRAME_HEIGHT || x >= FRAME_WIDTH) {
                    color = ',';
                } else {
                    color = frames[frame_idx][y][x];
                }

                if (always_escape) {
                    printf("%s", colors[(int)color]);
                } else {
                    if (color != last_color && colors[(int)color]) {
                        last_color = color;
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

        /* Frame control */
        if (frame_count && ++frames_shown >= frame_count) finish();
        frame_idx = frames[frame_idx+1] ? frame_idx+1 : 0;
        usleep(delay_ms * 1000);
    }

    return 0;
}
