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
    std::vector<TwoD<double>> data = {{0, 0}};

    Euclidean2Dd metric;
    CartesianMesh2D<double, VPImplicitTree> mesh(metric, MeshLimits<double>(-1.1, 1.1, 1000), MeshLimits<double>(-1.1, 1.1, 1000));
    VPImplicitTree<TwoD<double>, double> vpitree(metric, data);

    QuarticKernel kernel;
    Dataset<TwoD<double>> dataset(data);
    BandwidthMatrixConstantSelector<TwoD<double>, double> sel_s({{0.5, 0}, {0, 0.5}});
    BandwidthMatrixConstantSelector<TwoD<double>, double> sel_d({{0.7, 0}, {0, 0.3}});
    BandwidthMatrixConstantSelector<TwoD<double>, double> sel_f({{0.6, -0.3}, {-0.3, 0.4}});
    BandwidthMatrixConstantSelector<TwoD<double>, double> sel_f2({{0.7, 0.2}, {0.2, -0.06}});

    dataset.kde_corr(metric, mesh, sel_s, kernel);
    mesh.write_values("out/s.bin");
    dataset.kde_corr(metric, mesh, sel_d, kernel);
    mesh.write_values("out/d.bin");
    dataset.kde_corr(metric, mesh, sel_f, kernel);
    mesh.write_values("out/f.bin");
    dataset.kde_corr(metric, mesh, sel_f2, kernel);
    mesh.write_values("out/f2.bin");
}
