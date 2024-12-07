#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_COUNT 850
#define MAX_READINGS 40

typedef struct {
    long long result;
    long long readings[MAX_READINGS];
    int readings_count;
} calibration_t;

int input_read_file(const char *path, calibration_t *calibrations, int line_count);

int is_readings_possible(long long result, long long *readings, int readings_count);

int is_calibration_possible(calibration_t *calibration);

int main(void) {
    calibration_t calibrations[LINE_COUNT] = {0};

    if (input_read_file("input.txt", calibrations, LINE_COUNT) != 0) {
        perror("Error reading input");
        return 1;
    }

    long long total = 0;
    for (int i = 0; i < LINE_COUNT; ++i) {
        calibration_t *calibration = &calibrations[i];
        if (is_calibration_possible(calibration)) {
            total += calibration->result;
        }
    }
    printf("%lld\n", total);

    return 0;
}

int input_read_file(const char *path, calibration_t *calibrations, int line_count) {
    FILE *input_file = fopen(path, "r");
    if (input_file == NULL) {
        perror("Could not read file");
        return 1;
    }

    for (int i = 0; i < line_count; i++) {
        calibration_t *calibration = &calibrations[i];
        char buffer[2000];
        fgets(buffer, sizeof(buffer), input_file);
        char *token = strtok(buffer, ":");
        calibration->result = atoll(token);
        while (token = strtok(NULL, " ")) {
            calibration->readings[calibration->readings_count++] = atoll(token);
        }
    }

    fclose(input_file);
    return 0;
}

int is_readings_possible(long long result, long long *readings, int readings_count) {
    if (readings_count == 0 && result == 0) return 1;
    if (readings_count == 0) return 0;

    long long last_reading = readings[readings_count - 1];

    char result_str[100];
    char last_reading_str[100];
    sprintf(result_str, "%lld", result);
    sprintf(last_reading_str, "%lld", last_reading);
    unsigned long result_string_str_len = strlen(result_str);
    unsigned long last_reading_str_len = strlen(last_reading_str);

    if (result_string_str_len > last_reading_str_len &&
        strcmp(result_str + result_string_str_len - last_reading_str_len, last_reading_str) == 0) {
        result_str[result_string_str_len - last_reading_str_len] = '\0';
        long long new_result = atoll(result_str);
        if (is_readings_possible(new_result, readings, readings_count - 1))
            return 1;
    }
    if (result % last_reading == 0 && is_readings_possible(result / last_reading, readings, readings_count - 1))
        return 1;
    if ((result - last_reading) >= 0 && is_readings_possible(result - last_reading, readings, readings_count - 1))
        return 1;

    return 0;
}

int is_calibration_possible(calibration_t *calibration) {
    return is_readings_possible(calibration->result, calibration->readings, calibration->readings_count);
}

