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

#endif // __MESH_LIMITS__
