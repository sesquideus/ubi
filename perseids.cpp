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

    MeteorMesh<double, VPImplicitTree> perseid_mesh(metric,
        MeshLimits<double>(45, 70, 500),
        MeshLimits<double>(35, 70, 700),
        MeshLimits<double>(1, 1, 1),
        MeshLimits<double>(1, 1, 1),
        MeshLimits<double>(135, 150, 75)
    );

    Dataset<Meteor> dataset;
    dataset.load_tsv(argv[1]);

    QuarticKernel kernel;
    BandwidthNearestSelector<Meteor, double> selector(20);

    dataset.kde(metric, perseid_mesh, selector, kernel);
    perseid_mesh.write_values("out/perseids2.bin");

    return 0;
}
