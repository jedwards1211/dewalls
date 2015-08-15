#ifndef APPROX_H
#define APPROX_H

namespace dewalls {

inline double approx(double val)
{
    return floor(val * 1e6) * 1e-6;
}

template<class T>
inline bool approxEquals(UnitizedDouble<T> a, UnitizedDouble<T> b)
{
    return floor(a.get(a.unit()) * 1e6) == floor(b.get(b.unit()) * 1e6);
}

}

#endif // APPROX_H

