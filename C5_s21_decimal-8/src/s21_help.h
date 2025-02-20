#ifndef HELP_H
#define HELP_H

typedef union {
  int ui;
  float fl;
} float_bits;

// help function
void init_decimal(s21_decimal *value);
int is_null(s21_decimal value);
int get_bit(s21_decimal value, int bit);
void set_bit(s21_decimal *value, int posision, int bit);
int get_sign(s21_decimal value);
void set_sign(s21_decimal *value, int bit);
int get_scale(s21_decimal value);
void set_scale(s21_decimal *value, int scale);
int get_float_scale(float *value);
int get_last_bit(s21_decimal value);

int add_bit(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int shift_left(s21_decimal *value, int shift);
void increase_scale(s21_decimal *value, int num);
void decrease_scale(s21_decimal *value, int num);
void alignment_scale(s21_decimal *value_1, s21_decimal *value_2);
void convert(s21_decimal *value);

double my_fmod(double x, int y);
double my_pow(double base, int exponent);
#endif
