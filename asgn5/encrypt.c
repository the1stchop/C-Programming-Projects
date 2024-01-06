#include "randstate.h"
#include "numtheory.h"
#include "ss.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gmp.h>
#include <limits.h>

#define OPTIONS "i:o:n:vh"

int main(int argc, char **argv) {
    int opt = 0;

    //default values
    int v = 0;
    int h = 0;
    FILE *in_file = stdin;
    FILE *out_file = stdout;
    char *pub_file_name = "ss.pub";

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': in_file = fopen(optarg, "r"); break;
        case 'o': out_file = fopen(optarg, "w"); break;
        case 'n': pub_file_name = optarg; break;
        case 'v': v = 1; break;
        case 'h': h = 1; break;
        }
    }

    //help display
    if (h == 1) {
        printf("SYNOPSIS\n");
        printf("   Encrypts data using SS encryption.\n");
        printf("   Encrypted data is decrypted by the decrypt program.\n\n");
        printf("USAGE\n");
        printf("   ./encrypt [OPTIONS]\n\n");
        printf("OPTIONS");
        printf("   -h              Display program help and usage.\n");
        printf("   -v              Display verbose program output.\n");
        printf("   -i infile       Input file of data to encrypt (default: stdin).\n");
        printf("   -o outfile      Output file for encrypted data (default: stdout).\n");
        printf("   -n pbfile       Public key file (default: ss.pub).\n");
        return 0;
    }

    //read public key and username from pub_file
    mpz_t pub;
    mpz_inits(pub, NULL);
    char *username = malloc(sizeof(char) * (CHAR_MAX + 1));

    FILE *pub_file = fopen(pub_file_name, "r");
    ss_read_pub(pub, username, pub_file);

    //if verbose is enabled
    if (v == 1) {
        printf("user = %s\n", username);
        gmp_printf("n (%zu bits) = %Zx\n", mpz_sizeinbase(pub, 2), pub);
    }

    //encrypt in_file and put in out_file
    ss_encrypt_file(in_file, out_file, pub);

    //close all opened files + clear mpz_t vars
    fclose(in_file);
    fclose(out_file);
    fclose(pub_file);

    mpz_clears(pub, NULL);
    free(username);
}
