#ifndef __CARTESIAN__
#define __CARTESIAN__

#include "mesh/mesh.h"


template<template<typename> class Spat>
class CartesianMesh2D: public Mesh<TwoD, Spat> {
private:
    const MeshLimits x_;
    const MeshLimits y_;

    unsigned int address(unsigned int x, unsigned int y) const;
    void for_all_nodes(std::function<void(unsigned int, unsigned int)> func);
    void for_all_nodes(std::function<void(unsigned int, unsigned int)> func) const;
    void emplace(unsigned int x, unsigned int y);

public:
    CartesianMesh2D(const MetricFunctor<TwoD> & metric, const MeshLimits & x, const MeshLimits & y);

    inline TwoD at(unsigned int x, unsigned int y) const { return this->nodes_[this->address(x, y)].coord(); };

    inline real & value(unsigned int x, unsigned int y)       { return this->nodes_[this->address(x, y)].value; };
    inline real   value(unsigned int x, unsigned int y) const { return this->nodes_[this->address(x, y)].value; };

    using Mesh<TwoD, Spat>::write_values;
    std::ostream & write_values(std::ostream & stream) const override;
    virtual void write_values_text(std::ostream & stream) const override;
};

#include "cartesian.tpp"

#endif // __CARTESIAN__
