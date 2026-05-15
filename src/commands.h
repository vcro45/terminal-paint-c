#ifndef COMMANDS_H
#define COMMANDS_H

    int validateArguments(char* format, ...);
    void vertical_draw(Canvas* board, int col, int row_start, int row_end);
    void horizontal_draw(Canvas* board, int col_start, int col_end, int row);
    void up_diagonal_draw(Canvas* board, int col_start, int row_start, int col_end, int row_end);
    void down_diagonal_draw(Canvas* board, int col_start, int row_start, int col_end, int row_end);
    void write(Canvas* board);
    void erase(Canvas* board);
    void add(Canvas* board);
    void _delete(Canvas* board);
    void resize(Canvas* board);

#endif
