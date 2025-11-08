#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

#define MAX_INPUT 200

enum Command {
    QUIT,
    DIE,
    REPEAT,
    HELP,
};

struct Roll {
    bool valid;
    int count;
    int type;
};

enum Command parseLine(char *line);
int parseDice(char *line);
struct Roll getDie(char *token);
int rollDie(struct Roll roll);

int main(void) {
    srand(time(NULL));

    char buffer[MAX_INPUT];
    char last_die[MAX_INPUT] = "1d6\n";
    char tmp_buffer[MAX_INPUT];

    printf("Welcome to Dice\n");
    printf("Type h for help\n");
    printf("> ");

    while (fgets(buffer, MAX_INPUT, stdin) != NULL) {
        int diesum;
        switch (parseLine(buffer)) {
            case QUIT:
                return 0;

            case HELP:
                printf("Commands:\n");
                printf("\th\tPrint this message\n");
                printf("\tq\tQuit program\n");
                printf("\t[?count]d[dietype]...\tRolls die and prints the summed value, can handle multiple rolls\n");
                printf("\t.\tRerolls the last valid die\n");
                break;

            case DIE:
                strcpy(tmp_buffer, buffer);
                diesum = parseDice(buffer);

                if (diesum == 0) {
                    printf("Invalid Command\n");
                    break;
                }

                strcpy(last_die, tmp_buffer);
                printf("%d\n", diesum);
                break;

            case REPEAT:
                printf("Last Roll: %s", last_die);
                strcpy(buffer, last_die);
                diesum = parseDice(buffer);

                printf("%d\n", diesum);
                break;
        }
        printf("> ");
    }

    return 0;
}

enum Command parseLine(char *line) {
    switch (line[0]) {
        case 'q':
            return QUIT;
        case 'h':
            return HELP;
        case '.':
            return REPEAT;
        default:
            return DIE;
    }
}

int parseDice(char *line) {
    const char delims[] = " \t\n";

    char *token = strtok(line, delims);
    if (token == NULL) {
        return 0;
    }

    int sum = 0;
    struct Roll current_roll;

    while (token != NULL) {
        if (!(current_roll = getDie(token)).valid) {
            return 0;
        }
        sum += rollDie(current_roll);
        token = strtok(NULL, delims);
    }

    return sum;
}

struct Roll getDie(char *token) {
    struct Roll ret = {false, 0, 0};

    int str_pos = 0;
    while (isdigit(token[str_pos])) {
        ++str_pos;
    }

    if (str_pos == 0) {
        ret.count = 1;
    } else {
        char count_str[str_pos + 1];
        for (int i = 0; i < str_pos; ++i) {
            count_str[i] = token[i];
        }
        count_str[str_pos] = '\0';
        ret.count = atoi(count_str);
        if (ret.count <= 0) return ret;
    }

    if (token[str_pos] == 'd') {
        ++str_pos;
    } else {
        return ret;
    }

    ret.type = atoi(token + str_pos);
    if (ret.type <= 0) {
        return ret;
    }

    ret.valid = true;
    return ret;
}

int rollDie(struct Roll roll) {
    int sum = 0;
    for (int i = 0; i < roll.count; ++i) {
        sum += rand() % roll.type + 1;
    }
    return sum;
}
