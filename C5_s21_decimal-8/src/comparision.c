// #include "help.h"
#include "s21_decimal.h"

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  int return_code = 1;
  if (get_sign(value_1) == get_sign(value_2)) {
    alignment_scale(&value_1, &value_2);
    for (int i = 95; i >= 0; i--) {
      if (get_bit(value_1, i) != get_bit(value_2, i)) {
        return_code = 0;
        break;
      }
    }
  } else if (!(is_null(value_1) && is_null(value_2))) {
    return_code = 0;
  }
  return return_code;
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  int return_code = 0;
  int sign1 = get_sign(value_1), sign2 = get_sign(value_2);
  if (!(is_null(value_1) && is_null(value_2))) {
    if (sign1 != sign2) {
      if (sign1)
        return_code = 0;
      else
        return_code = 1;

    } else {
      alignment_scale(&value_1, &value_2);
      for (int i = 95; i >= 0; i--) {
        int bit1 = get_bit(value_1, i), bit2 = get_bit(value_2, i);
        if (bit1 != bit2) {
          if (bit1 != 0) {
            return_code = (sign1 == 0) ? 1 : 0;
            break;
          } else {
            return_code = (sign1 == 0) ? 0 : 1;
            break;
          }
        }
      }
    }
  }
  return return_code;
}

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  return (!s21_is_greater(value_1, value_2) && !s21_is_equal(value_1, value_2));
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return (s21_is_greater(value_1, value_2) || s21_is_equal(value_1, value_2))
             ? 1
             : 0;
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return (s21_is_less(value_1, value_2) || s21_is_equal(value_1, value_2));
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_equal(value_1, value_2);
}
