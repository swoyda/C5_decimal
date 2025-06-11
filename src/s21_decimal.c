#include "s21_decimal.h"

// int main(void) {
//     s21_decimal num2 = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
//       0x00000000};
//     s21_decimal num = {0x00000001, 0x00000000, 0x00000000, 0x00110000};
//     s21_decimal buf = {0};
//     num2.bits[0] =  784662527;
//     num2.bits[1] = 11641532;
//     num2.bits[2] = 0;
//     num2.bits[3] = 1114112;
//     s21_decimal_to_bits(num2);
//     // s21_decimal csharp = {0x039CB560, 0x00000000, 0x00000000, 0x00000000};
//     // num.bits[3] += 0x00010000;
//     // s21_negate(num, &buf);
//     // s21_cop_dec(num, &buf);
//     float res = -7.123, res2;
//     // int ser , ser2 = 154;339051156075640552161.43
//     // s21_round(num, &buf);339235623516377647677.59
//     // printf("part1\n");
//     // int err = s21_mul(num, num2, &buf);
//     // s21_div_10 (&num2, );
//     // s21_add(num2, num, &buf);
//     // s21_from_float_to_decimal(res, &buf);
//     // s21_from_float_to_decimal(res, &buf);
//     s21_round(num2, &buf);
//     // s21_truncate(num, &buf);
//     // s21_from_decimal_to_float(buf, &res);
//     // s21_from_decimal_to_float(num2, &res);
//     // s21_from_decimal_to_float(num, &res2);

//     // s21_from_decimal_to_int(buf, &ser);
//     // // // printf("%f\n", res);
//     // s21_decimal_to_bits(num);
//     // s21_decimal_to_bits(csharp);

//     for (int i = 0; i < 4; i++) {
//         printf("%0x ", buf.bits[i]);
//     }
//     // printf("%d\n", err);//4420975788288
//     // s21_from_decimal_to_float(buf, &res2);
//     // printf("buf = %f\n", (buf.bits[0] % 10) * 1000000000.);
//     // printf("in int %d\nin float %f\n", ser,res);
//     // printf("is eq -> %d\n", s21_is_less(num2, num));
// }

int s21_get_bit(s21_decimal value, int number) {
  int manBuffer, result = -1;
  // printf("%d\n", result);
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

// int s21_decimal_to_bits(s21_decimal value) {
//   int bits;
//   for (int i = 127; i >= 0; i--) {
//     bits = s21_get_bit(value, i);
//     printf("%d", bits);
//     if (i % 32 == 0 && i != 127) printf(" ");
//   }
//   printf("\n");

//   return bits;
// }

int s21_sub_bits(int bit_1, int bit_2, int bit_3, int shift, int manNumber,
                 s21_decimal *res) {
  int sub = bit_1 - bit_2 - bit_3;
  // printf("%d - %d - %d = ", bit_1, bit_2, bit_3);
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
