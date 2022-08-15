#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "types/meteor.h"
#include "types/dataset.h"
#include "spatial/structures.h"
#include "metric/metric.h"
#include "mesh/meshes.h"
#include "kde/kernel/kernels.h"


int main(int argc, char * argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./meteors <filename>" << std::endl;
        return 1;
    }

    MeteorMetric metric(1, 0, 0, 0);

    MeteorMesh<VPImplicitTree> mesh(metric,
        MeshLimits(-90, 90, 720),
        MeshLimits(0, 360, 1440),
        MeshLimits(20000, 20000, 1),
        MeshLimits(1, 1, 1),
        MeshLimits(0, 360, 1)
    );
/*
    MeteorMesh<double, VPImplicitTree> perseids(metric,
        MeshLimits<double>(50, 65, 150),
        MeshLimits<double>(35, 55, 200),
        MeshLimits<double>(1, 1, 1),
        MeshLimits<double>(1, 1, 1),
        MeshLimits<double>(130, 145, 150)
    );*/

    Dataset<Meteor> dataset;
    dataset.load_tsv(argv[1]);

    QuarticKernel kernel;
    BandwidthNearestSelector<Meteor> selector(20);

    dataset.kde(metric, mesh, selector, kernel);
    mesh.write_values("out/timeless.bin");

    return 0;
}
