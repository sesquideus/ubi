#ifndef __SPATIAL__
#define __SPATIAL__

#include "metric/metric.h"
#include "types/queueitem.h"
#include "spatial/node.h"
#include "utilities/utilities.h"


template<typename T>
class Spatial {
protected:
    long unsigned int size_;
    const MetricFunctor<T> & metric_;

public:
    Spatial(const MetricFunctor<T> & metric): metric_(metric) {};
    virtual ~Spatial(void) {};

    inline unsigned int size(void) const { return this->size_; };

    virtual void print(void) const = 0;
    virtual std::string name(void) const = 0;

    /*
    virtual void save(const std::string & filename) const = 0;
    virtual void load(const std::string & filename) = 0;
    */

    // Find a single nearest point (optimized)
    virtual QueueItem<T> find_nearest(const T & point) const = 0;

    // Find <count> nearest points
    virtual std::vector<QueueItem<T>> find_nearest(const T & point, unsigned int count) const = 0;

    // Find all points closer than <radius>
    virtual std::vector<QueueItem<T>> find_within(const T & point, const real radius) const = 0;
};


template<typename T>
class SpatialP {
protected:
    long unsigned int size_;
public:
    virtual ~SpatialP(void){};

    inline long unsigned int size(void) const { return this->size_; };

    virtual void print(void) const = 0;
    virtual std::string name(void) const = 0;

    // Find a single nearest point (optimized)
    virtual QueueItem<T*> find_nearest(const T & point) const = 0;

    // Find <count> nearest points
    virtual std::vector<QueueItem<T*>> find_nearest(const T & point, unsigned int count) const = 0;

    // Find all points closer than <radius>
    virtual std::vector<QueueItem<T*>> find_within(const T & point, const real radius) const = 0;
};

#endif // __SPATIAL__
