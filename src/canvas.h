#ifndef CANVAS_H
#define CANVAS_H

    typedef struct Canvas {
        int num_rows;
        int num_cols;
        char** table;
    } Canvas;

    void print_help(void);
    char** make_canvas(Canvas board);
    void display_canvas(Canvas board);
    char* getFileName(void);
    void save(Canvas board);
    void load(Canvas* board);

#endif
