#include "s21_decimal.h"

int s21_get_bit(s21_decimal value, int number) {
  int manBuffer, result = -1;
  if (number >= 0 && number <= 128) {
    if (number >= 0 && number < 32) {
      manBuffer = value.bits[0];
      manBuffer >>= number;
      manBuffer &= MIN_INT;
    } else if (number >= 32 && number < 64) {
      manBuffer = value.bits[1];
      manBuffer >>= number;
      manBuffer &= MIN_INT;
    } else if (number >= 64 && number < 96) {
      manBuffer = value.bits[2];
      manBuffer >>= number;
      manBuffer &= MIN_INT;
    } else if (number >= 96 && number < 128) {
      manBuffer = value.bits[3];
      manBuffer >>= number;
      manBuffer &= MIN_INT;
    }
    result = manBuffer;
  }

  return result;
}

int s21_get_sign(s21_decimal value) {
  unsigned int signBuffer = value.bits[3];
  signBuffer >>= 31;
  return signBuffer;
}

int s21_count_bit(long int number) {
  int count = 0;
  unsigned long int man = number;
  while (man > 0) {
    man /= 2;
    count++;
  }
  return count;
}
int s21_sum_bits(int bit_1, int bit_2, int bit_3, int shift, int manNumber,
                 s21_decimal *res) {
  int sum = bit_1 + bit_2 + bit_3;

  if (sum == 1) {
    bit_3 = 0;
    res->bits[manNumber] |= shift;
  } else if (sum == 2) {
    bit_3 = 1;
    res->bits[manNumber] &= ~shift;
  } else if (sum == 3) {
    bit_3 = 1;
    res->bits[manNumber] |= shift;
  }

  return bit_3;
}


int s21_sub_bits(int bit_1, int bit_2, int bit_3, int shift, int manNumber,
                 s21_decimal *res) {
  int sub = bit_1 - bit_2 - bit_3;
  if (sub == -2) {
    bit_3 = 1;
    res->bits[manNumber] &= ~shift;
  } else if (sub == -1) {
    bit_3 = 1;
    res->bits[manNumber] |= shift;
  } else if (sub == 0) {
    bit_3 = 0;
  } else if (sub == 1) {
    bit_3 = 0;
    res->bits[manNumber] |= shift;
  }
  // printf("%d\n", sub);

  return bit_3;
}
