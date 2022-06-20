#ifndef __DATASET__
#define __DATASET__

#include "mesh/meshes.h"
#include "kde/kde.h"
#include "kde/matrix.h"


template<typename T>
class Dataset {
private:
    std::vector<T> data_;

    static T from_tokens(const std::vector<std::string> & tokens);
    static T from_line(const std::string & line);

    static std::string header(void);

public:
    Dataset(void) = default;
    Dataset(const std::vector<T> & data);
    ~Dataset(void) = default;

    inline const std::vector<T> & data(void) const { return this->data_; };

    void load(std::vector<T> && data);
    void load_tsv(const std::string & filename, const unsigned int max = std::numeric_limits<unsigned int>::max());
    void save_tsv(const std::string & filename) const;

    void kde(
        const MetricFunctor<T> & metric,
        Mesh<T, VPImplicitTree> & mesh,
        const BandwidthSelector<T> & selector,
        const Kernel & kernel
    );

    void kde_corr(
        const MetricFunctor<T> & metric,
        Mesh<T, VPImplicitTree> & mesh,
        const BandwidthMatrixSelector<T> & selector,
        const Kernel & kernel
    );
};

#include "dataset.tpp"

#endif // __DATASET__
