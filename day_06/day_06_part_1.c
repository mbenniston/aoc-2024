#include <assert.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GRID_SIZE 130

typedef enum {
  DIRECTION_UP,
  DIRECTION_RIGHT,
  DIRECTION_DOWN,
  DIRECTION_LEFT,
} direction_e;

int input_read_file(const char *path, char *grid, int grid_size);
void find_initial_position(const char *grid, int grid_size, int *x, int *y);
int is_out_of_bounds(int x, int y, int grid_size);
int is_hazard(int x, int y, char *grid, int grid_size);
int count_visited(int *grid, int grid_size);
direction_e direction_turn_right(direction_e direction);

int main(void) {
  char grid[GRID_SIZE * GRID_SIZE];
  int visited_grid[GRID_SIZE * GRID_SIZE] = {0};

  if (input_read_file("input.txt", grid, GRID_SIZE) != 0) {
    perror("Error reading input");
    return 1;
  }

  int guard_x, guard_y;
  direction_e direction = DIRECTION_UP;

  find_initial_position(grid, GRID_SIZE, &guard_x, &guard_y);
  visited_grid[guard_y * GRID_SIZE + guard_x] = 1;

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
        break;
    } else {
      guard_x = new_position_x;
      guard_y = new_position_y;
    }
    visited_grid[guard_y * GRID_SIZE + guard_x] = 1;
  }

  printf("%d\n", count_visited(visited_grid, GRID_SIZE));

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

int count_visited(int *grid, int grid_size) {
  int visited = 0;
  for (int i = 0; i < grid_size * grid_size; i++) {
    if (grid[i]) {
      visited++;
    }
  }
  return visited;
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