//
// Created by Krzysztof Sota on 09/11/2022.
//

unsigned int parse_ipv4(unsigned int a, unsigned int b, unsigned int c, unsigned int d) {
    unsigned int addr = d;
    addr |= c << 8;
    addr |= b << 16;
    addr |= a << 24;
    return addr;
}



