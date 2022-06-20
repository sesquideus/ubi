#ifndef __VPPTREE__
#define __VPPTREE__

#include <memory>
#include <vector>
#include <queue>
#include <iostream>

#include "spatial/spatial.h"


/**
 * Vantage-point tree, version with pointers */
template<typename T>
class VPPointerTree: public SpatialP<T> {
private:
    std::vector<T> * points_;
    VPNode<T*> * root_;
    mutable real tau_;

    VPNode<T*> * build(typename std::vector<T>::iterator begin, typename std::vector<T>::iterator end);

    void search_one(VPNode<T*> * node, const T & target, QueueItem<T*> & candidate) const;
    void search_queued(VPNode<T*> * node, const T & target, unsigned int count, std::priority_queue<QueueItem<T*>> & pq) const;
    void search_radius(VPNode<T*> * node, const T & target, real radius, std::vector<QueueItem<T*>> & pq) const;

public:
    VPPointerTree(const MetricFunctor<T> & metric, typename std::vector<T> & items);
    ~VPPointerTree(void);

    void print(void) const override;
    inline std::string name(void) const override { return "VPPointerTree"; };

    // Find a single nearest point (optimized)
    QueueItem<T*> find_nearest(const T & point) const override;

    // Find <count> nearest points
    std::vector<QueueItem<T*>> find_nearest(const T & point, unsigned int count) const override;

    // Find all points closer than <radius>
    std::vector<QueueItem<T*>> find_within(const T & point, const real radius) const override;
};

#include "vpptree.tpp"

#endif // __VPPTREE__
