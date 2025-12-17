#include "functions.h"

static int is_prime_naive(int n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;

    for (int i = 3; i < n; i += 2) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

int prime_count(int a, int b) {
    if (a < 1) a = 1;
    if (b < a) return 0;

    int count = 0;
    for (int i = a; i <= b; i++) {
        if (is_prime_naive(i)) {
            count++;
        }
    }
    return count;
}

float area(float a, float b) {
    return a * b;
}

