#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#define R 10

unsigned long long hex_str_to_num(const char *hex) {
    unsigned long long num = 0;
    int i;
    for (i = 0; i < 6; i++) {
        num *= 16;
        if (hex[i] >= '0' && hex[i] <= '9') {
            num += hex[i] - '0';
        } else if (hex[i] >= 'A' && hex[i] <= 'F') {
            num += hex[i] - 'A' + 10;
        }
    }
    return num;
}

void num_to_hex_str(unsigned long long num, char *hex) {
    int i, digit;
    for (i = 5; i >= 0; i--) {
        digit = num % 16;
        if (digit < 10) {
            hex[i] = digit + '0';
        } else {
            hex[i] = digit - 10 + 'A';
        }
        num /= 16;
    }
    hex[6] = '\0';
}

bool is_valid_hex(const char *hex) {
    int i;
    if (strlen(hex) != 6) return false;
    for (i = 0; i < 6; i++) {
        if (!((hex[i] >= '0' && hex[i] <= '9') || (hex[i] >= 'A' && hex[i] <= 'F'))) {
            return false;
        }
    }
    return true;
}

int main(void) {
    int bull, cow, x, i, attempt, digit, x_rand;
    int check_s[16];
    int check_cow[16];

    unsigned long long low, high, guess_num;

    char user_number[7];
    char guess_str[7];
    char s[7];
    char number[7];

    bool guessed_by_computer;

    int comp_attempts;
    int user_attempts;
  
    bull = 0;
    cow = 0;
    x = 0;
    comp_attempts = 0;
    user_attempts = 0;
    guessed_by_computer = false;
    low = 0;
    high = (1ULL << 24) - 1;

    for (i = 0; i < 16; i++) {
        check_s[i] = 0;
        check_cow[i] = 0;
    }

    for (i = 0; i < 7; i++) {
        user_number[i] = '\0';
        guess_str[i] = '\0';
        s[i] = '\0';
        number[i] = '\0';
    }

    setlocale(LC_ALL, "Russian");

    // Ввод числа пользователя
    printf("Введите шестнадцатеричное шестизначное число (0123456789ABCDEF): ");
    do {
        scanf("%6s", user_number);
        if (!is_valid_hex(user_number)) {
            printf("Ошибка! Нужно ровно 6 символов (0-9, A-F). Попробуйте ещё: ");
        } else {
            break;
        }
    } while (1);

    printf("Вы загадали число: %s\n", user_number);
    printf("А теперь я загадал число, а вы угадываете!\n");

    // Компьютер угадывает
    printf("\n--- Компьютер пытается угадать ваше число (максимум %d ход ---\n", R);

    for (attempt = 1; attempt <= R; attempt++) {
        guess_num = (low + high) / 2;
        num_to_hex_str(guess_num, guess_str);

        printf("Ход %d: компьютер думает, что ваше число = %s\n", attempt, guess_str);

        if (strcmp(guess_str, user_number) == 0) {
            printf("Компьютер угадал ваше число за %d ходов! Это %s\n", attempt, guess_str);
            guessed_by_computer = true;
            comp_attempts = attempt;
            break;
        }

        if (hex_str_to_num(guess_str) < hex_str_to_num(user_number)) {
            low = guess_num + 1;
        } else {
            high = guess_num - 1;
        }

        if (low > high) break;
    }

    if (!guessed_by_computer) {
        printf("\nКомпьютер не угадал за %d ходов и читерит!\n", R);
        printf("Ваше число было: %s\n", user_number);
    } else {
        printf("Компьютер угадал честно за %d ходов!\n", comp_attempts);
    }

    // Игрок угадывает число компьютера
    printf("\n--- Теперь вы угадываете моё число! ---\n");

    srand(time(NULL));
    for (i = 0; i < 6; ++i) {
        x_rand = rand() % 16;
        if (x_rand < 10) {
            number[i] = x_rand + '0';
        } else {
            number[i] = x_rand - 10 + 'A';
        }
    }
    number[6] = '\0';

    bull = 0;
    user_attempts = 0;

    while (bull != 6) {
        for (i = 0; i < 16; ++i) {
            check_cow[i] = 0;
            check_s[i] = 0;
        }

        for (i = 0; i < 6; ++i) {
            if (number[i] <= '9') {
                ++check_cow[number[i] - '0'];
            } else {
                ++check_cow[number[i] - 'A' + 10];
            }
        }

        printf("Введи число: ");
        ++user_attempts;
        scanf("%s", s);

        if (s[0] == 'j') {
            break;
        }

        bull = 0;
        cow = 0;

        for (i = 0; i < 6; ++i) {
            if (s[i] <= '9') {
                ++check_s[s[i] - '0'];
            } else {
                ++check_s[s[i] - 'A' + 10];
            }
            if (s[i] == number[i]) {
                ++bull;
            }
        }

        for (i = 0; i < 16; ++i) {
            if (check_cow[i] > 0 && check_s[i] > 0) {
                if (check_cow[i] < check_s[i]) {
                    cow += check_cow[i];
                } else {
                    cow += check_s[i];
                }
            }
        }
        cow -= bull;
        printf(" %d коров и %d быков\n", cow, bull);
    }

    printf("Победа! Вот исходное число: %s\nТебе потребовалось ходов: %d\n", number, user_attempts);

    return 0;
}
