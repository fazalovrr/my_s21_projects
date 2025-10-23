#include <check.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_string.h"

typedef struct TestCases {
  Suite *suite;
  TCase *tcase;
  SRunner *runner;
} TestCases;

START_TEST(s21_insert_test) {
  {
    const char *str = "HelloWorld";
    const char *str_two = "Beautiful";
    char *result = s21_insert(str, str_two, 5);
    ck_assert_str_eq(result, "HelloBeautifulWorld");
    free(result);
  }
  {
    const char *str = "Beautiful";
    char *result = s21_insert(S21_NULL, str, 5);
    ck_assert_ptr_eq(result, S21_NULL);
    free(result);
  }
  {
    const char *str = "HelloWorld";
    char *result = s21_insert(str, S21_NULL, 5);
    ck_assert_ptr_eq(result, S21_NULL);
    free(result);
  }
  {
    const char *str = "Hi";
    const char *str_two = "Friend";
    char *result = s21_insert(str, str_two, 10);
    ck_assert_ptr_eq(result, S21_NULL);
  }
}
END_TEST

START_TEST(s21_memchr_test) {
  {
    const char *str = "abcdef";

    void *res = s21_memchr(str, 'c', 6);
    void *res_two = memchr(str, 'c', 6);

    ck_assert_ptr_eq(res, res_two);
  }
  {
    const char *str = "xyz";

    void *res = s21_memchr(str, 'x', 3);
    void *res_two = memchr(str, 'x', 3);

    ck_assert_ptr_eq(res, res_two);
  }
  {
    const char *str = "xyz";

    void *res = s21_memchr(str, 'z', 3);
    void *res_two = memchr(str, 'z', 3);

    ck_assert_ptr_eq(res, res_two);
  }
  {
    const char *str = "abcdef";

    void *res = s21_memchr(str, 'x', 6);
    void *res_two = memchr(str, 'x', 6);

    ck_assert_ptr_eq(res, res_two);
  }
}
END_TEST

START_TEST(s21_memcmp_test) {
  {
    const char *str = "xbcde";
    const char *str_two = "abcde";

    int res = s21_memcmp(str, str_two, 5);
    int res_two = memcmp(str, str_two, 5);

    ck_assert_int_gt(res, 0);
    ck_assert_int_gt(res_two, 0);
  }
  {
    const char *str = "abcde";
    const char *str_two = "xbcde";

    int res = s21_memcmp(str, str_two, 5);
    int res_two = memcmp(str, str_two, 5);

    ck_assert_int_lt(res, 0);
    ck_assert_int_lt(res_two, 0);
  }
  {
    const char *str = "same";
    const char *str_two = "same";

    int res = s21_memcmp(str, str_two, 4);
    int res_two = memcmp(str, str_two, 4);

    ck_assert_int_eq(res, 0);
    ck_assert_int_eq(res_two, 0);
  }
  {
    const char *str = "abcdf";
    const char *str_two = "abcde";

    int res = s21_memcmp(str, str_two, 5);
    int res_two = memcmp(str, str_two, 5);

    ck_assert_int_eq(res, res_two);
  }
  {
    const char *str = "abc";
    const char *str_two = "xyz";

    int res = s21_memcmp(str, str_two, 0);
    int res_two = memcmp(str, str_two, 0);

    ck_assert_int_eq(res, res_two);
  }
}
END_TEST

START_TEST(s21_memcpy_test) {
  {
    char str[7] = {0};
    char str_two[7] = {0};

    s21_memcpy(str, "abcdef", 6);
    memcpy(str_two, "abcdef", 6);

    ck_assert_str_eq(str, str_two);
  }
  {
    char str[7] = {0};
    char str_two[7] = {0};

    s21_memcpy(str, "abcdef", 3);
    memcpy(str_two, "abcdef", 3);

    ck_assert_str_eq(str, str_two);
  }
  {
    char str[7] = "abcdef";
    char str_two[7] = "abcdef";

    s21_memcpy(str, "123456", 0);
    memcpy(str_two, "123456", 0);

    ck_assert_str_eq(str, str_two);
  }
}
END_TEST

START_TEST(s21_memset_test) {
  {
    char str[6] = "abcde";
    char str_two[6] = "abcde";

    s21_memset(str, 'x', 5);
    memset(str_two, 'x', 5);
    ck_assert_mem_eq(str, str_two, 5);
  }
  {
    char str[6] = "12345";
    char str_two[6] = "12345";

    s21_memset(str, 0, 5);
    memset(str_two, 0, 5);

    for (int i = 0; i < 5; ++i) ck_assert_int_eq(str[i], str_two[i]);
  }
  {
    char str[6] = "12345";
    char str_two[6] = "12345";

    s21_memset(str, 'y', 3);
    memset(str_two, 'y', 3);

    ck_assert_mem_eq(str, str_two, 6);
  }
  {
    char str[6] = "hello";
    char str_two[6] = "hello";

    s21_memset(str, 'z', 0);
    memset(str_two, 'z', 0);

    ck_assert_str_eq(str, str_two);
  }
}
END_TEST

