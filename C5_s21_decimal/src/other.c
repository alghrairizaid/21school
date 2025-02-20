// #include "help.h"
#include "s21_decimal.h"

int s21_copy(s21_decimal value, s21_decimal *result) {
  init_decimal(result);
  for (int i = 0; i < 4; i++) result->bits[i] = value.bits[i];
  return !s21_is_equal(value, *result);
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  init_decimal(result);
  int scale = get_scale(value);
  if (scale > 0) decrease_scale(&value, scale);
  int return_code = s21_copy(value, result);
  return return_code;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  init_decimal(result);
  if (!s21_truncate(value, result) && get_sign(value) &&
      !s21_is_equal(value, *result)) {
    s21_sub(*result, ((s21_decimal){{1, 0, 0, 0}}), result);
  }
  return 0;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  init_decimal(result);
  s21_decimal alt;
  init_decimal(&alt);
  s21_truncate(value, result);

  if (!s21_is_equal(value, *result)) {
    s21_sub(value, *result, &alt);
    float res = 0;
    s21_from_decimal_to_float(alt, &res);
    if (res >= 0.5) s21_add((s21_decimal){{1, 0, 0, 0}}, *result, result);
  }
  return 0;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  init_decimal(result);
  if (get_sign(value))
    set_sign(&value, 0);
  else
    set_sign(&value, 1);
  return s21_copy(value, result);
}
