#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define GRID_SIZE 50

int input_read_file(const char *path, char *grid, int grid_size);

int count_anti_nodes_at(const char *grid, int *has_anti_node, int grid_size, int x, int y, char symbol);

int count_anti_nodes(char *grid, int *has_anti_node, int grid_size);

int main(void) {
    char grid[GRID_SIZE * GRID_SIZE];
    int has_anti_node[GRID_SIZE * GRID_SIZE] = {0};

    if (input_read_file("input.txt", grid, GRID_SIZE) != 0) {
        perror("Error reading input");
        return 1;
    }

    int count = count_anti_nodes(grid, has_anti_node, GRID_SIZE);
    printf("Count: %d", count);
    return 0;
}

int input_read_file(const char *path, char *grid, int grid_size) {
    FILE *input_file = fopen(path, "r");
    if (input_file == NULL) {
        perror("Could not read file");
        return 1;
    }

    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            grid[j + i * grid_size] = fgetc(input_file);
        }
        fgetc(input_file);
    }

    fclose(input_file);
    return 0;
}

int count_anti_nodes(char *grid, int *has_anti_node, int grid_size) {
    int count = 0;
    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            char symbol = grid[j + i * grid_size];
            if (symbol != '.') {
                count += count_anti_nodes_at(grid, has_anti_node, grid_size, j, i, symbol);
            }
        }
    }
    return count;
}


int count_anti_nodes_at(const char *grid, int *has_anti_node, int grid_size, int x, int y, char symbol) {
    int count = 0;
    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            if (i == y && j == x) continue;
            if (grid[j + i * grid_size] != symbol) continue;

            int dx = j - x;
            int dy = i - y;
            int anti_node_x = x;
            int anti_node_y = y;

            while (anti_node_x >= 0 && anti_node_x < grid_size && anti_node_y >= 0 && anti_node_y < grid_size) {
                int anti_node_index = anti_node_x + anti_node_y * grid_size;
                if (!has_anti_node[anti_node_index]) {
                    count++;
                    has_anti_node[anti_node_index] = 1;
                }
                anti_node_x -= dx;
                anti_node_y -= dy;
            }
        }
    }

    return count;
}