START_TEST(s21_sprintf_test) {
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%s:%s", "Hello", S21_NULL);
    int res_two = sprintf(str_two, "%s:%s", "Hello", NULL);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%50s", "Hello");
    int res_two = sprintf(str_two, "%50s", "Hello");

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%-50s", "Hello");
    int res_two = sprintf(str_two, "%-50s", "Hello");

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%c", 'c');
    int res_two = sprintf(str_two, "%c", 'c');

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%+d:%i", 123, -321);
    int res_two = sprintf(str_two, "%+d:%i", 123, -321);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%.*d", 20, -123456);
    int res_two = sprintf(str_two, "%.*d", 20, -123456);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%.10s", "test");
    int res_two = sprintf(str_two, "%.10s", "test");

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%.3s", "test");
    int res_two = sprintf(str_two, "%.3s", "test");

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "% 010d %+015d", 123456, 654321);
    int res_two = sprintf(str_two, "% 010d %+015d", 123456, 654321);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%+.5f %f", 12.345678, NAN);
    int res_two = sprintf(str_two, "%+.5f %f", 12.345678, NAN);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%#.0f", -12.345678);
    int res_two = sprintf(str_two, "%#.0f", -12.345678);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};
    char test[] = "Hello World";

    int res = s21_sprintf(str, "%p", test);
    int res_two = sprintf(str_two, "%p", test);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%hd %ld", 10000000, 12345678);
    int res_two = sprintf(str_two, "%hd %ld", 10000000, 12345678);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%#x:%#X:%lx:%hx", 123, 321, 111, 4444);
    int res_two = sprintf(str_two, "%#x:%#X:%lx:%hx", 123, 321, 111, 4444);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int len = 0;
    int len_two = 0;

    int res = s21_sprintf(str, "%o %n", 123321, &len);
    int res_two = sprintf(str_two, "%o %n", 123321, &len_two);

    ck_assert_int_eq(res, res_two);
    ck_assert_int_eq(len, len_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%#e:%.E", 0.0, 0.0);
    int res_two = sprintf(str_two, "%#e:%.E", 0.0, 0.0);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }

  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%e:%E", 11111111111.11, -11111.000000000000001);
    int res_two =
        sprintf(str_two, "%e:%E", 11111111111.11, -11111.000000000000001);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%%");
    int res_two = sprintf(str_two, "%%");

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%.0d", 0);
    int res_two = sprintf(str_two, "%.0d", 0);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%03.2d", 1);
    int res_two = sprintf(str_two, "%03.2d", 1);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%F", NAN);
    int res_two = sprintf(str_two, "%F", NAN);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%-0.5f", 123.0);
    int res_two = sprintf(str_two, "%-0.5f", 123.0);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%0.0u", 0);
    int res_two = sprintf(str_two, "%0.0u", 0);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str_two, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%#1.0o", 0);
    int res_two = sprintf(str_two, "%#1.0o", 0);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%#.0u", 10);
    int res_two = sprintf(str_two, "%#.0u", 10);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%f", 5.99999999);
    int res_two = sprintf(str_two, "%f", 5.99999999);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%.3f", 9.9999);
    int res_two = sprintf(str_two, "%.3f", 9.9999);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%g", 123456.789);
    int res_two = sprintf(str_two, "%g", 123456.789);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%c", 'A');
    int res_two = sprintf(str_two, "%c", 'A');

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%s", S21_NULL);
    int res_two = sprintf(str_two, "%s", S21_NULL);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%05d", -3);
    int res_two = sprintf(str_two, "%05d", -3);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "% f", 3.14);
    int res_two = sprintf(str_two, "% f", 3.14);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%*f", 10, 3.14159);
    int res_two = sprintf(str_two, "%*f", 10, 3.14159);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%.0g", 123.456);
    int res_two = sprintf(str_two, "%.0g", 123.456);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%g", -0.0);
    int res_two = sprintf(str_two, "%g", -0.0);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%.10g", 1.234567e-20);
    int res_two = sprintf(str_two, "%.10g", 1.234567e-20);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%+g", 42.0);
    int res_two = sprintf(str_two, "%+g", 42.0);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "% g", 42.0);
    int res_two = sprintf(str_two, "% g", 42.0);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%.3g", 1e-310);
    int res_two = sprintf(str_two, "%.3g", 1e-310);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%g", 0.0);
    int res_two = sprintf(str_two, "%g", 0.0);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%+E", 123.456);
    int res_two = sprintf(str_two, "%+E", 123.456);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "% E", 123.456);
    int res_two = sprintf(str_two, "% E", 123.456);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%*d", -10, 42);
    int res_two = sprintf(str_two, "%*d", -10, 42);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%.5d", -123);
    int res_two = sprintf(str_two, "%.5d", -123);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%g", DBL_MIN);
    int res_two = sprintf(str_two, "%g", DBL_MIN);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%#x", 0);
    int res_two = sprintf(str_two, "%#x", 0);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    void *ptr = S21_NULL;

    int res = s21_sprintf(str, "%p", ptr);
    int res_two = sprintf(str_two, "%p", ptr);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    long double val = 3.1426159;

    int res = s21_sprintf(str, "%Lf", val);
    int res_two = sprintf(str_two, "%Lf", val);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    double neg_nan = -NAN;

    int res = s21_sprintf(str, "%f", neg_nan);
    int res_two = sprintf(str_two, "%f", neg_nan);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    double neg_inf = -INFINITY;

    int res = s21_sprintf(str, "%+f% F", neg_inf, -neg_inf);
    int res_two = sprintf(str_two, "%+f% F", neg_inf, -neg_inf);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%");
    int res_two = sprintf(str_two, "%");

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%g", 123.456);
    int res_two = sprintf(str_two, "%g", 123.456);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%e", DBL_MIN);
    int res_two = sprintf(str_two, "%e", DBL_MIN);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%d", INT_MIN);
    int res_two = sprintf(str_two, "%d", INT_MIN);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%+2d", 0);
    int res_two = sprintf(str_two, "%+2d", 0);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%#o", 123);
    int res_two = sprintf(str_two, "%#o", 123);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
  {
    char str[1000] = {0};
    char str_two[1000] = {0};

    int res = s21_sprintf(str, "%0d", 0);
    int res_two = sprintf(str_two, "%0d", 0);

    ck_assert_int_eq(res, res_two);
    ck_assert_str_eq(str, str_two);
  }
}
END_TEST

