template<typename Num>
ThreeD<Num>::ThreeD(Num _x, Num _y, Num _z): x(_x), y(_y), z(_z) {}

template<typename Num>
Num ThreeD<Num>::operator[](ThreeD<Num>::Index i) const {
    switch (i) {
        case ThreeD<Num>::Index::X: return this->x;
        case ThreeD<Num>::Index::Y: return this->y;
        case ThreeD<Num>::Index::Z: return this->z;
    }
}

template<typename Num>
Num & ThreeD<Num>::operator[](ThreeD<Num>::Index i) {
    switch (i) {
        case ThreeD<Num>::Index::X: return this->x;
        case ThreeD<Num>::Index::Y: return this->y;
        case ThreeD<Num>::Index::Z: return this->z;
    }
}

template<typename Num>
ThreeD<Num> operator*(const ThreeD<Num> & a, const ThreeD<Num> & b) {
    return ThreeD(
        a.x * b.x,
        a.y * b.y,
        a.z * b.z
    );
}

template<typename Num>
ThreeD<Num> & operator+=(ThreeD<Num> & a, ThreeD<Num> b) {
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    return a;
}

template<typename Num>
std::ostream & operator<<(std::ostream & os, const ThreeD<Num> & m) {
    return os << m.x << " " << m.y < " " << m.z;
}

