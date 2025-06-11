#include <string.h>

#include "s21_decimal.h"
int s21_floor(s21_decimal value, s21_decimal *result) {
  int error = 0;
  if (result == NULL || (s21_get_exp(value) > 28)) {
    error = 1;
  } else if (s21_get_exp(value) == 0) {
    s21_cop_dec(value, result);
  } else if (s21_is_zero(value)) {
    s21_decimal zero = {0};
    s21_cop_dec(zero, result);
  } else {
    if (s21_get_sign(value)) {
      int tmp = 0, flag = 0;
      while (s21_get_exp(value)) {
        tmp = s21_div_10(&value, 1);
        value.bits[EXP] -= MIN_SCALE;
        if (tmp) flag = 1;
      }
      if (flag) {
        s21_decimal neg_min_int = {
            {0x00000001, 0x00000000, 0x00000000, 0x80000000}};
        s21_add(value, neg_min_int, &value);
      }
      s21_cop_dec(value, result);
    } else {
      s21_truncate(value, result);
    }
  }
  return error;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  int error = 0;
  if (result == NULL || (s21_get_exp(value) > 28)) {
    error = 1;
  } else if (s21_get_exp(value) > 0) {
    s21_decimal buf = value;
    int tmp = s21_div_10(&buf, s21_get_exp(value));
    buf.bits[EXP] &= MINUS;
    s21_decimal little_dec = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}};
    if (s21_get_sign(value)) s21_negate(little_dec, &little_dec);
    // s21_truncate(buf, &buf);
    // printf("%d\n", tmp);
    if (buf.bits[0] % 2 == 0 && tmp > 5) {
      s21_add(buf, little_dec, &buf);
    } else if (buf.bits[0] % 2 == 1 && tmp > 4) {
      // printf("a\n");
      s21_add(buf, little_dec, &buf);
    }
    s21_cop_dec(buf, result);
  } else {
    s21_cop_dec(value, result);
  }
  return error;
}
int s21_truncate(s21_decimal value, s21_decimal *result) {
  int error = 0;
  if (result == NULL || (s21_get_exp(value) > 28)) {
    error = 1;
  } else {
    s21_cop_dec(value, result);
    s21_div_10(result, s21_get_exp(*result));
    result->bits[3] &= MINUS;
  }
  return error;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  int error = 0;
  if (result == NULL || (s21_get_exp(value) > 28)) {
    error = 1;
  } else {
    s21_cop_dec(value, result);
    result->bits[3] ^= MINUS;
  }
  return error;
}

void s21_cop_dec(s21_decimal value, s21_decimal *result) {
  for (int i = 0; i < 4; i++) {
    result->bits[i] = value.bits[i];
  }
}

int s21_get_exp(s21_decimal value) {
  int exp = 0x00000000;
  exp = value.bits[EXP] << 1 >> 1 >> 16;
  return exp;
}

void s21_gen_dec(s21_decimal *dst) {
  s21_decimal buf = {0};
  s21_cop_dec(buf, dst);
}

// int s21_check_float_lower_29(float src) {
//   char cop_src[1000000];
//   int result = 1;
//   sscanf(cop_src, "%f", &src);
//   if (strlen(cop_src) > 28) {
//     result = 0;
//   }
//   return result;
// }

// int s21_float_pointer(float src){
//   char cop_src[30];
//   int length = 0;
//   if (src != (int)src) {
//     sprintf(cop_src, "%f", src);
//     printf("in string %s\n", cop_src);
//     char *dot_pos = strchr(cop_src, '.');
//     if (dot_pos != NULL) {
//       length = strlen(cop_src) - (dot_pos - cop_src);
//     }
//   }
//   if (src < 0) length--;
//   return length;
// }

int s21_div_10(s21_decimal *result, int move) {
  int last_num = 0;

  while (move > 0) {
    unsigned long long uns_buffer = result->bits[2];
    // printf("move = %d\n", move);
    for (int j = 2; j >= 0; j--) {
      if (j == 0) {
        last_num = uns_buffer % 10;
        result->bits[j] = uns_buffer / 10;
        // printf("uns = %u\n", uns_buffer);

      } else {
        last_num = uns_buffer % 10;
        result->bits[j] = uns_buffer / 10;
        uns_buffer = last_num * (4294967296) + result->bits[j - 1];
      }
    }
    move--;
    // s21_decimal_to_bits(*result);
  }
  return last_num;
}

void s21_help_float_to_dec(long double mantisa, s21_decimal *dst) {
  unsigned long long man = mantisa;
  for (int i = 0; i < 3; i++) {
    dst->bits[i] = man;
    man >>= 32;
  }
}

int s21_decimal_shift(s21_decimal *value, int shift) {
  int error = 0, bit, bit2;
  for (int i = 0; i < shift; i++) {
    bit = 0;
    bit2 = 0;
    for (int manNumber = 0; manNumber < EXP && shift; manNumber++) {
      bit = value->bits[manNumber] >> 31;
      value->bits[manNumber] <<= 1;
      value->bits[manNumber] |= bit2;
      bit2 = bit;
      if ((value->bits[EXP - 1] >> 31) == 1 && i != (shift - 1)) error = 1;
    }
  }

  return error;
}

int s21_is_zero(s21_decimal value) {
  int res = 1;
  s21_decimal zero = {0};
  for (int i = 0; i < 3; i++) {
    if (zero.bits[i] != value.bits[i]) res = 0;
  }
  return res;
}

void s21_remove_zeros(s21_decimal *value) {
  int last_n = 0, flag = 1;
  s21_decimal buf;
  // s21_decimal_to_bits(*value);

  while (flag) {
    s21_cop_dec(*value, &buf);
    last_n = s21_div_10(&buf, 1);
    // printf("%d \n", last_n);
    if (last_n || (buf.bits[3]) == MINUS)
      flag = 0;
    else {
      s21_cop_dec(buf, value);
      value->bits[3] -= MIN_SCALE;
    }
  }
  // s21_decimal_to_bits(*value);
  if (s21_is_zero(*value)) {
    value->bits[3] = 0;
  }
  // if (sign) s21_negate(*value, value);
}