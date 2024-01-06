#include "trie.h"
#include "code.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Creates a new TrieNode and returns a pointer to it
 * Allocate memory for TrieNode
 * Code is the code to be assigned to this new node
 * Returns the newly allocated node
 */
TrieNode *trie_node_create(uint16_t code) {
    //allocate memory for trieNode
    TrieNode *t = (TrieNode *) malloc(sizeof(TrieNode));
    t->code = code;

    for (int i = 0; i < ALPHABET; i++) {
        t->children[i] = NULL;
    }
    return t;
}
/*
 * Deletes Node n
 * Frees any allocated memory
 */
void trie_node_delete(TrieNode *n) {
    free(n);
}

/*
 * Constructor: Creates the root TrieNode and returns a pointer to it
 * Allocate memory for TrieNode
 * Code is START_CODE
 * Returns the newly allocated node
 */
TrieNode *trie_create(void) {
    TrieNode *t = (TrieNode *) malloc(sizeof(TrieNode));
    t->code = EMPTY_CODE;

    for (int i = 0; i < ALPHABET; i++) {
        t->children[i] = NULL;
    }
    return t;
}
/*
 * Resets the trie: called when code reaches MAX_CODE
 * Deletes all the children of root and frees allocated memory
 */
void trie_reset(TrieNode *root) {
    //recursive steps. if node has children, recurse.
    for (int i = 0; i < ALPHABET; i++) {
        if (root->children[i] != NULL) {
            trie_reset(root->children[i]);
            root->children[i] = NULL;
        }
    }
}
/*
 * Destructor: Deletes all nodes starting at n as the root
 * Deletes any children recursively before deleting the root
 * Frees all the memory allocated for TrieNodes n and below
 */
void trie_delete(TrieNode *n) {
    //recursive steps
    for (int i = 0; i < ALPHABET; i++) {
        //makes sure child has children
        if (n->children[i] != NULL) {
            trie_delete(n->children[i]);
            n->children[i] = NULL;
        }
    }

    //delete node
    trie_node_delete(n);
}
/*
 * Checks if node has any children called sym
 * Returns the address if found, NULL if absent
 */
TrieNode *trie_step(TrieNode *n, uint8_t sym) {
    return n->children[sym];
}
