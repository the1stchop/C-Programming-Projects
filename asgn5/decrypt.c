#include "randstate.h"
#include "numtheory.h"
#include "ss.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gmp.h>

#define OPTIONS "i:o:n:vh"

int main(int argc, char **argv) {
    int opt = 0;

    //default values
    int v = 0;
    int h = 0;
    FILE *in_file = stdin;
    FILE *out_file = stdout;
    char *priv_file_name = "ss.priv";

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': in_file = fopen(optarg, "r"); break;
        case 'o': out_file = fopen(optarg, "w"); break;
        case 'n': priv_file_name = optarg; break;
        case 'v': v = 1; break;
        case 'h': h = 1; break;
        }
    }

    //help display
    if (h == 1) {
        printf("SYNOPSIS\n");
        printf("   Decrypts data using SS decryption.\n");
        printf("   Encrypted data is encrypted by the encrypt program.\n\n");
        printf("USAGE\n");
        printf("   ./decrypt [OPTIONS]\n\n");
        printf("OPTIONS");
        printf("   -h              Display program help and usage.\n");
        printf("   -v              Display verbose program output.\n");
        printf("   -i infile       Input file of data to decrypt (default: stdin).\n");
        printf("   -o outfile      Output file for decrypted data (default: stdout).\n");
        printf("   -n pbfile       Private key file (default: ss.priv).\n");
        return 0;
    }

    //read private key from priv_file
    mpz_t priv, pq;
    mpz_inits(priv, pq, NULL);

    FILE *priv_file = fopen(priv_file_name, "r");
    ss_read_priv(pq, priv, priv_file);

    //if verbose is enabled
    if (v == 1) {
        gmp_printf("pq (%zu bits) = %Zx\n", mpz_sizeinbase(pq, 2), pq);
        gmp_printf("d (%zu bits) = %Zx\n", mpz_sizeinbase(priv, 2), priv);
    }

    //decrypt in_file and put in out_file
    ss_decrypt_file(in_file, out_file, priv, pq);

    //close opened files and clear mpz_t vars
    fclose(in_file);
    fclose(out_file);
    fclose(priv_file);

    mpz_clears(priv, pq, NULL);
}
