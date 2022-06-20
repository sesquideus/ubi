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

    MeteorMetric metric(1, 0, 0, 1);

    MeteorMesh<double, VPImplicitTree> mesh(metric,
        MeshLimits<double>(-90, 90, 360),
        MeshLimits<double>(0, 360, 720),
        MeshLimits<double>(20000, 20000, 1),
        MeshLimits<double>(1, 1, 1),
        MeshLimits<double>(261.5, 261.5, 1)
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
    BandwidthNearestSelector<Meteor, double> selector(20);

    dataset.kde(metric, mesh, selector, kernel);
    mesh.write_values("out/geminids.bin");

    return 0;
}
