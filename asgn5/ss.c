#include "ss.h"
#include "numtheory.h"
#include "randstate.h"
#include <stdlib.h>
#include <math.h>

//
// Generates the components for a new SS key.
//
// Provides:
//  p:  first prime
//  q: second prime
//  n: public modulus/exponent
//
// Requires:
//  nbits: minimum # of bits in n
//  iters: iterations of Miller-Rabin to use for primality check
//  all mpz_t arguments to be initialized
//
void ss_make_pub(mpz_t p, mpz_t q, mpz_t n, uint64_t nbits, uint64_t iters) {
    uint64_t upper = floor(2 * nbits / 5);
    uint64_t lower = floor(nbits / 5);
    uint64_t p_bits, q_bits;

    mpz_t pi, qi;
    mpz_inits(pi, qi, NULL);

    do {
        p_bits = (random() % (upper - lower + 1)) + lower;
        q_bits = nbits - (2 * p_bits);

        make_prime(p, p_bits, iters);
        make_prime(q, q_bits, iters);

        mpz_sub_ui(pi, p, 1);
        mpz_sub_ui(qi, q, 1);

    } while (mpz_divisible_p(qi, p) != 0 && mpz_divisible_p(pi, q) != 0);

    //calculate n = p * p * q
    mpz_mul(n, p, p);
    mpz_mul(n, n, q);

    mpz_clears(pi, qi, NULL);
}

//
// Generates components for a new SS private key.
//
// Provides:
//  d:  private exponent
//  pq: private modulus
//
// Requires:
//  p:  first prime number
//  q: second prime number
//  all mpz_t arguments to be initialized
//
void ss_make_priv(mpz_t d, mpz_t pq, const mpz_t p, const mpz_t q) {
    mpz_t pi, qi, n, num, den, lcm;
    mpz_inits(pi, qi, n, num, den, lcm, NULL);
    mpz_sub_ui(pi, p, 1);
    mpz_sub_ui(qi, q, 1);

    //calculate n = p * p * q
    mpz_mul(n, p, p);
    mpz_mul(n, n, q);

    //calculate lcm(p-1,q-1) or pq
    mpz_mul(num, pi, qi);
    gcd(den, pi, qi);
    mpz_fdiv_q(lcm, num, den);

    //inverse of n mod lcm(p-1,q-1)
    mod_inverse(d, n, lcm);

    //calculate pq = p * q
    mpz_mul(pq, p, q);

    mpz_clears(pi, qi, n, num, den, lcm, NULL);
}

//
// Export SS public key to output stream
//
// Requires:
//  n: public modulus/exponent
//  username: login name of keyholder ($USER)
//  pbfile: open and writable file stream
//
void ss_write_pub(const mpz_t n, const char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n", n);
    fprintf(pbfile, "%s\n", username);
}

//
// Export SS private key to output stream
//
// Requires:
//  pq: private modulus
//  d:  private exponent
//  pvfile: open and writable file stream
//
void ss_write_priv(const mpz_t pq, const mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n", pq);
    gmp_fprintf(pvfile, "%Zx\n", d);
}

//
// Import SS public key from input stream
//
// Provides:
//  n: public modulus
//  username: $USER of the pubkey creator
//
// Requires:
//  pbfile: open and readable file stream
//  username: requires sufficient space
//  all mpz_t arguments to be initialized
//
void ss_read_pub(mpz_t n, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n", n);
    fscanf(pbfile, "%s\n", username);
}
//
// Import SS private key from input stream
//
// Provides:
//  pq: private modulus
//  d:  private exponent
//
// Requires:
//  pvfile: open and readable file stream
//  all mpz_t arguments to be initialized
//
void ss_read_priv(mpz_t pq, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n", pq);
    gmp_fscanf(pvfile, "%Zx\n", d);
}

//
// Encrypt number m into number c
//
// Provides:
//  c: encrypted integer
//
// Requires:
//  m: original integer
//  n: public exponent/modulus
//  all mpz_t arguments to be initialized
//
void ss_encrypt(mpz_t c, const mpz_t m, const mpz_t n) {
    pow_mod(c, m, n, n);
}

//
// Encrypt an arbitrary file
//
// Provides:
//  fills outfile with the encrypted contents of infile
//
// Requires:
//  infile: open and readable file stream
//  outfile: open and writable file stream
//  n: public exponent and modulus
//
void ss_encrypt_file(FILE *infile, FILE *outfile, const mpz_t n) {
    mpz_t m, j, encryption;
    mpz_inits(m, j, encryption, NULL);

    //calculate k (block size)
    uint32_t k = ((mpz_sizeinbase(n, 2) / 2) - 1) / 8;

    //allocate memory of block
    uint8_t *block = malloc(k * sizeof(uint8_t));

    //reading infile and writing to outfile
    size_t element = fread(block + 1, sizeof(uint8_t), k - 1, infile);
    while (element > 0) {
        block[0] = 0xFF;

        mpz_import(m, element + 1, 1, sizeof(uint8_t), 1, 0, block);
        //encrypt block
        ss_encrypt(encryption, m, n);

        //print to outfile
        gmp_fprintf(outfile, "%Zx\n", encryption);

        element = fread(block + 1, sizeof(uint8_t), k - 1, infile);
    }
    mpz_clears(m, j, encryption, NULL);
    free(block);
}

//
// Decrypt number c into number m
//
// Provides:
//  m: decrypted/original integer
//
// Requires:
//  c: encrypted integer
//  d: private exponent
//  pq: private modulus
//  all mpz_t arguments to be initialized
//
void ss_decrypt(mpz_t m, const mpz_t c, const mpz_t d, const mpz_t pq) {
    pow_mod(m, c, d, pq);
}

//
// Decrypt a file back into its original form.
//
// Provides:
//  fills outfile with the unencrypted data from infile
//
// Requires:
//  infile: open and readable file stream to encrypted data
//  outfile: open and writable file stream
//  d: private exponent
//  pq: private modulus
//
void ss_decrypt_file(FILE *infile, FILE *outfile, const mpz_t d, const mpz_t pq) {
    mpz_t c, decryption;
    mpz_inits(c, decryption, NULL);

    //calculate k (block size)
    uint32_t k = ((mpz_sizeinbase(pq, 2) / 2) - 1) / 8;

    //allocate memory of block
    uint8_t *block = malloc(k * sizeof(uint8_t));

    while (gmp_fscanf(infile, "%Zx", c) > 0) {
        //decrypt file
        ss_decrypt(decryption, c, d, pq);
        size_t j;

        mpz_export(block, &j, 1, sizeof(uint8_t), 1, 0, decryption);

        fwrite(block + 1, sizeof(uint8_t), j - 1, outfile);
    }
    mpz_clears(c, decryption, NULL);
    free(block);
}
