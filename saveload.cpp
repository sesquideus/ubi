#include <iostream>


int main(int argc, char * argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./saveload <filename>" << std::endl;
        return 1;
    }

    MeteorMetric metric(1, 0, 0, 1);
    MeteorMesh<VPImplicitTree> mesh(metric,
        MeshLimits(-90, 90, 36),
        MeshLimits(0, 360, 72),
        MeshLimits(0, 75, 1),
        MeshLimits(1, 1, 1),
        MeshLimits(261.5, 261.5, 1)
    );
}