START_TEST(s21_sscanf_test) {
  {
    int std = 0, s21 = 0;
    int r_std = sscanf("42", "%d", &std);
    int r_s21 = s21_sscanf("42", "%d", &s21);
    ck_assert_int_eq(r_std, r_s21);
    ck_assert_int_eq(std, s21);
  }
  {
    int s21 = 0, std = 0;
    int r_std = sscanf("12345", "%3d", &std);
    int r_s21 = s21_sscanf("12345", "%3d", &s21);
    ck_assert_int_eq(r_std, r_s21);
    ck_assert_int_eq(std, s21);
  }
  {
    long s21 = 0, std = 0;
    int r_std = sscanf("123", "%ld", &std);
    int r_s21 = s21_sscanf("123", "%ld", &s21);
    ck_assert_int_eq(r_std, r_s21);
    ck_assert_int_eq(std, s21);
  }
  {
    unsigned int s21 = 0, std = 0;
    int r_std = sscanf("123", "%u", &std);
    int r_s21 = s21_sscanf("123", "%u", &s21);
    ck_assert_int_eq(r_std, r_s21);
    ck_assert_int_eq(std, s21);
  }
  {
    unsigned int s21 = 0, std = 0;
    int r_std = sscanf("1A", "%x", &std);
    int r_s21 = s21_sscanf("1A", "%x", &s21);
    ck_assert_int_eq(r_std, r_s21);
    ck_assert_int_eq(std, s21);
  }
  {
    unsigned int s21 = 0, std = 0;
    int r_std = sscanf("17", "%o", &std);
    int r_s21 = s21_sscanf("17", "%o", &s21);
    ck_assert_int_eq(r_std, r_s21);
    ck_assert_int_eq(std, s21);
  }
  {
    float s21 = 0, std = 0;
    int r_std = sscanf("123.456", "%f", &std);
    int r_s21 = s21_sscanf("123.456", "%f", &s21);
    ck_assert_int_eq(r_std, r_s21);
    ck_assert_float_eq(std, s21);
  }
  {
    long double s21 = 0, std = 0;
    int r_std = sscanf("123.456", "%Lf", &std);
    int r_s21 = s21_sscanf("123.456", "%Lf", &s21);
    ck_assert_int_eq(r_std, r_s21);
    ck_assert_float_eq((float)std, (float)s21);
  }
  {
    char std[100] = {0}, s21[100] = {0};
    int r_std = sscanf("Hello", "%s", std);
    int r_s21 = s21_sscanf("Hello", "%s", s21);
    ck_assert_int_eq(r_std, r_s21);
    ck_assert_str_eq(std, s21);
  }
  {
    char std[100] = {0}, s21[100] = {0};
    int r_std = sscanf("abcdef", "%3s", std);
    int r_s21 = s21_sscanf("abcdef", "%3s", s21);
    ck_assert_int_eq(r_std, r_s21);
    ck_assert_str_eq(std, s21);
  }
  {
    char std = '\0', s21 = '\0';
    int r_std = sscanf("X", "%c", &std);
    int r_s21 = s21_sscanf("X", "%c", &s21);
    ck_assert_int_eq(r_std, r_s21);
    ck_assert_int_eq(std, s21);
  }
  {
    void *std = NULL, *s21 = NULL;
    int r_std = sscanf("7FFF", "%p", &std);
    int r_s21 = s21_sscanf("7FFF", "%p", &s21);
    ck_assert_int_eq(r_std, r_s21);
    ck_assert_ptr_eq(std, s21);
  }
  {
    int std = -1, s21 = -1;
    int r_std = sscanf("abc123", "abc%n", &std);
    int r_s21 = s21_sscanf("abc123", "abc%n", &s21);
    ck_assert_int_eq(r_std, r_s21);
    ck_assert_int_eq(std, s21);
  }
  {
    int std = 0, s21 = 0;
    int r_std = sscanf("42 84", "%*d %d", &std);
    int r_s21 = s21_sscanf("42 84", "%*d %d", &s21);
    ck_assert_int_eq(r_std, r_s21);
    ck_assert_int_eq(std, s21);
  }
  {
    int std = 0, s21 = 0;
    int r_std = sscanf("123", "%z", &std);
    int r_s21 = s21_sscanf("123", "%z", &s21);
    ck_assert_int_eq(r_std, r_s21);
  }
  {
    int std = 0, s21 = 0;
    int r_std = sscanf("", "%d", &std);
    int r_s21 = s21_sscanf("", "%d", &s21);
    ck_assert_int_eq(r_std, r_s21);
  }
  {
    int std = 0, s21 = 0;
    const char *input = "42 0x2A";

    int r_std = sscanf(input, "%d %i", &std, &s21);
    int r_s21 = s21_sscanf(input, "%d %i", &std, &s21);

    ck_assert_int_eq(r_std, r_s21);
    ck_assert_int_eq(std, 42);
    ck_assert_int_eq(s21, 42);
  }
  {
    int val_std = 0, val_s21 = 0;

    const char *input = "12345XYZ";
    const char *format = "%3i";

    int r_std = sscanf(input, format, &val_std);
    int r_s21 = s21_sscanf(input, format, &val_s21);

    ck_assert_int_eq(r_std, r_s21);

    ck_assert_int_eq(val_std, 123);
    ck_assert_int_eq(val_s21, 123);
  }
  {
    int std_neg = 0, s21_neg = 0;
    int std_pos = 0, s21_pos = 0;

    const char *input_neg = "-456";
    const char *input_pos = "+789";

    int r_std_neg = sscanf(input_neg, "%i", &std_neg);
    int r_s21_neg = s21_sscanf(input_neg, "%i", &s21_neg);

    int r_std_pos = sscanf(input_pos, "%i", &std_pos);
    int r_s21_pos = s21_sscanf(input_pos, "%i", &s21_pos);

    ck_assert_int_eq(r_std_neg, r_s21_neg);
    ck_assert_int_eq(r_std_pos, r_s21_pos);

    ck_assert_int_eq(std_neg, s21_neg);
    ck_assert_int_eq(std_pos, s21_pos);
  }
  {
    int s21_value = 0;

    const char *only_plus = "+";
    const char *only_minus = "-";

    int r_plus = s21_sscanf(only_plus, "%i", &s21_value);
    int r_minus = s21_sscanf(only_minus, "%i", &s21_value);

    ck_assert_int_eq(r_plus, 0);
    ck_assert_int_eq(r_minus, 0);
  }
  {
    int s21_val = 0, val = 0;
    const char *input = "123456789";

    int res = s21_sscanf(input, "%5i", &s21_val);
    int res_two = sscanf(input, "%5i", &val);

    ck_assert_int_eq(res, res_two);
    ck_assert_int_eq(s21_val, val);
  }
  {
    int s21_value = 0;
    int value = 0;
    const char *input = "123456";

    int res = s21_sscanf(input, "%5d", &s21_value);
    int res_two = sscanf(input, "%5d", &value);

    ck_assert_int_eq(res, res_two);
    ck_assert_int_eq(s21_value, value);
  }
  {
    float s21_value = 0;
    float value = 0;
    const char *input = "3.1415926535";

    int res = s21_sscanf(input, "%7f", &s21_value);
    int res_two = sscanf(input, "%7f", &value);

    ck_assert_int_eq(res, res_two);
    ck_assert_float_eq(s21_value, value);
  }
  {
    char c1, c2;
    char s1[10] = "", s2[10] = "";
    int d1, d2;
    long ld1, ld2;
    short hd1, hd2;
    char str[] = "  A ABC 1 12345678910 12345";
    char format[] = "  %c %s %9d %ld %hd";

    int res = sscanf(str, format, &c1, s1, &d1, &ld1, &hd1);
    int res_two = s21_sscanf(str, format, &c2, s2, &d2, &ld2, &hd2);

    ck_assert_int_eq(res, res_two);
    ck_assert_int_eq(c1, c2);
    ck_assert_str_eq(s1, s2);
    ck_assert_int_eq(d1, d2);
    ck_assert_int_eq(ld1, ld2);
    ck_assert_int_eq(hd1, hd2);
  }
  {
    char c1, c2;
    char str[] = "A  % B";
    char format[] = "%*c %% %c";

    int res = sscanf(str, format, &c1);
    int res_two = s21_sscanf(str, format, &c2);

    ck_assert_int_eq(res, res_two);
    ck_assert_int_eq(c1, c2);
  }
  {
    float e1 = 0, e2 = 0;
    int res_two = 0, res = 0;

    res_two = sscanf("123e34", "%e", &e1);
    res = s21_sscanf("123e34", "%e", &e2);
    ck_assert_msg(e1 == e2, "float not equals");
    ck_assert_int_eq(res_two, res);

    res_two = sscanf("12e34", "%e", &e1);
    res = s21_sscanf("12e34", "%e", &e2);
    ck_assert_msg(e1 == e2, "float not equals");
    ck_assert_int_eq(res_two, res);

    res_two = sscanf("123.345e-34", "%9e", &e1);
    res = s21_sscanf("123.345e-34", "%9e", &e2);
    ck_assert_int_eq(res_two, res);
    ck_assert_float_eq(e1, e2);
    ck_assert_msg(e1 == e2, "float not equals");

    res_two = sscanf("123.345e34", "%8e", &e1);
    res = s21_sscanf("123.345e34", "%8e", &e2);
    ck_assert_float_eq(e1, e2);
    ck_assert_msg(e1 == e2, "float not equals");
    ck_assert_int_eq(res_two, res);
    long double Le1, Le2;

    res_two = sscanf("+123.345e34", "%8Le", &Le1);
    res = s21_sscanf("+123.345e34", "%8Le", &Le2);
    ck_assert_float_eq(Le1, Le2);
    ck_assert_msg(Le1 == Le2, "float not equals");
    ck_assert_int_eq(res_two, res);
  }
  {
    int res_two, res, n1 = 0, n2 = 0;
    long double e1 = -9, e2 = -9, g1 = -9, g2 = -9;
    char empty_str[] = "";
    char str[] = "  0.08762147E+03  1446567457.024E-11 0E+02 ";
    char wrong_str[] = "  35634E+02";

    res_two = sscanf(str, " %LE %LG %n %*e ", &e1, &g1, &n1);
    res = s21_sscanf(str, " %LE %LG %n %*e ", &e2, &g2, &n2);
    ck_assert_double_eq_tol(e1, e2, 1e-6);
    ck_assert_double_eq_tol(g1, g2, 1e-6);
    ck_assert_int_eq(g1, g2);
    ck_assert_int_eq(n1, n2);
    ck_assert_int_eq(res_two, res);
    long double Le1 = 0, Le2 = 0;

    res_two = sscanf(empty_str, "%Le", &Le1);
    res = s21_sscanf(empty_str, "%Le", &Le2);
    ck_assert_int_eq(Le1, Le2);
    ck_assert_double_eq_tol(Le1, Le2, 1e-6);
    ck_assert_int_eq(res_two, res);
    long double Lg1 = 0, Lg2 = 0;

    res_two = sscanf(wrong_str, " %9Lg", &Lg1);
    res = s21_sscanf(wrong_str, " %9Lg", &Lg2);
    ck_assert_int_eq(Lg1, Lg2);
    ck_assert_int_eq(res_two, res);
    float in1 = 0, in2 = 0, in3 = 0, in4 = 0;

    res_two = sscanf(" -50 +500 +", "%g %g", &in1, &in3);
    res = s21_sscanf(" -50 +500 +", "%g %g", &in2, &in4);
    ck_assert_int_eq(in1, in2);
    ck_assert_int_eq(in3, in4);
    ck_assert_int_eq(res_two, res);
  }
  {
    int res_two = 0, res = 0;
    void *p1 = 0, *p2 = 0;

    res_two = sscanf("0x123", "%4p", &p1);
    res = s21_sscanf("0x123", "%4p", &p2);
    ck_assert_ptr_eq(p1, p2);
    ck_assert_int_eq(res_two, res);

    res_two = sscanf("0x12345", "%p", &p1);
    res = s21_sscanf("0x12345", "%p", &p2);
    ck_assert_ptr_eq(p1, p2);
    ck_assert_int_eq(res_two, res);

    res_two = sscanf("123.456", "%p", &p1);
    res = s21_sscanf("123.456", "%p", &p2);
    ck_assert_ptr_eq(p1, p2);
    ck_assert_int_eq(res_two, res);
  }
  {
    unsigned u1 = 1, u2 = 1, res_two, res;
    short unsigned int hu1 = 1, hu2 = 2;
    long unsigned int lu1 = 1, lu2 = 2;
    char str[] = " 0    3147483647";
    char wrong_str[] = "  Verter_Hi 123456";
    char empty_str[] = "";

    res_two = sscanf(empty_str, "%10u", &u1);
    res = s21_sscanf(empty_str, "%10u", &u2);
    ck_assert_uint_eq(u1, u2);
    ck_assert_int_eq(res_two, res);

    res_two = sscanf(str, "%*u%10u", &u1);
    res = s21_sscanf(str, "%*u%10u", &u2);
    ck_assert_uint_eq(u1, u2);
    ck_assert_int_eq(res_two, res);

    res_two = sscanf(str, "%*d %lu", &lu1);
    res = s21_sscanf(str, "%*d %lu", &lu2);
    ck_assert_int_eq(lu1, lu2);
    ck_assert_int_eq(res_two, res);

    res_two = sscanf(wrong_str, "%*s %hu", &hu1);
    res = s21_sscanf(wrong_str, "%*s %hu", &hu2);
    ck_assert_uint_eq(hu1, hu2);
    ck_assert_int_eq(res_two, res);
  }
  {
    int res_two = 0, res = 0;
    unsigned int o1 = 1, o2 = 2;
    short unsigned int ho1 = 1, ho2 = 2;
    long unsigned int lo1 = 1, lo2 = 2;
    char A[] = "12345678";
    char B[] = "12345678123456789";

    res_two = sscanf(A, "%3o", &o1);
    res = s21_sscanf(A, "%3o", &o2);
    ck_assert_int_eq(o1, o2);
    ck_assert_int_eq(res_two, res);

    res_two = sscanf(B, "%9o", &o1);
    res = s21_sscanf(B, "%9o", &o2);
    ck_assert_int_eq(o1, o2);
    ck_assert_int_eq(res_two, res);

    res_two = sscanf(B, "%ho", &ho1);
    res = s21_sscanf(B, "%ho", &ho2);
    ck_assert_int_eq(ho1, ho2);
    ck_assert_int_eq(res_two, res);

    res_two = sscanf("123.456", "%lo", &lo1);
    res = s21_sscanf("123.456", "%lo", &lo2);
    ck_assert_uint_eq(lo1, lo2);
    ck_assert_int_eq(res_two, res);
  }
  {
    char empty_str[] = "";
    int in1 = 1, in2 = 1;
    short int in3 = 1, in4 = 1;
    long int in5 = 1, in6 = 1;
    int res_two, res;
    char str3[] = "  0x3F5  0600 -12345 987";
    char wrong_str3[] = "  0000xFF5  ";

    res_two = sscanf(str3, " %5i %hi %li %*i", &in1, &in3, &in5);
    res = s21_sscanf(str3, " %5i %hi %li %*i", &in2, &in4, &in6);
    ck_assert_int_eq(in1, in2);
    ck_assert_int_eq(in3, in4);
    ck_assert_int_eq(in5, in6);
    ck_assert_int_eq(res_two, res);

    res_two = sscanf(empty_str, "%i", &in1);
    res = s21_sscanf(empty_str, "%i", &in2);
    ck_assert_int_eq(in1, in2);
    ck_assert_int_eq(res_two, res);

    res_two = sscanf(wrong_str3, "%i", &in1);
    res = s21_sscanf(wrong_str3, "%i", &in2);
    ck_assert_uint_eq(in1, in2);
    ck_assert_int_eq(res_two, res);
    unsigned int octnmb1 = 1, octnmb2 = 2;

    res_two = sscanf(str3, "%*x%o", &octnmb1);
    res = s21_sscanf(str3, "%*x%o", &octnmb2);
    ck_assert_uint_eq(octnmb1, octnmb2);
    ck_assert_int_eq(res_two, res);
  }
  {
    unsigned u1 = 1, u2 = 1, res_two, res;
    short unsigned int hu1 = 1, hu2 = 2;
    long unsigned int lu1 = 1, lu2 = 2;
    char str[] = " 123123  3147487 123 123 4321";

    res_two = sscanf(str, "%*x %5x %lX %*X %hx", &u1, &lu1, &hu1);
    res = s21_sscanf(str, "%*x %5x %lX %*X %hx", &u2, &lu2, &hu2);

    ck_assert_uint_eq(u1, u2);
    ck_assert_uint_eq(lu1, lu2);
    ck_assert_uint_eq(hu1, hu2);
    ck_assert_int_eq(res_two, res);
  }
  {
    int s21_value = 0;
    int value = 0;
    const char *input = "123";
    const char *format = "%";

    int res = s21_sscanf(input, format, &s21_value);
    int res_two = sscanf(input, format, &value);

    ck_assert_int_eq(res, res_two);
    ck_assert_int_eq(s21_value, value);
  }
  {
    int s21_value = 0;
    int value = 0;
    const char *input = "123";
    const char *format = S21_NULL;

    int res = s21_sscanf(input, format, &s21_value);
    int res_two = sscanf(input, format, &value);

    ck_assert_int_eq(res, res_two);
    ck_assert_int_eq(s21_value, value);
  }
  {
    const char *input = "123";
    const char *format = "%d";
    int s21_val = 0;
    int val = 0;

    int res = s21_sscanf(input, format, &s21_val);
    int res_two = sscanf(input, format, &val);

    ck_assert_int_eq(res, res_two);
    ck_assert_int_eq(s21_val, val);
  }
  {
    const char *input = "%";
    const char *format = "%%";

    int res = s21_sscanf(input, format);
    int res_two = sscanf(input, format);

    ck_assert_int_eq(res, res_two);
  }
  {
    const char *input = "42abc";
    const char *format = "%d%n";
    int s21_val = 0, val = 0, parsed = 0, s21_parsed = 0;

    int res = s21_sscanf(input, format, &s21_val, &s21_parsed);
    int res_two = sscanf(input, format, &val, &parsed);

    ck_assert_int_eq(res, res_two);
    ck_assert_int_eq(s21_val, val);
    ck_assert_int_eq(s21_parsed, parsed);
  }
  {
    const char *input = "text";
    const char *format = "%n";
    int s21_val = -1;
    int val = -1;

    int res = s21_sscanf(input, format, &s21_val);
    int res_two = sscanf(input, format, &val);

    ck_assert_int_eq(res, res_two);
    ck_assert_int_eq(s21_val, val);
  }
  {
    const char *input = "abc";
    const char *format = "%%";

    int res = s21_sscanf(input, format);
    int res_two = sscanf(input, format);

    ck_assert_int_eq(res, res_two);
  }
  {
    const char *input = "12345";
    const char *format = "%*n";
    int val = 0;
    int s21_val = 0;

    int res = s21_sscanf(input, format, &val);
    int res_two = s21_sscanf(input, format, &s21_val);

    ck_assert_int_eq(res, res_two);
    ck_assert_int_eq(val, s21_val);
  }
  {
    const char *input = "-";
    const char *format = "%d";
    int s21_val = 0;
    int val = 0;

    int res = s21_sscanf(input, format, &s21_val);
    int res_two = s21_sscanf(input, format, &val);

    ck_assert_int_eq(res, res_two);
    ck_assert_int_eq(s21_val, val);
  }
  {
    const char *input = "T\t t";
    const char *format = "%d";
    int s21_val = 0;
    int val = 0;

    int res = s21_sscanf(input, format, &s21_val);
    int res_two = s21_sscanf(input, format, &val);

    ck_assert_int_eq(res, res_two);
    ck_assert_int_eq(s21_val, val);
  }
}
END_TEST

