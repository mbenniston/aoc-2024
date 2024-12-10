#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define GRID_SIZE 40

int input_read_file(const char *path, char *grid, int grid_size);

int get_start_score(char *grid, int grid_size, int x, int y, int *visited_nines_grid);

int main(void) {
    char grid[GRID_SIZE * GRID_SIZE];

    if (input_read_file("input.txt", grid, GRID_SIZE) != 0) {
        perror("Error reading input");
        return 1;
    }

    int total = 0;
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (grid[i + GRID_SIZE * j] == '0') {
                int visited_nines_grid[GRID_SIZE * GRID_SIZE] = {0};
                total += get_start_score(grid, GRID_SIZE, i, j, visited_nines_grid);
            }
        }
    }
    printf("%d\n", total);

    return 0;
}

int get_start_score(char *grid, int grid_size, int x, int y, int *visited_nines_grid) {
    char current_number = grid[x + y * grid_size];
    if (current_number == '9') {
        return 1;
    }

    int score = 0;

    for (int iy = -1; iy <= 1; ++iy) {
        for (int jx = -1; jx <= 1; ++jx) {
            if (iy == 0 && jx == 0) continue;
            if (iy != 0 && jx != 0) continue;

            int new_x = x + jx;
            int new_y = y + iy;
            if (new_x < 0 || new_x > grid_size)continue;
            if (new_y < 0 || new_y > grid_size)continue;

            char next_number = grid[new_x + new_y * grid_size];
            if (next_number == current_number + 1) {
                score += get_start_score(grid, grid_size, new_x, new_y, visited_nines_grid);
            }
        }
    }
    return score;
}


int input_read_file(const char *path, char *grid, int grid_size) {
    FILE *input_file = fopen(path, "r");
    if (input_file == NULL) {
        perror("Could not read file");
        return 1;
    }

    for (int i = 0; i < grid_size; ++i) {
        fread(grid + i * grid_size, 1, grid_size, input_file);
        fgetc(input_file);
    }

    fclose(input_file);
    return 0;
}
