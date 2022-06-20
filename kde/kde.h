#ifndef __KDE__
#define __KDE__

#include <memory>

#include "metric/metric.h"
#include "spatial/spatial.h"
#include "spatial/structures.h"
#include "mesh/mesh.h"

#include "utilities/utilities.h"
#include "bandwidth.h"
#include "bwms.h"
#include "kernel.h"


template<typename T, template<typename> class Spat>
class KDE {
private:
    const MetricFunctor<T> & metric_;
    std::vector<T> data_;
    Spat<T> * spatial_;

    double select_bandwidth(const T & point) const;
    double evaluate_at(const T & point, const Kernel & kernel, real bandwidth) const;
    void increment_for(Mesh<T, Spat> & mesh, const T & point, const Kernel & kernel, real bandwidth);

public:
    KDE(const MetricFunctor<T> & metric, const std::vector<T> & data);
    ~KDE(void);

    inline const Spat<T> & spatial(void) const { return *this->spatial_; };

    void evaluate_nodewise(Mesh<T, Spat> & mesh, const Kernel & kernel, const BandwidthSelector<T> & selector, real max_bandwidth) const;
    void evaluate_pointwise(Mesh<T, Spat> & mesh, const Kernel & kernel, const BandwidthSelector<T> & selector) const;
    void evaluate_pointwise_multiple(Mesh<T, Spat> & mesh, const Kernel & kernel, const BandwidthSelector<T> & selector) const;
    void evaluate_pointwise_corr(Mesh<T, Spat> & mesh, const Kernel & kernel, const BandwidthMatrixSelector<T> & selector) const;
};

#include "kde.tpp"

#endif // __KDE__
