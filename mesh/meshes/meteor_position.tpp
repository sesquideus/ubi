template<template<typename> class Spat>
MeteorPositionMesh<Spat>::MeteorPositionMesh(
    const MetricFunctor<Meteor> & metric,
    const MeshLimits & dec,
    const MeshLimits & ra
):
    Mesh<Meteor, Spat>(metric),
    dec_(dec),
    ra_(ra)
{
    unsigned int size = this->dec_.res() * this->ra_.res();
    std::cout << "Creating a meteor position mesh of " << this->dec_.res() << " × " << this->ra_.res()
              << " = " << size <<" elements" << std::endl;
    this->nodes_.reserve(size);

    this->for_all_nodes([&](unsigned int di, unsigned int ri){ this->emplace(di, ri); });
    std::cout << "Mesh complete" << std::endl;

    this->spatial_ = new Spat<MeshNode<Meteor>>(this->mesh_metric_, this->nodes_);
}

template<template<typename> class Spat>
unsigned int MeteorPositionMesh<Spat>::address(unsigned int di, unsigned int ri) const {
    if ((di < 0) || (di >= this->dec_.res()) || (ri < 0) || (ri >= this->ra_.res())) {
        throw std::range_error("Meteor position mesh: range error");
    } else {
        return di * this->ra_.res() + ri;
    }
}

template<template<typename> class Spat>
void MeteorPositionMesh<Spat>::for_all_nodes(std::function<void(unsigned int, unsigned int)> func) {
    for (unsigned int di = 0; di < this->dec_.res(); ++di) {
        for (unsigned int ri = 0; ri < this->ra_.res(); ++ri) {
            func(di, ri);
        }
    }
}

template<template<typename> class Spat>
void MeteorPositionMesh<Spat>::for_all_nodes(std::function<void(unsigned int, unsigned int)> func) const {
    for (unsigned int di = 0; di < this->dec_.res(); ++di) {
        for (unsigned int ri = 0; ri < this->ra_.res(); ++ri) {
            func(di, ri);
        }
    }
}

template<template<typename> class Spat>
void MeteorPositionMesh<Spat>::emplace(unsigned int di, unsigned int ri) {
    this->nodes_.emplace_back(MeshNode<Meteor>(
        this->address(di, ri),
        Meteor(
            this->address(di, ri),
            this->dec_.value(di),
            this->ra_.value(ri),
            0, 1, 0         // this does not matter, only mass must not be zero
        )
    ));
}

template<template<typename> class Spat>
std::ostream & MeteorPositionMesh<Spat>::write_values(std::ostream & stream) const {
    std::cout << "Writing out a meteor position mesh..." << std::endl;
    this->for_all_nodes([&](unsigned int di, unsigned int ri) -> void {
        real x = this->value(di, ri);
        stream.write(reinterpret_cast<const char*>(&x), sizeof x);
    });
    return stream;
}

template<template<typename> class Spat>
void MeteorPositionMesh<Spat>::write_values_text(std::ostream & stream) const {
    for (unsigned int di = 0; di < this->dec_.res(); ++di) {
        for (unsigned int ri = 0; ri < this->ra_.res(); ++ri) {
            stream << this->value(di, ri) << " ";
        }
        stream << std::endl;
    }
}
