#ifndef __EPANECHNIKOV__
#define __EPANECHNIKOV__

class EpanechnikovKernel: public Kernel {
private:
    constexpr static double norm1D = 0.75;
    constexpr static double norm2D = 3 / (4 * M_PI);
public:
    inline double evaluate(double x) const override {
        return x > 1 ? 0 : 1 - x * x;
    }

    inline double evaluate_squared(double x2) const override {
        return x2 > 1 ? 0 : 1 - x2;
    }

    inline std::string name(void) const override {
        return "Epanechnikov";
    }
};

#endif // __EPANECHNIKOV__
