#ifndef __GAUSSIAN__
#define __GAUSSIAN__

class GaussianKernel: public Kernel {
private:
    constexpr static double norm1D = 0.39894228040143267793994605993438; // Norm only valid in 1D (1 / sqrt(2*pi))
    constexpr static double norm2D = 1.0 / (2 * M_PI);
public:
    inline double cutoff(void) const override { return 3.0; }

    inline double evaluate(double x) const override {
        return GaussianKernel::norm1D * exp(-0.5 * pow(x, 2));
    }

    inline double evaluate_squared(double x2) const override {
        return GaussianKernel::norm2D * exp(-0.5 * x2);
    }
};

#endif // __GAUSSIAN__
