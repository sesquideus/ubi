#include "vptree.h"
#include "../node.h"


template<typename T>
VPTree<T>::VPTree(const MetricFunctor<T> & metric, typename std::vector<T> items):
    root_(nullptr),
    metric_(metric)
{
    this->root_ = this->build(items.begin(), items.end());
    this->size_ = items.size();
}

template<typename T>
VPTree<T>::~VPTree(void) {
    delete this->root_;
}

template<typename T>
VPNode<T> * VPTree<T>::build(typename std::vector<T>::iterator begin, typename std::vector<T>::iterator end) {
    if (begin >= end) {
        // this is already a leaf node with nothing to process further, do nothing
        return nullptr;
    }
    if (begin == end - 1) {
        // this is a leaf node, add and return it
        VPNode<T> * node = new VPNode<T>(*begin, 0);
        return node;
    } else {
        // there is at least one child, find the median with respect to a random pivot
        // the pivot is the element furthest from the vantage point
        auto pivot = std::max_element(begin, end, DistanceComparator(*begin, this->metric_));
        // create an iterator pointing to where the median SHOULD be
        auto median = begin + (end - begin + 1) / 2;
        // swap the pivot to beginning
        std::iter_swap(begin, pivot);
        // partition the interval with respect to the pivot (now at begin)
        std::nth_element(begin, median, end, DistanceComparator(*begin, this->metric_));
        //std::sort(begin + 1, end, DistanceComparator(*begin, this->metric_));

        // add the new node and recursively process the subintervals
        VPNode<T> * node = new VPNode<T>(*begin, this->metric_(*begin, *median));
        node->inside = this->build(begin + 1, median);
        node->outside = this->build(median, end);

        return node;
    }
}

template<typename T>
QueueItem<T> VPTree<T>::find_nearest(const T & point) const {
    QueueItem<T> candidate(this->root_->point, this->metric_(this->root_->point, point));
    this->tau_ = std::numeric_limits<real>::max();

    this->search_one(this->root_, point, candidate);
    return candidate;
}

template<typename T>
std::vector<QueueItem<T>> VPTree<T>::find_nearest(const T & point, unsigned int count) const {
    std::priority_queue<QueueItem<T>> pq;
    std::vector<QueueItem<T>> result;

    this->tau_ = std::numeric_limits<real>::max();
    this->search_queued(this->root_, point, count, pq);

    while (!pq.empty()) {
        result.push_back(pq.top());
        pq.pop();
    }
    std::reverse(result.begin(), result.end());
    return result;
}

template<typename T>
std::vector<QueueItem<T>> VPTree<T>::find_within(const T & point, real radius) const {
    std::vector<QueueItem<T>> result;
    this->search_radius(this->root_, point, radius, result);
    return result;
}

/** Search for a single point that is nearest to <target> starting at <node>
 *  Functionally the same as search_queued with k = 1 but faster
 * **/
template<typename T>
void VPTree<T>::search_one(VPNode<T> * node, const T & target, QueueItem<T> & candidate) const {
    if (node != nullptr) {
        real distance = this->metric_(node->point, target);

        if (distance < this->tau_) {
            candidate = QueueItem<T>(node->point, distance);
            this->tau_ = distance;
        }

        if (!node->is_leaf()) {
            if (distance < node->threshold) {
                this->search_one(node->inside, target, candidate);
                if (distance + this->tau_ >= node->threshold) {
                    this->search_one(node->outside, target, candidate);
                }
            } else {
                this->search_one(node->outside, target, candidate);
                if (distance - this->tau_ <= node->threshold) {
                    this->search_one(node->inside, target, candidate);
                }
            }
        }
    }
}

/* Search for k nearest points to <target>, starting at <node> */
template<typename T>
void VPTree<T>::search_queued(VPNode<T> * node, const T & target, unsigned int count, std::priority_queue<QueueItem<T>> & pq) const {
    if (node != nullptr) {
        real distance = this->metric_(node->point, target);

        if (distance < this->tau_) {
            if (pq.size() == count) {
                pq.pop();
            }
            pq.push(QueueItem<T>(node->point, distance));
            if (pq.size() == count) {
                this->tau_ = pq.top().distance;
            }
        }

        if (!node->is_leaf()) {
            if (distance < node->threshold) {
                this->search_queued(node->inside, target, count, pq);
                if (distance + this->tau_ >= node->threshold) {
                    this->search_queued(node->outside, target, count, pq);
                }
            } else {
                this->search_queued(node->outside, target, count, pq);
                if (distance - this->tau_ <= node->threshold) {
                    this->search_queued(node->inside, target, count, pq);
                }
            }
        }
    }
}

/* Find all points that are closer to <target> than <radius> */
template<typename T>
void VPTree<T>::search_radius(VPNode<T> * node, const T & target, real radius, std::vector<QueueItem<T>> & result) const {
    if (node != nullptr) {
        real distance = this->metric_(node->point, target);

        if (distance < radius) {
            result.push_back(QueueItem(node->point, distance));
        }

        if (distance < node->threshold) {
            this->search_radius(node->inside, target, radius, result);
            if (distance + radius >= node->threshold) {
                this->search_radius(node->outside, target, radius, result);
            }
        } else {
            this->search_radius(node->outside, target, radius, result);
            if (distance - radius <= node->threshold) {
                this->search_radius(node->inside, target, radius, result);
            }
        }
    }
}

template<typename T>
void VPTree<T>::print(void) const {
    if (this->root_ == nullptr) {
        std::cout << "VPTree is empty" << std::endl;
    } else {
        this->root_->print(0);
    }
}
