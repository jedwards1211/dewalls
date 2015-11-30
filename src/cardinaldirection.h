#ifndef DEWALLS_CARDINALDIRECTION_H
#define DEWALLS_CARDINALDIRECTION_H

#include "angle.h"
#include "unitizeddouble.h"
#include "dewallsexport.h"

namespace dewalls {

class DEWALLS_LIB_EXPORT CardinalDirection {
public:
    typedef UnitizedDouble<Angle> UAngle;

    inline CardinalDirection() : _ordinal(0) {}

    inline QString name() const { return names[_ordinal]; }
    inline int ordinal() const { return _ordinal; }
    inline UAngle angle() const { return angles[_ordinal]; }
    inline CardinalDirection opposite() const { return CardinalDirection((_ordinal + 2) % 4); }
    UAngle quadrant(CardinalDirection to, UAngle angle) const;

    static const CardinalDirection North;
    static const CardinalDirection East;
    static const CardinalDirection South;
    static const CardinalDirection West;

    inline bool operator ==(const CardinalDirection& rhs) const {
        return _ordinal == rhs._ordinal;
    }

    inline CardinalDirection& operator =(CardinalDirection rhs) {
        this->_ordinal = rhs._ordinal;
        return *this;
    }

private:
    inline CardinalDirection(int ordinal) : _ordinal(ordinal) {}

    UAngle nonnorm_quadrant(CardinalDirection to, UAngle angle) const;

    static const QString names[4];
    static const UAngle angles[4];

    int _ordinal;
};

} // namespace dewalls

#endif // DEWALLS_CARDINALDIRECTION_H
