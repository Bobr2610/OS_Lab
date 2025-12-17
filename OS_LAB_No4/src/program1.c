#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

static void write_str(int fd, const char* str) {
    write(fd, str, strlen(str));
}

static void write_int(int fd, int num) {
    char buf[32];
    int len = snprintf(buf, sizeof(buf), "%d", num);
    write(fd, buf, len);
}

static void write_float(int fd, float num) {
    char buf[32];
    int len = snprintf(buf, sizeof(buf), "%.4f", num);
    write(fd, buf, len);
}

static void write_newline(int fd) {
    write(fd, "\n", 1);
}

static ssize_t read_line(char* buffer, size_t size) {
    ssize_t total_read = 0;
    while (total_read < size - 1) {
        char c;
        ssize_t n = read(0, &c, 1);
        if (n <= 0) {
            return -1;
        }
        if (c == '\n') {
            break;
        }
        buffer[total_read++] = c;
    }
    buffer[total_read] = '\0';
    return total_read;
}

static int parse_int(const char* str, int* result) {
    if (!str || !*str) return 0;
    *result = atoi(str);
    return 1;
}

static int parse_float(const char* str, float* result) {
    if (!str || !*str) return 0;
    *result = atof(str);
    return 1;
}

static int tokenize(char* str, char* tokens[], int max_tokens) {
    int count = 0;
    char* token = strtok(str, " \t");
    while (token && count < max_tokens) {
        tokens[count++] = token;
        token = strtok(NULL, " \t");
    }
    return count;
}

int main(void) {
    write_str(1, "Commands: 1 a b | 2 a b | q\n");

    char input[256];

    while (1) {
        write_str(1, "> ");

        if (read_line(input, sizeof(input)) < 0) {
            break;
        }

        if (input[0] == 'q' || input[0] == 'Q') {
            break;
        }

        char* tokens[4];
        int token_count = tokenize(input, tokens, 4);

        if (token_count == 0) {
            write_str(1, "Error\n");
            continue;
        }

        int command;
        if (!parse_int(tokens[0], &command)) {
            write_str(1, "Error\n");
            continue;
        }

        switch (command) {
            case 1: {
                if (token_count != 3) {
                    write_str(1, "Error\n");
                    break;
                }
                int a_int, b_int;
                if (!parse_int(tokens[1], &a_int) || !parse_int(tokens[2], &b_int)) {
                    write_str(1, "Error\n");
                    break;
                }
                if (a_int < 1 || b_int < 1) {
                    write_str(1, "Error\n");
                    break;
                }
                int count = prime_count(a_int, b_int);
                write_int(1, count);
                write_newline(1);
                break;
            }

            case 2: {
                if (token_count != 3) {
                    write_str(1, "Error\n");
                    break;
                }
                float a_float, b_float;
                if (!parse_float(tokens[1], &a_float) || !parse_float(tokens[2], &b_float)) {
                    write_str(1, "Error\n");
                    break;
                }
                float result = area(a_float, b_float);
                write_float(1, result);
                write_newline(1);
                break;
            }

            default:
                write_str(1, "Error\n");
                break;
        }
    }

    return 0;
}

