#ifndef __BINARY__
#define __BINARY__

template<typename T>
class Binary {
    inline operator()(const T & what) const;
}

Binary<TwoD<double>>::operator()(const TwoD<double> & what) const {
    
}

#endif // __BINARY__
