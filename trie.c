//
// Created by Krzysztof Sota on 10/11/2022.
//
#include <stdio.h>
#include <assert.h>
#include "trie.h"

/* counting from 0th */
static inline size_t nth_bit(uint32_t base, char shift) {
    return (base>>shift)&1;
}

trie_node_t* new_trie_node() {
    trie_node_t* node = malloc(sizeof(trie_node_t));
    if (!node) exit(1);
    node->children[0] = node->children[1] = NULL;
    node->end = 0;
    return node;
}

void insert(trie_node_t* node, uint32_t base, char mask) {
    if(node) {
        char shift = 31;
        /* store first #mask bits of base */
        while(shift > (31-mask)) {
            size_t b = nth_bit(base, shift);;
            if(node->children[b] == NULL) node->children[b] = new_trie_node();
            node = node->children[b];
            shift--;
        }
        if(node->end) printf("prefix already present %u/%d\n", base, mask);
        else node->end = true;
    }
}

bool delete_rec(trie_node_t* node, uint32_t base, char mask, char shift) {
    if(node) {
        if(shift > 31-mask) {
            size_t b = nth_bit(base, shift);
            bool branch_safe_to_free = delete_rec(node->children[b], base, mask, shift - 1);
            if(branch_safe_to_free && node->children[b] != NULL) {
                free(node->children[b]);
                node->children[b] = NULL;
            }
            /* if backtracked branch was used solely by this prefix and no second child and no other prefix ended here
             * this node can be safely freed by the parent */
            return branch_safe_to_free && node->children[1 - b] == NULL && !node->end;
        } else {
            assert(shift == 31-mask);
            if(!node->end) printf("del(%u/%d) error: no such prefix\n", base, mask); // consumed prefix yet nothing ended here
            node->end = false;
            /* if no other prefix used the same path (no children)
             * this node can be safely freed by the parent */
            return node->children[0] == NULL && node->children[1] == NULL;
        }
    } else {
        return true;
    }
}

void delete(trie_node_t* node, uint32_t base, char mask) {
    delete_rec(node, base, mask, 31);
}


signed char find(trie_node_t* node, uint32_t ip) {
    if(node) {
        char shift = 31;
        char depth = 0;
        bool found = false;
        char deepest_end = 0;
        // go down the tree following subsequent bits of ip
        for(;;) {
            trie_node_t *child = node->children[nth_bit(ip, shift)];
            shift--;
            /* go further */
            if (child != NULL) {
                depth++;
                node = child;
                /* found prefix, save depth(=mask) */
                if (node->end) {
                    found = true;
                    deepest_end = depth;
                }
            /* reached leaf */
            } else {
                break;
            }
        }
        return (found ? (signed char)deepest_end : -1);
    } else {
        return -1;
    }
}

void free_children(trie_node_t* node) {
    if (node) {
        free_children(node->children[0]);
        free_children(node->children[1]);
        free(node->children[0]);
        free(node->children[1]);
        node->children[0] = NULL;
        node->children[1] = NULL;
    }
}

void free_tree(trie_node_t* root) {
    if(root) {
        free_children(root);
        free(root);
        root = NULL;
    }
}
