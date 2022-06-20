#include <cstdio>
#include <iomanip>
#include <vector>
#include <fstream>

#include "utilities/benchmark.h"
#include "types/meteor.h"
#include "utilities/utilities.h"
#include "metric/metric.h"
#include "spatial/spatial.h"
#include "spatial/node.h"
#include "spatial/structures.h"
#include "kde/kde.h"
#include "kde/kernel/kernels.h"
#include "mesh/mesh.h"
#include "mesh/meshes.h"
#include "utilities/generator.h"


constexpr unsigned int COUNT = 10;

int main() {
    GeneratorSpherical gen;
    std::cout << std::fixed;

    for (unsigned int i = 0; i < COUNT; ++i) {
        twod.push_back(gen.random_cube());
    }

    auto metric = spherical2D;
    std::cout << "Building a KDE" << std::endl;

    KDE<TwoD, double, VPImplicitTree> kde(metric, twod);
    std::cout << "KDE complete" << std::endl;

    constexpr int resolution_x = 720;
    constexpr int resolution_y = 360;
    constexpr double extent_x = M_PI;
    constexpr double extent_y = M_PI / 2;

    CartesianMesh2D<double, VPHeap> meshheap(metric, {resolution_x, resolution_y}, {-extent_x, -extent_y}, {extent_x, extent_y});

//    kde.evaluate_online(mesh, GaussianKernel(), 0.05);
    kde.evaluate_points(meshheap, QuarticKernel(), BandwidthNearest<TwoD, double>(kde.spatial(), 200));

    std::ofstream out;
    out.open("pd.bin");
    meshheap.write_values(out);
    out.close();
    out.open("points.bin");
    for (auto && p: twod) {
        out.write(reinterpret_cast<const char*>(&p), sizeof p);
    }
    out.close();

    //kde.evaluate_offline(mesh, BandwidthConstant<TwoD>(*vptree, 0.1), ConstantKernel());

   /* std::vector<double> results;
    auto get_results = [&]() -> void { results = kde.evaluate(grid, BandwidthConstant<TwoD>(*vptree, 0.3), ConstantKernel()); };
    double time = timed(get_results);
    std::cout << "Evaluated in " << time << " s (" << time / (size_x * size_y) << " per node)" << std::endl;
    for (int j = 0; j < size_y; ++j) {
        for (int i = 0; i < size_x; ++i) {
            values[j][i] = results[j * size_x + i];
            out.write(reinterpret_cast<char*>(&values[j][i]), sizeof values[j][i]);
        }
    }
    out.close();*/

   // delete vptree;
   // delete vptreep;
   // delete exhaustive;
   // delete vpheap;
}
