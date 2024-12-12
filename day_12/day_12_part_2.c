#include <stdio.h>
#include <stdlib.h>

#define GRID_SIZE 140

int input_read_file(const char *path, char *grid, int grid_size);

typedef enum {
    UP = 1,
    RIGHT = 2,
    DOWN = 4,
    LEFT = 8
} direction_e;

void check_direction(
        int tile_x, int tile_y,
        int dx, int dy,
        direction_e direction,
        const char *grid,
        int grid_size,
        unsigned int *perimeter_grid
) {
    char tile = grid[tile_x + tile_y * grid_size];
    int new_x = tile_x + dx;
    int new_y = tile_y + dy;

    int new_tile_index = new_x + new_y * grid_size;
    int new_tile = (new_x < 0 || new_x >= GRID_SIZE || new_y < 0 || new_y >= GRID_SIZE) ? 1 : grid[new_tile_index] !=
                                                                                              tile;
    int p = (new_x + 1) + (new_y + 1) * (grid_size + 2);
    if (new_tile) {
        perimeter_grid[p] |= direction;
    }
}

void count_fences(char *grid, int grid_size, int tile_x, int tile_y, char *visited_grid, int *total_neighbor_count,
                  unsigned int *perimeter_grid) {
    char tile = grid[tile_y * grid_size + tile_x];

    (*total_neighbor_count)++;
    visited_grid[tile_x + tile_y * grid_size] = 1;


    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx != 0 && dy != 0) continue;
            if (dx == 0 && dy == 0) continue;
            int new_x = tile_x + dx;
            int new_y = tile_y + dy;

            if (new_y < 0 || new_y >= grid_size) continue;
            if (new_x < 0 || new_x >= grid_size) continue;

            int new_tile_index = new_x + new_y * grid_size;
            char new_tile = grid[new_tile_index];

            if (new_tile == tile && !visited_grid[new_tile_index]) {
                count_fences(grid, grid_size, new_x, new_y, visited_grid, total_neighbor_count,
                             perimeter_grid);
            }
        }
    }

    check_direction(tile_x, tile_y, 1, 0, RIGHT, grid, grid_size, perimeter_grid);
    check_direction(tile_x, tile_y, -1, 0, LEFT, grid, grid_size, perimeter_grid);
    check_direction(tile_x, tile_y, 0, 1, UP, grid, grid_size, perimeter_grid);
    check_direction(tile_x, tile_y, 0, -1, DOWN, grid, grid_size, perimeter_grid);
}

int main(void) {
    char grid[GRID_SIZE * GRID_SIZE];
    char visited_grid[GRID_SIZE * GRID_SIZE] = {0};
    if (input_read_file("input.txt", grid, GRID_SIZE) != 0) {
        perror("Error reading input");
        return 1;
    }

    long long total = 0;
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (visited_grid[j + i * GRID_SIZE]) {
                continue;
            }
            unsigned int perimeter_grid[(GRID_SIZE + 2) * (GRID_SIZE + 2)] = {0};
            int total_sides = 0;
            int total_area = 0;

            count_fences(grid, GRID_SIZE, j, i, visited_grid, &total_area, perimeter_grid);

            for (int k = 0; k < GRID_SIZE + 2; ++k) {
                int in_left_fence = 0;
                int in_right_fence = 0;
                int in_up_fence = 0;
                int in_down_fence = 0;

                for (int b = 0; b < GRID_SIZE + 2; ++b) {
                    unsigned int horizontal_tile = perimeter_grid[b * (GRID_SIZE + 2) + k];
                    unsigned int vertical_tile = perimeter_grid[k * (GRID_SIZE + 2) + b];

                    if ((horizontal_tile & LEFT) != 0) {
                        if (in_left_fence == 0)
                            total_sides++;
                        in_left_fence = 1;
                    } else {
                        in_left_fence = 0;
                    }

                    if ((horizontal_tile & RIGHT) != 0) {
                        if (in_right_fence == 0)
                            total_sides++;
                        in_right_fence = 1;
                    } else {
                        in_right_fence = 0;
                    }

                    if ((vertical_tile & DOWN) != 0) {
                        if (in_down_fence == 0)
                            total_sides++;
                        in_down_fence = 1;
                    } else {
                        in_down_fence = 0;
                    }

                    if ((vertical_tile & UP) != 0) {
                        if (in_up_fence == 0)
                            total_sides++;
                        in_up_fence = 1;
                    } else {
                        in_up_fence = 0;
                    }
                }
            }

            total += (long long) total_sides * total_area;
        }
    }

    printf("Total price: %lld\n", total);

    return 0;
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
