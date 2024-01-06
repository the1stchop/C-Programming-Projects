#include "shell.h"

#include "gaps.h"

#include <stdio.h>

void shell_sort(Stats *stats, uint32_t *arr, uint32_t length) {
    for (int x = 0; x < GAPS; x++) {
        uint32_t gap = gaps[x];
        for (uint32_t i = gap; i < length; i++) {
            uint32_t j = i;
            uint32_t temp = move(stats, arr[i]);
            while ((j >= gap) && (cmp(stats, arr[j - gap], temp) == 1)) {
                arr[j] = move(stats, arr[j - gap]);
                j -= gap;
            }
            arr[j] = move(stats, temp);
        }
    }
}
