#ifndef __TUPLES__
#define __TUPLES__

#include <utility>
#include <ostream>
#include <fmt/core.h>

#include "types/real.h"


/**
 * N-dimensional vector (basically a tuple but all fields are same type) */
template<typename T, size_t N>
struct NDV {
    T x[N];
    NDV();
};


struct TwoD {
    real x;
    real y;
    TwoD(real _x, real _y): x(_x), y(_y) {}

    inline real operator[](unsigned int i) const {
        switch (i) {
            case 0: return this->x;
            case 1: return this->y;
            default: return 0;
        }
    }
};

inline TwoD operator+(const TwoD a, const TwoD b) {
    return TwoD(a.x + b.x, a.y + b.y);
}

inline TwoD operator-(const TwoD a, const TwoD b) {
    return TwoD(a.x - b.x, a.y - b.y);
}

inline TwoD operator*(const TwoD a, const TwoD b) {
    return TwoD(a.x - b.x, a.y - b.y);
}

inline TwoD & operator+=(TwoD & a, TwoD b) {
    a.x += b.x;
    a.y += b.y;
    return a;
}

inline TwoD & operator-=(TwoD & a, TwoD b) {
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

inline std::ostream & operator<<(std::ostream & os, const TwoD & m) {
    return os << fmt::format("{}\t{}", m.x, m.y);
}


struct ThreeD {
    enum class Index {
        X = 0,
        Y = 1,
        Z = 2
    };
    real x;
    real y;
    real z;
    ThreeD(real _x, real _y, real _z);

    inline real operator[](unsigned int i) const {
        switch (i) {
            case 0: return this->x;
            case 1: return this->y;
            case 2: return this->z;
            default: return 0;
        }
    }
};

inline ThreeD operator*(ThreeD a, ThreeD b);

inline ThreeD & operator+=(ThreeD & a, ThreeD b);

inline std::ostream & operator<<(std::ostream & os, const ThreeD & m) {
    return os << fmt::format("{}\t{}\t{}", m.x, m.y, m.z);
}

#endif // __TUPLES__
