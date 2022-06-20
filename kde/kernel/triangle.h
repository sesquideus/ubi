#ifndef __TRIANGLE__
#define __TRIANGLE__

class TriangleKernel: public Kernel {
public:
    inline double evaluate(double x) const override {
        return x > 1 ? 0 : 1 - x;
    }

    inline double evaluate_squared(double x2) const override {
        return x2 > 1 ? 0 : 1 - sqrt(x2);
    }

    inline std::string name(void) const override {
        return "triangle";
    }
};

#endif // __TRIANGLE__
