#include "batcher.h"
#include "heap.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "stats.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "ahbsqr:n:p:H"

int main(int argc, char **argv) {
    int opt = 0;

    Set sett = set_empty();

    Stats stats = { 0, 0 };
    reset(&stats);

    uint64_t default_seed = 13371453;
    uint32_t default_size = 100;
    uint32_t default_element = 100;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {

        case 'a': sett = set_insert(sett, 6); break;
        case 's': sett = set_insert(sett, 1); break;
        case 'b': sett = set_insert(sett, 2); break;
        case 'h': sett = set_insert(sett, 3); break;
        case 'q': sett = set_insert(sett, 4); break;
        case 'r': default_seed = atoi(optarg); break;
        case 'n': default_size = atoi(optarg); break;
        case 'p': default_element = atoi(optarg); break;
        case 'H': sett = set_insert(sett, 5); break;
        }
    }

    if (set_member(sett, 5) || sett == 0) {
        printf("SYNOPSIS\n");
        printf("   A collection of comparison-based sorting algorithms.\n\n");
        printf("USAGE\n");
        printf("   ./sorting_amd64 [-Hasbhqn:p:r:] [-n length] [-p elements] [-r seed]\n\n");
        printf("OPTIONS\n");
        printf("   -H              Display program help and usage.\n");
        printf("   -a              Enable all sorts.\n");
        printf("   -s              Enable Shell Sort.\n");
        printf("   -b              Enable Batcher Sort.\n");
        printf("   -h              Enable Heap Sort.\n");
        printf("   -q              Enable Quick Sort.\n");
        printf("   -n length       Specify number of array elements (default: 100).\n");
        printf("   -p elements     Specify number of elements to print (default: 100).\n");
        printf("   -r seed         Specify random seed (default: 13371453).\n");

        return 0;
    }

    if (set_member(sett, 6)) {
        sett = set_universal();
    }

    srandom(default_seed);

    if (set_member(sett, 1)) {
        uint32_t *arr = (uint32_t *) malloc(default_size * sizeof(uint32_t));
        for (uint32_t i = 0; i < default_size; i++) {
            arr[i] = random() & 0x3FFFFFFF;
        }

        shell_sort(&stats, arr, default_size);

        printf("Shell Sort, %d elements, %lu moves, %lu compares", default_size, stats.moves,
            stats.compares);
        if (default_size < default_element) {
            default_element = default_size;
        }

        for (uint32_t i = 0; i < default_element; i++) {
            if (i % 5 == 0) {
                printf("\n");
            }
            printf("%13" PRIu32, arr[i]);
        }
        printf("\n");

        free(arr);

        reset(&stats);
    }

    if (set_member(sett, 2)) {
        uint32_t *arr = (uint32_t *) malloc(default_size * sizeof(uint32_t));
        for (uint32_t i = 0; i < default_size; i++) {
            arr[i] = random() & 0x3FFFFFFF;
        }

        batcher_sort(&stats, arr, default_size);

        printf("Batcher Sort, %d elements, %lu moves, %lu compares", default_size, stats.moves,
            stats.compares);
        if (default_size < default_element) {
            default_element = default_size;
        }

        for (uint32_t i = 0; i < default_element; i++) {
            if (i % 5 == 0) {
                printf("\n");
            }
            printf("%13" PRIu32, arr[i]);
        }
        printf("\n");

        free(arr);

        reset(&stats);
    }

    if (set_member(sett, 3)) {
        uint32_t *arr = (uint32_t *) malloc(default_size * sizeof(uint32_t));
        for (uint32_t i = 0; i < default_size; i++) {
            arr[i] = random() & 0x3FFFFFFF;
        }

        heap_sort(&stats, arr, default_size);

        printf("Heap Sort, %d elements, %lu moves, %lu compares", default_size, stats.moves,
            stats.compares);
        if (default_size < default_element) {
            default_element = default_size;
        }

        for (uint32_t i = 0; i < default_element; i++) {
            if (i % 5 == 0) {
                printf("\n");
            }
            printf("%13" PRIu32, arr[i]);
        }
        printf("\n");

        free(arr);

        reset(&stats);
    }

    if (set_member(sett, 4)) {
        uint32_t *arr = (uint32_t *) malloc(default_size * sizeof(uint32_t));
        for (uint32_t i = 0; i < default_size; i++) {
            arr[i] = random() & 0x3FFFFFFF;
        }

        quick_sort(&stats, arr, default_size);

        printf("Quick Sort, %d elements, %lu moves, %lu compares", default_size, stats.moves,
            stats.compares);
        if (default_size < default_element) {
            default_element = default_size;
        }

        for (uint32_t i = 0; i < default_element; i++) {
            if (i % 5 == 0) {
                printf("\n");
            }
            printf("%13" PRIu32, arr[i]);
        }
        printf("\n");

        free(arr);

        reset(&stats);
    }
}
