#ifndef __VPITREE__
#define __VPITREE__

#include <memory>
#include <vector>
#include <queue>
#include <iostream>

#include "utilities/utilities.h"
#include "spatial/spatial.h"
#include "mesh/meshnode.h"
#include "types/node.h"


struct ArrayQueueItem {
    unsigned int address;
    real distance;

    ArrayQueueItem(unsigned int _address, real _distance):
        address(_address),
        distance(_distance)
    {}

    inline bool operator<(const ArrayQueueItem & other) const {
        return this->distance < other.distance;
    }
};

template<typename T>
struct ArrayItem {
    const T * address;
    real threshold;

    ArrayItem(const T * _address):
        address(_address),
        threshold(NAN)
    {}
};

template<typename T>
class VPImplicitTree: public Spatial<T> {
private:
    std::vector<T> points_;                             // internal storage of points
    std::vector<ArrayItem<T>> array_;                   // actual array

    mutable real tau_;

    void build(
        unsigned int index,
        typename std::vector<T*>::iterator begin,
        typename std::vector<T*>::iterator root,
        typename std::vector<T*>::iterator end
    );

    void search_one(unsigned int index, const T & target, QueueItem<T> & candidate) const;
    void search_queued(unsigned int index, const T & target, unsigned int count, std::priority_queue<QueueItem<T>> & pq) const;
    void search_radius(unsigned int index, const T & target, real radius, std::vector<QueueItem<T>> & pq) const;

    inline const T & point(unsigned int index) const { return *this->array_[index].address; };
    //const T & point(unsigned int index) const;
    void print(unsigned int address, unsigned int offset) const;


public:
    VPImplicitTree(const MetricFunctor<T> & metric, const std::vector<T> & items);

    void print(void) const override;
    void print_points(void) const;
    inline std::string name(void) const override { return "VP implicit tree"; };

    virtual void save(const std::string & filename) const override;
    virtual void load(const std::string & filename) override;

    // Find a single nearest point (optimized)
    QueueItem<T> find_nearest(const T & point) const override;

    // Find <count> nearest points
    std::vector<QueueItem<T>> find_nearest(const T & point, unsigned int count) const override;

    // Find all points closer than <radius>
    std::vector<QueueItem<T>> find_within(const T & point, const real radius) const override;
};

#include "vpitree.tpp"

#endif // __VPITREE__
