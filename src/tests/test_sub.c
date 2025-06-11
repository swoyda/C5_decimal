#include "test_decimal.h"

static s21_decimal num1[] = {
    {{0x00000019, 0x00000000, 0x00000000, 0x00010000}},  // i= 0: 2.5
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},  // i= 1: 0
    {{0x00000000, 0x00000000, 0x00000000, 0x80010000}},  // i= 2: 0.0
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},  // i= 3: 0
    {{0x00000000, 0x00000000, 0x00000000, 0x80010000}},  // i= 4: 0.0
    {{0x00000001, 0x00000000, 0x00000000,
      0x800E0000}},  // i= 5: -0.00000000000001
    {{0x00000019, 0x00000000, 0x00000000, 0x00010000}},  // i= 6: 2.5
    {{0x00000183, 0x00000000, 0x00000000, 0x00020000}},  // i= 7: 3.87
};

static s21_decimal num2[] = {
    {{0x00000019, 0x00000000, 0x00000000, 0x00010000}},  // i= 0: 2.5
    {{0x00000000, 0x00000000, 0x00000000, 0x80000000}},  // i= 1: -0
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},  // i= 2: 0
    {{0x00000000, 0x00000000, 0x00000000, 0x80010000}},  // i= 3: -0.0
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},  // i= 4: 0
    {{0x00000006, 0x00000000, 0x00000000,
      0x000F0000}},  // i= 5: 0.000000000000006
    {{0x00000064, 0x00000000, 0x00000000, 0x00000000}},  // i= 6: 100
    {{0x00004DA6, 0x00000000, 0x00000000, 0x00020000}},  // i= 7: 198.78
};

static s21_decimal result[] = {
    {{0x00000000, 0x00000000, 0x00000000, 0x00010000}},  // i= 0: 0.0
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},  // i= 1: 0
    {{0x00000000, 0x00000000, 0x00000000, 0x80000000}},  // i= 2: 0.0
    {{0x00000000, 0x00000000, 0x00000000, 0x00010000}},  // i= 3: -0.0
    {{0x00000000, 0x00000000, 0x00000000, 0x80000000}},  // i= 4: 0.0
    {{0x00000010, 0x00000000, 0x00000000,
      0x800F0000}},  // i= 5: -0.000000000000016
    {{0x000003CF, 0x00000000, 0x00000000, 0x80010000}},  // i= 6: -97.5
    {{0x00004C23, 0x00000000, 0x00000000, 0x80020000}},  // i= 7: -194.91
};

static s21_decimal err_num1[] = {
    {{0x00000001, 0x00000000, 0x00000000, 0x80000000}},  // -1
    {{0x00000001, 0x00000000, 0x00000000, 0x00000000}},  // 1
    {{0x0000000A, 0x00000000, 0x00000000, 0x00000000}},  // 10
};

static s21_decimal err_num2[] = {
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
      0x00000000}},  // 79228162514264337593543950335
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
      0x80000000}},  //-79228162514264337593543950335
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
      0x80000000}},  //-79228162514264337593543950335
};

static int err_result[] = {
    2,
    1,
    1,
};

START_TEST(test) {
  for (size_t i = 0; i < sizeof(num1) / sizeof(s21_decimal); ++i) {
    s21_decimal tmp = {{0}};
    int ret = s21_sub(num1[i], num2[i], &tmp);
    // printf("Aaa%d ", i);
    ck_assert_int_eq(tmp.bits[0], result[i].bits[0]);
    ck_assert_int_eq(tmp.bits[1], result[i].bits[1]);
    ck_assert_int_eq(tmp.bits[2], result[i].bits[2]);
    // ck_assert_int_eq(tmp.bits[3], result[i].bits[3]);
    ck_assert_int_eq(ret, 0);
  }
}
END_TEST

START_TEST(error_test) {
  for (size_t i = 0; i < sizeof(err_num1) / sizeof(s21_decimal); ++i) {
    s21_decimal tmp = {{0}};
    // printf("Aaa%d ", i);
    int ret = s21_sub(err_num1[i], err_num2[i], &tmp);
    ck_assert_int_eq(ret, err_result[i]);
  }
}

Suite *suite_sub(void) {
  Suite *s;
  TCase *tc;

  s = suite_create("\033[102m s21_sub \033[0m");
  tc = tcase_create("\033[101m s21_sub \033[0m");

  if (s != NULL && tc != NULL) {
    tcase_add_test(tc, test);
    tcase_add_test(tc, error_test);
    suite_add_tcase(s, tc);
  }

  return (s);
}
