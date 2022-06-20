#ifndef __BWMS__
#define __BWMS__

#include <vector>
#include "spatial/spatial.h"
#include "kde/kernel.h"
#include "kde/matrix.h"


template<typename T>
class BandwidthMatrixSelector {
protected:
    BandwidthMatrix<T> process(const T & point, const std::vector<QueueItem<T>> & neighbours) const {
        std::vector<T> diffs;
        diffs.reserve(neighbours.size());
        std::transform(neighbours.begin(), neighbours.end(), std::back_inserter(diffs),
            [&point](const QueueItem<T> & x) -> T {
                return point - x.point;
            }
        );
        return BandwidthMatrix<T>::from_data(diffs);
    }

    BandwidthMatrix<T> process(
        const T & point,
        const std::vector<QueueItem<T>> & neighbours,
        const std::vector<real> & weights
    ) const {
        std::vector<T> diffs();
        diffs.reserve(neighbours.size());
        std::transform(neighbours.begin(), neighbours.end(), std::back_inserter(diffs),
            [&point](const QueueItem<T> & x) -> T {
                return point - x.point;
            }
        );
        return BandwidthMatrix<T>(diffs, weights);
    }

public:
    virtual ~BandwidthMatrixSelector(void) = default;
    virtual BandwidthMatrix<T> for_point(const T & point, const Spatial<T> & spatial) const = 0;
};


/** Constant covariance matrix selector **/
template<typename T>
class BandwidthMatrixConstantSelector: public BandwidthMatrixSelector<T> {
private:
    typename MatrixTraits<T>::matrix matrix_;

public:
    BandwidthMatrixConstantSelector(const typename MatrixTraits<T>::matrix & matrix): matrix_(matrix) {
        std::cout << "Constant bandwidth matrix selector created from matrix\n" << this->matrix_;
    }

    BandwidthMatrix<T> for_point(const T & point, const Spatial<T> & spatial) const override {
        (void) point;
        (void) spatial;
        return this->matrix_;
    }
};


/** Covariance selector that takes all points within some fixed radius **/
template<typename T>
class BandwidthMatrixRadiusSelector: public BandwidthMatrixSelector<T> {
private:
    real r_;

public:
    BandwidthMatrixRadiusSelector(real r): r_(r) {
        std::cout << "Constant-radius bandwidth matrix selector created with radius " << this->r_ << std::endl;
    }

    BandwidthMatrix<T> for_point(const T & point, const Spatial<T> & spatial) const override {
        return this->process(point, spatial.find_within(point, this->r_));
    }
};


/** Covariance selector that takes k nearest points **/
template<typename T>
class BandwidthMatrixNearestSelector: public BandwidthMatrixSelector<T> {
private:
    unsigned int k_;

public:
    BandwidthMatrixNearestSelector(unsigned int k): k_(k) {}

    BandwidthMatrix<T> for_point(const T & point, const Spatial<T> & spatial) const override {
        return this->process(point, spatial.find_nearest(point, this->k_));
    }
};


/** Covariance selector that takes weighted k nearest points **/
template<typename T>
class BandwidthMatrixMetaSelector: public BandwidthMatrixSelector<T> {
private:
    const Kernel & kernel_;
    const real metabw_;

public:
    BandwidthMatrixMetaSelector(const Kernel & kernel, const real metabw):
        kernel_(kernel),
        metabw_(metabw)
    {}

    BandwidthMatrix<T> for_point(const T & point, const Spatial<T> & spatial) const override {
        auto neighbours = spatial.find_within(point, this->metabw_ * this->kernel_.cutoff());
    }
};

#endif // __BWMS__
