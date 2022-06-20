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
    std::random_device rd_;
    std::seed_seq sd_({rd_(), rd_(), rd_(), rd_()});
    std::mt19937 mt_(sd_);
    std::normal_distribution<real> nd(0, 0.01);
    std::uniform_real_distribution<real> urd(0, 2 * M_PI);

    std::vector<TwoD> data;
/*
    for (unsigned int i = 0; i < 1000; ++i) {
        double a = nd(mt_);
        double b = nd(mt_);
        data.push_back({0 + 0.5 * a - 4 * b, -5 - 1 * a - 3 * b});
        data.push_back({9 - 0.7 * a + 2 * b, 6 + 0.5 * a - 2 * b});
    }
    for (unsigned int i = 0; i < 300; ++i) {
        double a = nd(mt_);
        double b = nd(mt_);
        data.push_back({12 - 1.1 * a + 3.2 * b, -8.5 - 0.1 * a - 0.5 * b});
    }*/

 /*   double c = 500;
    for (double x = 0; x < c; x += 1) {
        double r = 5 + nd(mt_);
        double p = urd(mt_);
        data.push_back({r * sin(p), r * cos(p)});
    }
*/
    data.push_back({0, 0});
    for (real x = 0.03; x < 1; x += 0.01) {
        data.push_back({-x + nd(mt_), x + nd(mt_)});
        data.push_back({x + nd(mt_), x + nd(mt_)});
        data.push_back({x + nd(mt_), -x + nd(mt_)});
        data.push_back({-x + nd(mt_), -x + nd(mt_)});
    }

    Euclidean2D metric;
    CartesianMesh2D<VPImplicitTree> mesh(metric, MeshLimits(-2, 2, 1000), MeshLimits(-2, 2, 1000));
    VPImplicitTree<TwoD> vpitree(metric, data);

    QuarticKernel kernel;
    Dataset<TwoD> dataset(data);
    dataset.save_tsv("out/x.tsv");
    BandwidthNearestSelector<TwoD> sel_s1(2);
    BandwidthNearestSelector<TwoD> sel_s(20);
    BandwidthMatrixNearestSelector<TwoD> sel_f(20);

    dataset.kde(metric, mesh, sel_s1, kernel);
    mesh.write_values("out/x/s1.bin");
    dataset.kde(metric, mesh, sel_s, kernel);
    mesh.write_values("out/x/s.bin");
    dataset.kde_corr(metric, mesh, sel_f, kernel);
    mesh.write_values("out/x/f.bin");
}
