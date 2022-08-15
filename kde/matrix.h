#ifndef __BANDWIDTH_MATRIX__
#define __BANDWIDTH_MATRIX__

#include <mlpack/core.hpp>
#include <mlpack/methods/quic_svd/quic_svd.hpp>

#include "types/real.h"
#include "types/tuples.h"
#include "kde/kde.h"


template<typename T>
struct MatrixTraits;

template<>
struct MatrixTraits<TwoD> {
    static constexpr unsigned int dim = 2;
    typedef typename arma::Mat<real>::fixed<dim, dim> matrix;
    typedef typename arma::Col<real>::fixed<dim> vector;
    static vector marshall(const TwoD & x) { return vector({x.x, x.y}); }
};

template<>
struct MatrixTraits<ThreeD> {
    static constexpr unsigned int dim = 3;
    typedef arma::Mat<real>::fixed<dim, dim> matrix;
    typedef arma::Col<real>::fixed<dim> vector;
    vector marshall(const ThreeD & x) { return vector({x.x, x.y, x.z}); }
};

template<>
struct MatrixTraits<Meteor> {
    static constexpr unsigned int dim = 5;
    typedef typename arma::mat::fixed<dim, dim> matrix;
    typedef typename arma::vec::fixed<dim> vector;
    vector marshall(const Meteor & x) { return vector({x.dec(), x.ra(), x.v(), x.mag(), x.lsun()}); }
};

template<typename T>
class BandwidthMatrix {
private:
    typename MatrixTraits<T>::matrix cov_;          // Covariance matrix
    typename MatrixTraits<T>::matrix invcov_;       // Inverted covariance matrix (set on initialization)
    typename MatrixTraits<T>::matrix eigenvectors_; // Set of eigenvectors, ordered by eigenvalues descending
    typename MatrixTraits<T>::vector eigenvalues_;  // Set of eigenvalues
    real invsqrtdet_;                               // 1 / sqrt(det(cov))

public:
    BandwidthMatrix(const typename MatrixTraits<T>::matrix & matrix);

    static BandwidthMatrix<T> from_data(
        const std::vector<T> & data,
        std::vector<real> weights = {}
    ) {
        if (weights.size() != data.size()) {
            weights = std::vector<real>(data.size(), 1.0);
        }

        // Construct an almost-empty covariance matrix of correct size
        typename MatrixTraits<T>::matrix cov = 1e-6 * (typename MatrixTraits<T>::matrix(arma::fill::eye));

        real tw = 0;
        for (unsigned int i = 0; i < data.size(); ++i) {
            for (unsigned int u = 0; u < MatrixTraits<T>::dim; ++u) {
                for (unsigned int v = 0; v < MatrixTraits<T>::dim; ++v) {
                    cov(u, v) += data[i][u] * data[i][v] * weights[i];
                }
            }
            tw += weights[i];
        }
        cov /= sqrt(tw);

        return BandwidthMatrix<T>(cov);
    }

    inline real evaluate_at(const T & point) const { return sqrt(this->evaluate_squared_at(point)); };
    real evaluate_squared_at(const T & point) const;  // compute squared distance at point (with respect to origin)

    inline real principal(void) const { return sqrt(this->eigenvalues_[0]); }
    inline real invsqrtdet(void) const { return this->invsqrtdet_; };
};


template<typename T>
BandwidthMatrix<T>::BandwidthMatrix(
    const typename MatrixTraits<T>::matrix & matrix
):
    cov_(matrix)
{
    this->invcov_ = arma::inv_sympd(this->cov_);
    this->invsqrtdet_ = sqrt(arma::det(this->invcov_));

    typename MatrixTraits<T>::matrix sigma, v;
    mlpack::svd::QUIC_SVD svd(this->cov_, this->eigenvectors_, v, sigma, 1e-3, 1e-3);
    this->eigenvalues_ = arma::diagvec(sigma);
}


template<typename T>
real BandwidthMatrix<T>::evaluate_squared_at(const T & point) const {
    typename MatrixTraits<T>::vector x = MatrixTraits<T>::marshall(point);
    return arma::dot(x.t() * this->invcov_, x);
}

#endif // __BANDWIDTH_MATRIX__
