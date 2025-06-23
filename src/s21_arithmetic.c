#include "s21_decimal.h"

#define DEGREE 16
#define MAN_LIM 32
#define FIRST_BIT_EXP 96

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int bit_1, bit_2, bit_3, bitCounter, shift, rem_buf, manNumber,
      manPartCounter, error, sign_1, sign_2;
  rem_buf = 0;
  error = 0;
  bit_3 = 0;
  bitCounter = 0;
  manNumber = 0;
  manPartCounter = 0;
  sign_1 = s21_get_sign(value_1);
  sign_2 = s21_get_sign(value_2);
  value_1.bits[EXP] = value_1.bits[EXP] << 1 >> 1;
  value_2.bits[EXP] = value_2.bits[EXP] << 1 >> 1;
  error = s21_normalization(&value_1, &value_2);
  for (int manNumber = 0; manNumber <= EXP; manNumber++)
    result->bits[manNumber] = 0;
  result->bits[EXP] |= (value_1.bits[EXP] > value_2.bits[EXP])
                           ? value_1.bits[EXP]
                           : value_2.bits[EXP];
  while (bitCounter <= FIRST_BIT_EXP) {
    shift = MIN_INT << manPartCounter;
    if (manPartCounter == MAN_LIM) {
      manNumber++;
      manPartCounter = 0;
    }
    if (bitCounter == FIRST_BIT_EXP) {
      if (bit_3) {
        int exp_res;
        s21_div_10(result, 1);
        int remainder = s21_div_10(&value_1, 1);
        int remainder2 = s21_div_10(&value_2, 1);
        remainder += remainder2;
        if (result->bits[0] % 2 == 0 && remainder > 5) {
          rem_buf = 1;
        } else if (result->bits[0] % 2 == 1 && remainder > 4) {
          rem_buf = 1;
        }

        result->bits[0] = 0;
        result->bits[1] = 0;
        result->bits[2] = 0;
        exp_res = s21_get_exp(*result);
        if (exp_res <= 0 && sign_1 == 0) {
          error = 1;
          return error;
        } else if (exp_res <= 0) {
          error = 2;
          return error;
        }
        exp_res--;
        result->bits[EXP] = exp_res << DEGREE;

        bitCounter = 0;
        manNumber = 0;
        manPartCounter = 0;
        bit_3 = 0;
        shift = 1;

      } else
        break;
    }
    bit_1 = s21_get_bit(value_1, bitCounter);
    bit_2 = s21_get_bit(value_2, bitCounter);
    if (sign_1 == sign_2) {
      bit_3 = s21_sum_bits(bit_1, bit_2, bit_3, shift, manNumber, result);
    } else if (s21_is_greater_or_equal(value_1, value_2)) {
      bit_3 = s21_sub_bits(bit_1, bit_2, bit_3, shift, manNumber, result);
    } else
      bit_3 = s21_sub_bits(bit_2, bit_1, bit_3, shift, manNumber, result);

    manPartCounter++;
    bitCounter++;
  }

  if (s21_is_greater_or_equal(value_1, value_2)) {
    if (sign_1 > sign_2)
      s21_negate(*result, result);
    else if ((sign_1 == sign_2) && sign_1)
      s21_negate(*result, result);

  } else if (sign_2 > sign_1)
    s21_negate(*result, result);
  else if ((sign_1 == sign_2) && sign_1)
    s21_negate(*result, result);
  if (s21_is_zero(*result)) {
    result->bits[EXP] &= SCALE;
  }
  if (rem_buf) {
    s21_decimal one = {{0x00000001, 0x00000000, 0x00000000, result->bits[EXP]}};
    error = s21_add(*result, one, result);
  }

  return error;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error;
  value_2.bits[EXP] ^= MINUS;
  error = s21_add(value_1, value_2, result);
  return error;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int bit_1, error, sign_1 = s21_get_sign(value_1),
                    sign_2 = s21_get_sign(value_2);
  value_1.bits[EXP] &= SCALE;
  value_1.bits[EXP] &= SCALE;
  s21_decimal bufferValue = {0}, bufferResult = {0};
  bufferValue = value_1;
  error = 0;
  for (int bitCounter = 0; bitCounter < FIRST_BIT_EXP; bitCounter++) {
    bit_1 = s21_get_bit(value_2, bitCounter);
    if (bit_1) {
      for (int manNumber; manNumber <= EXP; manNumber++)
        result->bits[manNumber] = 0;
      bufferValue = value_1;
      int err_shift = s21_decimal_shift(&bufferValue, bitCounter);
      error = s21_add(bufferResult, bufferValue, result);
      if (err_shift) {
        error = 1;
      }
      bufferResult = *result;
    }
    if (error) break;
  }
  if (error && (sign_1 != sign_2)) error = 2;
  if (sign_1 != sign_2) result->bits[EXP] |= MINUS;
  return error;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error, dividerLength, dividendLength, shift, exp, remainderCounter;
  float remainder;
  s21_decimal bufferValue = {0}, bufferResultSub = {0}, bufferResult = {0};
  bufferResultSub = value_1;
  bufferValue = value_2;
  error = 0;
  exp = 0;
  remainder = 0;
  remainderCounter = 0;

  dividendLength = s21_count_bit(value_1.bits[0]) +
                   s21_count_bit(value_1.bits[1]) +
                   s21_count_bit(value_1.bits[2]);
  dividerLength = s21_count_bit(value_2.bits[0]) +
                  s21_count_bit(value_2.bits[1]) +
                  s21_count_bit(value_2.bits[2]);
  shift = dividendLength - dividerLength;

  for (int bitCounter = 0; bitCounter < 96; bitCounter++) {
    dividendLength = s21_count_bit(bufferResultSub.bits[0]) +
                     s21_count_bit(bufferResultSub.bits[1]) +
                     s21_count_bit(bufferResultSub.bits[2]);
    bufferValue = value_2;

    error = s21_decimal_shift(result, 1);
    dividerLength = s21_count_bit(bufferValue.bits[0]) +
                    s21_count_bit(bufferValue.bits[1]) +
                    s21_count_bit(bufferValue.bits[2]);
    s21_decimal_shift(&bufferValue, shift);

    if (s21_is_greater_or_equal(bufferResultSub, bufferValue)) {
      for (int manNumber = 0; manNumber <= EXP; manNumber++)
        bufferResult.bits[manNumber] = 0;
      s21_sub(bufferResultSub, bufferValue, &bufferResult);
      bufferResultSub = bufferResult;
      result->bits[0] |= 1;
      
    } else if (shift == 0 && s21_is_zero(bufferResultSub)) {
      break;
    } else if (shift == 0) {
      remainderCounter--;
      remainder += (s21_get_bit(bufferResultSub, dividendLength - 1) *
                    pow(2, remainderCounter));
      
      bufferResultSub.bits[0] = 0;
      result->bits[0] >>= 1;
      exp++;
    }
    if (shift != 0) shift--;
  }
  s21_from_float_to_decimal(remainder, &bufferValue);

  for (int manNumber = 0; manNumber <= EXP; manNumber++)
    bufferResult.bits[manNumber] = 0;
  s21_add(*result, bufferValue, &bufferResult);
  *result = bufferResult;

  return error;
}

