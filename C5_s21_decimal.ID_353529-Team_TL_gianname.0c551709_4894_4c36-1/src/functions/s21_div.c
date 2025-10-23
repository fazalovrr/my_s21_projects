#include <stdint.h>
#include <string.h>

#include "../s21_decimal.h"
#include "sup_func.h"

typedef struct {
  uint32_t w[3];
} u96;

typedef struct {
  u96 Q;
  u96 R;
  int frac_digits;
  int scale_adjust;
  int sign;
  int error;
  int ret_code;
} div_ctx;

typedef struct {
  int needed;
  uint32_t digit;
  int tie;
} round_info;

/* -------------------- u96 utils -------------------- */

static int is_zero_dec(s21_decimal d) {
  int res = 0;
  res = !(d.bits[0] | d.bits[1] | d.bits[2]);
  return res;
}

static u96 u96_from_dec(s21_decimal d) {
  u96 r;
  r.w[0] = (uint32_t)d.bits[0];
  r.w[1] = (uint32_t)d.bits[1];
  r.w[2] = (uint32_t)d.bits[2];
  return r;
}

static void dec_set_u96(s21_decimal *d, u96 a) {
  d->bits[0] = (int)a.w[0];
  d->bits[1] = (int)a.w[1];
  d->bits[2] = (int)a.w[2];
}

static int u96_is_zero(u96 a) {
  int res = 0;
  res = !(a.w[0] | a.w[1] | a.w[2]);
  return res;
}

static int u96_cmp(u96 a, u96 b) {
  int res = 0;
  int i = 2;
  while (i >= 0 && res == 0) {
    if (a.w[i] != b.w[i]) {
      res = (a.w[i] > b.w[i]) ? 1 : -1;
    }
    i--;
  }
  return res;
}

static void u96_sub_inplace(u96 *a, u96 b) {
  uint64_t x;
  uint64_t borrow = 0;

  x = (uint64_t)a->w[0] - b.w[0] - borrow;
  a->w[0] = (uint32_t)x;
  borrow = (x >> 63) & 1u;

  x = (uint64_t)a->w[1] - b.w[1] - borrow;
  a->w[1] = (uint32_t)x;
  borrow = (x >> 63) & 1u;

  x = (uint64_t)a->w[2] - b.w[2] - borrow;
  a->w[2] = (uint32_t)x;
}

static int u96_add_small(u96 *a, uint32_t v) {
  uint64_t s0 = (uint64_t)a->w[0] + v;
  a->w[0] = (uint32_t)s0;
  uint64_t c = s0 >> 32;

  uint64_t s1 = (uint64_t)a->w[1] + c;
  a->w[1] = (uint32_t)s1;
  c = s1 >> 32;

  uint64_t s2 = (uint64_t)a->w[2] + c;
  a->w[2] = (uint32_t)s2;
  return (int)(s2 >> 32);
}

static int u96_mul10(u96 *a) {
  uint64_t x0 = ((uint64_t)a->w[0] << 3) + ((uint64_t)a->w[0] << 1);
  uint64_t x1 = ((uint64_t)a->w[1] << 3) + ((uint64_t)a->w[1] << 1);
  uint64_t x2 = ((uint64_t)a->w[2] << 3) + ((uint64_t)a->w[2] << 1);

  x1 += (x0 >> 32);
  x2 += (x1 >> 32);

  a->w[0] = (uint32_t)x0;
  a->w[1] = (uint32_t)x1;
  a->w[2] = (uint32_t)x2;

  return (int)((x2 >> 32) != 0);
}

