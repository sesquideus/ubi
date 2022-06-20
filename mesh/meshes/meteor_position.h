#ifndef __METEOR_POSITION__
#define __METEOR_POSITION__

#include "mesh/mesh.h"
#include "types/meteor.h"


template<template<typename> class Spat>
class MeteorPositionMesh: public Mesh<Meteor, Spat> {
private:
    const MeshLimits dec_;
    const MeshLimits ra_;

    unsigned int address(unsigned int di, unsigned int ri) const;
    void for_all_nodes(std::function<void(unsigned int, unsigned int)> func);
    void for_all_nodes(std::function<void(unsigned int, unsigned int)> func) const;

    void emplace(unsigned int di, unsigned int ri);

public:
    MeteorPositionMesh(const MetricFunctor<Meteor> & metric, const MeshLimits & dec, const MeshLimits & ra);

    inline Meteor at(unsigned int di, unsigned int ri) const { return this->nodes_[this->address(di, ri)].coord(); };
    inline real & value(unsigned int di, unsigned int ri)       { return this->nodes_[this->address(di, ri)].value; };
    inline real   value(unsigned int di, unsigned int ri) const { return this->nodes_[this->address(di, ri)].value; };

    std::ostream & write_values(std::ostream & stream) const override;
    virtual void write_values_text(std::ostream & stream) const override;
};

#include "meteor_position.tpp"

#endif // __METEOR_POSITION__
