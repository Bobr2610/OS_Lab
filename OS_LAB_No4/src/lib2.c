#include "functions.h"

#define MAX_SIEVE_SIZE 1000000

int prime_count(int a, int b) {
    if (a < 1) a = 1;
    if (b < 2) return 0;
    if (b < a) return 0;

    // Ограничение на максимальный размер для статического массива
    if (b >= MAX_SIEVE_SIZE) {
        b = MAX_SIEVE_SIZE - 1;
    }

    static char is_prime[MAX_SIEVE_SIZE];

    // Инициализация массива
    for (int i = 0; i <= b; i++) {
        is_prime[i] = 1;
    }
    is_prime[0] = 0;
    is_prime[1] = 0;

    for (int i = 2; i * i <= b; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= b; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    int count = 0;
    for (int i = (a < 2 ? 2 : a); i <= b; i++) {
        if (is_prime[i]) {
            count++;
        }
    }

    return count;
}

float area(float a, float b) {
    return (a * b) / 2.0f;
}

