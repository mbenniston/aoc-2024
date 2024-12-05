#include <ctype.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RULE_COUNT 1176
#define UPDATES_COUNT 200
#define MAX_PAGE_COUNT 200

typedef struct {
  int left;
  int right;
} rule_t;

typedef struct {
  int pages[MAX_PAGE_COUNT];
  int page_count;
} update_t;

int input_read_file(const char *path, rule_t *rules, int rule_count,
                    update_t *updates, int update_count);

int does_update_follow_rules(update_t *update, rule_t *rules, int rule_count);
int find_page(int *pages, int page_count, int page);

int main(void) {
  rule_t rules[RULE_COUNT] = {0};
  update_t updates[UPDATES_COUNT] = {0};
  if (input_read_file("input.txt", rules, RULE_COUNT, updates, UPDATES_COUNT) !=
      0) {
    perror("Error reading input");
    return 1;
  }

  int middle_total = 0;
  for (int u = 0; u < UPDATES_COUNT; u++) {
    update_t *update = &updates[u];
    update_t sorted_update = {0};

    if (!does_update_follow_rules(update, rules, RULE_COUNT)) {
      for (int i = 0; i < update->page_count; i++) {
        sorted_update.pages[sorted_update.page_count++] = update->pages[i];
        for (int j = sorted_update.page_count - 1; j > 0; j--) {
          if (does_update_follow_rules(&sorted_update, rules, RULE_COUNT)) {
            break;
          }
          int temp = sorted_update.pages[j];
          sorted_update.pages[j] = sorted_update.pages[j - 1];
          sorted_update.pages[j - 1] = temp;
        }
      }

      middle_total += sorted_update.pages[sorted_update.page_count / 2];
    }
  }
  printf("%d\n", middle_total);

  return 0;
}

int find_page(int *pages, int page_count, int page) {
  for (int i = 0; i < page_count; i++) {
    if (pages[i] == page)
      return i;
  }
  return -1;
}

int does_update_follow_rules(update_t *update, rule_t *rules, int rule_count) {
  for (int r = 0; r < rule_count; r++) {
    rule_t rule = rules[r];
    int left_index = find_page(update->pages, update->page_count, rule.left);
    int right_index = find_page(update->pages, update->page_count, rule.right);

    if (left_index != -1 && right_index != -1 && left_index > right_index) {
      return 0;
    }
  }

  return 1;
}

int input_read_file(const char *path, rule_t *rules, int rule_count,
                    update_t *updates, int update_count) {
  FILE *input_file = fopen(path, "r");
  if (input_file == NULL) {
    perror("Could not read file");
    return 1;
  }

  // read rules

  for (int i = 0; i < rule_count; i++) {
    fscanf(input_file, "%d|%d\n", &rules[i].left, &rules[i].right);
  }

  fscanf(input_file, "\n");

  // read updates

  for (int i = 0; i < update_count; i++) {
    update_t *update = &updates[i];
    char buffer[2000];
    fgets(buffer, sizeof(buffer), input_file);
    char *token = strtok(buffer, ",");
    update->pages[update->page_count++] = atoi(token);
    while (token = strtok(NULL, ",")) {
      update->pages[update->page_count++] = atoi(token);
    }
  }

  fclose(input_file);
  return 0;
}