#include <stdio.h>
#include "utils.h"
#include "trie.h"

trie_node_t* root;

int add(unsigned int base, char mask) {
    if(mask < 0 || 32 < mask) {
        return -1;
    }
    insert(root, base, mask);
    return 0;
}

// Changed signature to avoid implementation-defined conversion
signed char check(unsigned int ip) {
    return find(root, ip);
}

int del(unsigned int base, char mask) {
    /* only wrong value of mask is treated as a "wrong" argument
     * trying to delete a prefix that wasn't previously added
     * will be noted, but won't be an error */
    if(mask < 0 || 32 < mask) {
        return -1;
    }
    delete(root, base, mask);
    return 0;
}

/* few simple tests */
void basic_test(void) {
    unsigned int base255 = parse_ipv4(255, 0, 0, 0);
    unsigned int ip1 = parse_ipv4(255, 0, 0, 1);

    // add mask 1 and 32
    if(add(base255, 1) == -1) printf("add error\n");
    if(add(base255, 31) == -1) printf("add error\n");

    // check that largest mask is 31
    char max_mask = check(ip1);
    printf("max mask for ip=%u -> %d (==31)\n", ip1, max_mask);

    // delete mask 31
    if(del(base255, 31) == -1) printf("del error\n");

    // check that largest mask is 1
    max_mask = check(ip1);
    printf("max mask for ip=%u -> %d (==1)\n", ip1, max_mask);

}

void additions_deletions_test(void){
    unsigned int base = parse_ipv4(255, 0, 0, 0);
    if(add(base, 0) == -1) printf("add error for %u/%d\n", base, 0);
    for (char mask = 1; mask <= 32; mask++) {
        if(add(base, mask) == -1) printf("add error for %u/%d\n", base, mask);

    }

    base = parse_ipv4(0, 255, 0, 0);
    for (char mask = 1; mask <= 32; mask++) {
        if(add(base, mask) == -1) printf("add error for %u/%d\n", base, mask);

    }

    base = parse_ipv4(255, 0, 0, 0);
    for (char mask = 1; mask <= 32; mask++) {
        if(del(base, mask) == -1) printf("del error for %u/%d\n", base, mask);
    }

    base = parse_ipv4(0, 255, 0, 0);
    for (char mask = 1; mask <= 32; mask++) {
        if(del(base, mask) == -1) printf("del error for %u/%d\n", base, mask);
    }
    if(del(base, 0) == -1) printf("del error for %u/%d\n", base, 0);

}

void basic_test2(void) {
    unsigned int base0 = parse_ipv4(0, 0, 0, 0);
    unsigned int base255 = parse_ipv4(255, 0, 0, 0);
    unsigned int base255_255 = parse_ipv4(255, 255, 0, 0);
    unsigned int base255_255_255 = parse_ipv4(255, 255, 255, 0);
    unsigned int base255_255_255_255 = parse_ipv4(255, 255, 255, 255);
    unsigned int ip255_255_255_255 = base255_255_255_255;

    // add 5 increasingly better (smaller) prefixes containing ip255_255_255_255
    if(add(base0, 0) == -1) printf("add error\n");
    if(add(base255, 8) == -1) printf("add error\n");
    if(add(base255_255, 16) == -1) printf("add error\n");
    if(add(base255_255_255, 24) == -1) printf("add error\n");
    if(add(ip255_255_255_255, 32) == -1) printf("add error\n");

    char max_mask = check(ip255_255_255_255);
    printf("max mask for ip=%u -> %d (==32)\n", ip255_255_255_255, max_mask);

    // delete smallest one
    if(del(base255_255_255_255, 32) == -1) printf("del error\n");

    max_mask = check(ip255_255_255_255);
    printf("max mask for ip=%u -> %d (==24)\n", ip255_255_255_255, max_mask);
}

int main() {
    root = new_trie_node();

    /* few tests - uncomment one at a time due to global root */
    //basic_test();
    basic_test2();
    //additions_deletions_test();

    free_tree(root);
    return 0;
}
