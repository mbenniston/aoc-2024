#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define GUARD_COUNT 500
#define GRID_WIDTH 101
#define GRID_HEIGHT 103
#define SIMULATE_SECONDS 100

typedef struct {
    int position_x;
    int position_y;
    int velocity_x;
    int velocity_y;
} guard_t;

int input_read_file(const char *path, guard_t *guards, int guard_count);

int main(void) {
    guard_t guards[GUARD_COUNT];
    if (input_read_file("input.txt", guards, GUARD_COUNT) != 0) {
        perror("Error reading input");
        return 1;
    }

    int quadrants[4] = {0};

    for (int i = 0; i < GUARD_COUNT; i++) {
        guards[i].position_x += guards[i].velocity_x * SIMULATE_SECONDS;
        guards[i].position_y += guards[i].velocity_y * SIMULATE_SECONDS;
        guards[i].position_x %= GRID_WIDTH;
        guards[i].position_y %= GRID_HEIGHT;

        if (guards[i].position_x < 0) {
            guards[i].position_x += GRID_WIDTH;
        }

        if (guards[i].position_y < 0) {
            guards[i].position_y += GRID_HEIGHT;
        }

        if (guards[i].position_x == GRID_WIDTH / 2 || guards[i].position_y == GRID_HEIGHT / 2) continue;

        int index = 0;
        if (guards[i].position_x < GRID_WIDTH / 2) index |= 1;
        if (guards[i].position_y < GRID_HEIGHT / 2) index |= 2;

        quadrants[index]++;
    }


    long total = 1;
    for (int i = 0; i < sizeof(quadrants) / sizeof(int); i++) {
        total *= quadrants[i];
    }
    printf("%ld\n", total);

    return 0;
}


int input_read_file(const char *path, guard_t *guards, int guard_count) {
    FILE *input_file = fopen(path, "r");
    if (input_file == NULL) {
        perror("Could not read file");
        return 1;
    }

    for (int i = 0; i < GUARD_COUNT; i++) {
        guard_t *guard = guards + i;
        fscanf(input_file, "p=%d,%d v=%d,%d\n", &guard->position_x, &guard->position_y, &guard->velocity_x,
               &guard->velocity_y);
    }

    fclose(input_file);
    return 0;
}
