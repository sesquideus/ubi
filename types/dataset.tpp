#include "dataset.h"

template<typename T>
Dataset<T>::Dataset(const std::vector<T> & data):
    data_(data)
{}

template<>
std::string Dataset<TwoD>::header(void) {
    return "x\ty";
}

template<typename T>
T Dataset<T>::from_line(const std::string & line) {
    std::stringstream stream(line);
    std::string token;
    std::vector<std::string> tokens;

    while (getline(stream, token, '\t')) {
        tokens.push_back(token);
    }
    return Dataset<T>::from_tokens(tokens);
}

template<>
Meteor Dataset<Meteor>::from_tokens(const std::vector<std::string> & tokens) {
    double dec = radians(std::stod(tokens[3]));
    double ra = radians(std::stod(tokens[2]));
    double vx = std::stod(tokens[4]);
    double vy = std::stod(tokens[5]);
    double vz = std::stod(tokens[6]);
    double mag = std::stod(tokens[7]);
    double sunlon = radians(std::stod(tokens[8]));
    if (mag > 7) {
        throw std::underflow_error("Mass too small, ignoring");
    }
    return Meteor(dec, ra, std::sqrt(vx*vx + vy*vy + vz*vz), mag, sunlon);
}

template<>
TwoD Dataset<TwoD>::from_tokens(const std::vector<std::string> & tokens) {
    return TwoD(std::stod(tokens[0]), std::stod(tokens[1]));
}

template<typename T>
void Dataset<T>::load(std::vector<T> && data) {
    this->data_ = std::move(data);
}

template<typename T>
void Dataset<T>::load_tsv(const std::string & filename, const unsigned int max) {
    std::string line;
    std::ifstream file(filename, std::ios::in);

    this->data_.clear();

    if (file.is_open()) {
        getline(file, line);
        for (unsigned int i = 0; i < max; ++i) {
            try {
                if (getline(file, line)) {
                    this->data_.push_back(from_line(line));
                } else {
                    break;
                }
            } catch (std::invalid_argument & e) {
                std::cerr << "Parsing error on line " << i << ": " << e.what() << std::endl;
            } catch (std::overflow_error & e) {
                std::cerr << "Overflow error on line " << i << ": " << e.what() << std::endl;
            } catch (std::underflow_error & e) {
                std::cerr << "Underflow error on line " << i << ": " << e.what() << std::endl;
            }
        }
    } else {
        std::cerr << "Could not open file " << filename << std::endl;
        throw std::runtime_error("Could not open file");
    }
    std::cout << this->data_.size() << " data points loaded" << std::endl;
}

template<typename T>
void Dataset<T>::save_tsv(const std::string & filename) const {
    std::cout << "Saving the dataset to file \"" << filename << "\"" << std::endl;
    std::ofstream file(filename);

    if (file.is_open()) {
        file << Dataset<T>::header() << std::endl;
        for (auto && item: this->data_) {
            file << item << std::endl;
        }
    } else {
        throw std::runtime_error("Could not open file");
    }
}

template<typename T>
void Dataset<T>::kde(
    const MetricFunctor<T> & metric,
    Mesh<T, VPImplicitTree> & mesh,
    const BandwidthSelector<T> & selector,
    const Kernel & kernel
) {
    KDE<T, VPImplicitTree> kde(metric, this->data_);
    kde.evaluate_pointwise(mesh, kernel, selector);
}


template<typename T>
void Dataset<T>::kde_corr(
    const MetricFunctor<T> & metric,
    Mesh<T, VPImplicitTree> & mesh,
    const BandwidthMatrixSelector<T> & selector,
    const Kernel & kernel
) {
    KDE<T, VPImplicitTree> kde(metric, this->data_);
    kde.evaluate_pointwise_corr(mesh, kernel, selector);
}
