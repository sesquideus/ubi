#ifndef __METRIC__
#define __METRIC__

#include <functional>
#include <cmath>

#include "types/real.h"
#include "types/tuples.h"
#include "mesh/meshnode.h"


inline real spherical_distance(const real lat1, const real lon1, const real lat2, const real lon2) {
    return std::acos(std::sin(lat1) * std::sin(lat2) + std::cos(lat1) * std::cos(lat2) * std::cos(lon2 - lon1));
}

inline real spherical_distance_hav(const real lat1, const real lon1, const real lat2, const real lon2) {
    return 2 * std::sin(
        std::sqrt(
            std::pow(sin(0.5 * (lat2 - lat1)), 2.0) +
            std::cos(lat1) * std::cos(lat2) * std::pow(std::sin(0.5 * (lon2 - lon1)), 2.0)
        )
    );
}

inline double spherical_azimuth_tan(const double lat1, const double lon1, const double lat2, const double lon2) {
    double dlon = lon2 - lon1;
    return cos(lat2) * sin(dlon) / (cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dlon));
}

inline void spherical_difference(const double lat1, const double lon1, const double lat2, const double lon2, double & dlat, double & dlon) {
    dlat = cos(lat2) * sin(lon2 - lon1);
    dlon = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(lon2 - lon1);
    double corr = 1.0 / sqrt(dlat * dlat + dlon * dlon);
    dlat *= corr;
    dlon *= corr;
}

inline double angular_difference(const double a1, const double a2) {
    return fmod(a2 - a1 + M_PI, 2 * M_PI) - M_PI;
}


template<typename T>
class MetricFunctor {
public:
    virtual ~MetricFunctor(void) = default;
    virtual real operator()(T a, T b) const = 0;
};

class Euclidean1D: public MetricFunctor<real> {
public:
    inline real operator()(real a, real b) const {
        return std::abs(a - b);
    }
};

class Euclidean2Dsqr: public MetricFunctor<TwoD> {
public:
    inline real operator()(TwoD a, TwoD b) const {
        return std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2);
    }
};

class Euclidean2D: public MetricFunctor<TwoD> {
public:
    inline real operator()(TwoD a, TwoD b) const {
        return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
    }
};

class Spherical: public MetricFunctor<TwoD> {
public:
    inline real operator()(TwoD a, TwoD b) const {
        return spherical_distance(a.x, a.y, b.x, b.y);
    }
};

class Toroidal: public MetricFunctor<TwoD> {
private:
    real xext_;
    real yext_;

public:
    Toroidal(const real xext, const real yext):
        xext_(xext),
        yext_(yext)
    {}

    inline real operator()(TwoD a, TwoD b) const {
        real dx = std::abs(a.x - b.x);
        real dy = std::abs(a.y - b.y);
        return std::sqrt(std::pow(std::min(dx, this->xext_ - dx), 2) + std::pow(std::min(dy, this->yext_ - dy), 2));
    }
};

class Manhattan2D: public MetricFunctor<TwoD> {
public:
    inline real operator()(TwoD a, TwoD b) const {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y);
    }
};

template<typename T>
class MeshMetricFunctor: public MetricFunctor<MeshNode<T>> {
private:
   const MetricFunctor<T> & inner_;

public:
   MeshMetricFunctor(const MetricFunctor<T> & inner): inner_(inner) {}

   inline real operator()(MeshNode<T> a, MeshNode<T> b) const override {
       return this->inner_(a.coord(), b.coord());
   }
};

#endif // __METRIC__
