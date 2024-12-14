#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_MACHINES 10000

typedef struct {
    int button_a_x;
    int button_a_y;
    int button_b_x;
    int button_b_y;
    int prize_x;
    int prize_y;
} claw_machine_t;

int input_read_file(const char *path, claw_machine_t *machines, int *machines_count, int max_machines);

int solve_Nb(long long Xa, long long Ya, long long Xb, long long Yb, long long Sx, long long Sy, long long *Nb);

long long solve_Na(long long Nb, long long Xa, long long Xb, long long Sx);

claw_machine_t read_claw_machine(FILE *file);

int main(void) {
    claw_machine_t machines[MAX_MACHINES];
    int machine_count = 0;
    if (input_read_file("input.txt", machines, &machine_count, MAX_MACHINES) != 0) {
        perror("Error reading input");
        return 1;
    }

    long long total_cost = 0;
    for (int i = 0; i < machine_count; i++) {
        const claw_machine_t machine = machines[i];
        long long Nb;
        long long new_prize_x = machine.prize_x + 10000000000000ll;
        long long new_prize_y = machine.prize_y + 10000000000000ll;

        if (solve_Nb(machine.button_a_x, machine.button_a_y, machine.button_b_x, machine.button_b_y,
                     new_prize_x,
                     new_prize_y, &Nb)) {
            long long Na = solve_Na(Nb, machine.button_a_x, machine.button_b_x, new_prize_x);
            long long expected_x = Na * machine.button_a_x + Nb * machine.button_b_x;
            long long expected_y = Na * machine.button_a_y + Nb * machine.button_b_y;

            if (expected_x != new_prize_x || expected_y != new_prize_y) {
                continue;
            }

            total_cost += Nb + Na * 3;
        }
    }

    printf("%lld\n", total_cost);

    return 0;
}

int solve_Nb(long long Xa, long long Ya, long long Xb, long long Yb, long long Sx, long long Sy, long long *Nb) {
    long long top = Sy * Xa - Sx * Ya;
    long long bottom = Yb * Xa - Xb * Ya;

    if (top % bottom != 0) return 0;

    *Nb = top / bottom;

    return 1;
}

long long solve_Na(long long Nb, long long Xa, long long Xb, long long Sx) {
    return Sx / Xa - Nb * Xb / Xa;
}

claw_machine_t read_claw_machine(FILE *file) {
    claw_machine_t claw_machine = {0};
    fscanf(file, "Button A: X+%d, Y+%d\n", &claw_machine.button_a_x, &claw_machine.button_a_y);
    fscanf(file, "Button B: X+%d, Y+%d\n", &claw_machine.button_b_x, &claw_machine.button_b_y);
    fscanf(file, "Prize: X=%d, Y=%d\n", &claw_machine.prize_x, &claw_machine.prize_y);
    fscanf(file, "\n");
    return claw_machine;
}

int input_read_file(const char *path, claw_machine_t *machines, int *machines_count, int max_machines) {
    FILE *input_file = fopen(path, "r");
    if (input_file == NULL) {
        perror("Could not read file");
        return 1;
    }

    while (!feof(input_file)) {
        assert(*machines_count < MAX_MACHINES);
        machines[(*machines_count)++] = read_claw_machine(input_file);
    }

    fclose(input_file);
    return 0;
}
