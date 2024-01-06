#include "word.h"
#include "code.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * Creates a new Word with symbols syms and length len
 * Allocates new array and copies the symbols over
 */
Word *word_create(uint8_t *syms, uint32_t len) {

    //allocate memory for word pointer
    Word *word = (Word *) malloc(sizeof(Word));

    //if making empty word
    if (len == 0) {
        word->syms = NULL;
        word->len = len;
        return word;
    }

    //allocate memory for syms
    word->syms = (uint8_t *) malloc(sizeof(uint8_t) * len);

    //copy syms array
    for (uint32_t i = 0; i < len; i++) {
        word->syms[i] = syms[i];
    }

    word->len = len;

    return word;
}

/*
 * Creates a new word by appending symbol sym to word w
 * Updates the length of the new word and copies symbols over
 * Returns a pointer to the newly allocated word
 */
Word *word_append_sym(Word *w, uint8_t sym) {
    Word *new_word = (Word *) malloc(sizeof(Word));

    //if w is empty
    if (w->len == 0) {
        new_word->syms = malloc(sizeof(uint8_t));
        new_word->syms[0] = sym;
        new_word->len = 1;
        return new_word;
    }

    //allocating memory for new word size len + 1
    new_word->syms = malloc(sizeof(uint8_t) * (w->len + 1));

    //copying over to new word
    for (uint32_t i = 0; i < w->len + 1; i++) {
        new_word->syms[i] = w->syms[i];
    }
    //append final symbol
    new_word->syms[w->len] = sym;

    new_word->len = w->len + 1;

    return new_word;
}

/*
 * Deletes the word w
 * Frees up associated space
 */
void word_delete(Word *w) {
    //freeing syms
    free(w->syms);
    //freeing word
    free(w);
}

/*
 * Constructor:
 * Creates a new table big enough to fit MAX_CODE
 * Creates the first element at EMPTY_CODE and returns it
 */
WordTable *wt_create(void) {
    //allocates memory for new table
    WordTable *new_table = (WordTable *) calloc(MAX_CODE, sizeof(Word));

    //creates first element at EMPTY_CODE
    new_table[EMPTY_CODE] = word_create(NULL, 0);

    return new_table;
}

/*
 * Deletes all words except EMPTY_CODE
 * Frees associated memory
 */
void wt_reset(WordTable *wt) {
    for (uint32_t i = 2; i < MAX_CODE; i++) {
        if (wt[i] != NULL) {
            word_delete(wt[i]);
            wt[i] = NULL;
        }
    }
}

/*
 * Destructor: Deletes all words and tables
 * Frees up associated memory
 */
void wt_delete(WordTable *wt) {
    for (uint32_t i = 0; i < MAX_CODE; i++) {
        if (wt[i] != NULL) {
            word_delete(wt[i]);
            wt[i] = NULL;
        }
    }

    free(wt);
}
