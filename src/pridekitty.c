/*
 * Pridekitty - Minimal Terminal Animation
 * Based on original by K. Lange
 */

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/ioctl.h>

#include "animation.c"

const char * colors[256] = {NULL};
const char * output = "  ";
int terminal_width = 80;
int terminal_height = 24;

void finish() {
    printf("\033[?25h\033[0m\033[H\033[2J");
    exit(0);
}

void SIGINT_handler(int sig) { (void)sig; finish(); }

int main() {
    signal(SIGINT, SIGINT_handler);
    
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    terminal_width = w.ws_col;
    terminal_height = w.ws_row;

    int min_col = (FRAME_WIDTH - terminal_width/2) / 2;
    int max_col = (FRAME_WIDTH + terminal_width/2) / 2;
    int min_row = (FRAME_HEIGHT - (terminal_height-1)) / 2;
    int max_row = (FRAME_HEIGHT + (terminal_height-1)) / 2;

    printf("\033[H\033[2J\033[?25l");

    colors[','] = "\033[48;5;17m"; 
    colors['.'] = "\033[48;5;231m";
    colors['@'] = "\033[48;5;230m";
    colors['$'] = "\033[48;5;45m";
    colors['-'] = "\033[48;5;162m";
    colors['>'] = "\033[48;5;196m";

    size_t frame_idx = 0;
    while(1) {
        printf("\033[H");
        for(int y = min_row; y < max_row; y++) {
            for(int x = min_col; x < max_col; x++) {
                char color = frames[frame_idx][y][x];
                printf("%s%s", colors[(int)color], output);
            }
            putchar('\n');
        }
        frame_idx = frames[frame_idx+1] ? frame_idx+1 : 0;
        usleep(90000);
    }
    return 0;
}
