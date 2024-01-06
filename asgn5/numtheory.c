#include "numtheory.h"
#include "randstate.h"
#include <stdlib.h>

void gcd(mpz_t g, const mpz_t a, const mpz_t b) {
    mpz_t t, A, B;
    mpz_inits(t, A, B, NULL);

    mpz_set(B, b);
    mpz_set(A, a);

    while (mpz_cmp_ui(B, 0) != 0) {
        mpz_set(t, B);
        mpz_mod(B, A, B);
        mpz_set(A, t);
    }
    mpz_set(g, A);

    mpz_clears(t, A, B, NULL);
}

void mod_inverse(mpz_t o, const mpz_t a, const mpz_t n) {
    mpz_t r, ri, t, ti, q, temp;
    mpz_inits(r, ri, t, ti, q, temp, NULL);

    mpz_set(r, n);
    mpz_set(ri, a);
    mpz_set_ui(t, 0);
    mpz_set_ui(ti, 1);

    while (mpz_cmp_ui(ri, 0) != 0) {
        mpz_fdiv_q(q, r, ri);

        //(r,r') <-- (r',r-q*r')
        mpz_set(temp, r);
        mpz_set(r, ri);
        mpz_mul(ri, q, ri);
        mpz_sub(ri, temp, ri);

        //(t,t') <-- (t',t-q*t')
        mpz_set(temp, t);
        mpz_set(t, ti);
        mpz_mul(ti, q, ti);
        mpz_sub(ti, temp, ti);
    }
    if (mpz_cmp_ui(r, 1) > 0) {
        mpz_set_ui(o, 0);
        mpz_clears(r, ri, t, ti, q, temp, NULL);
        return;
    }
    if (mpz_cmp_ui(t, 0) < 0) {
        mpz_add(t, t, n);
    }
    mpz_set(o, t);
    mpz_clears(r, ri, t, ti, q, temp, NULL);
    return;
}
void pow_mod(mpz_t o, const mpz_t a, const mpz_t d, const mpz_t n) {
    mpz_t v, p, exp;
    mpz_inits(v, p, exp, NULL);

    mpz_set(exp, d);
    mpz_set_ui(v, 1);
    mpz_set(p, a);

    while (mpz_cmp_ui(exp, 0) > 0) {
        if (mpz_odd_p(exp)) {
            mpz_mul(v, v, p);
            mpz_mod(v, v, n);
        }
        mpz_mul(p, p, p);
        mpz_mod(p, p, n);

        mpz_fdiv_q_ui(exp, exp, 2);
    }
    mpz_set(o, v);

    mpz_clears(v, p, exp, NULL);
}

bool is_prime(const mpz_t n, uint64_t iters) {
    //if n = 0 or n = 1, return false
    if ((mpz_cmp_ui(n, 2) < 0) || (mpz_cmp_ui(n, 4) == 0)) {
        return false;
    }
    //if n is 2 or 3, return true
    if (mpz_cmp_ui(n, 4) < 0) {
        return true;
    }

    mpz_t s, r, y, j, a, N, temp;
    mpz_inits(s, r, y, j, a, N, temp, NULL);

    //the n-1 = 2^s *r such that r is odd thing
    mpz_sub_ui(N, n, 1);
    mpz_set(r, N);
    mpz_set_ui(s, 0);
    mpz_set_ui(temp, 2);

    while (mpz_even_p(r)) {
        mpz_divexact_ui(r, r, 2);
        mpz_add_ui(s, s, 1);
    }

    mpz_sub_ui(s, s, 1);
    //rest of function
    for (uint64_t i = 0; i < iters; i++) {
        mpz_urandomm(a, state, N);
        mpz_add_ui(a, a, 2);

        pow_mod(y, a, r, n);
        if ((mpz_cmp_ui(y, 1) != 0) && (mpz_cmp(y, N) != 0)) {
            mpz_set_ui(j, 1);
            while ((mpz_cmp(j, s) <= 0) && (mpz_cmp(y, N) != 0)) {
                pow_mod(y, y, temp, n);
                if (mpz_cmp_ui(y, 1) == 0) {
                    mpz_clears(s, r, y, j, a, N, temp, NULL);
                    return false;
                }
                mpz_add_ui(j, j, 1);
            }
            if (mpz_cmp(y, N) != 0) {
                mpz_clears(s, r, y, j, a, N, temp, NULL);
                return false;
            }
        }
    }
    mpz_clears(s, r, y, j, a, N, temp, NULL);
    return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    mpz_t lower, upper, base;
    mpz_inits(lower, upper, base, NULL);

    mpz_set_ui(base, 2);

    //range [lower, upper]
    mpz_setbit(lower, bits - 1);
    mpz_setbit(upper, bits);

    while (is_prime(p, iters) != true) {
        mpz_urandomm(p, state, upper);
        mpz_setbit(p, bits);
    }
    mpz_clears(lower, upper, base, NULL);
}