START_TEST(s21_strchr_test) {
  {
    const char *str = "abcdef";

    char *res = s21_strchr(str, 'c');
    char *res_two = strchr(str, 'c');

    ck_assert_ptr_eq(res, res_two);
  }
  {
    const char *str = "xyz";

    char *res = s21_strchr(str, 'x');
    char *res_two = strchr(str, 'x');

    ck_assert_ptr_eq(res, res_two);
  }
  {
    const char *str = "12345";

    char *res = s21_strchr(str, '5');
    char *res_two = strchr(str, '5');

    ck_assert_ptr_eq(res, res_two);
  }
  {
    const char *str = "abcdef";

    char *res = s21_strchr(str, 'z');
    char *res_two = strchr(str, 'z');
    ck_assert_ptr_eq(res, res_two);
  }
  {
    const char *str = "data";

    char *res = s21_strchr(str, '\0');
    char *res_two = s21_strchr(str, '\0');

    ck_assert_ptr_eq(res, res_two);
  }
}
END_TEST

START_TEST(s21_strcspn_test) {
  {
    const char *str = "abcdef";
    const char *str_two = "123456";

    s21_size_t res = s21_strcspn(str, str_two);
    s21_size_t res_two = strcspn(str, str_two);

    ck_assert_int_eq(res, res_two);
  }
  {
    const char *str = "abcdef";
    const char *str_two = "d";

    s21_size_t res = s21_strcspn(str, str_two);
    s21_size_t res_two = strcspn(str, str_two);

    ck_assert_int_eq(res, res_two);
  }
  {
    const char *str = "abcdef";
    const char *str_two = "a";

    s21_size_t res = s21_strcspn(str, str_two);
    s21_size_t res_two = strcspn(str, str_two);

    ck_assert_int_eq(res, res_two);
  }
  {
    const char *str = "abcdef";
    const char *str_two = "f";

    s21_size_t res = s21_strcspn(str, str_two);
    s21_size_t res_two = strcspn(str, str_two);

    ck_assert_int_eq(res, res_two);
  }
  {
    const char *str = "abcdef";
    const char *str_two = "";

    s21_size_t res = s21_strcspn(str, str_two);
    s21_size_t res_two = strcspn(str, str_two);

    ck_assert_int_eq(res, res_two);
  }
  {
    const char *str = "";
    const char *str_two = "abc";

    s21_size_t res = s21_strcspn(str, str_two);
    s21_size_t res_two = s21_strcspn(str, str_two);

    ck_assert_int_eq(res, res_two);
  }
}
END_TEST

