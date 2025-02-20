// #include "help.h"
#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int return_code = 0;
  int sign1 = get_sign(value_1), sign2 = get_sign(value_2);
  init_decimal(result);
  if (sign1 == sign2) {
    alignment_scale(&value_1, &value_2);
    if (add_bit(value_1, value_2, result) == 0) {
      set_sign(result, sign1);
      set_scale(result, get_scale(value_1));
    } else if (!get_scale(value_1) || !get_scale(value_2)) {
      if (!sign1)
        return_code = 1;
      else
        return_code = 2;
    } else if (get_scale(value_1) > 0 && get_scale(value_2) > 0) {
      decrease_scale(&value_1, 1);
      decrease_scale(&value_2, 1);
      set_scale(&value_1, get_scale(value_1) - 1);
      set_scale(&value_2, get_scale(value_2) - 1);
      return_code = s21_add(value_1, value_2, result);
    }
  } else if (!sign1 && sign2) {
    set_sign(&value_2, 0);
    return_code = s21_sub(value_1, value_2, result);
  } else if (sign1 && !sign2) {
    set_sign(&value_1, 0);
    return_code = s21_sub(value_2, value_1, result);
  }
  return return_code;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int return_code = 0;
  int sign1 = get_sign(value_1), sign2 = get_sign(value_2);
  init_decimal(result);
  if (sign1 == sign2) {
    alignment_scale(&value_1, &value_2);
    if (!s21_is_equal(value_1, value_2)) {
      set_sign(&value_1, 0);
      set_sign(&value_2, 0);
      if (s21_is_greater(value_2, value_1)) {
        convert(&value_1);
        return_code = s21_add(value_1, value_2, result);
        set_sign(result, !sign2);
      } else {
        convert(&value_2);
        return_code = s21_add(value_2, value_1, result);
        set_sign(result, sign1);
      }
    }
  } else {
    set_sign(&value_1, 0);
    set_sign(&value_2, 0);
    return_code = s21_add(value_1, value_2, result);
    if (return_code && sign1)
      return_code = 2;
    else if (return_code)
      return_code = 1;
    set_sign(result, sign1);
  }
  return return_code;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  init_decimal(result);
  int return_code = 1;
  int sign = 0, sign1 = get_sign(value_1), sign2 = get_sign(value_2);
  if (sign1 != sign2) sign = 1;
  set_sign(&value_1, 0);
  set_sign(&value_2, 0);
  if (!value_2.bits[0] && !value_2.bits[1] && !value_2.bits[1]) {
    return_code = 3;
  } else {
    set_scale(&value_1, 0);
    set_scale(&value_2, 0);
    s21_decimal tmp = {0};
    for (int i = get_last_bit(value_1); i >= 0; i--) {
      if (get_bit(value_1, i)) set_bit(&tmp, 0, 1);
      if (s21_is_greater_or_equal(tmp, value_2) == 1) {
        s21_sub(tmp, value_2, &tmp);
        if (i != 0) shift_left(&tmp, 1);
        if (get_bit(value_1, i - 1)) set_bit(&tmp, 0, 1);
        shift_left(result, 1);
        set_bit(result, 0, 1);
      } else {
        shift_left(result, 1);
        if (i != 0) shift_left(&tmp, 1);
        if ((i - 1) >= 0 && get_bit(value_1, i - 1)) set_bit(&tmp, 0, 1);
      }
    }
    return_code = 0;
  }
  set_sign(result, sign);
  return return_code;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  init_decimal(result);
  int return_code = 0;
  int sign = (get_sign(value_1) != get_sign(value_2)) ? 1 : 0;
  set_sign(&value_1, 0);
  set_sign(&value_2, 0);
  int last_bit = get_last_bit(value_1);

  for (int i = 0; i <= last_bit; i++) {
    s21_decimal tmp = {{0, 0, 0, 0}};
    int bit_1 = get_bit(value_1, i);
    if (bit_1) {
      tmp = value_2;
      shift_left(&tmp, i);
      if (add_bit(*result, tmp, result)) return_code = 1;
    }
  }

  if (return_code) {
    while (get_scale(value_1) > 0 || get_scale(value_2) > 0) {
      s21_decimal *val_1, *val_2;
      if (get_last_bit(value_1) > get_last_bit(value_2) &&
          get_scale(value_1) > 0) {
        val_1 = &value_1;
        val_2 = &value_2;
      } else if (get_last_bit(value_2) > get_last_bit(value_1) &&
                 get_scale(value_2) > 0) {
        val_1 = &value_2;
        val_2 = &value_1;
      } else
        break;
      int scale_2 = get_scale(*val_1);
      decrease_scale(val_1, 1);
      set_scale(val_1, --scale_2);
      return_code = s21_mul(*val_1, *val_2, result);
    }
  }

  int scale = get_scale(value_1) + get_scale(value_2);
  set_scale(result, scale);
  set_sign(result, sign);
  if (return_code && sign) return_code = 2;
  return return_code;
}
