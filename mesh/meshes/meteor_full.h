#ifndef __METEOR_FULL__
#define __METEOR_FULL__

#include "mesh/mesh.h"
#include "types/meteor.h"


struct MeteorIndex {
    unsigned int dec;
    unsigned int ra;
    unsigned int v;
    unsigned int lnm;
    unsigned int lsun;
};


template<template<typename> class Spat>
class MeteorMesh: public Mesh<Meteor, Spat> {
private:
    const MeshLimits dec_;
    const MeshLimits ra_;
    const MeshLimits v_;
    const MeshLimits lnm_;
    const MeshLimits lsun_;

    unsigned int address(const MeteorIndex & index) const;
    void for_all_nodes(std::function<void(const MeteorIndex & index)> func);
    void for_all_nodes(std::function<void(const MeteorIndex & index)> func) const;

    void emplace(const MeteorIndex & index);

public:
    MeteorMesh(
        const MetricFunctor<Meteor> & metric,
        const MeshLimits & dec,
        const MeshLimits & ra,
        const MeshLimits & v,
        const MeshLimits & lnm,
        const MeshLimits & lsun
    );

    inline Meteor at(const MeteorIndex & index) const { return this->nodes_[this->address(index)].coord(); };
    inline real & value(const MeteorIndex & index)       { return this->nodes_[this->address(index)].value; };
    inline real   value(const MeteorIndex & index) const { return this->nodes_[this->address(index)].value; };

    using Mesh<Meteor, Spat>::write_values;
    std::ostream & write_values(std::ostream & stream) const override;
    virtual void write_values_text(std::ostream & stream) const override;
};

#include "meteor_full.tpp"

#endif // __METEOR_FULL__
