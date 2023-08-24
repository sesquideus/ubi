#include <cstdio>
#include <iomanip>
#include <vector>
#include <fstream>
#include <functional>

#include "utilities/benchmark.h"
#include "types/meteor.h"
#include "utilities/utilities.h"
#include "spatial/spatial.h"
#include "metric/metric.h"
#include "spatial/node.h"
#include "spatial/structures.h"
#include "kde/kde.h"
#include "kde/kernel/kernels.h"
#include "mesh/mesh.h"
#include "mesh/meshes.h"
#include "utilities/generator.h"

constexpr int COUNT = 100000;
constexpr int TESTCOUNT = 1000;


template<typename T, typename Real>
std::function<void(void)> benchmark_radius(const Spatial<T, Real> * spatial) {
    GeneratorMeteor generator;
    return [&](void) -> void {
        for (unsigned int i = 0; i < TESTCOUNT; ++i) {
            auto ref = Meteor(0xDEAD, 43, 56, 50000, 0.001, 345);
            spatial->find_within(ref, 0.001f);
        }
    };
}

template<typename T, typename Real>
void benchmark_build(const Metric<T, Real> & metric) {
    std::vector<T> data;
    GeneratorMeteor generator;

    for (int i = 0; i < COUNT; ++i) {
        data.push_back(generator.random_uniform());
    }

    Exhaustive<T, Real> * exhaustive = nullptr;
    VPTree<T, Real> * vptree = nullptr;
    VPPointerTree<T, Real> * vpptree = nullptr;
    VPImplicitTree<T, Real> * vpitree = nullptr;

    auto build_exhaustive = [&]() -> void { exhaustive = new Exhaustive<T, Real>(metric, data); };
    std::cout << "Exhaustive of " << COUNT << " elements built in " << timed(build_exhaustive) << " s" << std::endl;

    auto build_tree = [&]() -> void { vptree = new VPTree<T, Real>(metric, data); };
    std::cout << "VPTree of " << COUNT << " elements built in " << timed(build_tree) << " s" << std::endl;

    auto build_ptree = [&]() -> void { vpptree = new VPPointerTree<T, Real>(metric, data); };
    std::cout << "VPPointerTree of " << COUNT << " elements built in " << timed(build_ptree) << " s" << std::endl;

    auto build_itree = [&]() -> void { vpitree = new VPImplicitTree<T, Real>(metric, data); };
    std::cout << "VPImplicitTree of " << COUNT << " elements built in " << timed(build_itree) << " s" << std::endl;

    std::cout << "Exhaustive: find within radius " << timed(benchmark_radius(exhaustive)) << " s" << std::endl;
    std::cout << "VPTree: find within radius " << timed(benchmark_radius(vptree)) << " s" << std::endl;
    //std::cout << "VPPointerTree: find within radius " << timed(benchmark_radius(vpptree)) << " s" << std::endl;
    std::cout << "VPImplicitTree: find within radius " << timed(benchmark_radius(vpitree)) << " s" << std::endl;

    delete exhaustive;
    delete vptree;
    delete vpptree;
    delete vpitree;
}

template<typename T, typename Float>
void test_vpitree(void) {
    auto metric = euclidean2D;

    std::random_device rd;
    std::seed_seq sd{rd(), rd(), rd(), rd()};
    std::mt19937 mt(sd);
    std::uniform_real_distribution<Float> ud(0, 1);
    std::normal_distribution<Float> nd(0, 0.5);

    VPImplicitTree<T, Float> * vpitree = nullptr;
    Exhaustive<T, Float> * exhaustive = nullptr;

    std::vector<T> data;

    for (unsigned int i = 0; i < 1000; ++i) {
        data.clear();
        for (unsigned int j = 0; j < COUNT; ++j) {
            data.push_back({ud(mt), ud(mt)});
        }

        vpitree = new VPImplicitTree<T, Float>(metric, data);
        exhaustive = new Exhaustive<T, Float>(metric, data);

        for (unsigned int j = 0; j < 1; ++j) {
            T ref = {ud(mt), ud(mt)};

            auto x = exhaustive->find_within(ref, 0.05);
            std::sort(x.begin(), x.end());
            auto y = vpitree->find_within(ref, 0.05);
            std::sort(y.begin(), y.end());
            if (!(std::equal(x.begin(), x.end(), y.begin()))) {
                throw std::overflow_error("FU");
            }
        }
    }
    std::cout << "Test OK" << std::endl;

    delete exhaustive;
    delete vpitree;
}

int main(void) {
    benchmark_build<Meteor, double>(Meteor::position_metric);
    //test_vpitree<TwoD, double>();
    return 0;
}
