#include "kde.h"


template<typename T, template<typename> class Spat>
KDE<T, Spat>::KDE(const MetricFunctor<T> & metric, const std::vector<T> & data):
    metric_(metric),
    data_(data),
    spatial_(nullptr)
{
    this->spatial_ = new Spat<T>(metric, this->data_);
}

template<typename T, template<typename> class Spat>
KDE<T, Spat>::~KDE(void) {
    delete this->spatial_;
}

template<typename T, template<typename> class Spat>
double KDE<T, Spat>::evaluate_at(const T & point, const Kernel & kernel, real bandwidth) const {
    auto neighbours = this->spatial_->find_within(point, kernel.cutoff() * bandwidth);

    std::function<double(QueueItem<T> neighbour)> func = [&kernel, bandwidth](QueueItem<T> neighbour) -> double {
        return kernel.evaluate(neighbour.distance, bandwidth);
    };
    auto p = ::map(func, neighbours);

    double pd = 0;
    for (auto && neighbour: neighbours) {
        pd += kernel.evaluate(neighbour.distance, bandwidth);
    }
    return pd;
}

template<typename T, template<typename> class Spat>
void KDE<T, Spat>::increment_for(Mesh<T, Spat> & mesh, const T & point, const Kernel & kernel, real bandwidth) {
    auto nodes = mesh.find_within(point, kernel.cutoff() * bandwidth);
    for (auto && node: nodes) std::cout << node.point.coord() << std::endl;
}

template<typename T, template<typename> class Spat>
void KDE<T, Spat>::evaluate_nodewise(Mesh<T, Spat> & mesh, const Kernel & kernel, const BandwidthSelector<T> & selector, real max_bandwidth) const {
    std::cout << "Evaluating node-wise for " << mesh.nodes().size() << " nodes" << std::endl;
    for (unsigned int i = 0; i < mesh.nodes().size(); ++i) {
        mesh.set_value_at(i, this->evaluate_at(mesh.nodes()[i].coord(), kernel, max_bandwidth));
    }
}

template<typename T, template<typename> class Spat>
void KDE<T, Spat>::evaluate_pointwise(
    Mesh<T, Spat> & mesh,
    const Kernel & kernel,
    const BandwidthSelector<T> & selector
) const {
    mesh.zero();
    std::cout << "Evaluating point-wise with scalar bandwidth for " << mesh.nodes().size() << " nodes" << std::endl;

    unsigned int i = 0;
    for (auto && point: this->data_) {
        std::cout << "Evaluating " << i++ << " / " << this->data_.size() << " " << point << std::endl;

        double bandwidth = selector.for_point(point, *this->spatial_);
        auto nodes = mesh.find_within(point, kernel.cutoff() * bandwidth);

        //std::cout << "Evaluating at " << point << ", " << nodes.size() << " found within " << bandwidth << std::endl;
        for (auto && node: nodes) {
            mesh.increment_at(node.point.index(), kernel.evaluate(node.distance, bandwidth) / (bandwidth * bandwidth));
        }
    }
}

template<typename T, template<typename> class Spat>
void KDE<T, Spat>::evaluate_pointwise_multiple(
    Mesh<T, Spat> & mesh,
    const Kernel & kernel,
    const BandwidthSelector<T> & selector
) const {
    mesh.zero();
    std::cout << "Evaluating point-wise with multiple scalar bandwidth for " << mesh.nodes().size() << " nodes" << std::endl;

    unsigned int i = 0;
    for (auto && point: this->data_) {
        std::cout << "Evaluating " << i++ << " / " << this->data_.size() << " " << point << std::endl;

        double bandwidth = selector.for_point(point, *this->spatial_);
        auto nodes = mesh.find_within(point, kernel.cutoff() * bandwidth);

        //std::cout << "Evaluating at " << point << ", " << nodes.size() << " found within " << bandwidth << std::endl;
        for (auto && node: nodes) {
            mesh.increment_at(node.point.index(), kernel.evaluate(node.distance, bandwidth) / (bandwidth * bandwidth));
        }
    }
}

template<typename T, template<typename> class Spat>
void KDE<T, Spat>::evaluate_pointwise_corr(
    Mesh<T, Spat> & mesh,
    const Kernel & kernel,
    const BandwidthMatrixSelector<T> & selector
) const {
    mesh.zero();
    std::cout << "Evaluating point-wise with matrix for " << mesh.nodes().size() << " nodes" << std::endl;

    unsigned int i = 0;
    for (auto && point: this->data_) {
        std::cout << "Evaluating " << i++ << " / " << this->data_.size() << " " << point << std::endl;

        BandwidthMatrix<T> bm = selector.for_point(point, *this->spatial_);
        auto nodes = mesh.find_within(point, kernel.cutoff() * bm.principal());

        for (auto && node: nodes) {
            mesh.increment_at(node.point.index(), kernel.evaluate_squared(bm.evaluate_squared_at(node.point.coord() - point)) / bm.invsqrtdet());
        }
    }
}
