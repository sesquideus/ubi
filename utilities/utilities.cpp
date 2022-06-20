#include "utilities.h"

double radians(double x) {
    return x * DegreesToRadians;
}

double degrees(double x) {
    return x * RadiansToDegrees;
}

long int bit_ceil(long int x) {
    static const unsigned int t[5] = {
        0xFFFF0000,
        0x0000FF00,
        0x000000F0,
        0x0000000C,
        0x00000002
    };

    int y = (((x & (x - 1)) == 0) ? 0 : 1);
    int j = 16;
    int i;

    for (i = 0; i < 5; i++) {
        int k = (((x & t[i]) == 0) ? 0 : j);
        y += k;
        x >>= k;
        j >>= 1;
    }

    return 1 << y;
}

long int bit_floor(long int x) {
    return x & (bit_ceil((x >> 1) + 1));
}

double timed(std::function<void(void)> func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    return (double) std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() / 1e6;
}

void print_timed(const std::string & message, std::function<void(void)> func) {
    double time = timed(func);
    std::cout << message << " done in " << time << " seconds" << std::endl;
}

template<typename T>
void print_text(const std::vector<T> & vector) {
    for (auto && item: vector) {
        std::cout << item;
    }
    std::cout << std::endl;
}


