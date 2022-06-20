#ifndef __QUARTIC__
#define __QUARTIC__


class QuarticKernel: public Kernel {
public:
    inline double evaluate(double x) const override {
        return x > 1 ? 0 : pow((1 - pow(x, 2)), 2);
    }

    inline double evaluate_squared(double x2) const override {
        return x2 > 1 ? 0 : pow(1 - x2, 2);
    }

    inline std::string name(void) const override {
        return "quartic";
    }
};


template<>
struct NormalizationFactor<QuarticKernel, double> {
    constexpr static double value = 15.0 / 16.0;
};

template<>
struct NormalizationFactor<QuarticKernel, TwoD> {
    constexpr static double value = 3.0 / M_PI;
};

template<>
struct NormalizationFactor<QuarticKernel, ThreeD> {
    constexpr static double value = 105.0 / (32.0 * M_PI);
};


#endif // __QUARTIC__

