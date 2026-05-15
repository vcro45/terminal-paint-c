// This program simulates a text based version of paint.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "canvas.h"
#include "commands.h"

int main(int argc, char** argv) {
    Canvas board;
    bool validRowCount = false;
    bool defaultBoard = true;

    board.num_rows = 10, board.num_cols = 10;
    if (argc == 3) {
        if (atoi(argv[1]) != 0) {
            if (atoi(argv[1]) > 0) {
                board.num_rows = atoi(argv[1]);
                validRowCount = true;
            }
            else {
                printf("The number of rows is less than 1.\n");
            }
        }
        else {
            printf("The number of rows is not an integer.\n");
        }

        if (validRowCount) {
            if (atoi(argv[2]) != 0) {
                if (atoi(argv[2]) > 0) {
                    board.num_cols = atoi(argv[2]);
                    defaultBoard = false;
                }
                else {
                    printf("The number of columns is less than 1.\n");
                    board.num_rows = 10;
                }
            }
            else {
                printf("The number of columns is not an integer.\n");
                board.num_rows = 10;
            }
        }
    }
    else if (argc != 1) {
        printf("Wrong number of command line arguments entered.\n");
        printf("Usage: ./paint.out [num_rows num_cols]\n");
    }

    if (defaultBoard && argc != 1) {
        printf("Making default board of 10 X 10.\n");
    }

    board.table = make_canvas(board);
    display_canvas(board);

    char command = '\0';
    do {
        printf("Enter your command: ");
        do {
            scanf("%c", &command);
        } while (!isalpha(command));

        if (command == 'h') {
            print_help();
        }
        else if (command == 'w') {
            write(&board);
        }
        else if (command == 'e') {
            erase(&board);
        }
        else if (command == 'r') {
            resize(&board);
        }
        else if (command == 'a') {
            add(&board);
        }
        else if (command == 'd') {
            _delete(&board);
        }
        else if (command == 's') {
            save(board);
        }
        else if (command == 'l') {
            load(&board);
        }
        else if (command != 'q') {
            printf("Unrecognized command. Type h for help.\n");
        }

        if (command != 'q') {
            display_canvas(board);
        }

        fflush(stdin);
    }
    while (command != 'q');
    free(board.table);

    return 0;
}
