#ifndef S21_DECIMAL
#define S21_DECIMAL

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXP 3
#define MIN_INT 0x00000001
#define MAX_INT 0xffffffff
#define MINUS 0x80000000
#define MIN_SCALE 0x00010000
#define SCALE 0x00FF0000
#define MAX_NUMBER 2e96 - 1
#define MAX_EXP 28
#define MIN_NUMBER 1e-28

typedef struct {
  unsigned int bits[4];
} s21_decimal;

// Работа с битами

// Достать бит
int s21_get_bit(s21_decimal value, int number);
// Достать знак
int s21_get_sign(s21_decimal value);
// Количество битов в блоке
int s21_count_bit(long int number);
// Сумма битов
int s21_sum_bits(int bit_1, int bit_2, int bit_3, int shift, int manNumber,
                 s21_decimal *res);

// int s21_decimal_to_bits(s21_decimal value);
int s21_sub_bits(int bit_1, int bit_2, int bit_3, int shift, int manNumber,
                 s21_decimal *res);
// Арифметические операторы

// Сложение "+"
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
// Вычитание "-"
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
// Умножение "*"
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
// Деление "/"             -
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
// Тестовое умножение
s21_decimal multiplication(s21_decimal value, int exp);
// Приведение к общему знаменателю
int s21_normalization(s21_decimal *value_1, s21_decimal *value_2);

// Операторы сравнение

// Меньше <
int s21_is_less(s21_decimal s21_decimal_one, s21_decimal s21_decimal_two);
// Меньше или равно <=
int s21_is_less_or_equal(s21_decimal s21_decimal_one,
                         s21_decimal s21_decimal_two);
// Больше >
int s21_is_greater(s21_decimal s21_decimal_one, s21_decimal s21_decimal_two);
// Больше или равно >=
int s21_is_greater_or_equal(s21_decimal s21_decimal_one,
                            s21_decimal s21_decimal_two);
// Равно ==
int s21_is_equal(s21_decimal s21_decimal_one, s21_decimal s21_decimal_two);
// Не равно !=
int s21_is_not_equal(s21_decimal s21_decimal_one, s21_decimal s21_decimal_two);

// Преобразователи

// Из int
int s21_from_int_to_decimal(int src, s21_decimal *dst);
// Из float
int s21_from_float_to_decimal(float src, s21_decimal *dst);
// В int
int s21_from_decimal_to_int(s21_decimal src, int *dst);
// В float
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// Другие функции

// Округляет указанное Decimal число до ближайшего целого числа в сторону
// отрицательной бесконечности. сделал через s21_add :)
int s21_floor(s21_decimal value, s21_decimal *result);
// Округляет Decimal до ближайшего целого числа.
int s21_round(s21_decimal value, s21_decimal *result);
// выкидывает остаток
int s21_truncate(s21_decimal value, s21_decimal *result);
//Возвращает результат умножения указанного Decimal на -1.
int s21_negate(s21_decimal value, s21_decimal *result);

// Вспомогательные функции

// Копирует один decimal  в другой
void s21_cop_dec(s21_decimal value, s21_decimal *result);
// Смотрим exp (обычно нужно для проверок, что exp <28)
int s21_get_exp(s21_decimal value);
// Фактически зануляем рандомно подаваемый дец
void s21_gen_dec(s21_decimal *dst);
// Теоретическая проверка для проверки, что подаваемое число влезет в дец, по
// факту такое число не влезет во флоат
// int s21_check_float_lower_29(float src);
// Считаем длину дроби
int s21_float_pointer(float src);
// разгрузка функции trancate
int s21_div_10(s21_decimal *result, int move);
// void s21_move_bits_1 (s21_decimal *result);
// разгрузка функции float_to_dec
void s21_help_float_to_dec(long double mantisa, s21_decimal *dst);
// проверка для 0.00 == -0.00000
int s21_is_zero(s21_decimal value);
// сдвиг влево
int s21_decimal_shift(s21_decimal *value, int shift);
// 1.400 -> 1.4
void s21_remove_zeros(s21_decimal *value);
#endif
