#include <stdio.h>

#include "s21_decimal.h"

void init_decimal(s21_decimal *value) {
  for (int i = 0; i < 4; i++) value->bits[i] = 0;
}

int get_bit(s21_decimal value, int bit) {
  return (value.bits[bit / 32] & (1u << (bit % 32))) != 0;
}

void set_bit(s21_decimal *value, int posision, int bit) {
  if (posision / 32 < 4) {
    if (bit)
      value->bits[posision / 32] |= (1u << (posision % 32));
    else
      value->bits[posision / 32] &= ~(1u << (posision % 32));
  }
}

int get_sign(s21_decimal value) { return (value.bits[3] & (1u << 31)) != 0; }

void set_sign(s21_decimal *value, int bit) {
  if (bit) {
    value->bits[3] |= (1u << 31);
  } else {
    value->bits[3] &= ~(1u << 31);
  }
}

int get_scale(s21_decimal value) { return ((value.bits[3] >> 16) & 255); }

void set_scale(s21_decimal *value, int scale) {
  if (scale >= 0 && scale <= 28) {
    int sign = get_sign(*value);
    value->bits[3] &= ~(255 << 16);
    value->bits[3] |= scale << 16;
    if (sign) set_sign(value, 1);
  }
}

int get_float_scale(float *value) {
  int exp = (*((int *)value) & ~(1u << 31)) >> 23;
  return exp - 127;
}

int add_bit(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int return_code = 0, carry = 0;
  for (int i = 0; i < 96; i++) {
    int bit1 = get_bit(value_1, i), bit2 = get_bit(value_2, i);
    if (!bit1 && !bit2)
      carry ? set_bit(result, i, (carry = 0) + 1) : set_bit(result, i, 0);
    else if (bit1 != bit2)
      carry ? set_bit(result, i, (carry = 1) - 1) : set_bit(result, i, 1);
    else
      carry ? set_bit(result, i, (carry = 1))
            : set_bit(result, i, (carry = 1) - 1);

    if (i == 95 && carry == 1 && !get_bit(value_1, 97) && !get_bit(value_2, 97))
      return_code = 1;
  }
  return return_code;
}

int get_last_bit(s21_decimal value) {
  int last_bit = 95;
  while (last_bit >= 0 && !get_bit(value, last_bit)) last_bit--;
  return last_bit;
}

int shift_left(s21_decimal *value, int shift) {
  int return_code = 1;
  int last_bit = get_last_bit(*value);
  if (last_bit + shift > 95) {
    return_code = 0;
  } else {
    for (int i = 0; i < shift; i++) {
      int transfer_byte_1 = get_bit(*value, 31);
      int transfer_byte_2 = get_bit(*value, 63);
      for (int k = 0; k < 3; k++) value->bits[k] <<= 1;
      if (transfer_byte_1) set_bit(value, 32, 1);
      if (transfer_byte_2) set_bit(value, 64, 1);
    }
  }
  return return_code;
}

void increase_scale(s21_decimal *value, int num) {
  int scale = get_scale(*value);
  if (scale + num < 29) {
    set_scale(value, scale + num);
    s21_decimal aux_val1 = *value, aux_val2 = *value;
    shift_left(&aux_val1, 1);
    shift_left(&aux_val2, 3);
    if (add_bit(aux_val1, aux_val2, value)) set_bit(value, 96, 1);
  }
}

void decrease_scale(s21_decimal *value, int num) {
  for (int i = 0; i < num; i++) {
    unsigned long long over_flow = value->bits[2];
    for (int j = 2; j >= 0; j--) {
      value->bits[j] = over_flow / 10;
      if (j == 0)
        over_flow = (over_flow % 10) * (S21_MAX_UINT + 1) + value->bits[j];
      else
        over_flow = (over_flow % 10) * (S21_MAX_UINT + 1) + value->bits[j - 1];
    }
  }
  set_scale(value, (get_scale(*value) - num));
}

void alignment_scale(s21_decimal *value_1, s21_decimal *value_2) {
  if (get_scale(*value_1) < get_scale(*value_2)) {
    alignment_scale(value_2, value_1);
  } else if (get_scale(*value_1) > get_scale(*value_2)) {
    int low_scale = get_scale(*value_2), high_scale = get_scale(*value_1);
    while ((high_scale - low_scale) > 0 && !get_bit(*value_2, 96)) {
      increase_scale(value_2, 1);
      low_scale++;
    }
    while (high_scale - low_scale > 0) {
      decrease_scale(value_1, high_scale - low_scale);
      set_scale(value_1, low_scale);
      high_scale--;
    }
  }
}

int is_null(s21_decimal value) {
  int return_code = 1;
  for (int i = 0; i < 3; i++) {
    if (value.bits[i] != 0) {
      return_code = 0;
      break;
    }
  }
  return return_code;
}

void convert(s21_decimal *value) {
  s21_decimal tmp = {{0, 0, 0, 0}}, add = {{1, 0, 0, 0}};
  for (int i = 0; i < 3; i++) value->bits[i] = ~value->bits[i];
  add_bit(*value, add, &tmp);
  for (int i = 0; i < 3; i++) value->bits[i] = tmp.bits[i];
  set_bit(value, 97, 1);
}

double my_fmod(double x, int y) {
  double result = 0.0;
  if (y != 0) {
    result = x - ((int)(x / y)) * y;
  }
  return result;
}

double my_pow(double base, int exponent) {
  double result = 1;
  if (exponent != 0) {
    int exp = exponent;
    int positive_exponent = exp > 0 ? exp : -exp;
    for (int i = 0; i < positive_exponent; i++) {
      result *= base;
    }
    if (exp < 0) {
      result = 1 / result;
    }
  }

  return result;
}
