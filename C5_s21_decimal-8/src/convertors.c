// #include "help.h"
#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int return_code = 1;
  if (dst) {
    init_decimal(dst);
    if (src < 0) {
      set_sign(dst, 1);
      src *= -1;
    }
    dst->bits[0] = src;
    return_code = 0;
  }
  return return_code;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int return_code = 1;
  int exp = get_float_scale(&src);
  if (dst && src != 0 && !isinf(src)) {
    init_decimal(dst);
    int sign = 0;
    if (src < 0) {
      src *= -1;
      sign = 1;
    }
    // to get scale and drag the decimal point:
    double tmp = (double)src;
    int scale = 0;
    while (scale < 28 && (int)tmp / (int)my_pow(2, 21) == 0) {
      tmp *= 10;
      scale++;
    }
    if (scale <= 28 && (exp > -94 && exp < 96)) {
      float_bits fb;
      tmp = (float)tmp;
      while (my_fmod(tmp, 10) == 0 && scale > 0) {
        scale--;
        tmp /= 10;
      }
      fb.fl = tmp;
      exp = get_float_scale(&fb.fl);
      set_bit(dst, exp, 1);
      for (int i = exp - 1, j = 22; j >= 0; i--, j--) {
        set_bit(dst, i, (fb.ui & (1 << j)));
      }
      set_sign(dst, sign);
      set_scale(dst, scale);
      return_code = 0;
    }
  }
  return return_code;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int return_code = 1;
  if (src.bits[1] == 0 && src.bits[2] == 0) {
    *dst = src.bits[0];
    if (get_sign(src) == 1) {
      *dst *= -1;
    }
    int scale = get_scale(src);
    *dst /= my_pow(10, scale);
    return_code = 0;
  }
  return return_code;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int return_code = 1;
  if (dst) {
    double tmp = 0.0;
    int exp = 0;
    for (int i = 0; i < 96; i++) {
      if ((src.bits[i / 32] & (1u << i % 32)) != 0) {
        tmp += my_pow(2, i);
      }
    }
    if ((exp = get_scale(src)) > 0) {
      for (int i = exp; i > 0; i--) {
        tmp /= 10.0;
      }
    }
    *dst = (float)tmp;
    if (get_sign(src)) {
      *dst *= -1;
    }
    return_code = 0;
  }
  return return_code;
}
