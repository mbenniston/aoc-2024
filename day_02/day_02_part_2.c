#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_COUNT (1000)
#define MAX_NUMBER_COUNT (8)

void input_read_line(FILE *file, int *line, int *count);

void input_read_lines(FILE *file, int *lines, int *number_counts,
                      int line_count);

int input_read_file(const char *path, int *lines, int *number_counts,
                    int line_count);

int is_safe(int *line, int count);
int is_safe_with_problem_dampener(int *line, int count);

int compare_ints(const void *left, const void *right);

int main(void) {
  int lines[MAX_NUMBER_COUNT * LINE_COUNT] = {0};
  int line_counts[LINE_COUNT] = {0};

  if (input_read_file("input.txt", lines, line_counts, LINE_COUNT) != 0) {
    perror("Error reading input");
    return 1;
  }

  int num_safe_lines = 0;
  for (int i = 0; i < LINE_COUNT; i++) {
    int *line = &lines[i * MAX_NUMBER_COUNT];
    int count = line_counts[i];

    if (is_safe_with_problem_dampener(line, count)) {
      num_safe_lines++;
    }
  }
  printf("Num safe reports: %d\n", num_safe_lines);

  return 0;
}

int is_safe_with_problem_dampener(int *line, int count) {
  if (is_safe(line, count))
    return 1;

  for (int i = 0; i < count; i++) {
    int new_line[MAX_NUMBER_COUNT];
    int y = 0;
    for (int j = 0; j < count - 1; j++) {
      if (y == i)
        y++;
      new_line[j] = line[y++];
    }

    if (is_safe(new_line, count - 1))
      return 1;
  }

  return 0;
}

int is_safe(int *line, int count) {
  int is_increasing = 0;

  for (int i = 0; i < count - 1; i++) {
    int current = line[i];
    int next = line[i + 1];

    int trend = next - current;
    int diff = abs(trend);

    if (diff == 0 || diff > 3)
      return 0;
    if (i > 0) {
      if ((is_increasing && trend < 0) || (!is_increasing && trend > 0))
        return 0;

    } else {
      is_increasing = trend > 0;
    }
  }

  return 1;
}

int compare_ints(const void *left, const void *right) {
  int left_int = *(const int *)left;
  int right_int = *(const int *)right;

  if (left_int < right_int)
    return -1;
  if (left_int > right_int)
    return 1;
  return 0;
}

void input_read_line(FILE *file, int *line, int *count) {
  char buffer[1024];
  *count = 0;
  fgets(buffer, sizeof(buffer), file);
  char *token = strtok(buffer, " ");
  while (token != NULL) {
    line[(*count)++] = atoi(token);
    token = strtok(NULL, " ");
  }
}

void input_read_lines(FILE *file, int *lines, int *number_counts,
                      int line_count) {
  int current_line = 0;
  while (!feof(file) && current_line < line_count) {
    input_read_line(file, lines + current_line * MAX_NUMBER_COUNT,
                    &number_counts[current_line]);
    current_line++;
  }
}

int input_read_file(const char *path, int *lines, int *number_counts,
                    int line_count) {
  FILE *input_file = fopen(path, "r");
  if (input_file == NULL) {
    perror("Could not read file");
    return 1;
  }

  input_read_lines(input_file, lines, number_counts, line_count);
  fclose(input_file);
  return 0;
}
