#include "s21_decimal.h"
#define ZERO 0x00000000
int s21_is_less(s21_decimal s21_decimal_one, s21_decimal s21_decimal_two) {
  int result = 0, sign_one = s21_get_sign(s21_decimal_one),
      sign_two = s21_get_sign(s21_decimal_two);

  if (s21_is_equal(s21_decimal_one, s21_decimal_two)) {
    result = 0;
  } else if ((sign_one) && (sign_two == 0)) {
    result = 1;
  } else if ((sign_one == 0) && (sign_two)) {
    result = 0;
  } else {
    s21_decimal_one.bits[EXP] = s21_decimal_one.bits[EXP] << 1 >> 1;
    s21_decimal_two.bits[EXP] = s21_decimal_two.bits[EXP] << 1 >> 1;
    s21_normalization(&s21_decimal_one, &s21_decimal_two);
    for (int i = 2; i >= 0; i--) {
      unsigned int left = s21_decimal_one.bits[i],
                   right = s21_decimal_two.bits[i];
      if (left < right) {
        result = 1;
        i = -2;
      } else if (left > right) {
        result = 0;
        break;
      }
    }
    if (sign_one) result = result ? 0 : 1;
  }
  return result;
}

int s21_is_less_or_equal(s21_decimal s21_decimal_one,
                         s21_decimal s21_decimal_two) {
  int result = 0;
  if (s21_is_equal(s21_decimal_one, s21_decimal_two) ||
      s21_is_less(s21_decimal_one, s21_decimal_two))
    result = 1;
  else
    result = 0;
  return result;
}

int s21_is_greater(s21_decimal s21_decimal_one, s21_decimal s21_decimal_two) {
  int result = 0;
  if (!s21_is_less(s21_decimal_one, s21_decimal_two) &&
      !s21_is_equal(s21_decimal_one, s21_decimal_two))
    result = 1;
  else
    result = 0;
  return result;
}

int s21_is_greater_or_equal(s21_decimal s21_decimal_one,
                            s21_decimal s21_decimal_two) {
  int result = 0;
  if (s21_is_equal(s21_decimal_one, s21_decimal_two) ||
      !s21_is_less(s21_decimal_one, s21_decimal_two))
    result = 1;
  else
    result = 0;
  return result;
}

int s21_is_equal(s21_decimal s21_decimal_one, s21_decimal s21_decimal_two) {
  int result = 1, sign_one = s21_get_sign(s21_decimal_one),
      sign_two = s21_get_sign(s21_decimal_two);
  ;
  if (s21_is_zero(s21_decimal_one) && s21_is_zero(s21_decimal_two))
    result = 1;
  else if (sign_one != sign_two) {
    result = 0;
  } else {
    s21_remove_zeros(&s21_decimal_one);
    s21_remove_zeros(&s21_decimal_two);

    s21_decimal_one.bits[EXP] = s21_decimal_one.bits[EXP] << 1 >> 1;
    s21_decimal_two.bits[EXP] = s21_decimal_two.bits[EXP] << 1 >> 1;
    s21_normalization(&s21_decimal_one, &s21_decimal_two);
    for (int i = 0; i < 4; i++) {
      if (s21_decimal_one.bits[i] != s21_decimal_two.bits[i]) result = 0;
    }
  }
  return result;
}

int s21_is_not_equal(s21_decimal s21_decimal_one, s21_decimal s21_decimal_two) {
  return !s21_is_equal(s21_decimal_one, s21_decimal_two);
}
