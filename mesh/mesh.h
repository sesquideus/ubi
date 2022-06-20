#ifndef __MESH__
#define __MESH__

#include <memory>
#include <vector>
#include <ostream>
#include <fstream>

#include "utilities/utilities.h"
#include "spatial/structures.h"
#include "kde/kernel.h"

#include "meshnode.h"
#include "meshlimits.h"


template<typename T, template<typename> class Spat>
class Mesh {
protected:
    std::vector<MeshNode<T>> nodes_;
    Spat<MeshNode<T>> * spatial_;
    const MetricFunctor<T> & metric_;
    MeshMetricFunctor<T> mesh_metric_;

public:
    Mesh(const MetricFunctor<T> & metric);
    virtual ~Mesh(void) { delete this->spatial_; };

    void zero(void);

    //inline const Spat<T> * spatial(void) const { return this->spatial_; };
    inline const std::vector<MeshNode<T>> & nodes(void) const { return this->nodes_; };

    void set_value_at(unsigned int index, real value);
    void increment_at(unsigned int index, real extra);

    std::vector<QueueItem<MeshNode<T>>> find_within(const T & point, real radius);

    void write_values(const std::string & filename) const;
    virtual std::ostream & write_values(std::ostream & stream) const = 0;
    virtual void write_values_text(std::ostream & stream) const = 0;
};

#include "mesh.tpp"

#endif // __MESH__
