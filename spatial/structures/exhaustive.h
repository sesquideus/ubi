#ifndef __EXHAUSTIVE__
#define __EXHAUSTIVE__

#include <memory>

#include "utilities/utilities.h"
#include "spatial/spatial.h"


template<typename T>
class Exhaustive: public Spatial<T> {
private:
    mutable std::vector<T> data_;

public:
    Exhaustive(const MetricFunctor<T> & metric, typename std::vector<T> items);
    ~Exhaustive(void) = default;

    void print(void) const override;
    inline std::string name(void) const override { return "Exhaustive"; };

    /*
    virtual void save(const std::string & filename) const override;
    virtual void load(const std::string & filename) override;
    */

    QueueItem<T> find_nearest(const T & point) const override;
    std::vector<QueueItem<T>> find_nearest(const T & point, unsigned int count) const override;
    std::vector<QueueItem<T>> find_within(const T & point, const real radius) const override;
};

#include "exhaustive.tpp"

#endif // _EXHAUSTIVE__
