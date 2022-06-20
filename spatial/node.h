#ifndef __NODE__
#define __NODE__

#include <memory>

#include "metric/metric.h"
#include "mesh/meshnode.h"
#include "utilities/utilities.h"


template<typename T>
class DistanceComparator {
private:
    const T & _pivot;
    MetricFunctor<T> _metric;
public:
    DistanceComparator(const T & pivot, const MetricFunctor<T> & metric):
        _pivot(pivot),
        _metric(metric)
    {};

    bool operator()(const T & first, const T & second) {
        return this->_metric(this->_pivot, first) < this->_metric(this->_pivot, second);
    };
};

template<typename T>
struct VPNode {
    T point;
    real threshold;
    VPNode<T> * inside;
    VPNode<T> * outside;

    VPNode(T point, real threshold);
    ~VPNode(void);

    inline bool is_leaf(void) const {
        return (this->inside == nullptr) && (this->outside == nullptr);
    };

    void print(unsigned int space);
};

#include "node.tpp"

#endif // __NODE__
