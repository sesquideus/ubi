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

    MeteorMetric metric(1, 1e-4, 0, 0);

    MeteorMesh<VPImplicitTree> mesh(metric,
        MeshLimits(-90, 90, 720),       // dec
        MeshLimits(0, 360, 1440),       // ra
        MeshLimits(30000, 36000, 6),    // v
        MeshLimits(1, 1, 1),            // lnm
        MeshLimits(0, 360, 1)           // lsun
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
    mesh.write_values("out/3035.bin");

    return 0;
}