static int u96_divmod(u96 a, u96 b, u96 *q, u96 *r) {
  int err = 0;
  u96 qq = (u96){{0, 0, 0}};
  u96 rr = (u96){{0, 0, 0}};
  int i = 0;

  while (i < 96) {
    uint32_t msb = a.w[2] >> 31;

    uint32_t c0 = (rr.w[0] >> 31) & 1u;
    uint32_t c1 = (rr.w[1] >> 31) & 1u;
    rr.w[0] <<= 1;
    rr.w[1] = (rr.w[1] << 1) | c0;
    rr.w[2] = (rr.w[2] << 1) | c1;
    rr.w[0] |= msb;

    uint32_t a_c0 = (a.w[0] >> 31) & 1u;
    uint32_t a_c1 = (a.w[1] >> 31) & 1u;
    a.w[0] <<= 1;
    a.w[1] = (a.w[1] << 1) | a_c0;
    a.w[2] = (a.w[2] << 1) | a_c1;

    uint32_t q_c0 = (qq.w[0] >> 31) & 1u;
    uint32_t q_c1 = (qq.w[1] >> 31) & 1u;
    qq.w[0] <<= 1;
    qq.w[1] = (qq.w[1] << 1) | q_c0;
    qq.w[2] = (qq.w[2] << 1) | q_c1;

    if (u96_cmp(rr, b) >= 0) {
      u96_sub_inplace(&rr, b);
      qq.w[0] |= 1u;
    }
    i++;
  }
  *q = qq;
  *r = rr;
  return err;
}

static void bankers_round(u96 *Q, uint32_t next_digit, int is_tie) {
  int round_up = (next_digit > 5) +
                 ((int)(next_digit == 5) & ((~is_tie) | (Q->w[0] & 1u)));
  round_up = !!round_up;
  (void)u96_add_small(Q, (uint32_t)round_up);
}

static void ctx_init(div_ctx *ctx, s21_decimal v1, s21_decimal v2) {
  ctx->Q = (u96){{0, 0, 0}};
  ctx->R = (u96){{0, 0, 0}};
  ctx->frac_digits = 0;
  ctx->scale_adjust = get_scale(v1) - get_scale(v2);
  ctx->sign = get_sign(v1) ^ get_sign(v2);
  ctx->error = 0;
  ctx->ret_code = 0;
}

static void sanitize_operands(s21_decimal *a, s21_decimal *b) {
  set_sign(a, 0);
  set_sign(b, 0);
}

static void do_integer_div(const u96 A, const u96 B, div_ctx *ctx) {
  u96 q = (u96){{0, 0, 0}}, r = (u96){{0, 0, 0}};
  u96_divmod(A, B, &q, &r);
  ctx->Q = q;
  ctx->R = r;
}

static void scale_negative_exponent(const u96 B, int need, int max_pow,
                                    div_ctx *ctx) {
  int i = 0;
  while (i < need && !ctx->error) {
    if (u96_mul10(&ctx->R)) {
      ctx->error = 1;
      ctx->ret_code = ctx->sign ? 2 : 1;
    } else {
      u96 dq = (u96){{0, 0, 0}}, rr = (u96){{0, 0, 0}};
      u96_divmod(ctx->R, B, &dq, &rr);
      u96 tmp = ctx->Q;
      int ovf = u96_mul10(&tmp) || u96_add_small(&tmp, dq.w[0]);
      if (ovf) {
        ctx->error = 1;
        ctx->ret_code = ctx->sign ? 2 : 1;
      } else {
        ctx->Q = tmp;
        ctx->R = rr;
        ctx->frac_digits++;
      }
      //}
    }
    i++;
  }
  ctx->scale_adjust = 0;
  (void)max_pow;
}

