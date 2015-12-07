#include "length.h"
#include "defaultunit.h"
#include <QGlobalStatic>

#include <iostream>

namespace dewalls {

QString Length::Name("length");

constexpr long double FeetToMeters = 0.3048L;
constexpr long double YardsToMeters = 3.0L * FeetToMeters;
constexpr long double InchesToMeters = FeetToMeters / 12.0L;

constexpr long double MetersToFeet = 1.0L / FeetToMeters;
constexpr long double MetersToYards = 1.0L / YardsToMeters;
constexpr long double MetersToInches = 1.0L / InchesToMeters;

QString Length::symbolFor(Unit unit) {
    switch (unit) {
    case Meters:
        return "m";
    case Centimeters:
        return "cm";
    case Kilometers:
        return "km";
    case Feet:
        return "ft";
    case Yards:
        return "yd";
    case Inches:
        return "in";
    default:
        return "<invalid>";
    }
}

long double Length::toBase(long double quantity, Unit fromUnit) {
    switch (fromUnit) {
    case Meters:
        return quantity;
    case Centimeters:
        return quantity * 0.01L;
    case Kilometers:
        return quantity * 1000.0L;
    case Feet:
        return quantity * FeetToMeters;
    case Yards:
        return quantity * YardsToMeters;
    case Inches:
        return quantity * InchesToMeters;
    default:
        return NAN;
    }
}

long double Length::fromBase(long double quantity, Unit toUnit) {
    switch (toUnit) {
    case Meters:
        return quantity;
    case Centimeters:
        return quantity * 100.0L;
    case Kilometers:
        return quantity * 0.001L;
    case Feet:
        return quantity * MetersToFeet;
    case Yards:
        return quantity * MetersToYards;
    case Inches:
        return quantity * MetersToInches;
    default:
        return NAN;
    }
}

long double Length::convert(long double quantity, Unit fromUnit, Unit toUnit) {
    return fromBase(toBase(quantity, fromUnit), toUnit);
}

} // namespace dewalls

