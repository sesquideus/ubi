#ifndef __METEOR__
#define __METEOR__

#include <cmath>
#include <string>
#include <iostream>
#include <iomanip>
#include <random>
#include <cstdio>
#include <cassert>

#include "types/real.h"
#include "utilities/utilities.h"
#include "metric/metric.h"


class Meteor {
private:
    real dec_, ra_, v_;
    real mag_;
    real lsun_;

public:
    Meteor(real dec, real ra, real v, real mag, real lsun);
    Meteor(const Meteor & other);
    static Meteor from_degrees(real dec_deg, real ra_deg, real v, real mag, real lsun_deg);
    static Meteor random(void);

    inline real spherical_distance(const Meteor & other) const;

    inline real dec(void) const { return this->dec_; }
    inline real dec_deg(void) const { return degrees(this->dec_); }
    inline real ra(void) const { return this->ra_; }
    inline real ra_deg(void) const { return degrees(this->ra_); }
    inline real v(void) const { return this->v_; }
    inline real mag(void) const { return this->mag_; }
    inline real lsun(void) const { return this->lsun_; }
    inline real lsun_deg(void) const { return degrees(this->lsun_); }

    inline real operator[](unsigned int i) const {
        switch (i) {
            case 0: return this->dec_;
            case 1: return this->ra_;
            case 2: return this->v_;
            case 3: return this->mag_;
            case 4: return this->lsun_;
            default: return 0;
        }
    }

    friend Meteor operator-(const Meteor & first, const Meteor & second);
};

std::ostream & operator<<(std::ostream & os, const Meteor & m);

Meteor operator-(const Meteor & first, const Meteor & second);

class MeteorPositionMetric: public MetricFunctor<const Meteor &> {
public:
    inline real operator()(const Meteor & x, const Meteor & y) const override {
        return spherical_distance_hav(x.dec(), x.ra(), y.dec(), y.ra());
    }
};


class MeteorMetric: public MetricFunctor<Meteor> {
private:
    real ca_;
    real cv_;
    real cm_;
    real cl_;

public:
    MeteorMetric(const real ca, const real cv, const real cm, const real cl):
        ca_(ca * ca),
        cv_(cv * cv),
        cm_(cm * cm),
        cl_(cl * cl)
    {}

    inline real operator()(Meteor x, Meteor y) const override {
        return std::sqrt(
            this->ca_ * std::pow(::spherical_distance_hav(x.dec(), x.ra(), y.dec(), y.ra()), 2) +
            this->cv_ * std::pow(x.v() - y.v(), 2) +
            this->cm_ * std::pow(x.mag() - y.mag(), 2) +
            this->cl_ * std::pow(std::sin((x.lsun() - y.lsun()) * 0.5), 2)
        );
    }
};

#include "meteor.tpp"

#endif // __METEOR__