START_TEST(s21_strerror_test) {
  {
    char *res = s21_strerror(0);
    char *res_two = strerror(0);

    ck_assert_str_eq(res, res_two);
  }
  {
    char *res = s21_strerror(133);
    char *res_two = strerror(133);

    ck_assert_str_eq(res, res_two);
  }
  {
    char *res = s21_strerror(999);
    char *res_two = strerror(999);

    ck_assert_str_eq(res, res_two);
  }
  {
    char *res = s21_strerror(-5);
    char *res_two = strerror(-5);

    ck_assert_str_eq(res, res_two);
  }
}
END_TEST

START_TEST(s21_strncat_test) {
  {
    char str[20] = "Hello";
    const char *str_two = "World";

    char *res = s21_strncat(str, str_two, 5);
    char *res_two = strncat(str, str_two, 5);

    ck_assert_str_eq(res, res_two);
  }
  {
    char str[20] = "Good";
    const char *str_two = "Evening";

    char *res = s21_strncat(str, str_two, 4);
    char *res_two = strncat(str, str_two, 4);

    ck_assert_str_eq(res, res_two);
  }
  {
    char str[20] = "Start";
    const char *str_two = "Finish";

    char *res = s21_strncat(str, str_two, 0);
    char *res_two = strncat(str, str_two, 0);

    ck_assert_str_eq(res, res_two);
  }
  {
    char str[20] = "Base";
    const char *str_two = "";

    char *res = s21_strncat(str, str_two, 3);
    char *res_two = strncat(str, str_two, 3);

    ck_assert_str_eq(res, res_two);
  }
  {
    char str[20] = "Wow";
    const char *str_two = "Amazing!";

    char *res = s21_strncat(str, str_two, 100);
    char *res_two = strncat(str, str_two, 100);

    ck_assert_str_eq(res, res_two);
  }
}
END_TEST

