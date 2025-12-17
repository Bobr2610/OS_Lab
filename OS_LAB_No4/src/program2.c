#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

typedef int (*prime_count_func)(int, int);
typedef float (*area_func)(float, float);

static const char *LIB1_PATH = "./libfunctions1.so";
static const char *LIB2_PATH = "./libfunctions2.so";

static void *current_lib = NULL;
static int current_lib_num = 0;

static prime_count_func fn_prime_count = NULL;
static area_func fn_area = NULL;

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

static int load_library(const char *path, int lib_num) {
    if (current_lib != NULL) {
        dlclose(current_lib);
        current_lib = NULL;
        fn_prime_count = NULL;
        fn_area = NULL;
    }

    current_lib = dlopen(path, RTLD_NOW);
    if (current_lib == NULL) {
        char error_buf[256];
        const char* dl_error = dlerror();
        if (dl_error) {
            int len = snprintf(error_buf, sizeof(error_buf), "Error loading library %s: %s\n", path, dl_error);
            write(2, error_buf, len);
        }
        return -1;
    }

    dlerror();

    fn_prime_count = (prime_count_func)dlsym(current_lib, "prime_count");
    const char *error = dlerror();
    if (error != NULL) {
        char error_buf[256];
        int len = snprintf(error_buf, sizeof(error_buf), "Error loading prime_count: %s\n", error);
        write(2, error_buf, len);
        dlclose(current_lib);
        current_lib = NULL;
        return -1;
    }

    fn_area = (area_func)dlsym(current_lib, "area");
    error = dlerror();
    if (error != NULL) {
        char error_buf[256];
        int len = snprintf(error_buf, sizeof(error_buf), "Error loading area: %s\n", error);
        write(2, error_buf, len);
        dlclose(current_lib);
        current_lib = NULL;
        fn_prime_count = NULL;
        return -1;
    }

    current_lib_num = lib_num;
    return 0;
}

static void switch_library(void) {
    const char *path;
    int new_lib_num;

    if (current_lib_num != 2) {
        path = LIB2_PATH;
        new_lib_num = 2;
    } else {
        path = LIB1_PATH;
        new_lib_num = 1;
    }

    if (load_library(path, new_lib_num) == 0) {
        write_str(1, "Library ");
        write_int(1, new_lib_num);
        write_newline(1);
    }
}

static void print_current_lib_info(void) {
    if (current_lib_num == 1) {
        write_str(1, "Lib 1");
    } else if (current_lib_num == 2) {
        write_str(1, "Lib 2");
    } else {
        write_str(1, "No lib");
    }
}

int main(void) {
    write_str(1, "Commands: 0 | 1 a b | 2 a b | q\n");

    if (load_library(LIB1_PATH, 1) != 0) {
        write_str(2, "Error\n");
        return 1;
    }

    char input[256];

    while (1) {
        print_current_lib_info();
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
            case 0:
                switch_library();
                break;

            case 1:
                if (fn_prime_count == NULL) {
                    write_str(1, "Error\n");
                    break;
                }
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
                int count = fn_prime_count(a_int, b_int);
                write_int(1, count);
                write_newline(1);
                break;

            case 2:
                if (fn_area == NULL) {
                    write_str(1, "Error\n");
                    break;
                }
                if (token_count != 3) {
                    write_str(1, "Error\n");
                    break;
                }
                float a_float, b_float;
                if (!parse_float(tokens[1], &a_float) || !parse_float(tokens[2], &b_float)) {
                    write_str(1, "Error\n");
                    break;
                }
                float result = fn_area(a_float, b_float);
                write_float(1, result);
                write_newline(1);
                break;

            default:
                write_str(1, "Error\n");
                break;
        }
    }

    if (current_lib != NULL) {
        dlclose(current_lib);
    }

    return 0;
}

