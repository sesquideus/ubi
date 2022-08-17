template<typename T>
VPImplicitTree<T>::VPImplicitTree(const MetricFunctor<T> & metric, const std::vector<T> & items):
    Spatial<T>(metric),
    points_(items)
{
    this->size_ = items.size();
    this->array_.resize(items.size(), nullptr);
    std::vector<T*> pointers(items.size(), nullptr);

    for (unsigned int i = 0; i < this->size_; ++i) {
        pointers[i] = &(this->points_[i]);
    }

    this->build(0, pointers.begin(), pointers.begin(), pointers.end());
}

template<typename T>
void VPImplicitTree<T>::build(
    unsigned int index,
    typename std::vector<T*>::iterator begin,
    typename std::vector<T*>::iterator root,
    typename std::vector<T*>::iterator end)
{
    if (begin >= end) {
        // if there are no more nodes to process, make this a leaf node
        if (index < this->size_) {
            this->array_[index].threshold = 0.0;
        }
        return;
    } else {
        if (begin == end - 1) {
            // if there is exactly one node to process, make it a leaf node
            this->array_[index].address = *begin;
            this->array_[index].threshold = 0.0;
        } else {
            // otherwise there is at least one child
            // build a comparator function with respect to the vantage point
            auto comparator_root = [&](const T * first, const T * second) -> bool {
                return this->metric_(**root, *first) < this->metric_(**root, *second);
            };
            // the pivot is the element furthest from the vantage point
            auto pivot = std::max_element(begin, end, comparator_root);
            // swap the pivot to the beginning
            std::iter_swap(begin, pivot);
            // create an iterator pointing to where the inner-outer division should be
            auto median = begin + full_inner(end - begin) + 1;
            // partition the interval with respect to the division (which is now at begin)
            auto comparator_pivot = [&](const T * first, const T * second) -> bool {
                return this->metric_(**begin, *first) < this->metric_(**begin, *second);
            };
            std::nth_element(begin, median, end, comparator_pivot);

            // add the new node and recursively process the subintervals
            this->array_[index].address = *begin;
            this->array_[index].threshold = this->metric_(**begin, **median);
            this->build(2 * index + 1, begin + 1, begin, median);
            this->build(2 * index + 2, median, begin, end);
        }
    }
}

template<typename T>
void VPImplicitTree<T>::load(const std::string & filename) {
    try {
        std::ifstream in(filename);
        in.read(reinterpret_cast<char *>(&this->size_), sizeof(this->size_));
        this->points_.reserve(this->size_);
        in.read(reinterpret_cast<char *>(this->points_.data()), this->size_ * sizeof(T));
        this->array_.reserve(this->size_);
        in.read(reinterpret_cast<char *>(this->array_.data()), this->size_ * sizeof(T));
    } catch (const std::exception & e) {
        throw e;
    }
}

template<typename T>
void VPImplicitTree<T>::save(const std::string & filename) const {
    std::ofstream out(filename);
    out.write(reinterpret_cast<const char *>(this->size_), sizeof(this->size_));
    out.write(reinterpret_cast<const char *>(this->points_.data()), this->size_ * sizeof(T));
    out.write(reinterpret_cast<const char *>(this->array_.data()), this->size_ * sizeof(T));
}

template<typename T>
QueueItem<T> VPImplicitTree<T>::find_nearest(const T & point) const {
    QueueItem<T> candidate(this->points_[0], this->metric_(this->points_[0], point));
    this->tau_ = std::numeric_limits<real>::max();

    this->search_one(0, point, candidate);
    return candidate;
}

template<typename T>
std::vector<QueueItem<T>> VPImplicitTree<T>::find_nearest(const T & point, unsigned int count) const {
    std::priority_queue<QueueItem<T>> pq;
    std::vector<QueueItem<T>> result;

    this->tau_ = std::numeric_limits<real>::max();
    this->search_queued(0, point, count, pq);

    while (!pq.empty()) {
        result.push_back(pq.top());
        pq.pop();
    }
    std::reverse(result.begin(), result.end());
    return result;
}

