#include "node.h"


long int full_inner(long int total) {
    long int s = bit_floor(total >> 1);
    if (total < 2) {
        return 0;
    }
    if (total - (s << 1) < s) {
        return total - s;
    } else {
        return (s << 1) - 1;
    }
}

