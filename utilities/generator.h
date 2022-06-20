#ifndef __GENERATOR__
#define __GENERATOR__

#include "utilities.h"


class Generator {
protected:
    std::random_device rd_;
    std::seed_seq sd_;
    mutable std::mt19937 mt_;
public:
    Generator(void);
};

class GeneratorSpherical: public Generator {
public:
    TwoD<double> random_uniform_spherical(void) const;
    TwoD<double> random_cosz_spherical(void) const;
    TwoD<double> random_cube(void) const;
    TwoD<double> random_gaussian_centered(void) const;
};

template<typename Real>
class Generator1D: public Generator {
public:
    Real random_uniform(Real min, Real max) const;
};

class GeneratorMeteor: public Generator {
public:
    Meteor random_uniform(void) const;
};

#include "generator.tpp"

#endif // __GENERATOR__
