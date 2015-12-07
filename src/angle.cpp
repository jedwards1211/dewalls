#include "angle.h"
#include <cmath>

namespace dewalls {

constexpr long double PI = acosl(-1.0L);
constexpr long double DegreesToRadians = PI / 180.0L;
constexpr long double GradiansToRadians = PI / 200.0L;
constexpr long double MilsNATOToRadians = PI / 3200.0L;
constexpr long double RadiansToDegrees = 180.0L / PI;
constexpr long double RadiansToGradians = 200.0L / PI;
constexpr long double RadiansToMilsNATO = 3200.0L / PI;

QString Angle::Name("angle");

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

