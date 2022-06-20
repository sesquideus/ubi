#ifndef __KDTREE__
#define __KDTREE__

/*
template<typename T>
class KDTree {
private:
    Node<T> * _root;
    const Metric<T> & _metric;
    mutable double _tau;

    Node<T> * build(typename std::vector<T>::iterator begin, typename std::vector<T>::iterator end);

    void search_one(Node<T> * node, const T & target, QueueItem<T> & candidate) const;
    void search_queued(Node<T> * node, const T & target, unsigned int k, std::priority_queue<QueueItem<T>> & pq) const;

public:
    VPTree(const Metric<T> & metric, typename std::vector<T> items);
    ~VPTree(void);

    void print(int space) const;

    // Find a single nearest point (optimized)
    QueueItem<T> find_nearest(const T & point);

    // Find k nearest points
    std::vector<QueueItem<T>> find_k_nearest(const T & point, int k = 1);

    // Find all points closer than radius
    std::vector<QueueItem<T>> find_within(const T & point, const double radius);
};
*/

#endif // __KDTREE__
