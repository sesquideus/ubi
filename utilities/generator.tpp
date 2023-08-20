Generator::Generator(void):
    rd_(),
    sd_({rd_(), rd_(), rd_(), rd_()}),
    mt_(sd_)
{}


TwoD GeneratorSpherical::random_uniform_spherical(void) const {
    std::uniform_real_distribution<double> ud(-1, 1);

    double x = 1, y = 1, z = 1;
    while (x*x + y*y + z*z >= 1) {
        x = ud(this->mt_);
        y = ud(this->mt_);
        z = ud(this->mt_);
    }
    double dec = asin(z / sqrt(x*x + y*y + z*z));
    double ra = atan2(y, x);
    return {dec, ra};
}

TwoD GeneratorSpherical::random_cosz_spherical(void) const {
    std::uniform_real_distribution<double> ud(-1, 1);

    double x = 1, y = 1, z = 1;
    while (x*x + y*y + z*z >= 1) {
        z = ud(this->mt_);
        y = ud(this->mt_);
        x = ud(this->mt_) * 0.5;
    }
    double dec = asin(z / sqrt(x*x + y*y + z*z));
    double ra = atan2(y, x);
    return {dec, ra};
}

TwoD GeneratorSpherical::random_cube(void) const {
    std::uniform_real_distribution<double> ud(-1, 1);

    double x = ud(this->mt_);
    double y = ud(this->mt_);
    double z = ud(this->mt_);
    double dec = asin(z / sqrt(x*x + y*y + z*z));
    double ra = atan2(y, x);
    return {dec, ra};
}

TwoD GeneratorSpherical::random_gaussian_centered(void) const {
    std::normal_distribution<double> nd(0, 1);

    double dec = nd(this->mt_) * radians(20);
    double ra = nd(this->mt_) * radians(40);
    return {dec, ra};
}

template<typename Real>
Real Generator1D<Real>::random_uniform(Real min, Real max) const {
    std::uniform_real_distribution<Real> ud(min, max);
    return ud(mt_);
}

Meteor GeneratorMeteor::random_uniform(void) const {
    std::uniform_real_distribution<double> ud(-1, 1);
    std::uniform_real_distribution<double> ud1(0, 1);
    std::exponential_distribution<double> ed(10);

    double x = 1, y = 1, z = 1;
    while (x*x + y*y + z*z >= 1) {
        x = ud(this->mt_);
        y = ud(this->mt_);
        z = ud(this->mt_);
    }
    double dec = asin(z / sqrt(x*x + y*y + z*z));
    double ra = atan2(y, x);

    double v = ud1(this->mt_) * 50000 + 10000;
    double m = ed(this->mt_);
    double t = ud1(this->mt_) * 360.0;
    return Meteor(dec, ra, v, m, t);
}
