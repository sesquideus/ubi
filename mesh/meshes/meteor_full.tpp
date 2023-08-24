template<template<typename> class Spat>
MeteorMesh<Spat>::MeteorMesh(
    const MetricFunctor<Meteor> & metric,
    const MeshLimits & dec,
    const MeshLimits & ra,
    const MeshLimits & v,
    const MeshLimits & mag,
    const MeshLimits & lsun
):
    Mesh<Meteor, Spat>(metric),
    dec_(dec),
    ra_(ra),
    v_(v),
    mag_(mag),
    lsun_(lsun)
{
    unsigned int size = this->dec_.res() * this->ra_.res() * this->v_.res() * this->mag_.res() * this->lsun_.res();
    std::cout << "Creating a meteor mesh of " <<
        this->dec_.res() << " × " <<
        this->ra_.res() << " × " <<
        this->v_.res() << " × " <<
        this->mag_.res() << " × " <<
        this->lsun_.res() << " = " << size << " elements" << std::endl;
    this->nodes_.reserve(size);

    this->for_all_nodes([&](const MeteorIndex & index){
        this->emplace(index);
    });
    std::cout << "Mesh created" << std::endl;

    std::cout << "Building a spatial data structure" << std::endl;
    this->spatial_ = new Spat<MeshNode<Meteor>>(this->mesh_metric_, this->nodes_);
    std::cout << "Spatial structure complete" << std::endl;
}

template<template<typename> class Spat>
unsigned int MeteorMesh<Spat>::address(const MeteorIndex & index) const {
    /*if (
        (index.dec  < 0) || (index.dec  >= this->dec_.res() ) ||
        (index.ra   < 0) || (index.ra   >= this->ra_.res()  ) ||
        (index.v    < 0) || (index.v    >= this->v_.res()   ) ||
        (index.mag  < 0) || (index.mag  >= this->mag_.res() ) ||
        (index.lsun < 0) || (index.lsun >= this->lsun_.res())
    ) {
        throw std::range_error("Meteor position mesh: range error");
    } else {*/
        return ((((index.dec * this->ra_.res()) + index.ra) * this->v_.res() + index.v) * this->mag_.res() + index.mag) * this->lsun_.res() + index.lsun;
    /*}*/
}

template<template<typename> class Spat>
void MeteorMesh<Spat>::for_all_nodes(std::function<void(const MeteorIndex & index)> func) {
    for (unsigned int di = 0; di < this->dec_.res(); ++di) {
        for (unsigned int ri = 0; ri < this->ra_.res(); ++ri) {
            for (unsigned int vi = 0; vi < this->v_.res(); ++vi) {
                for (unsigned int mi = 0; mi < this->mag_.res(); ++mi) {
                    for (unsigned int li = 0; li < this->lsun_.res(); ++li) {
                        func(MeteorIndex({di, ri, vi, mi, li}));
                    }
                }
            }
        }
    }
}

template<template<typename> class Spat>
void MeteorMesh<Spat>::for_all_nodes(std::function<void(const MeteorIndex & index)> func) const {
    for (unsigned int di = 0; di < this->dec_.res(); ++di) {
        for (unsigned int ri = 0; ri < this->ra_.res(); ++ri) {
            for (unsigned int vi = 0; vi < this->v_.res(); ++vi) {
                for (unsigned int mi = 0; mi < this->mag_.res(); ++mi) {
                    for (unsigned int li = 0; li < this->lsun_.res(); ++li) {
                        func(MeteorIndex({di, ri, vi, mi, li}));
                    }
                }
            }
        }
    }
}

template<template<typename> class Spat>
void MeteorMesh<Spat>::emplace(const MeteorIndex & index) {
    this->nodes_.emplace_back(MeshNode<Meteor>(
        this->address(index),
        Meteor::from_degrees(
            this->dec_.value(index.dec),
            this->ra_.value(index.ra),
            this->v_.value(index.v),
            this->mag_.value(index.mag),
            this->lsun_.value(index.lsun)
        )
    ));
}

template<template<typename> class Spat>
std::ostream & MeteorMesh<Spat>::write_meta(std::ostream & stream) const {
    // Implement this later
    return stream;
}

template<template<typename> class Spat>
std::ostream & MeteorMesh<Spat>::write_values(std::ostream & stream) const {
    std::cout << "Writing out a meteor position mesh..." << std::endl;
    this->for_all_nodes([&](const MeteorIndex & index) -> void {
        real value = this->value(index);
        stream.write(reinterpret_cast<const char*>(&value), sizeof value);
    });
    return stream;
}

template<template<typename> class Spat>
void MeteorMesh<Spat>::write_values_text(std::ostream & stream) const {
    this->for_all_nodes([&](const MeteorIndex & index) -> void {
        stream << this->value(index);
    });
}
