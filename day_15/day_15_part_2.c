#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define GRID_WIDTH 100
#define GRID_HEIGHT 50
#define INSTRUCTION_LINES 20
#define INSTRUCTIONS_PER_LINE 1000

typedef enum {
    OBJECT_BOX_LEFT,
    OBJECT_BOX_RIGHT,
    OBJECT_WALL,
    OBJECT_EMPTY,
} object_e;

typedef enum {
    DIRECTION_UP,
    DIRECTION_RIGHT,
    DIRECTION_DOWN,
    DIRECTION_LEFT
} direction_e;

int input_read_file(const char *path, object_e *grid, int grid_width, int grid_height, direction_e *instructions,
                    int instruction_lines,
                    int instructions_per_line, int *robot_x, int *robot_y);

void apply_direction(direction_e direction_e, int *robot_x, int *robot_y);

int can_move(object_e *grid, int grid_width, int grid_height, int robot_x, int robot_y, direction_e direction);

int try_move(object_e *grid, int grid_width, int grid_height, int *robot_x, int *robot_y, direction_e direction);

int main(void) {
    object_e grid[GRID_WIDTH * GRID_HEIGHT];
    direction_e instructions[INSTRUCTION_LINES * INSTRUCTIONS_PER_LINE];
    int robot_x = 0;
    int robot_y = 0;

    if (input_read_file("input.txt", grid, GRID_WIDTH, GRID_HEIGHT, instructions, INSTRUCTION_LINES,
                        INSTRUCTIONS_PER_LINE, &robot_x,
                        &robot_y) != 0) {
        perror("Error reading input");
        return 1;
    }

    for (int i = 0; i < INSTRUCTION_LINES * INSTRUCTIONS_PER_LINE; ++i) {
        try_move(grid, GRID_WIDTH, GRID_HEIGHT, &robot_x, &robot_y, instructions[i]);
    }


    int total = 0;
    for (int i = 0; i < GRID_HEIGHT; ++i) {
        for (int j = 0; j < GRID_WIDTH; ++j) {
            if (grid[i * GRID_WIDTH + j] == OBJECT_BOX_LEFT) {
                total += i * 100 + j;
            }
        }
    }
    printf("%d\n", total);


    return 0;
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

int can_move(object_e *grid, int grid_width, int grid_height, int robot_x, int robot_y, direction_e direction) {
    int new_robot_x = robot_x;
    int new_robot_y = robot_y;
    apply_direction(direction, &new_robot_x, &new_robot_y);

    if (grid[new_robot_x + new_robot_y * grid_width] == OBJECT_WALL) {
        return 0;
    }

    if (grid[new_robot_x + new_robot_y * grid_width] == OBJECT_EMPTY) {
        return 1;
    }

    if ((direction == DIRECTION_LEFT || direction == DIRECTION_RIGHT) && (grid[new_robot_x + new_robot_y * grid_width]
                                                                          == OBJECT_BOX_LEFT || grid[
                                                                              new_robot_x + new_robot_y * grid_width]
                                                                          == OBJECT_BOX_RIGHT)) {
        int new_box_x = new_robot_x;
        int new_box_y = new_robot_y;

        if (can_move(grid, grid_width, grid_height, new_box_x, new_box_y, direction)) {
            return 1;
        }
        return 0;
    }


    if ((direction == DIRECTION_UP || direction == DIRECTION_DOWN) && grid[new_robot_x + new_robot_y * grid_width]
        == OBJECT_BOX_LEFT) {
        int new_box_left_x = new_robot_x;
        int new_box_left_y = new_robot_y;
        int new_box_right_x = new_robot_x + 1;
        int new_box_right_y = new_robot_y;

        return can_move(grid, grid_width, grid_height, new_box_left_x, new_box_left_y, direction) &&
               can_move(grid, grid_width, grid_height, new_box_right_x, new_box_right_y, direction);
    }

    if ((direction == DIRECTION_UP || direction == DIRECTION_DOWN) && grid[new_robot_x + new_robot_y * grid_width]
        == OBJECT_BOX_RIGHT) {
        int new_box_left_x = new_robot_x - 1;
        int new_box_left_y = new_robot_y;
        int new_box_right_x = new_robot_x;
        int new_box_right_y = new_robot_y;

        return can_move(grid, grid_width, grid_height, new_box_left_x, new_box_left_y, direction) &&
               can_move(grid, grid_width, grid_height, new_box_right_x, new_box_right_y, direction);
    }

    assert(0);
}

int try_move(object_e *grid, int grid_width, int grid_height, int *robot_x, int *robot_y, direction_e direction) {
    int new_robot_x = *robot_x;
    int new_robot_y = *robot_y;
    apply_direction(direction, &new_robot_x, &new_robot_y);

    if (grid[new_robot_x + new_robot_y * grid_width] == OBJECT_WALL) {
        return 0;
    }

    if (grid[new_robot_x + new_robot_y * grid_width] == OBJECT_EMPTY) {
        *robot_x = new_robot_x;
        *robot_y = new_robot_y;
        return 1;
    }

    if ((direction == DIRECTION_LEFT || direction == DIRECTION_RIGHT) && (grid[new_robot_x + new_robot_y * grid_width]
                                                                          == OBJECT_BOX_LEFT || grid[
                                                                              new_robot_x + new_robot_y * grid_width]
                                                                          == OBJECT_BOX_RIGHT)) {
        int new_box_x = new_robot_x;
        int new_box_y = new_robot_y;

        if (try_move(grid, grid_width, grid_height, &new_box_x, &new_box_y, direction)) {
            *robot_x = new_robot_x;
            *robot_y = new_robot_y;
            object_e object = grid[new_robot_x + new_robot_y * grid_width];
            grid[new_box_x + new_box_y * grid_width] = object;
            grid[new_robot_x + new_robot_y * grid_width] = OBJECT_EMPTY;
            return 1;
        }

        return 0;
    }

    if ((direction == DIRECTION_UP || direction == DIRECTION_DOWN) && grid[new_robot_x + new_robot_y * grid_width]
        == OBJECT_BOX_LEFT) {
        if (can_move(grid, grid_width, grid_height, new_robot_x, new_robot_y, direction) && can_move(
                grid, grid_width, grid_height, new_robot_x + 1, new_robot_y, direction)) {
            int new_box_left_x = new_robot_x;
            int new_box_left_y = new_robot_y;
            int new_box_right_x = new_robot_x + 1;
            int new_box_right_y = new_robot_y;

            if (try_move(grid, grid_width, grid_height, &new_box_left_x, &new_box_left_y, direction) &&
                try_move(grid, grid_width, grid_height, &new_box_right_x, &new_box_right_y, direction)
            ) {
                *robot_x = new_robot_x;
                *robot_y = new_robot_y;
                grid[new_box_left_x + new_box_left_y * grid_width] = OBJECT_BOX_LEFT;
                grid[new_box_right_x + new_box_right_y * grid_width] = OBJECT_BOX_RIGHT;
                grid[new_robot_x + new_robot_y * grid_width] = OBJECT_EMPTY;
                grid[(new_robot_x + 1) + new_robot_y * grid_width] = OBJECT_EMPTY;
                return 1;
            }
            assert(0);
        }

        return 0;
    }


    if ((direction == DIRECTION_UP || direction == DIRECTION_DOWN) && grid[new_robot_x + new_robot_y * grid_width]
        == OBJECT_BOX_RIGHT) {
        if (can_move(grid, grid_width, grid_height, new_robot_x, new_robot_y, direction) && can_move(
                grid, grid_width, grid_height, new_robot_x - 1, new_robot_y, direction)) {
            int new_box_left_x = new_robot_x - 1;
            int new_box_left_y = new_robot_y;
            int new_box_right_x = new_robot_x;
            int new_box_right_y = new_robot_y;

            if (try_move(grid, grid_width, grid_height, &new_box_left_x, &new_box_left_y, direction) &&
                try_move(grid, grid_width, grid_height, &new_box_right_x, &new_box_right_y, direction)
            ) {
                *robot_x = new_robot_x;
                *robot_y = new_robot_y;
                grid[new_box_left_x + new_box_left_y * grid_width] = OBJECT_BOX_LEFT;
                grid[new_box_right_x + new_box_right_y * grid_width] = OBJECT_BOX_RIGHT;
                grid[new_robot_x + new_robot_y * grid_width] = OBJECT_EMPTY;
                grid[(new_robot_x - 1) + new_robot_y * grid_width] = OBJECT_EMPTY;
                return 1;
            }
            assert(0);
        }

        return 0;
    }
}


int input_read_file(const char *path, object_e *grid, int grid_width, int grid_height, direction_e *instructions,
                    int instruction_lines,
                    int instructions_per_line, int *robot_x, int *robot_y) {
    FILE *input_file = fopen(path, "r");
    if (input_file == NULL) {
        perror("Could not read file");
        return 1;
    }

    for (int j = 0; j < grid_height; j++) {
        for (int i = 0; i < grid_width; i += 2) {
            int character = fgetc(input_file);
            switch (character) {
                case '#':
                    grid[(j * grid_width + i)] = OBJECT_WALL;
                    grid[(j * grid_width + i) + 1] = OBJECT_WALL;
                    break;
                case 'O':
                    grid[(j * grid_width + i)] = OBJECT_BOX_LEFT;
                    grid[(j * grid_width + i) + 1] = OBJECT_BOX_RIGHT;
                    break;
                case '@':
                    *robot_x = i;
                    *robot_y = j;
                    grid[(j * grid_width + i)] = OBJECT_EMPTY;
                    grid[(j * grid_width + i) + 1] = OBJECT_EMPTY;
                    break;
                case '.':
                    grid[(j * grid_width + i)] = OBJECT_EMPTY;
                    grid[(j * grid_width + i) + 1] = OBJECT_EMPTY;
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