static void try_extend_fraction(const u96 B, int max_frac, round_info *ri,
                                div_ctx *ctx) {
  int stop = 0;
  while (!u96_is_zero(ctx->R) && ctx->frac_digits < max_frac && !stop &&
         !ctx->error) {
    u96 dq = (u96){{0, 0, 0}}, rr = (u96){{0, 0, 0}};
    if (u96_mul10(&ctx->R)) {
      ri->needed = 1;
      ri->digit = 6;
      stop = 1;
    } else {
      u96_divmod(ctx->R, B, &dq, &rr);
      u96 tmp = ctx->Q;
      int mul_ovf = u96_mul10(&tmp);
      if (mul_ovf) {
        u96 twice = rr;
        uint32_t c0 = (twice.w[0] >> 31) & 1u;
        uint32_t c1 = (twice.w[1] >> 31) & 1u;
        twice.w[0] <<= 1;
        twice.w[1] = (twice.w[1] << 1) | c0;
        twice.w[2] = (twice.w[2] << 1) | c1;
        ri->tie = (u96_cmp(twice, B) == 0);
        ri->needed = 1;
        ri->digit = dq.w[0];
        stop = 1;
      } else {
        ctx->Q = tmp;
        int add_ovf = u96_add_small(&ctx->Q, dq.w[0]);
        if (add_ovf) {
          u96 twice = rr;
          uint32_t t0 = (twice.w[0] >> 31) & 1u;
          uint32_t t1 = (twice.w[1] >> 31) & 1u;
          twice.w[0] <<= 1;
          twice.w[1] = (twice.w[1] << 1) | t0;
          twice.w[2] = (twice.w[2] << 1) | t1;
          ri->tie = (u96_cmp(twice, B) == 0);
          ri->needed = 1;
          ri->digit = dq.w[0];
          stop = 1;
        } else {
          ctx->R = rr;
          ctx->frac_digits++;
        }
      }
    }
  }
}

static void last_digit_rounding_probe(const u96 B, round_info *ri,
                                      div_ctx *ctx) {
  if (ri->needed || u96_is_zero(ctx->R)) return;
  u96 Rt = ctx->R;
  int overflow10 = u96_mul10(&Rt);
  if (!overflow10) {
    u96 dq = (u96){{0, 0, 0}}, rr = (u96){{0, 0, 0}};
    u96_divmod(Rt, B, &dq, &rr);
    ri->digit = dq.w[0];
    u96 twice = rr;
    uint32_t t0 = (twice.w[0] >> 31) & 1u;
    uint32_t t1 = (twice.w[1] >> 31) & 1u;
    twice.w[0] <<= 1;
    twice.w[1] = (twice.w[1] << 1) | t0;
    twice.w[2] = (twice.w[2] << 1) | t1;
    ri->tie = (u96_cmp(twice, B) == 0);
  } else {
    ri->digit = 6;
  }
  ri->needed = 1;
}

static void finalize_result(const div_ctx *ctx, int max_frac,
                            s21_decimal *res) {
  int final_scale = ctx->scale_adjust + ctx->frac_digits;
  if (final_scale > max_frac) final_scale = max_frac;
  dec_set_u96(res, ctx->Q);
  set_scale(res, final_scale);
  set_sign(res, ctx->sign);
  if (is_zero_dec(*res)) set_sign(res, 0);
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  const int max_frac = MAX_POW;
  if (!result) return 1;
  if (is_zero_dec(value_2)) return 3;

  memset(result, 0, sizeof(*result));
  div_ctx ctx;
  ctx_init(&ctx, value_1, value_2);
  sanitize_operands(&value_1, &value_2);

  u96 A = u96_from_dec(value_1);
  u96 B = u96_from_dec(value_2);
  do_integer_div(A, B, &ctx);

  if (!ctx.error && ctx.scale_adjust < 0) {
    scale_negative_exponent(B, -ctx.scale_adjust, max_frac, &ctx);
  }

  round_info ri = {0, 0, 0};
  if (!ctx.error) {
    try_extend_fraction(B, max_frac, &ri, &ctx);
    last_digit_rounding_probe(B, &ri, &ctx);
    if (!ctx.error && ri.needed) bankers_round(&ctx.Q, ri.digit, ri.tie);
  }

  if (!ctx.error) {
    finalize_result(&ctx, max_frac, result);
    return 0;
  }

  return ctx.ret_code ? ctx.ret_code : 3;
}
