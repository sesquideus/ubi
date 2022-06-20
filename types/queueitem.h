#ifndef __QUEUE_ITEM__
#define __QUEUE_ITEM__

#include <iostream>
#include "metric/metric.h"

template<typename T>
struct QueueItem {
    T point;
    real distance;

    QueueItem(T _point, real _distance):
        point(_point),
        distance(_distance)
    {}

    inline bool operator<(const QueueItem & other) const {
        return this->distance < other.distance;
    }

    inline bool operator==(const QueueItem & other) const {
        return (this->point == other.point) && ((this->distance - other.distance) < 1e-12);
    }
};

template<typename T>
class QueueMetricFunctor {
private:
    const MetricFunctor<T> & metric_;
public:
    QueueMetricFunctor(const MetricFunctor<T> & metric): metric_(metric) {}

    inline real operator()(QueueItem<T> a, QueueItem<T> b) const {
        return this->metric_(a.point, b.point);
    }
};

template<typename T>
std::ostream & operator<<(std::ostream & os, const QueueItem<T> & item) {
    os << item.point << " (" << item.distance << ")";
    return os;
}

#endif // __QUEUE_ITEM__
