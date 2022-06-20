#include "benchmark.h"


namespace benchmark {
    template<typename T>
    void nearest_one(Spatial<T> & spatial, const T & ref, unsigned int repeat) {
        std::cout << spatial.name() << " found nearest point in " <<
            timed([&spatial, ref, repeat]() -> void {
                for (unsigned int i = 0; i < repeat; ++i) {
                    spatial.find_nearest(ref);
                }
            }) / repeat << " us (" << repeat << " repetitions)" << std::endl;
    }

    template<typename T>
    void nearest_many(Spatial<T> & spatial, const T & ref, unsigned int count, unsigned int repeat) {
        std::vector<QueueItem<T>> nearest;
        std::cout << spatial.name() << " found nearest " << count << " points in " <<
            timed([&spatial, &nearest, ref, count, repeat]() -> void {
                for (unsigned int i = 0; i < repeat; ++i) {
                    nearest = spatial.find_nearest(ref, count);
                }
            }) / repeat << " us (" << repeat << " repetitions)" << std::endl;
    }

    template<typename T>
    void radius(Spatial<T> & spatial, const T & ref, real radius, unsigned int repeat) {
        std::vector<QueueItem<T>> nearest;
        std::cout << spatial.name() << " found points within " << radius << " in " <<
            timed([&spatial, &nearest, radius, ref, repeat]() -> void {
                for (unsigned int i = 0; i < repeat; ++i) {
                    nearest = spatial.find_within(ref, radius);
                }
            }) / repeat << " us (" << repeat << " repetitions)" << std::endl;
        std::cout << nearest.size() << " elements within radius " << radius << std::endl;
    }
}
