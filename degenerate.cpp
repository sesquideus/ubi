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

    std::vector<TwoD<double>> data;
    for (double x = -1; x <= 1; x += 0.2) {
        data.push_back({0.3 - 0.8 * x, x - 0.2});
    }

    Euclidean2Dd metric;
    CartesianMesh2D<double, VPImplicitTree> mesh(metric, MeshLimits<double>(-2, 2, 1000), MeshLimits<double>(-2, 2, 1000));
    VPImplicitTree<TwoD<double>, double> vpitree(metric, data);

    QuarticKernel kernel;
    Dataset<TwoD<double>> dataset(data);
    dataset.save_tsv("out/degenerate/degenerate.tsv");
    BandwidthMatrixNearestSelector<TwoD<double>, double> sel_f(5);

    dataset.kde_corr(metric, mesh, sel_f, kernel);
    mesh.write_values("out/degenerate/degenerate.bin");
}
