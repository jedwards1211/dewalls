#include "cardinaldirection.h"

namespace dewalls {

typedef UnitizedDouble<Angle> UAngle;

const CardinalDirection CardinalDirection::North(0);
const CardinalDirection CardinalDirection::East(1);
const CardinalDirection CardinalDirection::South(2);
const CardinalDirection CardinalDirection::West(3);

const QString CardinalDirection::names[4] = {"North", "East", "South", "West"};
const UAngle CardinalDirection::angles[4] = {
    UAngle(0, Angle::deg()),
    UAngle(90, Angle::deg()),
    UAngle(180, Angle::deg()),
    UAngle(270, Angle::deg())
};

UAngle CardinalDirection::nonnorm_quadrant(CardinalDirection to, UAngle rotation) const
{
    if (to._ordinal == (_ordinal + 1) % 4)
    {
        return angle() + rotation;
    }
    else if (_ordinal == (to._ordinal + 1) % 4)
    {
        return angle() - rotation;
    }
    throw std::invalid_argument("invalid from/to combination");
}

UAngle CardinalDirection::quadrant(CardinalDirection to, UAngle rotation) const
{
    UAngle result = nonnorm_quadrant(to, rotation) % UAngle(360.0, Angle::deg());
    if (result < North.angle()) {
        result += UAngle(360.0, Angle::deg());
    }
    return result;
}

} // namespace dewalls

