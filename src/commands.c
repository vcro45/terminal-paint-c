#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "canvas.h"
#include "commands.h"

int validateArguments(char* format, ...) {
    /**
    checks if command arguments are of correct type
    @format: a string of format specifiers
    @... (args): variadic list of pointers that are assigned values if their argument is correct
    @returns: the number of arguments read as the correct type
    */
    va_list args;
    va_start(args, format);

    int numArgsRead = 0;
    char ch = '?';
    int CUR_MAX = 20;
    char* buffer = (char*)malloc(sizeof(char) * CUR_MAX);
    int length = 0;

    while (ch != '\n') {
        if (length == CUR_MAX) {
            CUR_MAX *= 2;
            buffer = realloc(buffer, CUR_MAX);
        }
        ch = getc(stdin);
        buffer[length] = ch;
        length++;
    }

    numArgsRead = vsscanf(buffer, format, args);
    va_end(args);
    free(buffer);

    return numArgsRead;
}

void vertical_draw(Canvas* board, int col, int row_start, int row_end) {
    /**
    draws a vertical line on the current canvas
    @board: a pointer to a struct holding the canvas contents
    @col: the column being drawn in
    @row_start: the row the line starts from
    @row_end: the row the line ends in
    @modifies: board
    */
    if (row_start > row_end) {
        int temp = row_end;
        row_end = row_start;
        row_start = temp;
    }

    for (int row = row_start; row <= row_end; ++row) {
        if (board->table[row][col] == '|' || board->table[row][col] == '*') {
            board->table[row][col] = '|';
        }
        else {
            board->table[row][col] = '+';
        }
    }
}

void horizontal_draw(Canvas* board, int col_start, int col_end, int row) {
    /**
    draws a horizontal line on the current canvas
    @board: a pointer to a struct holding the canvas contents
    @col_start: the column the line starts from
    @col_end: the column the line ends in
    @row: the row being drawn in
    @modifies: board
    */
    if (col_start > col_end) {
        int temp = col_end;
        col_end = col_start;
        col_start = temp;
    }

    for (int col = col_start; col <= col_end; ++col) {
        if (board->table[row][col] == '-' || board->table[row][col] == '*') {
            board->table[row][col] = '-';
        }
        else {
            board->table[row][col] = '+';
        }
    }
}

void up_diagonal_draw(Canvas* board, int col_start, int row_start, int col_end, int row_end) {
    /**
    draws an upwards diagonal line on the current canvas
    @board: a pointer to a struct holding the canvas contents
    @col_start: the column the line starts from
    @row_start: the row the line starts from
    @col_end: the column the line ends in
    @row_end: the row the line ends in
    @modifies: board
    */
    if (col_start > col_end) {
        int temp_col = col_end;
        col_end = col_start;
        col_start = temp_col;

        int temp_row = row_end;
        row_end = row_start;
        row_start = temp_row;
    }

    for (int col = col_start, row = row_start; col <= col_end; ++col, ++row) {
        if (board->table[row][col] == '/' || board->table[row][col] == '*') {
            board->table[row][col] = '/';
        }
        else {
            board->table[row][col] = '+';
        }
    }
}

void down_diagonal_draw(Canvas* board, int col_start, int row_start, int col_end, int row_end) {
    /**
    draws an downwards diagonal line on the current canvas
    @board: a pointer to a struct holding the canvas contents
    @col_start: the column the line starts from
    @row_start: the row the line starts from
    @col_end: the column the line ends in
    @row_end: the row the line ends in
    @modifies: board
    */
    if (col_start > col_end) {
        int temp_col = col_end;
        col_end = col_start;
        col_start = temp_col;

        int temp_row = row_end;
        row_end = row_start;
        row_start = temp_row;
    }

    for (int col = col_start, row = row_start; col <= col_end; ++col, --row) {
        if (board->table[row][col] == '\\' || board->table[row][col] == '*') {
            board->table[row][col] = '\\';
        }
        else {
            board->table[row][col] = '+';
        }
    }
}

void write(Canvas* board) {
    /**
    draws a line on the current canvas connecting user chosen coordinates
    @board: a pointer to a struct holding the canvas contents
    @modifies: board
    */
    int row_start, col_start, row_end, col_end;
    int numArgsRead = validateArguments("%d %d %d %d", &row_start, &col_start, &row_end, &col_end);

    if (numArgsRead != 4 || row_start < 0 || row_start >= board->num_rows || col_start < 0 || col_start >= board->num_cols || row_end < 0 || row_end >= board->num_rows || col_end < 0 || col_end >= board->num_cols) {
        printf("Improper draw command.\n");
        return;
    }

    if (row_start == row_end && col_start == col_end) {
        if (board->table[row_start][col_start] == '*' || board->table[row_start][col_start] == '-') {
            board->table[row_start][col_start] = '-';
        }
        else {
            board->table[row_start][col_start] = '+';
        }
        return;
    }

    if (col_start == col_end) {
        vertical_draw(board, col_start, row_start, row_end);
        return;
    }
    else if (row_start == row_end) {
        horizontal_draw(board, col_start, col_end, row_start);
        return;
    }
    else {
        double slope = (double)(row_end - row_start) / (double)(col_end - col_start);
        if (slope == 1) {
            up_diagonal_draw(board, col_start, row_start, col_end, row_end);
            return;
        }
        else if (slope == -1) {
            down_diagonal_draw(board, col_start, row_start, col_end, row_end);
            return;
        }
        else {
            printf("Improper draw command.\n");
            return;
        }
    }
}

