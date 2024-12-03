#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int input_read_file(const char *path, char **contents, int *length);

int consume_char(const char *input, int length, int *offset, char c);

int consume_digit(const char *input, int length, int *offset, char *out_digit);

int consume_num(const char *input, int length, int *offset, int *out_num, char terminator);

int consume_mul(const char *input, int length, int *offset, int *left, int *right);


int main(void) {
    char *contents = NULL;
    int contents_length = 0;
    if (input_read_file("input.txt", &contents, &contents_length) != 0) {
        perror("Error reading input");
        return 1;
    }

    int total = 0;
    for (int i = 0; i < contents_length;) {
        int new_offset = i;
        int left, right;
        if (consume_mul(contents, contents_length, &new_offset, &left, &right)) {
            total += left * right;
            i = new_offset;
        } else {
            i++;
        }
    }

    printf("Total: %d\n", total);

    free(contents);

    return 0;
}

int input_read_file(const char *path, char **contents, int *length) {
    FILE *input_file = fopen(path, "r");
    if (input_file == NULL) {
        perror("Could not read file");
        return 1;
    }

    fseek(input_file, 0, SEEK_END);
    *length = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);
    *contents = malloc(*length);
    fread(*contents, 1, *length, input_file);
    fclose(input_file);

    return 0;
}

int consume_char(const char *input, int length, int *offset, char c) {
    if (*offset > length) return 0;
    char in_c = input[*offset];
    if (in_c != c) return 0;

    (*offset)++;
    return 1;
}

int consume_digit(const char *input, int length, int *offset, char *out_digit) {
    if (*offset > length) return 0;
    char in_c = input[*offset];
    if (!isdigit(in_c)) return 0;
    *out_digit = in_c;

    (*offset)++;
    return 1;
}

int consume_num(const char *input, int length, int *offset, int *out_num, char terminator) {
    *out_num = 0;
    char digits[3] = {0};
    int num_digits = 0;

    if (!consume_digit(input, length, offset, &digits[0])) return 0;
    num_digits++;
    if (consume_digit(input, length, offset, &digits[1])) {
        num_digits++;
    }
    if (consume_digit(input, length, offset, &digits[2])) {
        num_digits++;
    }
    if (!consume_char(input, length, offset, terminator)) return 0;

    for (int i = 0; i < num_digits; i++) {
        *out_num = *out_num * 10 + (digits[i] - '0');
    }

    return 1;
}


int consume_mul(const char *input, int length, int *offset, int *left, int *right) {
    if (!consume_char(input, length, offset, 'm')) return 0;
    if (!consume_char(input, length, offset, 'u')) return 0;
    if (!consume_char(input, length, offset, 'l')) return 0;
    if (!consume_char(input, length, offset, '(')) return 0;

    if (!consume_num(input, length, offset, left, ',')) return 0;
    if (!consume_num(input, length, offset, right, ')')) return 0;

    return 1;
}
