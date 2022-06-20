#ifndef __CONSTANT__
#define __CONSTANT__

class ConstantKernel: public Kernel {
public:
    inline double evaluate(double x) const override {
        return x > 1 ? 0 : 0.5;
    }

    inline double evaluate_squared(double x2) const override {
        return x2 > 1 ? 0 : 0.5;
    }

    inline std::string name(void) const override {
        return "constant";
    }
};

#endif // __CONSTANT__
