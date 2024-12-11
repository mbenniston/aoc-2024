#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STONES_COUNT 5531200

int apply_rule(long long *stones, int *num_stones, int stone_index);

int main(void) {
    long long *stones = malloc(MAX_STONES_COUNT * sizeof(long long));
    stones[0] = 5;
    stones[1] = 89749;
    stones[2] = 6061;
    stones[3] = 43;
    stones[4] = 867;
    stones[5] = 1965860;
    stones[6] = 0;
    stones[7] = 206250;

    int stone_count = 8;
    for (int i = 0; i < 25; ++i) {
        for (int j = 0; j < stone_count; j++) {
            if (apply_rule(stones, &stone_count, j)) j++;
        }
    }

    free(stones);
    printf("%d\n", stone_count);

    return 0;
}


int apply_rule(long long *stones, int *num_stones, int stone_index) {
    if (stones[stone_index] == 0) {
        stones[stone_index] = 1;
        return 0;
    }
    {
        char buffer[200];
        sprintf(buffer, "%lld", stones[stone_index]);
        int num_digits = strlen(buffer);
        if (num_digits % 2 == 0) {
            (*num_stones)++;
            for (int i = (*num_stones - 1); i > stone_index + 1; --i) {
                stones[i] = stones[i - 1];
            }
            char new_buffer[200];
            strncpy(new_buffer, buffer, num_digits / 2);
            stones[stone_index + 1] = atoll(buffer + num_digits / 2);
            stones[stone_index] = atoll(new_buffer);
            return 1;
        }
    }

    stones[stone_index] = stones[stone_index] * 2024;
    return 0;
}
