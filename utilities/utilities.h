#ifndef __UTILITIES__
#define __UTILITIES__

#include <functional>
#include <random>
#include <chrono>
#include <iostream>
#include <numeric>
#include <algorithm>

#include "types/tuples.h"


constexpr double RadiansToDegrees = 180.0 / M_PI;
constexpr double DegreesToRadians = M_PI / 180.0;

constexpr double M_2PI = 2.0 * M_PI;
double radians(double x);
double degrees(double x);

long int bit_ceil(long int x);
long int bit_floor(long int x);

// Printing functions
template<typename T>
void print_text(const std::vector<T> & vector);

template<typename T>
void print_binary(const std::vector<T> & vector);

double timed(std::function<void(void)> func);
void print_timed(const std::string & message, std::function<void(void)> func);

// Functional shorthands
template<typename In, typename Out>
std::vector<Out> map(std::function<Out(In)> function, const std::vector<In> & in) {
    std::vector<Out> result;
    result.reserve(in.size());
    std::transform(in.begin(), in.end(), std::back_inserter(result), function);
    return result;
}

template<typename T>
T reduce(const std::vector<T> & vector, T init) {
    return std::accumulate(vector.begin(), vector.end(), init);
}

#endif // __UTILITIES__
