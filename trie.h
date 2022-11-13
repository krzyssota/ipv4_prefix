//
// Created by Krzysztof Sota on 10/11/2022.
//
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef IPV4_PREFIX_TRIE_H
#define IPV4_PREFIX_TRIE_H

typedef struct trie_node {
    struct trie_node* children[2]; // 0, 1 bit
    bool end; // did prefix added end here
} trie_node_t;

trie_node_t* new_trie_node();

void insert(trie_node_t* node, uint32_t base, char mask);

signed char find(trie_node_t* node, uint32_t ip);

void delete(trie_node_t* node, uint32_t base, char mask);

void free_tree(trie_node_t* root);

#endif //IPV4_PREFIX_TRIE_H
