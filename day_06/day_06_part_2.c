#include <assert.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GRID_SIZE 130

typedef enum {
  DIRECTION_UP = 1,
  DIRECTION_RIGHT = 2,
  DIRECTION_DOWN = 4,
  DIRECTION_LEFT = 8,
} direction_e;

int input_read_file(const char *path, char *grid, int grid_size);
void find_initial_position(const char *grid, int grid_size, int *x, int *y);
int is_out_of_bounds(int x, int y, int grid_size);
int is_hazard(int x, int y, char *grid, int grid_size);

direction_e direction_turn_right(direction_e direction);

int is_loop(char *grid, int *visited_grid, int start_x, int start_y,
            int grid_size);

int main(void) {
  char grid[GRID_SIZE * GRID_SIZE];

  if (input_read_file("input.txt", grid, GRID_SIZE) != 0) {
    perror("Error reading input");
    return 1;
  }
  int count = 0;
  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++) {
      char edited_grid[GRID_SIZE * GRID_SIZE];
      int visited_grid[GRID_SIZE * GRID_SIZE] = {0};
      int start_x, start_y;

      memcpy(edited_grid, grid, GRID_SIZE * GRID_SIZE);
      find_initial_position(edited_grid, GRID_SIZE, &start_x, &start_y);
      if (i == start_x && start_y == j)
        continue;
      edited_grid[j * GRID_SIZE + i] = '#';

      count += is_loop(edited_grid, visited_grid, start_x, start_y, GRID_SIZE);
    }
  }

  printf("%d\n", count);

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
      grid[i * grid_size + j] = fgetc(input_file);
    }
    fgetc(input_file);
  }

  fclose(input_file);
  return 0;
}

int is_loop(char *grid, int *visited_grid, int start_x, int start_y,
            int grid_size) {
  int guard_x = start_x, guard_y = start_y;
  direction_e direction = DIRECTION_UP;

  while (1) {
    int new_position_x = guard_x, new_position_y = guard_y;

    switch (direction) {
    case DIRECTION_UP:
      new_position_y--;
      break;
    case DIRECTION_DOWN:
      new_position_y++;
      break;
    case DIRECTION_LEFT:
      new_position_x--;
      break;
    case DIRECTION_RIGHT:
      new_position_x++;
      break;
    }

    if (is_hazard(new_position_x, new_position_y, grid, GRID_SIZE)) {
      direction = direction_turn_right(direction);
      if (is_out_of_bounds(new_position_x, new_position_y, GRID_SIZE))
        return 0;
    } else {
      int index = guard_y * grid_size + guard_x;

      if ((visited_grid[index] & direction) > 0) {
        return 1;
      }

      visited_grid[index] |= direction;

      guard_x = new_position_x;
      guard_y = new_position_y;
    }
  }

  return 0;
}

void find_initial_position(const char *grid, int grid_size, int *x, int *y) {
  for (int i = 0; i < grid_size; i++) {
    for (int j = 0; j < grid_size; j++) {
      if (grid[i * grid_size + j] == '^') {
        *x = j;
        *y = i;
        return;
      }
    }
  }
  assert(0);
}

int is_out_of_bounds(int x, int y, int grid_size) {
  if (x < 0 || x >= grid_size)
    return 1;
  if (y < 0 || y >= grid_size)
    return 1;
  return 0;
}

int is_hazard(int x, int y, char *grid, int grid_size) {
  if (is_out_of_bounds(x, y, grid_size))
    return 1;
  return grid[y * grid_size + x] == '#';
}

direction_e direction_turn_right(direction_e direction) {
  switch (direction) {
  case DIRECTION_UP:
    return DIRECTION_RIGHT;
  case DIRECTION_DOWN:
    return DIRECTION_LEFT;
  case DIRECTION_RIGHT:
    return DIRECTION_DOWN;
  case DIRECTION_LEFT:
    return DIRECTION_UP;
  }
}
