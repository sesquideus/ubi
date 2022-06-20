#ifndef __KERNEL__
#define __KERNEL__

#include "utilities/utilities.h"


template<typename K, typename T>
struct NormalizationFactor;


class Kernel {
public:
    virtual ~Kernel(void) = default;

    virtual double cutoff(void) const { return 1.0; };
    virtual double evaluate(double x) const = 0;
    virtual double evaluate_squared(double x2) const = 0;

    virtual std::string name(void) const = 0;

    inline double evaluate(double distance, double bandwidth) const { return this->evaluate(distance / bandwidth); }
    inline double operator()(double distance, double bandwidth) const { return this->evaluate(distance / bandwidth); }
};

/*
#include "../spatial/metric.h"
#include "kernel/kernels.h"

double eu(TwoD x, TwoD y) {
    return x.first - y.first;
}

template<class K, double (*M)(TwoD, TwoD)>
class Norm {
public:
    double operator()(void) const;
};


template<>
double Norm<ConstantKernel, eu>::operator()(void) const {
    return 1.0 / M_PI;
}*/


#endif // __KERNEL__
