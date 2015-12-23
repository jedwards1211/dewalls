#ifndef DEWALLS_UNITIZEDDOUBLE_H
#define DEWALLS_UNITIZEDDOUBLE_H

#include <iostream>
#include <cmath>
#include <QString>
#include "dewallsexport.h"
#include "length.h"
#include "angle.h"

namespace dewalls {

template<class T>
class UnitizedDouble
{
public:
    typedef typename T::Unit Unit;

    UnitizedDouble();
    UnitizedDouble(double quantity, Unit unit);
    UnitizedDouble(UnitizedDouble&& other);
    UnitizedDouble(const UnitizedDouble& other) = default;

    Unit unit() const;
    double get(Unit toUnit) const;
    UnitizedDouble<T> in(Unit unit) const;
    inline bool isValid() const { return _unit != T::Invalid; }
    inline void clear() { _unit = T::Invalid; }

    inline bool isZero() const { return _unit && _quantity == 0.0; }
    inline bool isNonzero() const { return _unit && _quantity != 0.0; }
    inline bool isNegative() const { return _unit && _quantity < 0.0; }
    inline bool isPositive() const { return _unit && _quantity > 0.0; }
    double signum() const;

    friend void swap(UnitizedDouble<T>& first, UnitizedDouble<T>& second)
    {
        using std::swap;

        swap(first._quantity, second._quantity);
        swap(first._unit, second._unit);
    }

    UnitizedDouble<T>& operator =(UnitizedDouble<T> other);
    UnitizedDouble<T>& operator +=(const UnitizedDouble<T>& rhs);
    UnitizedDouble<T>  operator -();
    UnitizedDouble<T>& operator -=(const UnitizedDouble<T>& rhs);
    UnitizedDouble<T>& operator *=(const double& rhs);
    UnitizedDouble<T>& operator /=(const double& rhs);
    UnitizedDouble<T>& operator %=(const UnitizedDouble<T>& rhs);

    QString toString() const;