template<typename T>
std::vector<QueueItem<T>> VPImplicitTree<T>::find_within(const T & point, real radius) const {
    std::vector<QueueItem<T>> result;
    this->search_radius(0, point, radius, result);
    return result;
}

/** Search for a single point that is nearest to <target> starting at <node>
 *  Functionally the same as search_queued with k = 1 but faster
 * **/
template<typename T>
void VPImplicitTree<T>::search_one(unsigned int index, const T & target, QueueItem<T> & candidate) const {
    if (index < this->size_) {
        real distance = this->metric_(this->point(index), target);

        if (distance < this->tau_) {
            candidate = QueueItem<T>(this->point(index), distance);
            this->tau_ = distance;
        }

        real threshold = this->array_[index].threshold;
        if (distance < threshold) {
            this->search_one(2 * index + 1, target, candidate);
            if (distance + this->tau_ >= threshold) {
                this->search_one(2 * index + 2, target, candidate);
            }
        } else {
            this->search_one(2 * index + 2, target, candidate);
            if (distance - this->tau_ <= threshold) {
                this->search_one(2 * index + 1, target, candidate);
            }
        }
    }
}

/* Search for k nearest points to <target>, starting at <node> */
template<typename T>
void VPImplicitTree<T>::search_queued(unsigned int index, const T & target, unsigned int count, std::priority_queue<QueueItem<T>> & pq) const {
    if (index < this->size_) {
        real distance = this->metric_(this->point(index), target);

        if (distance < this->tau_) {
            if (pq.size() == count) {
                pq.pop();
            }
            pq.push(QueueItem<T>(this->point(index), distance));
            if (pq.size() == count) {
                this->tau_ = pq.top().distance;
            }
        }

        real threshold = this->array_[index].threshold;
        if (2 * index < this->size_) {
            if (distance < threshold) {
                this->search_queued(2 * index + 1, target, count, pq);
                if (distance + this->tau_ >= threshold) {
                    this->search_queued(2 * index + 2, target, count, pq);
                }
            } else {
                this->search_queued(2 * index + 2, target, count, pq);
                if (distance - this->tau_ <= threshold) {
                    this->search_queued(2 * index + 1, target, count, pq);
                }
            }
        }
    }
}

/* Find all points that are closer to <target> than <radius> */
template<typename T>
void VPImplicitTree<T>::search_radius(unsigned int index, const T & target, real radius, std::vector<QueueItem<T>> & result) const {
    if (index < this->size_) {
        real distance = this->metric_(this->point(index), target);

        if (distance < radius) {
            result.push_back(QueueItem(this->point(index), distance));
        }

        real threshold = this->array_[index].threshold;
        if (distance < threshold) {
            this->search_radius(2 * index + 1, target, radius, result);
            if (distance + radius >= threshold) {
                this->search_radius(2 * index + 2, target, radius, result);
            }
        } else {
            this->search_radius(2 * index + 2, target, radius, result);
            if (distance - radius <= threshold) {
                this->search_radius(2 * index + 1, target, radius, result);
            }
        }
    }
}

template<typename T>
void VPImplicitTree<T>::print(void) const {
    this->print(0, 0);

    std::cout << std::endl;
    for (unsigned int i = 0; i < this->size_; ++i) {
        std::cout << i << " points to " << this->array_[i].address << ", value " << this->point(i);
        std::cout << " (threshold " << this->array_[i].threshold << ")" << std::endl;
    }
    std::cout << "----------------------------------------------" << std::endl;
}

template<typename T>
void VPImplicitTree<T>::print(unsigned int root, unsigned int offset) const {
    if (root < this->size_) {
        this->print(root * 2 + 1, offset + 1);
        for (unsigned int i = 0; i < offset; ++i) {
            std::cout << "    ";
        }
        std::cout << root << "(" << this->array_[root].address << ") " << this->array_[root].threshold << std::endl;
        this->print(root * 2 + 2, offset + 1);
    } else {
        return;
    }
}

template<typename T>
void VPImplicitTree<T>::print_points(void) const {
    for (unsigned int i = 0; i < this->size_; ++i) {
        std::cout << this->points_[i] << " ";
    }
    std::cout << std::endl;
}