s21_decimal multiplication(s21_decimal value, int exp) {
  int portablePartMan = 0;
  long long int newNumber = 0;
  for (int manNumber = 0; manNumber < EXP && exp; manNumber++) {
    newNumber = value.bits[manNumber] * pow(10, exp);
    value.bits[manNumber] *= pow(10, exp);
    value.bits[manNumber] |= portablePartMan;
    portablePartMan = newNumber >> MAN_LIM;
  }

  return value;
}

int s21_normalization(s21_decimal *value_1, s21_decimal *value_2) {
  int exp_1, exp_2, exp_res, decrease, flagPer, part3, error;
  unsigned long long portablePartMan, newNumber, bufferValue, perepoln;
  exp_1 = value_1->bits[EXP] >> DEGREE;
  exp_2 = value_2->bits[EXP] >> DEGREE;
  exp_res = abs(exp_1 - exp_2);
  decrease = 0;
  portablePartMan = 0;
  newNumber = 0;
  bufferValue = 0;
  perepoln = 0;
  flagPer = 0;
  part3 = 0;
  error = 0;

  s21_decimal smallerNumber, largerNumber;

  if (exp_1 > exp_2) {
    for (int manNumber = 0; manNumber < EXP; manNumber++) {
      smallerNumber.bits[manNumber] = value_2->bits[manNumber];
      largerNumber.bits[manNumber] = value_1->bits[manNumber];
    }
  } else {
    for (int manNumber = 0; manNumber < EXP; manNumber++) {
      smallerNumber.bits[manNumber] = value_1->bits[manNumber];
      largerNumber.bits[manNumber] = value_2->bits[manNumber];
    }
  }
  smallerNumber.bits[EXP] = 0;
  largerNumber.bits[EXP] = 0;

  unsigned long part2 = 0, ten = 10;
  for (int manNumber = 0; manNumber < EXP; manNumber++) {
    bufferValue = smallerNumber.bits[manNumber];
    perepoln = 0;
    for (int i = 0; i < exp_res; i++) {
      newNumber = bufferValue * ten;
      bufferValue *= 10;
      perepoln *= 10;
      if (manNumber == 2 &&
          s21_count_bit(newNumber) >= MAN_LIM) {  // !!!!!!!!!!!!!!!!
        decrease = exp_res - i;
        break;
      }
      smallerNumber.bits[manNumber] *= 10;
      if (s21_count_bit(newNumber) > MAN_LIM) {
        portablePartMan = newNumber >> MAN_LIM;
        if (s21_count_bit(portablePartMan * 10) > MAN_LIM && !flagPer) {
          perepoln = portablePartMan;
          flagPer = 1;
        }
      }
      if (s21_count_bit(perepoln) > MAN_LIM) {
        part3 = perepoln >> MAN_LIM;
        if (manNumber > 0) error = 1;
      }

      if (i == exp_res - 1) {
        smallerNumber.bits[manNumber] += part2;
        part2 = portablePartMan;
        if (manNumber == 2) smallerNumber.bits[manNumber] += part3;
        if (s21_count_bit(portablePartMan) > MAN_LIM)
          part3 = portablePartMan >> MAN_LIM;
        portablePartMan = 0;
      }
    }
  }
  portablePartMan = 0;

  if (decrease) {
    smallerNumber = multiplication((exp_1 > exp_2) ? *value_2 : *value_1,
                                   exp_res - decrease);
    s21_div_10(&largerNumber, decrease);
  }

  if (exp_1 > exp_2) {
    for (int manNumber = 0; manNumber < EXP; manNumber++) {
      value_2->bits[manNumber] = smallerNumber.bits[manNumber];
      value_1->bits[manNumber] = largerNumber.bits[manNumber];
    }
    exp_1 -= decrease;
    value_1->bits[EXP] = (exp_1 << DEGREE);
  } else {
    for (int manNumber = 0; manNumber < EXP; manNumber++) {
      value_1->bits[manNumber] = smallerNumber.bits[manNumber];
      value_2->bits[manNumber] = largerNumber.bits[manNumber];
    }
    exp_2 -= decrease;
    value_2->bits[EXP] = exp_2 << DEGREE;
  }

  return error;
}