    friend std::ostream& operator<<(std::ostream& os, const UnitizedDouble<T>& obj)
    {
        return os << obj.toString().toStdString();
    }

private:
    Unit _unit;
    double _quantity;
};

template class DEWALLS_LIB_EXPORT UnitizedDouble<Length>;
template class DEWALLS_LIB_EXPORT UnitizedDouble<Angle>;

template<class T>
inline UnitizedDouble<T>::UnitizedDouble()
    : _unit(T::Invalid)
{

}

template<class T>
inline UnitizedDouble<T>::UnitizedDouble(UnitizedDouble&& other)
    : UnitizedDouble()
{
    swap(*this, other);
}

template<class T>
inline double UnitizedDouble<T>::signum() const
{
    if (_unit) {
        return _quantity > 0.0 ? 1.0 : _quantity == 0.0 ? 0.0 : -1.0;
    }
    return NAN;
}

template<class T>
inline UnitizedDouble<T>& UnitizedDouble<T>::operator =(UnitizedDouble<T> other)
{
    swap(*this, other);
    return *this;
}

template<class T>
inline UnitizedDouble<T>& UnitizedDouble<T>::operator +=(const UnitizedDouble<T>& rhs)
{
    if (!rhs._unit) _unit = T::Invalid;
    else if (_unit) _quantity += rhs.get(_unit);
    return *this;
}

template<class T>
inline UnitizedDouble<T> UnitizedDouble<T>::operator -()
{
    return UnitizedDouble<T>(-_quantity, _unit);
}

template<class T>
inline UnitizedDouble<T>& UnitizedDouble<T>::operator -=(const UnitizedDouble<T>& rhs)
{
    if (!rhs._unit) _unit = T::Invalid;
    else if (_unit) _quantity -= rhs.get(_unit);
    return *this;
}

template<class T>
inline UnitizedDouble<T>& UnitizedDouble<T>::operator *=(const double& rhs)
{
    _quantity *= rhs;
    return *this;
}

template<class T>
inline UnitizedDouble<T>& UnitizedDouble<T>::operator /=(const double& rhs)
{
    _quantity /= rhs;
    return *this;
}

template<class T>
inline UnitizedDouble<T>& UnitizedDouble<T>::operator %=(const UnitizedDouble<T>& rhs)
{
    if (!rhs._unit) _unit = T::Invalid;
    else if (_unit) _quantity = fmod(_quantity, rhs.get(_unit));
    return *this;
}

template<class T>
inline bool operator ==(const UnitizedDouble<T>& lhs, const UnitizedDouble<T>& rhs)
{
    return lhs.unit() ? rhs.unit() && lhs.get(lhs.unit()) == rhs.get(lhs.unit())
                      : !rhs.unit();
}

template<class T>
inline bool operator !=(const UnitizedDouble<T>& lhs, const UnitizedDouble<T>& rhs)
{
    return !operator ==(lhs, rhs);
}

template<class T>
inline bool operator < (const UnitizedDouble<T>& lhs, const UnitizedDouble<T>& rhs)
{
    return lhs.unit() && lhs.get(lhs.unit()) < rhs.get(lhs.unit());
}

template<class T>
inline bool operator > (const UnitizedDouble<T>& lhs, const UnitizedDouble<T>& rhs)
{
    return operator < (rhs, lhs);
}

template<class T>
inline bool operator <=(const UnitizedDouble<T>& lhs, const UnitizedDouble<T>& rhs)
{
    return !operator > (rhs, lhs);
}

template<class T>
inline bool operator >=(const UnitizedDouble<T>& lhs, const UnitizedDouble<T>& rhs)
{
    return !operator < (rhs, lhs);
}

template<class T>
inline UnitizedDouble<T> operator +(UnitizedDouble<T> lhs, const UnitizedDouble<T>& rhs)
{
    lhs += rhs;
    return lhs;
}

template<class T>
inline UnitizedDouble<T> operator -(UnitizedDouble<T> lhs, const UnitizedDouble<T>& rhs)
{
    lhs -= rhs;
    return lhs;
}

template<class T>
inline UnitizedDouble<T> operator *(UnitizedDouble<T> lhs, const double& rhs)
{
    lhs *= rhs;
    return lhs;
}

template<class T>
inline UnitizedDouble<T> operator *(double lhs, const UnitizedDouble<T>& rhs)
{
    return rhs * lhs;
}

template<class T>
inline UnitizedDouble<T> operator /(UnitizedDouble<T> lhs, const double& rhs)
{
    lhs /= rhs;
    return lhs;
}

template<class T>
inline UnitizedDouble<T> operator /(double lhs, const UnitizedDouble<T>& rhs)
{
    return rhs.unit() ? UnitizedDouble<T>(lhs / rhs.get(rhs.unit()), rhs.unit())
                      : UnitizedDouble<T>();
}

template<class T>
inline double operator / (const UnitizedDouble<T>& lhs, const UnitizedDouble<T>& rhs)
{
    return lhs.unit() && rhs.unit() ? lhs.get(lhs.unit()) / rhs.get(lhs.unit())
                                    : NAN;
}

template<class T>
inline UnitizedDouble<T> operator %(UnitizedDouble<T> lhs, const UnitizedDouble<T>& rhs)
{
    lhs %= rhs;
    return lhs;
}

template<class T>
inline UnitizedDouble<T>::UnitizedDouble(double quantity, Unit unit):
    _unit(unit),
    _quantity(quantity)
{

}

template<class T>
inline typename T::Unit UnitizedDouble<T>::unit() const
{
    return _unit;
}

template<class T>
inline double UnitizedDouble<T>::get(Unit toUnit) const
{
    return T::convert(_quantity, _unit, toUnit);
}

template<class T>
inline UnitizedDouble<T> UnitizedDouble<T>::in(typename T::Unit unit) const
{
    return UnitizedDouble<T>(get(unit), unit);
}

template<class T>
inline QString UnitizedDouble<T>::toString() const
{
    if (!_unit) return "";
    return QString("%1 %2").arg(_quantity).arg(T::symbolFor(_unit));
}

} // namespace dewalls

#endif // DEWALLS_UNITIZEDDOUBLE_H
