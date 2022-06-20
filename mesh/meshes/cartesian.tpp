#include "cartesian.h"


template<template<typename> class Spat>
CartesianMesh2D<Spat>::CartesianMesh2D(
        const MetricFunctor<TwoD> & metric,
        const MeshLimits & x,
        const MeshLimits & y
):
    Mesh<TwoD, Spat>(metric),
    x_(x),
    y_(y)
{
    unsigned int size = this->x_.res() * this->y_.res();
    std::cout << "Creating a Cartesian 2D mesh of " <<
        this->x_.res() << " × " <<
        this->y_.res() << " = " << size <<" elements" << std::endl;
    this->nodes_.reserve(size);

    this->for_all_nodes([&](unsigned int xx, unsigned int yy){
        this->emplace(xx, yy);
    });
    std::cout << "Mesh created" << std::endl;

    this->spatial_ = new Spat<MeshNode<TwoD>>(this->mesh_metric_, this->nodes_);
}

template<template<typename> class Spat>
unsigned int CartesianMesh2D<Spat>::address(unsigned int x, unsigned int y) const {
//    if ((x < 0) || (x >= this->res_x_) || (y < 0) || (y >= this->res_y_)) {
//        throw std::range_error("Cartesian2D mesh range error");
//    } else {
        return y * this->x_.res() + x;
//    }
}

template<template<typename> class Spat>
void CartesianMesh2D<Spat>::for_all_nodes(std::function<void(unsigned int x, unsigned int y)> func) {
    for (unsigned int yi = 0; yi < this->y_.res(); ++yi) {
        for (unsigned int xi = 0; xi < this->x_.res(); ++xi) {
            func(xi, yi);
        }
    }
}

template<template<typename> class Spat>
void CartesianMesh2D<Spat>::for_all_nodes(std::function<void(unsigned int x, unsigned int y)> func) const {
    for (unsigned int yi = 0; yi < this->y_.res(); ++yi) {
        for (unsigned int xi = 0; xi < this->x_.res(); ++xi) {
            func(xi, yi);
        }
    }
}

template<template<typename> class Spat>
void CartesianMesh2D<Spat>::emplace(unsigned int x, unsigned int y) {
    this->nodes_.emplace_back(MeshNode<TwoD>(this->address(x, y), {
        this->x_.value(x),
        this->y_.value(y)
    }));
}

template<template<typename> class Spat>
std::ostream & CartesianMesh2D<Spat>::write_values(std::ostream & stream) const {
    std::cout << "Writing out a Cartesian 2D mesh..." << std::endl;
    this->for_all_nodes([&](unsigned int x, unsigned int y) -> void {
        real value = this->value(x, y);
        stream.write(reinterpret_cast<const char*>(&value), sizeof value);
    });
    return stream;
}

template<template<typename> class Spat>
void CartesianMesh2D<Spat>::write_values_text(std::ostream & stream) const {
    this->for_all_nodes([&](unsigned int x, unsigned int y) -> void {
        stream << this->value(x, y);
    });
}
