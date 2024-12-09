#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 19999
#define MAX_EXPAND_LENGTH 200000

int input_read_file(const char *path, char *line, int max_length);

int main(void) {
    char data[MAX_LINE_LENGTH] = {'.'};
    int data_expanded[MAX_EXPAND_LENGTH] = {'.'};
    int data_length = MAX_LINE_LENGTH;

    if (input_read_file("input.txt", data, MAX_LINE_LENGTH) != 0) {
        perror("Error reading input");
        return 1;
    }

    int data_length_expanded_length = 0;
    for (int j = 0; j < data_length; j += 2) {
        int num_taken = data[j] - '0';
        int num_free = data[j + 1] - '0';

        while (num_taken > 0) {
            data_expanded[data_length_expanded_length++] = j / 2;
            num_taken--;
            assert(data_length_expanded_length <= MAX_EXPAND_LENGTH);
        }
        while (num_free > 0) {
            data_expanded[data_length_expanded_length++] = -1;
            num_free--;
            assert(data_length_expanded_length <= MAX_EXPAND_LENGTH);
        }
    }

    for (int i = data_length_expanded_length - 1; i >= 0; i--) {
        if (data_expanded[i] != -1) {
            for (int j = 0; j < i; j++) {
                if (data_expanded[j] == -1) {
                    data_expanded[j] = data_expanded[i];
                    data_expanded[i] = -1;
                    break;
                }
            }
        }
    }

    unsigned long long check_sum = 0;

    for (long long i = 0; i < data_length_expanded_length; i++) {
        if (data_expanded[i] != -1) {
            check_sum += (data_expanded[i]) * i;
        }
    }

    printf("%lld %d\n", check_sum, data_length);

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
