std::random_device rd;
std::seed_seq sd{rd(), rd(), rd(), rd()};
std::mt19937 mt(sd);
std::uniform_real_distribution<real> ud(0, 1);
std::exponential_distribution<real> ed(10);

Meteor::Meteor(real dec, real ra, real v, real mag, real lsun):
    dec_(dec),
    ra_(ra),
    v_(v),
    mag_(mag),
    lsun_(lsun)
{}

Meteor Meteor::from_degrees(real dec_deg, real ra_deg, real v, real mass, real lsun_deg) {
    assert(mass > 0);
    return Meteor(radians(dec_deg), radians(ra_deg), v, std::log(mass), radians(lsun_deg));
}

Meteor::Meteor(const Meteor & other):
    dec_(other.dec_),
    ra_(other.ra_),
    v_(other.v_),
    mag_(other.mag_),
    lsun_(other.lsun_)
{}

real Meteor::spherical_distance(const Meteor & other) const {
    return ::spherical_distance_hav(this->dec(), this->ra(), other.dec(), other.ra());
}

std::ostream & operator<<(std::ostream & os, const Meteor & m) {
    return os << "Meteor at " << m.dec_deg() << "° " << m.ra_deg() << "°, " << m.v() << " m/s, ln m " << m.mag() << " t " << m.lsun_deg();
}

Meteor Meteor::random(void) {
    real dec = ud(mt) * 180.0 - 90.0;
    real ra = ud(mt) * 360.0;
    real v = ud(mt) * 60000 + 10000;
    real m = ed(mt);
    real t = ud(mt) * 360.0;
    return Meteor::from_degrees(dec, ra, v, m, t);
}

Meteor operator-(const Meteor & first, const Meteor & second) {
    real dlat;
    real dlon;
    ::spherical_difference(first.dec_, first.ra_, second.dec_, second.ra_, dlat, dlon);

    return Meteor(
        dlat,
        dlon,
        first.v_ - second.v_,
        first.mag_ - second.mag_,
        ::angular_difference(first.lsun_, second.lsun_)
    );
}
