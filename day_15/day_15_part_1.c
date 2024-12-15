#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define GRID_SIZE 50
#define INSTRUCTION_LINES 20
#define INSTRUCTIONS_PER_LINE 1000

typedef enum {
    OBJECT_BOX,
    OBJECT_WALL,
    OBJECT_EMPTY,
} object_e;

typedef enum {
    DIRECTION_UP,
    DIRECTION_RIGHT,
    DIRECTION_DOWN,
    DIRECTION_LEFT
} direction_e;

int input_read_file(const char *path, object_e *grid, int grid_size, direction_e *instructions, int instruction_lines,
                    int instructions_per_line, int *robot_x, int *robot_y);

void apply_direction(direction_e direction_e, int *robot_x, int *robot_y);

int try_move(object_e *grid, int grid_size, int *object_x, int *object_y, direction_e direction);

int main(void) {
    object_e grid[GRID_SIZE * GRID_SIZE];
    direction_e instructions[INSTRUCTION_LINES * INSTRUCTIONS_PER_LINE];
    int robot_x = 0;
    int robot_y = 0;

    if (input_read_file("input.txt", grid, GRID_SIZE, instructions, INSTRUCTION_LINES, INSTRUCTIONS_PER_LINE, &robot_x,
                        &robot_y) != 0) {
        perror("Error reading input");
        return 1;
    }

    for (int i = 0; i < INSTRUCTION_LINES * INSTRUCTIONS_PER_LINE; ++i) {
        try_move(grid, GRID_SIZE, &robot_x, &robot_y, instructions[i]);
    }

    int total = 0;
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (grid[i * GRID_SIZE + j] == OBJECT_BOX) {
                total += i * 100 + j;
            }
        }
    }
    printf("%d\n", total);


    return 0;
}

int try_move(object_e *grid, int grid_size, int *object_x, int *object_y, direction_e direction) {
    int new_robot_x = *object_x;
    int new_robot_y = *object_y;
    apply_direction(direction, &new_robot_x, &new_robot_y);

    if (grid[new_robot_x + new_robot_y * grid_size] == OBJECT_WALL) {
        return 0;
    }

    if (grid[new_robot_x + new_robot_y * grid_size] == OBJECT_EMPTY) {
        *object_x = new_robot_x;
        *object_y = new_robot_y;
        return 1;
    }

    if (grid[new_robot_x + new_robot_y * grid_size] == OBJECT_BOX) {
        int new_box_x = new_robot_x;
        int new_box_y = new_robot_y;
        if (try_move(grid, grid_size, &new_box_x, &new_box_y, direction)) {
            *object_x = new_robot_x;
            *object_y = new_robot_y;
            grid[new_box_x + new_box_y * grid_size] = OBJECT_BOX;
            grid[new_robot_x + new_robot_y * grid_size] = OBJECT_EMPTY;
            return 1;
        }
        return 0;
    }
}


void apply_direction(direction_e direction_e, int *robot_x, int *robot_y) {
    switch (direction_e) {
        case DIRECTION_UP:
            *robot_y -= 1;
            break;
        case DIRECTION_RIGHT:
            *robot_x += 1;
            break;
        case DIRECTION_DOWN:
            *robot_y += 1;
            break;
        case DIRECTION_LEFT:
            *robot_x -= 1;
            break;
    }
}

int input_read_file(const char *path, object_e *grid, int grid_size, direction_e *instructions, int instruction_lines,
                    int instructions_per_line, int *robot_x, int *robot_y) {
    FILE *input_file = fopen(path, "r");
    if (input_file == NULL) {
        perror("Could not read file");
        return 1;
    }

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            int character = fgetc(input_file);
            switch (character) {
                case '#':
                    grid[i * GRID_SIZE + j] = OBJECT_WALL;
                    break;
                case 'O':
                    grid[i * GRID_SIZE + j] = OBJECT_BOX;
                    break;
                case '@':
                    *robot_x = j;
                    *robot_y = i;
                    grid[i * GRID_SIZE + j] = OBJECT_EMPTY;
                    break;
                case '.':
                    grid[i * GRID_SIZE + j] = OBJECT_EMPTY;
                    break;
                default:
                    assert(0);
            }
        }
        fgetc(input_file);
    }

    fgetc(input_file);

    for (int i = 0; i < instruction_lines; i++) {
        for (int j = 0; j < instructions_per_line; j++) {
            int character = fgetc(input_file);
            switch (character) {
                case '^':
                    instructions[i * instructions_per_line + j] = DIRECTION_UP;
                    break;
                case '>':
                    instructions[i * instructions_per_line + j] = DIRECTION_RIGHT;
                    break;
                case 'v':
                    instructions[i * instructions_per_line + j] = DIRECTION_DOWN;
                    break;
                case '<':
                    instructions[i * instructions_per_line + j] = DIRECTION_LEFT;
                    break;
                default:
                    assert(0);
            }
        }
        fgetc(input_file);
    }

    fclose(input_file);
    return 0;
}
