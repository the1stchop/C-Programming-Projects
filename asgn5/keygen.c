#include "randstate.h"
#include "numtheory.h"
#include "ss.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gmp.h>
#include <time.h>
#include <sys/stat.h>

#define OPTIONS "b:i:n:d:s:vh"

int main(int argc, char **argv) {
    int opt = 0;

    //default values
    int v = 0;
    int h = 0;
    uint64_t bits = 256;
    uint64_t iters = 50;
    char *pub_file_name = "ss.pub";
    char *priv_file_name = "ss.priv";
    uint64_t seed = time(NULL);

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'b': bits = atoi(optarg);
        case 'i': iters = atoi(optarg); break;
        case 'n': pub_file_name = optarg; break;
        case 'd': priv_file_name = optarg; break;
        case 's': seed = atoi(optarg); break;
        case 'v': v = 1; break;
        case 'h': h = 1; break;
        }
    }

    //help
    if (h == 1) {
        printf("SYNOPSIS\n");
        printf("   Generates an SS public/private key pair.\n\n");
        printf("USAGE\n");
        printf("   ./keygen [OPTIONS]\n\n");
        printf("OPTIONS\n");
        printf("   -h              Display program help and usage.\n");
        printf("   -v              Display verbose program output.\n");
        printf("   -b bits         Minimum bits needed for public key n (default: 256).\n");
        printf("   -i iterations   Miller-Rabin iterations for testing primes (default: 50).\n");
        printf("   -n pbfile       Public key file (default: ss.pub).\n");
        printf("   -d pvfile       Private key file (default: ss.priv).\n");
        printf("   -s seed         Random seed for testing.\n");
        return 0;
    }

    //open public and private key files
    FILE *pub_file = fopen(pub_file_name, "w");
    FILE *priv_file = fopen(priv_file_name, "w");

    //set private key file permissions
    fchmod(fileno(priv_file), 0600);

    //initialize random state
    randstate_init(seed);

    //make public and private keys
    mpz_t p, q, pq, pub, priv;
    mpz_inits(p, q, pq, pub, priv, NULL);

    ss_make_pub(p, q, pub, bits, iters);
    ss_make_priv(priv, pq, p, q);

    //get username
    char *username = getenv("USERNAME");

    //write keys to respective files
    ss_write_pub(pub, username, pub_file);
    ss_write_priv(pq, priv, priv_file);

    //verbose output
    if (v == 1) {
        printf("user = %s\n", username);
        gmp_printf("p   (%zu bits) = %Zd\n", mpz_sizeinbase(p, 2), p);
        gmp_printf("q   (%zu bits) = %Zd\n", mpz_sizeinbase(q, 2), q);
        gmp_printf("n   (%zu bits) = %Zd\n", mpz_sizeinbase(pub, 2), pub);
        gmp_printf("pq  (%zu bits) = %Zd\n", mpz_sizeinbase(pq, 2), pq);
        gmp_printf("d   (%zu bits) = %Zd\n", mpz_sizeinbase(priv, 2), priv);
    }

    //clearing memory
    mpz_clears(p, q, pq, pub, priv, NULL);

    //closing files
    fclose(pub_file);
    fclose(priv_file);
}
