#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CACHE_SIZE 2000000

typedef struct {
    long long *stones;
    int stones_count;
    int *iteration_count;
    long long *stones_create_count;
} attempt_cache_t;

void add_to_cache(attempt_cache_t *cache, long long stone, long long count, int iterations);

int get_cache_index(attempt_cache_t *cache, long long stone, int count);

long long get_stones_count(attempt_cache_t *cache, long long start_stone, int iterations);

int main(void) {
    long long stones[] = {5, 89749, 6061, 43, 867, 1965860, 0, 206250,};
    long long total = 0;

    attempt_cache_t cache;
    cache.stones = malloc(MAX_CACHE_SIZE * sizeof(long long));
    cache.iteration_count = malloc(MAX_CACHE_SIZE * sizeof(int));
    cache.stones_create_count = malloc(MAX_CACHE_SIZE * sizeof(long long));
    cache.stones_count = 0;

    for (int k = 0; k < sizeof(stones) / sizeof(long long); ++k) {
        total += get_stones_count(&cache, stones[k], 75);
    }

    free(cache.stones);
    free(cache.iteration_count);
    free(cache.stones_create_count);

    printf("%lld\n", total);

    return 0;
}

char new_buffer[200] = {0};
char buffer[200] = {0};

long long get_stones_count(attempt_cache_t *cache, long long start_stone, int iterations) {
    int cache_index = get_cache_index(cache, start_stone, iterations);
    if (cache_index != -1) {
        return cache->stones_create_count[cache_index];
    }

    if (iterations == 0) return 1;

    if (start_stone == 0) {
        long long count = get_stones_count(cache, 1, iterations - 1);
        add_to_cache(cache, start_stone, count, iterations);
        return count;
    }
    {
        memset(new_buffer, 0, 200);
        memset(buffer, 0, 200);
        sprintf(buffer, "%lld", start_stone);
        int num_digits = strlen(buffer);
        if (num_digits % 2 == 0) {
            strncpy(new_buffer, buffer, num_digits / 2);
            long long lnumber = atoll(buffer + num_digits / 2);
            long long rnumber = atoll(new_buffer);
            long long count =
                    get_stones_count(cache, lnumber, iterations - 1) + get_stones_count(cache, rnumber, iterations - 1);
            add_to_cache(cache, start_stone, count, iterations);
            return count;

        }
    }

    {
        long long count =
                get_stones_count(cache, start_stone * 2024, iterations - 1);
        add_to_cache(cache, start_stone, count, iterations);
        return count;
    }
}


void add_to_cache(attempt_cache_t *cache, long long stone, long long count, int iterations) {
    if (cache->stones_count >= MAX_CACHE_SIZE) {
        puts("cache full");
        return;
    }
    int new_index = cache->stones_count++;
    cache->stones[new_index] = stone;
    cache->stones_create_count[new_index] = count;
    cache->iteration_count[new_index] = iterations;
}

int get_cache_index(attempt_cache_t *cache, long long stone, int count) {
    for (int i = 0; i < cache->stones_count; ++i) {
        if (cache->stones[i] == stone && cache->iteration_count[i] == count) return i;
    }
    return -1;
}

