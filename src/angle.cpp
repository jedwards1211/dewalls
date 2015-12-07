#include "angle.h"
#include <cmath>

namespace dewalls {

const long double PI = acosl(-1.0L);
const long double DegreesToRadians = PI / 180.0L;
const long double GradiansToRadians = PI / 200.0L;
const long double MilsNATOToRadians = PI / 3200.0L;
const long double RadiansToDegrees = 180.0L / PI;
const long double RadiansToGradians = 200.0L / PI;
const long double RadiansToMilsNATO = 3200.0L / PI;

QString Angle::Name("angle");

QString Angle::symbolFor(Unit unit) {
    switch (unit) {
    case Radians:
        return "rad";
    case Degrees:
        return "deg";
    case Gradians:
        return "grad";
    case MilsNATO:
        return "mil";
    case PercentGrade:
        return "%";
    default:
        return "<invalid>";
    }
}

long double Angle::toBase(long double quantity, Unit fromUnit) {
    switch (fromUnit) {
    case Radians:
        return quantity;
    case Degrees:
        return quantity * DegreesToRadians;
    case Gradians:
        return quantity * GradiansToRadians;
    case MilsNATO:
        return quantity * MilsNATOToRadians;
    case PercentGrade:
        return atan(quantity * 0.01L);
    default:
        return NAN;
    }
}

long double Angle::fromBase(long double quantity, Unit toUnit) {
    switch (toUnit) {
    case Radians:
        return quantity;
    case Degrees:
        return quantity * RadiansToDegrees;
    case Gradians:
        return quantity * RadiansToGradians;
    case MilsNATO:
        return quantity * RadiansToMilsNATO;
    case PercentGrade:
        return 100.0L * tan(quantity);
    default:
        return NAN;
    }
}

long double Angle::convert(long double quantity, Unit fromUnit, Unit toUnit) {
    return fromBase(toBase(quantity, fromUnit), toUnit);
}

} // namespace dewalls

