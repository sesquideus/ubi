#ifndef __TRIWEIGHT__
#define __TRIWEIGHT__

class TriweightKernel: public Kernel {
private:
    constexpr static double norm = 35.0 / 32.0;
    constexpr static double norm2 = 4 / M_PI;
public:
    inline double evaluate(double x) const override {
        return x > 1 ? 0 : pow((1 - pow(x, 2)), 3);
    }

    inline double evaluate_squared(double x2) const override {
        return x2 > 1 ? 0 : pow(1 - x2, 3);
    }

    inline std::string name(void) const override {
        return "triweight";
    }
};

#endif // __TRIWEIGHT__

