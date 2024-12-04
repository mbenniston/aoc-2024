#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int input_read_file(const char *path, char *contents, int size);


int xmas_count(char *contents, int x, int y, int size);

int has_xmas_vertical(char *contents, int x, int y, int size);

int has_xmas_vertical_reverse(char *contents, int x, int y, int size);

int has_xmas_horizontal(char *contents, int x, int y, int size);

int has_xmas_horizontal_reverse(char *contents, int x, int y, int size);

int has_xmas_diagonal(char *contents, int x, int y, int size);

int has_xmas_diagonal_reverse(char *contents, int x, int y, int size);

int has_xmas_diagonal_left(char *contents, int x, int y, int size);

int has_xmas_diagonal_left_reverse(char *contents, int x, int y, int size);

int get_char(const char *contents, int x, int y, int size, char *c);


#define BOARD_SIZE 140

int main(void) {
    char contents[BOARD_SIZE * BOARD_SIZE];
    if (input_read_file("input.txt", contents, BOARD_SIZE) != 0) {
        perror("Error reading input");
        return 1;
    }

    int count = 0;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            count += xmas_count(contents, j, i, BOARD_SIZE);
        }
    }
    printf("Total %d\n", count);

    return 0;
}

int input_read_file(const char *path, char *contents, int size) {
    FILE *input_file = fopen(path, "r");
    if (input_file == NULL) {
        perror("Could not read file");
        return 1;
    }
    char *buffer = malloc(size + 2);
    for (int i = 0; i < size; i++) {
        fgets(buffer, size + 2, input_file);
        memcpy(contents + size * i, buffer, size);
    }
    free(buffer);

    fclose(input_file);
    return 0;
}


int xmas_count(char *contents, int x, int y, int size) {
    return (has_xmas_vertical(contents, x, y, size))
           + ((has_xmas_vertical_reverse(contents, x, y, size)))
           + (has_xmas_horizontal(contents, x, y, size))
           + (has_xmas_horizontal_reverse(contents, x, y, size))
           + (has_xmas_diagonal(contents, x, y, size))
           + (has_xmas_diagonal_reverse(contents, x, y, size))
           + (has_xmas_diagonal_left(contents, x, y, size))
           + (has_xmas_diagonal_left_reverse(contents, x, y, size));
}


int has_xmas_vertical(char *contents, int x, int y, int size) {
    char c;
    if (!get_char(contents, x, y, size, &c) || c != 'X') return 0;
    if (!get_char(contents, x, y + 1, size, &c) || c != 'M') return 0;
    if (!get_char(contents, x, y + 2, size, &c) || c != 'A') return 0;
    if (!get_char(contents, x, y + 3, size, &c) || c != 'S') return 0;
    return 1;
}


int has_xmas_vertical_reverse(char *contents, int x, int y, int size) {
    char c;
    if (!get_char(contents, x, y, size, &c) || c != 'S') return 0;
    if (!get_char(contents, x, y + 1, size, &c) || c != 'A') return 0;
    if (!get_char(contents, x, y + 2, size, &c) || c != 'M') return 0;
    if (!get_char(contents, x, y + 3, size, &c) || c != 'X') return 0;
    return 1;
}

int has_xmas_horizontal(char *contents, int x, int y, int size) {
    char c;
    if (!get_char(contents, x, y, size, &c) || c != 'X') return 0;
    if (!get_char(contents, x + 1, y, size, &c) || c != 'M') return 0;
    if (!get_char(contents, x + 2, y, size, &c) || c != 'A') return 0;
    if (!get_char(contents, x + 3, y, size, &c) || c != 'S') return 0;
    return 1;
}

int has_xmas_horizontal_reverse(char *contents, int x, int y, int size) {
    char c;
    if (!get_char(contents, x, y, size, &c) || c != 'S') return 0;
    if (!get_char(contents, x + 1, y, size, &c) || c != 'A') return 0;
    if (!get_char(contents, x + 2, y, size, &c) || c != 'M') return 0;
    if (!get_char(contents, x + 3, y, size, &c) || c != 'X') return 0;
    return 1;
}


int has_xmas_diagonal(char *contents, int x, int y, int size) {
    char c;
    if (!get_char(contents, x, y, size, &c) || c != 'X') return 0;
    if (!get_char(contents, x - 1, y + 1, size, &c) || c != 'M') return 0;
    if (!get_char(contents, x - 2, y + 2, size, &c) || c != 'A') return 0;
    if (!get_char(contents, x - 3, y + 3, size, &c) || c != 'S') return 0;
    return 1;
}

int has_xmas_diagonal_reverse(char *contents, int x, int y, int size) {
    char c;
    if (!get_char(contents, x, y, size, &c) || c != 'S') return 0;
    if (!get_char(contents, x - 1, y + 1, size, &c) || c != 'A') return 0;
    if (!get_char(contents, x - 2, y + 2, size, &c) || c != 'M') return 0;
    if (!get_char(contents, x - 3, y + 3, size, &c) || c != 'X') return 0;
    return 1;
}


int has_xmas_diagonal_left(char *contents, int x, int y, int size) {
    char c;
    if (!get_char(contents, x, y, size, &c) || c != 'X') return 0;
    if (!get_char(contents, x + 1, y + 1, size, &c) || c != 'M') return 0;
    if (!get_char(contents, x + 2, y + 2, size, &c) || c != 'A') return 0;
    if (!get_char(contents, x + 3, y + 3, size, &c) || c != 'S') return 0;
    return 1;
}

int has_xmas_diagonal_left_reverse(char *contents, int x, int y, int size) {
    char c;
    if (!get_char(contents, x, y, size, &c) || c != 'S') return 0;
    if (!get_char(contents, x + 1, y + 1, size, &c) || c != 'A') return 0;
    if (!get_char(contents, x + 2, y + 2, size, &c) || c != 'M') return 0;
    if (!get_char(contents, x + 3, y + 3, size, &c) || c != 'X') return 0;
    return 1;
}


int get_char(const char *contents, int x, int y, int size, char *c) {
    if (x < 0 || x >= size) return 0;
    if (y < 0 || y >= size) return 0;
    *c = contents[y * size + x];

    return 1;
}
