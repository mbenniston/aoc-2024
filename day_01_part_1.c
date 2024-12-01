#include <stdio.h>
#include <stdlib.h>

#define LINE_COUNT (1000)

void input_read_line(FILE *file, int *first, int *second);

void input_read_lines(FILE *file, int *left_numbers, int *right_numbers, int line_count);

int input_read_file(const char *path, int *left_numbers, int *right_numbers, int line_count);

int compare_ints(const void *left, const void *right);

int main(void) {
    int left_numbers[LINE_COUNT] = {0};
    int right_numbers[LINE_COUNT] = {0};

    if (input_read_file("input.txt", left_numbers, right_numbers, LINE_COUNT) != 0) {
        perror("Error reading input");
        return 1;
    }


    // sort both lists
    qsort(left_numbers, LINE_COUNT, sizeof(int), compare_ints);
    qsort(right_numbers, LINE_COUNT, sizeof(int), compare_ints);

    // calculate distance between each pair and make total
    int total_distance = 0;
    for (int i = 0; i < LINE_COUNT; i++) {
        total_distance += abs(left_numbers[i] - right_numbers[i]);
    }

    printf("Total distance: %d", total_distance);

    return 0;
}

int compare_ints(const void *left, const void *right) {
    int left_int = *(const int *) left;
    int right_int = *(const int *) right;

    if (left_int < right_int) return -1;
    if (left_int > right_int) return 1;
    return 0;
}

void input_read_line(FILE *file, int *first, int *second) {
    fscanf(file, "%d   %d\n", first, second);
}

void input_read_lines(FILE *file, int *left_numbers, int *right_numbers, int line_count) {
    int current_line = 0;
    while (!feof(file) && current_line < line_count) {
        input_read_line(file, &left_numbers[current_line], &right_numbers[current_line]);
        current_line++;
    }
}

int input_read_file(const char *path, int *left_numbers, int *right_numbers, int line_count) {
    FILE *input_file = fopen(path, "r");
    if (input_file == NULL) {
        perror("Could not read file");
        return 1;
    }

    input_read_lines(input_file, left_numbers, right_numbers, line_count);
    fclose(input_file);
    return 0;
}