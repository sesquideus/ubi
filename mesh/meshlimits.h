#ifndef __MESH_LIMITS__
#define __MESH_LIMITS__


class MeshLimits {
private:
    unsigned int res_;
    real min_;
    real max_;
    real ext_;
public:
    MeshLimits(real min, real max, unsigned int res):
        res_(res),
        min_(min),
        max_(max),
        ext_(max - min)
    {
        assert(max >= min);
    };

    inline unsigned int res(void) const { return this->res_; };
    inline real min(void) const { return this->min_; };
    inline real max(void) const { return this->max_; };
    inline real ext(void) const { return this->ext_; };
    inline real value(unsigned int index) const {
        return this->min_ + this->ext_ * (((real) index + 0.5) / (real) this->res_);
    };
};


class Range {
protected:
    unsigned int res_;
    real min_;
    real max_;
    real ext_;

    inline unsigned int res(void) const { return this->res_; };
    inline real min(void) const { return this->min_; };
    inline real max(void) const { return this->max_; };
    inline real ext(void) const { return this->ext_; };
    inline real linear_interpolation(unsigned int index) {
        return this->min_ + this->ext_ * ((static_cast<real>(index) + 0.5) / static_cast<real>(this->res_));
    };
public:
    virtual real value(unsigned int index) const = 0;
};


class LinearRange: public Range {
    LinearRange(real min, real max, unsigned int res):
        min_(min),
        max_(max),
        ext_(max - min)
    {
        assert(max >= min);
    };

    inline real value(unsigned int index) const override {
        return this->linear_interpolation(index);
    };
};


class LogRange {
private:
public:
    LogRange(real min, real max, unsigned int res):
        min_(std::log(min)),
        max_(std::log(max)),
        ext_(max_ - min_)
    {
        assert(max >= min);
    };

    inline real value(unsigned int index) const override {
        return std::exp(this->linear_interpolation(index));
    };
};

#endif // __MESH_LIMITS__
