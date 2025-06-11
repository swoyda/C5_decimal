#include "s21_decimal.h"

#define max_exp_lenght 6
#define pos_end_of_number_or_e 9
int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int error = 0;
  double res = 0;
  if (s21_get_exp(src) > MAX_EXP ||
      dst == NULL) {  //тут больше проверок, MAX_EXP postavit
    error = 1;
  } else {
    int sign = s21_get_sign(src);
    res = (src.bits[0] + (pow(2, 32) * src.bits[1]) +
           (pow(2, 64) * src.bits[2])) /
          (pow(10, ((src.bits[3] << 1) >> 1) >> 16));
    if (sign) res = res * -1;
    *dst = res;
  }
  return error;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int error = 0;
  s21_truncate(src, &src);
  if (s21_get_exp(src) > MAX_EXP || dst == NULL || src.bits[1] > 0 ||
      src.bits[1] > 0) {  // int у нас от -2147483647 до +2147483647 не знаю на
                          // сколько нужна эта проверка
    error = 1;
  } else {
    *dst = src.bits[0];
    if (s21_get_exp(src) > 0) *dst /= 10 * s21_get_exp(src);
    if ((src.bits[3] & MINUS) == MINUS) *dst *= -1;
  }
  return error;
}
// Если числа слишком малы (0 < |x| < 1e-28), вернуть ошибку и значение, равное
// 0 Если числа слишком велики (|x| > 79,228,162,514,264,337,593,543,950,335)
// или равны бесконечности, вернуть ошибку
int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int error = 0;
  if (dst == NULL || (0 < abs(src) && abs(src) < MIN_NUMBER) ||
      src == INFINITY) {
    error = 1;
  } else {
    dst->bits[3] = 0;
    dst->bits[2] = 0;
    dst->bits[1] = 0;
    dst->bits[0] = abs(src);
    if (src < 0) s21_negate(*dst, dst);
  }
  return error;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int error = 0;
  s21_gen_dec(dst);
  if (dst == NULL || src == NAN || (0 < fabs(src) && fabs(src) < MIN_NUMBER) ||
      src >= powf(2.0, 96) || src <= -powf(2.0, 96)) {
    error = 1;
  } else if (src != 0) {
    s21_gen_dec(dst);
    if (src < 0) {
      dst->bits[3] = MINUS;
    }
    // int poinetr = s21_float_pointer(src);
    // printf("point = %d\n", poinetr);
    // if (poinetr) {
    //   while (poinetr > 0) {
    //     dst -> bits[3] += 0x00010000;
    //     poinetr--;
    //   }
    // }
    //
    int exp;
    char float_in_string[32], cop_float[pos_end_of_number_or_e];
    sprintf(float_in_string, "%e", src);
    // printf("%s\n", float_in_string);
    // int trash = 0;
    // for (int i = pos_end_of_number_or_e; i > 1; i--) {
    //   if (float_in_string[i] == '0') trash++;
    // }
    for (int i = 0; i < pos_end_of_number_or_e; i++) {
      if (float_in_string[i] == 'e') break;
      cop_float[i] = float_in_string[i];
    }
    if (src > 0) {
      exp = ((int)float_in_string[10] - '0') * 10 + (int)float_in_string[11] -
            '0';
      if (float_in_string[9] == '+') exp *= -1;
    } else {
      exp = ((int)float_in_string[11] - '0') * 10 + (int)float_in_string[12] -
            '0';
      if (float_in_string[10] == '+') exp *= -1;
    }
    // for (int i = 10; i < 12; i++) {
    //   if (float_in_string[i] == '\0') break;
    //   cop_exp[i - 10] = float_in_string[i] + '0';
    // }
    // printf("%lf\n", cop_exp);
    // for (int i = 10; i < 12; i++) {
    //   cop_exp[i - 10] = cop_exp[i - 10] - '0';
    // }
    double mantisa = atof(cop_float);
    (void)cop_float;
    if (src < 0) mantisa *= -1;
    //
    int dop_counter = 0;
    if (exp < -max_exp_lenght) dop_counter = exp * -1 - max_exp_lenght;
    for (int i = max_exp_lenght; i > 0; i--) {
      mantisa *= 10;
    }
    // printf("%d\n", dop_counter);
    for (int i = max_exp_lenght + exp; i > 0; i--) {
      dst->bits[3] += MIN_SCALE;
    }
    // printf("bits 3 -> %0x\n", dst->bits[3]);
    // printf("%Lf %s\n", mantisa, cop_float);
    // printf("%lf\n", mantisa);

    // printf("%lf\n", mantisa);
    while (dop_counter > 0) {
      mantisa *= 10;
      dop_counter--;
    }
    // printf("%ld\n", int_mantisa);
    s21_help_float_to_dec(mantisa, dst);
    s21_remove_zeros(dst);
  }
  return error;
}
