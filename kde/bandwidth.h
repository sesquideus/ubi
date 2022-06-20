#ifndef __BANDWIDTH__
#define __BANDWIDTH__

#include <vector>
#include "spatial/spatial.h"
#include "kde/kernel.h"
#include "kde/matrix.h"


template<typename T>
class BandwidthSelector {
protected:
    real scale_;
public:
    BandwidthSelector(const real scale = 1.0): scale_(scale) {}
    virtual ~BandwidthSelector(void) = default;

    virtual real for_point(const T & point, const Spatial<T> & spatial) const = 0;
};


/* Trivial bandwidth selector: constant for every point */
template<typename T>
class BandwidthConstantSelector: public BandwidthSelector<T> {
private:
    real r_;

public:
    BandwidthConstantSelector(real r):
        BandwidthSelector<T>(1.0),
        r_(r)
    {}

    inline real for_point(const T & point, const Spatial<T> & spatial) const override {
        (void) point;
        (void) spatial;
        return this->r_;
    }
};


/* Bandwidth selector for k-th nearest neighbour */
template<typename T>
class BandwidthNearestSelector: public BandwidthSelector<T> {
private:
    unsigned int k_;

public:
    BandwidthNearestSelector(unsigned int k, const real scale = 1.0):
        BandwidthSelector<T>(scale),
        k_(k)
    {}

    inline real for_point(const T & point, const Spatial<T> & spatial) const override {
        return this->scale_ * spatial.find_nearest(point, this->k_).back().distance;
    }
};


/* Weighted k nearest neighbours selector */
template<typename T>
class BandwidthWeightedNearestSelector: public BandwidthSelector<T> {
private:
    const real k_;
    const std::vector<real> weights_;

public:
    BandwidthWeightedNearestSelector(const std::vector<real> & weights, const real scale = 1.0):
        BandwidthSelector<T>(scale),
        k_(weights.size()),
        weights_(weights)
    {};

    real for_point(const T & point, const Spatial<T> & spatial) const override {
        auto neighbours = spatial.find_nearest(point, this->k_);
        real sum = 0.0;
        real sw = 0.0;

        for (unsigned int i = 0; i < this->k_; ++i) {
            sum += neighbours[i].distance * this->weights_[i];
            sw += this->weights_[i];
        }
        return this.scale_ * sum / sw;
    }
};


/* Meta bandwidth selector */
template<typename T>
class BandwidthMetaKernelSelector: public BandwidthSelector<T> {
private:
    const Kernel & kernel_;
    real metabandwidth_;

public:
    BandwidthMetaKernelSelector(const Kernel & kernel, const real metabandwidth, const real scale = 1.0):
        BandwidthSelector<T>(scale),
        kernel_(kernel),
        metabandwidth_(metabandwidth)
    {}

    inline real for_point(const T & point, const Spatial<T> & spatial) const override {
        auto neighbours = spatial.find_within(point, this->metabandwidth_);
        real sum = 0;
        real sw = 0;

        for (auto && n: neighbours) {
            real weight = this->kernel_.evaluate(n.distance, this->metabandwidth_);
            sum += n.distance * weight;
            sw += weight;
        }
        return this.scale_ * sum / sw;
    }
};

#endif // __BANDWIDTH__
