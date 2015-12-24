#include "../lib/catch.hpp"

#include "vector.h"
#include "wallstypes.h"
#include "unitizedmath.h"

#include <iostream>

using namespace dewalls;

typedef UnitizedDouble<Length> ULength;
typedef UnitizedDouble<Angle> UAngle;

void testInstance(ULength instY, ULength targetY, ULength fromY, ULength toY, ULength horizDist, ULength inch, QList<TapingMethodMeasurement> tape) {
    WallsUnits units;
    units.setInch(inch);
    units.setIncd(ULength(1, Length::Inches));
    units.setIncs(ULength(1, Length::Inches));
    units.setIncv(UAngle(2, Angle::Degrees));
    units.setTape(tape);

    std::cout << "===============================" << std::endl;
    std::cout << "  instY: " << instY << std::endl;
    std::cout << "  targetY: " << targetY << std::endl;
    std::cout << "  fromY: " << fromY << std::endl;
    std::cout << "  toY: " << toY << std::endl;
    std::cout << "  horizDist: " << horizDist << std::endl;
    std::cout << "  inch: " << inch << std::endl;

    ULength tapeFromY = tape[0] == TapingMethodMeasurement::InstrumentHeight ? instY : fromY;
    ULength tapeToY   = tape[1] == TapingMethodMeasurement::TargetHeight     ? targetY : toY;

    Vector vector;
    vector.setDistance(usqrt(usq(horizDist) + usq(tapeToY - tapeFromY)) - units.incd());
    vector.setFrontAzimuth(UAngle(0, Angle::Degrees) - units.inca());
    vector.setFrontInclination(uatan((targetY - instY) / horizDist) - units.incv());
    vector.setInstHeight(instY - fromY - units.incs());
    vector.setTargetHeight(targetY - toY - units.incs());
    vector.setUnits(units);

    ULength expectedDist = usqrt(usq(toY + inch - fromY) + usq(horizDist));
    UAngle  expectedInc  = uatan((toY + inch - fromY) / horizDist);

    ULength instHeightAboveTape = instY - tapeFromY;
    ULength targetHeightAboveTape = targetY - tapeToY;
    if (uabs(instHeightAboveTape - targetHeightAboveTape) > vector.distance()) {
        CHECK_THROWS( vector.applyHeightCorrections() );
    }
    else {
        vector.applyHeightCorrections();
        CHECK( (vector.distance() + units.incd()).get(Length::Meters) == Approx( expectedDist.get(Length::Meters) ) );
        CHECK( (vector.frontInclination() + units.incv()).get(Angle::Degrees) == Approx( expectedInc.get(Angle::Degrees) ) );
    }
}

QList<TapingMethodMeasurement> IT({TapingMethodMeasurement::InstrumentHeight, TapingMethodMeasurement::TargetHeight});
QList<TapingMethodMeasurement> IS({TapingMethodMeasurement::InstrumentHeight, TapingMethodMeasurement::Station});
QList<TapingMethodMeasurement> ST({TapingMethodMeasurement::Station, TapingMethodMeasurement::TargetHeight});
QList<TapingMethodMeasurement> SS({TapingMethodMeasurement::Station, TapingMethodMeasurement::Station});

void testInstance(ULength instY, ULength targetY, ULength fromY, ULength toY, ULength horizDist, ULength inch) {
    testInstance(instY, targetY, fromY, toY, horizDist, inch, IT);
    testInstance(instY, targetY, fromY, toY, horizDist, inch, IS);
    testInstance(instY, targetY, fromY, toY, horizDist, inch, ST);
    testInstance(instY, targetY, fromY, toY, horizDist, inch, SS);
}

TEST_CASE( "applyHeightCorrections", "[dewalls]" ) {
    testInstance(ULength(3, Length::Meters), ULength(8, Length::Meters), ULength(2, Length::Meters), ULength(4, Length::Meters), ULength(7, Length::Meters), ULength(2, Length::Meters));
    testInstance(ULength(3, Length::Meters), ULength(8, Length::Meters), ULength(2, Length::Meters), ULength(9, Length::Meters), ULength(7, Length::Meters), ULength(2, Length::Meters));
    testInstance(ULength(3, Length::Meters), ULength(8, Length::Meters), ULength(4, Length::Meters), ULength(7, Length::Meters), ULength(7, Length::Meters), ULength(2, Length::Meters));
    testInstance(ULength(3, Length::Meters), ULength(8, Length::Meters), ULength(4, Length::Meters), ULength(68, Length::Meters), ULength(7, Length::Meters), ULength(2, Length::Meters));
    testInstance(ULength(3, Length::Meters), ULength(8, Length::Meters), ULength(0, Length::Meters), ULength(0, Length::Meters), ULength(7, Length::Meters), ULength(2, Length::Meters));
    testInstance(ULength(3, Length::Meters), ULength(8, Length::Meters), ULength(68, Length::Meters), ULength(68, Length::Meters), ULength(7, Length::Meters), ULength(2, Length::Meters));
    testInstance(ULength(3, Length::Meters), ULength(58, Length::Meters), ULength(2, Length::Meters), ULength(-62, Length::Meters), ULength(7, Length::Meters), ULength(2, Length::Meters));
}
