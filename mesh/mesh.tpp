#include "mesh.h"

template<typename T, template<typename> class Spat>
Mesh<T, Spat>::Mesh(const MetricFunctor<T> & metric):
    spatial_(nullptr),
    metric_(metric),
    mesh_metric_(MeshMetricFunctor<T>(metric))
{}

template<typename T, template<typename> class Spat>
void Mesh<T, Spat>::zero(void) {
    std::cout << "Zeroing the mesh values" << std::endl;
    for (auto && node: this->nodes_) {
        node.value = 0;
    }
}

template<typename T, template<typename> class Spat>
void Mesh<T, Spat>::write_values(const std::string & filename) const {
    std::cout << "Writing out mesh values to \"" << filename << "\"" << std::endl;
    std::ofstream out;
    out.open(filename);
    this->write_values(out);
    out.close();
}

template<typename T, template<typename> class Spat>
void Mesh<T, Spat>::set_value_at(unsigned int index, real value) {
    this->nodes_[index].value = value;
}

template<typename T, template<typename> class Spat>
void Mesh<T, Spat>::increment_at(unsigned int index, real extra) {
    this->nodes_[index].value += extra;
}

template<typename T, template<typename> class Spat>
std::vector<QueueItem<MeshNode<T>>> Mesh<T, Spat>::find_within(const T & point, real radius) {
    return this->spatial_->find_within(MeshNode<T>(0xDEADBEEF, point), radius);
}
