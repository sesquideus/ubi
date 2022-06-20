#include <cmath>
#include <iostream>
#include <iomanip>

#include "types/meteor.h"
#include "types/dataset.h"
#include "metric/metric.h"
#include "kde/matrix.h"
#include "kde/bwms.h"
#include "kde/kernel/kernels.h"


int main() {
    std::cout << std::setprecision(6) << std::setw(10) << std::fixed;
    std::random_device rd;
    std::seed_seq sd({rd(), rd(), rd(), rd()});
    std::mt19937 mt(sd);
    std::normal_distribution<double> nd(0, 0.3);

    std::vector<TwoD<double>> data;
    for (unsigned int i = 0; i < 1000; ++i) {
        double a = nd(mt);
        double b = nd(mt);
        data.push_back({a - 0.4 * b, a * 0.3 + b * 0.12});
    }

    Euclidean2Dd metric;
    CartesianMesh2D<double, VPImplicitTree> mesh(metric, MeshLimits<double>(-2, 2, 1000), MeshLimits<double>(-2, 2, 1000));
    VPImplicitTree<TwoD<double>, double> vpitree(metric, data);

    QuarticKernel kernel;
    Dataset<TwoD<double>> dataset(data);
    dataset.save_tsv("out/correlated.tsv");
    BandwidthNearestSelector<TwoD<double>, double> sel_s(80);
    BandwidthMatrixNearestSelector<TwoD<double>, double> sel_f(80);

    dataset.kde(metric, mesh, sel_s, kernel);
    mesh.write_values("out/galaxy/s.bin");
    dataset.kde_corr(metric, mesh, sel_f, kernel);
    mesh.write_values("out/galaxy/f.bin");
}
