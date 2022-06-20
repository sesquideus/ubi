#include "node.h"


template<typename T>
VPNode<T>::VPNode(T point_, real threshold_):
    point(point_),
    threshold(threshold_),
    inside(nullptr),
    outside(nullptr)
{}

template<typename T>
VPNode<T>::~VPNode(void) {
    delete this->inside;
    delete this->outside;
}

template<typename T>
void VPNode<T>::print(unsigned int space) {
    if (this->inside != nullptr) this->inside->print(space + 1);

    for (unsigned int i = 0; i < space; ++i) std::cout << "        ";
    std::cout << this->point << " (radius " << this->threshold << ")";
    if ((this->inside == nullptr) && (this->outside == nullptr)) std::cout << " (leaf)";
    std::cout << std::endl;

    if (this->outside != nullptr) this->outside->print(space + 1);
}