void erase(Canvas* board) {
    /**
    erases a point on the current canvas
    @board: a pointer to a struct holding the canvas contents
    @modifies: board
    */
    int row, col;
    int numArgsRead = validateArguments("%d %d", &row, &col);

    if (numArgsRead != 2 || row < 0 || row >= board->num_rows || col < 0 || col >= board->num_cols) {
        printf("Improper erase command.\n");
    }
    else {
        board->table[row][col] = '*';
    }
}

void add(Canvas* board) {
    /**
    adds either a row or column at user chosen index
    @board: a pointer to a struct holding the canvas contents
    @modifies: board
    */
    char axis;
    int pos;

    int numArgsRead = validateArguments(" %c %d", &axis, &pos);

    if (numArgsRead != 2 || (axis != 'r' && axis != 'c')) {
        printf("Improper add command.\n");
    }
    else if (axis == 'r') {
        if (pos < 0 || pos > board->num_rows) {
            printf("Improper add command.\n");
        }
        else {
            board->num_rows++;
            board->table = (char**)realloc(board->table, board->num_rows * sizeof(char*));
            board->table[(board->num_rows - 1)] = (char*)realloc(board->table[(board->num_rows - 1)], board->num_cols);

            for (int row = (board->num_rows - 1); row > pos; --row) {
                strcpy(board->table[row], board->table[row-1]);
            }

            for (int col = 0; col < board->num_cols; ++col) {
                board->table[pos][col] = '*';
            }
        }
    }
    else if (axis == 'c') {
        if (pos < 0 || pos > board->num_cols) {
            printf("Improper add command.\n");
        }
        else {
            board->num_cols++;
            for (int row = 0; row < board->num_rows; ++row) {
                board->table[row] = realloc(board->table[row], board->num_cols);

                for (int col = (board->num_cols - 1); col > pos; --col) {
                    board->table[row][col] = board->table[row][col-1];
                }

                board->table[row][pos] = '*';
            }
        }
    }
}

void _delete(Canvas* board) {
    /**
    deletes either a row or column at user chosen index
    @board: a pointer to a struct holding the canvas contents
    @modifies: board
    */
    char axis;
    int pos;

    int numArgsRead = validateArguments(" %c %d", &axis, &pos);

    if (numArgsRead != 2 || (axis != 'r' && axis != 'c')) {
        printf("Improper delete command.\n");
    }
    else if (axis == 'r') {
        if (pos < 0 || pos >= board->num_rows) {
            printf("Improper delete command.\n");
        }
        else {
            board->num_rows--;

            for (int row = pos; row < board->num_rows; ++row) {
                strcpy(board->table[row], board->table[row+1]);
            }
            board->table = (char**)realloc(board->table, board->num_rows * sizeof(char*));
        }
    }
    else if (axis == 'c') {
        if (pos < 0 || pos >= board->num_cols) {
            printf("Improper delete command.\n");
        }
        else {
            board->num_cols--;
            for (int row = 0; row < board->num_rows; ++row) {

                for (int col = pos; col < board->num_cols; ++col) {
                    board->table[row][col] = board->table[row][col+1];
                }

                board->table[row] = realloc(board->table[row], board->num_cols);
            }
        }
    }
}

void resize(Canvas* board) {
    /**
    resizes the current canvas
    @board: a pointer to a struct holding the canvas contents
    @modifies: board
    */
    int new_row_dim, new_col_dim;

    int numArgsRead = validateArguments("%d %d", &new_row_dim, &new_col_dim);

    if (numArgsRead != 2 || new_row_dim < 1 || new_col_dim < 1) {
        printf("Improper resize command.\n");
    }
    else {
        if (new_row_dim > board->num_rows) {
            int row_diff = new_row_dim - board->num_rows;
            for (int i = 0; i < row_diff; ++i) {
                board->num_rows++;
                board->table = (char**)realloc(board->table, board->num_rows * sizeof(char*));
                board->table[(board->num_rows - 1)] = (char*)realloc(board->table[(board->num_rows - 1)], board->num_cols);

                for (int col = 0; col < board->num_cols; ++col) {
                    board->table[(board->num_rows - 1)][col] = '*';
                }
            }
        }
        else if (new_row_dim < board->num_rows) {
            int row_diff = board->num_rows - new_row_dim;
            for (int i = 0; i < row_diff; ++i) {
                board->num_rows--;
                board->table = (char**)realloc(board->table, board->num_rows * sizeof(char*));
            }
        }

        if (new_col_dim > board->num_cols) {
            int col_diff = new_col_dim - board->num_cols;
            for (int i = 0; i < col_diff; ++i) {
                board->num_cols++;
                for (int row = 0; row < board->num_rows; ++row) {
                    board->table[row] = realloc(board->table[row], board->num_cols);
                    board->table[row][(board->num_cols - 1)] = '*';
                }
            }
        }
        else if (new_col_dim < board->num_cols) {
            int col_diff = board->num_cols - new_col_dim;
            for (int i = 0; i < col_diff; ++i) {
                board->num_cols--;
                for (int row = 0; row < board->num_rows; ++row) {
                    board->table[row] = realloc(board->table[row], board->num_cols);
                }
            }
        }
    }
}