START_TEST(s21_strncmp_test) {
  {
    const char *str = "abc";
    const char *str_two = "xyz";

    int res = s21_strncmp(str, str_two, 0);
    int res_two = strncmp(str, str_two, 0);

    ck_assert_int_eq(res, res_two);
  }
  {
    const char *str = "apple";
    const char *str_two = "apples";

    int res = s21_strncmp(str, str_two, 5);
    int res_two = strncmp(str, str_two, 5);

    ck_assert_int_eq(res, res_two);
  }
  {
    const char *str = "abc";
    const char *str_two = "abc";

    int res = s21_strncmp(str, str_two, 10);
    int res_two = strncmp(str, str_two, 10);

    ck_assert_int_eq(res, res_two);
  }
  {
    const char *str = "qwerty";
    const char *str_two = "abc";

    int res = s21_strncmp(str, str_two, 4);
    int res_two = strncmp(str, str_two, 4);

    ck_assert_int_eq((res > 0) - (res < 0), (res_two > 0) - (res_two < 0));
  }
}
END_TEST

START_TEST(s21_strncpy_test) {
  {
    char str[10] = {0};
    const char *str_two = "Hello";

    char *res = s21_strncpy(str, str_two, 5);
    char *res_two = strncpy(str, str_two, 5);

    ck_assert_str_eq(res, res_two);
  }
  {
    char str[10] = {0};
    const char *str_two = "Hi";

    char *res = s21_strncpy(str, str_two, 5);
    char *res_two = strncpy(str, str_two, 5);

    ck_assert_str_eq(res, res_two);
  }
  {
    char str[10] = "Init";
    const char *str_two = "Data";

    char *res = s21_strncpy(str, str_two, 5);
    char *res_two = strncpy(str, str_two, 5);

    ck_assert_str_eq(res, res_two);
  }
  {
    char str[10] = "OldText";
    const char *str_two = "";

    char *res = s21_strncpy(str, str_two, 5);
    char *res_two = strncpy(str, str_two, 5);

    ck_assert_str_eq(res, res_two);
  }
}
END_TEST

