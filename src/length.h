#ifndef DEWALLS_LENGTH_H
#define DEWALLS_LENGTH_H

#include <QString>

namespace dewalls {

class Length
{
public:
    enum Unit {
        Invalid = 0,
        Meters = 1,
        Centimeters = 2,
        Kilometers = 3,
        Feet = 4,
        Yards = 5,
        Inches = 6
    };

    static QString Name;

    static long double toBase(long double quantity, Unit fromUnit);
    static long double fromBase(long double quantity, Unit toUnit);
    static long double convert(long double quantity, Unit fromUnit, Unit toUnit);
    static QString symbolFor(Unit unit);
};

} // namespace dewalls

#endif // DEWALLS_LENGTH_H
