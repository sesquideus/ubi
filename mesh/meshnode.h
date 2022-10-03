#ifndef __MESHNODE__
#define __MESHNODE__

#include "utilities/utilities.h"


template<typename T>
class MeshNode {
private:
    unsigned int index_;
    T coord_;

public:
    double value;

    MeshNode(unsigned int index, const T & coord):
        index_(index),
        coord_(coord),
        value(0.0)
    {}

    inline unsigned int index(void) const { return this->index_; };
    inline const T & coord(void) const { return this->coord_; };
};

template<typename T>
std::ostream & operator<<(std::ostream & os, const MeshNode<T> & m) {
    os << "Mesh node [" << m.coord() << "] value " << m.value;
    return os;
}


#endif // __MESHNODE__
