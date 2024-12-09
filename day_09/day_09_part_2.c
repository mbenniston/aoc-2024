#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 19999
#define MAX_BLOCK_COUNT 19999
#define MAX_EXPAND_LENGTH 200000

typedef struct {
    int used_count;
    int free_count;
    int id;
} block_t;

int input_read_file(const char *path, char *line, int max_length);

void parse_blocks(char *line, int line_length, block_t *blocks, int *block_count, int max_block_count);

int main(void) {
    char data[MAX_LINE_LENGTH] = {'.'};
    if (input_read_file("input.txt", data, MAX_LINE_LENGTH) != 0) {
        perror("Error reading input");
        return 1;
    }

    block_t blocks[MAX_BLOCK_COUNT] = {0};
    int block_count = 0;
    parse_blocks(data, MAX_LINE_LENGTH, blocks, &block_count, MAX_BLOCK_COUNT);

    for (int i = block_count - 1; i >= 1;) {
        block_t *block_to_merge = &blocks[i];
        int merged = 0;
        for (int j = 0; j < i; ++j) {
            block_t *merge_candidate = &blocks[j];
            if (merge_candidate->free_count >= block_to_merge->used_count) {
                blocks[i - 1].free_count += block_to_merge->used_count + block_to_merge->free_count;

                block_t new_block;
                new_block.id = block_to_merge->id;
                new_block.free_count = merge_candidate->free_count - block_to_merge->
                                       used_count;
                new_block.used_count = block_to_merge->used_count;
                merge_candidate->free_count = 0;

                for (int k = i; k > j + 1; --k) {
                    blocks[k] = blocks[k - 1];
                }
                blocks[j + 1] = new_block;
                merged = 1;
                break;
            }
        }
        if (!merged) {
            i--;
        }
    }

    unsigned long long check_sum = 0;

    long long data_length_expanded_length = 0;

    for (int j = 0; j < block_count; j++) {
        int num_taken = blocks[j].used_count;
        int num_free = blocks[j].free_count;
        int id = blocks[j].id;

        if (num_taken > 0) {
            long long start = id * num_taken * data_length_expanded_length;
            long long sum = id * ((num_taken - 1) * num_taken) / 2;
            check_sum += start + sum;
        }
        data_length_expanded_length += num_taken + num_free;
    }

    printf("%lld\n", check_sum);

    return 0;
}

int input_read_file(const char *path, char *line, int max_length) {
    FILE *input_file = fopen(path, "r");
    if (input_file == NULL) {
        perror("Could not read file");
        return 1;
    }

    fread(line, 1, max_length, input_file);
    fclose(input_file);
    return 0;
}

void parse_blocks(char *line, int line_length, block_t *blocks, int *block_count, int max_block_count) {
    for (int j = 0; j < line_length; j += 2) {
        int num_taken = line[j] - '0';
        int num_free = j + 1 < line_length ? line[j + 1] - '0' : 0;

        int index = (*block_count)++;
        blocks[index].used_count = num_taken;
        blocks[index].free_count = num_free;
        blocks[index].id = j / 2;
        assert(index < max_block_count);
    }
}