START_TEST(s21_strrchr_test) {
  {
    const char *str = "abracadabra";
    int ch = 'a';
    char *res = s21_strrchr(str, ch);
    char *res_two = strrchr(str, ch);
    ck_assert_ptr_eq(res, res_two);
  }
  {
    const char *str = "teststring";
    int ch = 'z';
    char *res = s21_strrchr(str, ch);
    char *res_two = strrchr(str, ch);
    ck_assert_ptr_eq(res, res_two);
  }
  {
    const char *str = "hello";
    int ch = '\0';
    char *res = s21_strrchr(str, ch);
    char *res_two = strrchr(str, ch);
    ck_assert_ptr_eq(res, res_two);
  }
  {
    const char *str = "";
    int ch = 'a';
    char *res = s21_strrchr(str, ch);
    char *res_two = strrchr(str, ch);
    ck_assert_ptr_eq(res, res_two);
  }
  {
    const char *str = "apple";
    int ch = 'a';
    char *res = s21_strrchr(str, ch);
    char *res_two = strrchr(str, ch);
    ck_assert_ptr_eq(res, res_two);
  }
}
END_TEST

START_TEST(s21_strstr_test) {
  {
    const char *str = "Hello, world!";
    const char *str_two = "world";
    char *res = s21_strstr(str, str_two);
    char *res_two = strstr(str, str_two);
    ck_assert_ptr_eq(res, res_two);
  }
  {
    const char *str = "Hello, world!";
    const char *str_two = "planet";
    char *res = s21_strstr(str, str_two);
    char *res_two = strstr(str, str_two);
    ck_assert_ptr_eq(res, res_two);
  }
  {
    const char *str = "Hello";
    const char *str_two = "";
    char *res = s21_strstr(str, str_two);
    char *res_two = strstr(str, str_two);
    ck_assert_ptr_eq(res, res_two);
  }
  {
    const char *str = "";
    const char *str_two = "Hi";
    char *res = s21_strstr(str, str_two);
    char *res_two = strstr(str, str_two);
    ck_assert_ptr_eq(res, res_two);
  }
  {
    const char *str = "match";
    const char *str_two = "match";
    char *res = s21_strstr(str, str_two);
    char *res_two = strstr(str, str_two);
    ck_assert_ptr_eq(res, res_two);
  }
}
END_TEST

