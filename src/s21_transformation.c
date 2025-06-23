#include "s21_decimal.h"

#define max_exp_lenght 6
#define pos_end_of_number_or_e 9
int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int error = 0;
  double res = 0;
  if (s21_get_exp(src) > MAX_EXP ||
      dst == NULL) {
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
      src.bits[1] > 0) {
    error = 1;
  } else {
    *dst = src.bits[0];
    if (s21_get_exp(src) > 0) *dst /= 10 * s21_get_exp(src);
    if ((src.bits[3] & MINUS) == MINUS) *dst *= -1;
  }
  return error;
}
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
    int exp;
    char float_in_string[32], cop_float[pos_end_of_number_or_e];
    sprintf(float_in_string, "%e", src);
    
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
    double mantisa = atof(cop_float);
    (void)cop_float;
    if (src < 0) mantisa *= -1;
    int dop_counter = 0;
    if (exp < -max_exp_lenght) dop_counter = exp * -1 - max_exp_lenght;
    for (int i = max_exp_lenght; i > 0; i--) {
      mantisa *= 10;
    }
    
    for (int i = max_exp_lenght + exp; i > 0; i--) {
      dst->bits[3] += MIN_SCALE;
    }
    
    while (dop_counter > 0) {
      mantisa *= 10;
      dop_counter--;
    }
    s21_help_float_to_dec(mantisa, dst);
    s21_remove_zeros(dst);
  }
  return error;
}
