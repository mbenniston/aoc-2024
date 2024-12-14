#include <stdio.h>
#include <stdlib.h>

#define GUARD_COUNT 500
#define GRID_WIDTH 101
#define GRID_HEIGHT 103
#define SIMULATE_SECONDS 10000

typedef struct {
    int position_x;
    int position_y;
    int velocity_x;
    int velocity_y;
} guard_t;

int input_read_file(const char *path, guard_t *guards, int guard_count);

void print_grid(guard_t *guards, int guard_count, int grid_width, int grid_height);

int main(void) {
    guard_t guards[GUARD_COUNT];
    if (input_read_file("input.txt", guards, GUARD_COUNT) != 0) {
        perror("Error reading input");
        return 1;
    }

    for (int s = 0; s < SIMULATE_SECONDS; s++) {
        for (int i = 0; i < GUARD_COUNT; i++) {
            guards[i].position_x += guards[i].velocity_x;
            guards[i].position_y += guards[i].velocity_y;
            guards[i].position_x %= GRID_WIDTH;
            guards[i].position_y %= GRID_HEIGHT;

            if (guards[i].position_x < 0) {
                guards[i].position_x += GRID_WIDTH;
            }

            if (guards[i].position_y < 0) {
                guards[i].position_y += GRID_HEIGHT;
            }
        }

        print_grid(guards, GUARD_COUNT, GRID_WIDTH, GRID_HEIGHT);
        printf("%d\n", s);
    }

    return 0;
}

void print_grid(guard_t *guards, int guard_count, int grid_width, int grid_height) {
    for (int i = 0; i < grid_height; i++) {
        for (int j = 0; j < grid_width; j++) {
            int has_guard = 0;
            for (int n = 0; n < guard_count; n++) {
                if (guards[n].position_x == j && guards[n].position_y == i) {
                    has_guard = 1;
                    break;
                }
            }
            if (has_guard) {
                printf("X");
            } else
                printf(" ");
        }
        printf("\n");
    }
}

int input_read_file(const char *path, guard_t *guards, int guard_count) {
    FILE *input_file = fopen(path, "r");
    if (input_file == NULL) {
        perror("Could not read file");
        return 1;
    }

    for (int i = 0; i < guard_count; i++) {
        guard_t *guard = guards + i;
        fscanf(input_file, "p=%d,%d v=%d,%d\n", &guard->position_x, &guard->position_y, &guard->velocity_x,
               &guard->velocity_y);
    }

    fclose(input_file);
    return 0;
}