START_TEST(s21_strtok_test) {
  {
    char str[] = "one,two,three";
    char str_two[] = "one,two,three";
    char delim[] = ",";
    int i = 0;

    char *res = s21_strtok(str, delim);
    char *res_two = strtok(str_two, delim);
    while (res != S21_NULL) {
      ck_assert_str_eq(res, res_two);
      res = s21_strtok(S21_NULL, delim);
      res_two = strtok(S21_NULL, delim);
      i++;
    }
    ck_assert_int_eq(i, 3);
  }
  {
    char str[] = "";
    char str_two[] = "";
    char delim[] = "";

    char *res = s21_strtok(str, delim);
    char *res_two = strtok(str_two, delim);

    ck_assert_ptr_eq(res, res_two);
  }
  {
    char str[] = "Hello, World!";
    char str_two[] = "Hello, World!";
    char delim[] = "";

    char *res = s21_strtok(str, delim);
    char *res_two = strtok(str_two, delim);

    ck_assert_str_eq(res, res_two);
  }
  {
    char str[] = "";
    char str_two[] = "";
    char delim[] = ",";

    char *res = s21_strtok(str, delim);
    char *res_two = strtok(str_two, delim);

    ck_assert_ptr_eq(res, res_two);
  }
  {
    char str[] = "abc";
    char str_two[] = "abc";
    char delim[] = ",";

    char *res = s21_strtok(str, delim);
    char *res_two = strtok(str_two, delim);

    ck_assert_str_eq(res, res_two);
  }
  {
    char str[] = "a,b;c:d";
    char str_two[] = "a,b;c:d";
    char delim[] = ",;:";
    int i = 0;

    char *res = s21_strtok(str, delim);
    char *res_two = strtok(str_two, delim);

    while (res != S21_NULL) {
      ck_assert_str_eq(res, res_two);
      res = s21_strtok(S21_NULL, delim);
      res_two = strtok(S21_NULL, delim);
      i++;
    }
    ck_assert_int_eq(i, 4);
  }
}
END_TEST

START_TEST(s21_to_lower_test) {
  {
    const char *str = "HELLO";
    char *res = s21_to_lower(str);
    ck_assert_str_eq(res, "hello");
    free(res);
  }
  {
    const char *str = "HeLLo WoRLd!";
    char *res = s21_to_lower(str);
    ck_assert_str_eq(res, "hello world!");
    free(res);
  }
  {
    const char *str = "";
    char *res = s21_to_lower(str);
    ck_assert_str_eq(res, "");
    free(res);
  }
  {
    const char *str = S21_NULL;
    char *res = s21_to_lower(str);
    ck_assert_ptr_eq(res, S21_NULL);
  }
}
END_TEST

START_TEST(s21_to_upper_test) {
  {
    const char *src = "hello world";
    char *res = s21_to_upper(src);
    ck_assert_str_eq(res, "HELLO WORLD");
    free(res);
  }
  {
    const char *src = "HeLLo123";
    char *res = s21_to_upper(src);
    ck_assert_str_eq(res, "HELLO123");
    free(res);
  }
  {
    const char *src = "";
    char *res = s21_to_upper(src);
    ck_assert_str_eq(res, "");
    free(res);
  }
  {
    const char *src = S21_NULL;
    char *res = s21_to_upper(src);
    ck_assert_ptr_eq(res, S21_NULL);
  }
  {
    const char *src = "123!@#";
    char *res = s21_to_upper(src);
    ck_assert_str_eq(res, "123!@#");
    free(res);
  }
  {
    const char *src = "abc";
    char *res = s21_to_upper(src);
    ck_assert_str_eq(res, "ABC");
    free(res);
  }
}
END_TEST

START_TEST(s21_trim_test) {
  {
    const char *src = "  Hello world!  ";
    const char *trim_chars = " ";
    char *res = s21_trim(src, trim_chars);
    ck_assert_str_eq(res, "Hello world!");
    free(res);
  }
  {
    const char *src = "+++";
    const char *trim_chars = "+";
    char *res = s21_trim(src, trim_chars);
    ck_assert_str_eq(res, "");
    free(res);
  }
  {
    const char *src = "Code";
    const char *trim_chars = " ";
    char *res = s21_trim(src, trim_chars);
    ck_assert_str_eq(res, "Code");
    free(res);
  }
  {
    char *res = s21_trim(S21_NULL, " ");
    ck_assert_ptr_eq(res, S21_NULL);
  }
  {
    char *res = s21_trim("***Test***", S21_NULL);
    ck_assert_ptr_eq(res, S21_NULL);
  }
}
END_TEST

TestCases init_test_cases(void) {
  TestCases tc;
  tc.suite = suite_create("s21_string");
  tc.tcase = tcase_create("s21_string");
  tc.runner = srunner_create(tc.suite);
  suite_add_tcase(tc.suite, tc.tcase);
  return tc;
}

void add_tests_to_case(TCase *tc) {
  tcase_add_test(tc, s21_insert_test);
  tcase_add_test(tc, s21_memchr_test);
  tcase_add_test(tc, s21_memcmp_test);
  tcase_add_test(tc, s21_memcpy_test);
  tcase_add_test(tc, s21_memset_test);
  tcase_add_test(tc, s21_sprintf_test);
  tcase_add_test(tc, s21_sscanf_test);
  tcase_add_test(tc, s21_strchr_test);
  tcase_add_test(tc, s21_strcspn_test);
  tcase_add_test(tc, s21_strerror_test);
  tcase_add_test(tc, s21_strncat_test);
  tcase_add_test(tc, s21_strncmp_test);
  tcase_add_test(tc, s21_strncpy_test);
  tcase_add_test(tc, s21_strrchr_test);
  tcase_add_test(tc, s21_strstr_test);
  tcase_add_test(tc, s21_strtok_test);
  tcase_add_test(tc, s21_to_lower_test);
  tcase_add_test(tc, s21_to_upper_test);
  tcase_add_test(tc, s21_trim_test);
}

int run_tests(SRunner *runner) {
  int failed_tests;
  srunner_run_all(runner, CK_NORMAL);
  failed_tests = srunner_ntests_failed(runner);
  srunner_free(runner);
  return failed_tests;
}

int main(void) {
  TestCases tc = init_test_cases();
  add_tests_to_case(tc.tcase);
  int failed_tests = run_tests(tc.runner);
  return failed_tests == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}