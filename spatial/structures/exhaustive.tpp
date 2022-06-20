template<typename T>
Exhaustive<T>::Exhaustive(const MetricFunctor<T> & metric, typename std::vector<T> items):
    Spatial<T>(metric),
    data_(items)
{
    this->size_ = items.size();
}

template<typename T>
QueueItem<T> Exhaustive<T>::find_nearest(const T & point) const {
    auto min = std::min_element(this->data_.begin(), this->data_.end(), DistanceComparator(point, this->metric_));
    return QueueItem<T>(*min, this->metric_(*min, point));
}

template<typename T>
std::vector<QueueItem<T>> Exhaustive<T>::find_nearest(const T & point, unsigned int count) const {
    auto begin = this->data_.begin();
    auto end = this->data_.end();
    auto comparator = DistanceComparator(point, this->metric_);
    std::nth_element(begin, begin + count, end, comparator);

    std::vector<T> points;
    std::copy(begin, begin + count, std::back_inserter(points));
    std::sort(points.begin(), points.end(), comparator);

    std::vector<QueueItem<T>> result;
    for(auto && p: points) {
        result.push_back(QueueItem(p, this->metric_(p, point)));
    }
    return result;
}

template<typename T>
std::vector<QueueItem<T>> Exhaustive<T>::find_within(const T & point, real radius) const {
    std::vector<QueueItem<T>> result;
    real distance;

    for (auto && p: this->data_) {
        if ((distance = this->metric_(p, point)) <= radius) {
            result.push_back(QueueItem(p, distance));
        }
    }

    return result;
}

template<typename T>
void Exhaustive<T>::print(void) const {
    for (auto && p: this->data_) {
        std::cout << p << std::endl;
    }
}

/*
template<typename T>
void Exhaustive<T>::save(const std::string & filename) const {
    std::ostream file(filename);
    for (auto && p: this->data_) {
        file << Binary<T>(p);
    }
    file.close();
}*/

