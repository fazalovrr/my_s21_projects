#ifndef SUP_FUNC_H
#define SUP_FUNC_H
#include "../s21_decimal.h"

int is_zero(s21_decimal value);
void clear_decimal(s21_decimal *dec);
decimal_sign get_sign(s21_decimal value);
void set_sign(s21_decimal *dec, int sign);
int get_scale(s21_decimal value);
void set_scale(s21_decimal *dst, int scale);
big_decimal decimal_to_bigdecimal(s21_decimal src);
int normalize_bigdecimal_scale(big_decimal *value_1, big_decimal *value_2);
int get_overflow(big_decimal *src);
int normalize_bigdecimal_for_decimal(big_decimal *src);
void bigdecimal_to_decimal(big_decimal src, s21_decimal *dst);
int is_less_by_mod(s21_decimal value_1, s21_decimal value_2);
int is_equal_by_mod(s21_decimal value_1, s21_decimal value_2);
int pointleft(big_decimal *src);
// void set_bit(s21_decimal *decl, int index, int bit);
int pointright(big_decimal *src);
big_decimal sum_of_positive(big_decimal value_1, big_decimal value_2);
big_decimal sub_of_positive(big_decimal value_1, big_decimal value_2);

int is_less_or_equal_by_mod(s21_decimal a, s21_decimal b);
// int is_less_or_equal_by_mod_big(big_decimal a, big_decimal b);
// void clear_big_decimal(big_decimal *value);
// int is_zero_big(big_decimal value);

#endif  // SUP_FUNC_H
