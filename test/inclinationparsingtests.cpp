#include "../lib/catch.hpp"
#include "../src/wallsparser.h"
#include "approx.h"

using namespace dewalls;

typedef UnitizedDouble<Angle> UAngle;

TEST_CASE( "inclination parsing tests" , "[dewalls, inclination]" ) {
    SECTION( "defaultUnit works") {
        CHECK( WallsParser("2.5").inclination(Angle::Degrees) == UAngle(2.5, Angle::Degrees) );
        CHECK( WallsParser("2.5").inclination(Angle::Gradians) == UAngle(2.5, Angle::Gradians) );
    }
    SECTION( "units suffixes work" ) {
        CHECK( WallsParser("2.5g").inclination(Angle::Degrees) == UAngle(2.5, Angle::Gradians) );
        CHECK( WallsParser("2.5m").inclination(Angle::Degrees) == UAngle(2.5, Angle::MilsNATO) );
        CHECK( WallsParser("2.5p").inclination(Angle::Degrees) == UAngle(2.5, Angle::PercentGrade) );
    }
    SECTION( "negative numbers work" ) {
        CHECK( WallsParser("-2.5").inclination(Angle::Degrees) == UAngle(-2.5, Angle::Degrees) );
    }
    SECTION( "degrees:minutes:seconds work" ) {
        CHECK( WallsParser("5:4:23").inclination(Angle::Degrees) == UAngle(5 + (4 + 23 / 60.0) / 60.0, Angle::Degrees) );
        CHECK( WallsParser("5:4").inclination(Angle::Degrees) == UAngle(5 + 4 / 60.0, Angle::Degrees) );
        CHECK( WallsParser("-5::23").inclination(Angle::Degrees) == UAngle(-5 - (23 / 60.0) / 60.0, Angle::Degrees) );
        CHECK( approxEquals(WallsParser("::23.5").inclination(Angle::Degrees), UAngle((23.5 / 60.0) / 60.0, Angle::Degrees)) );
        CHECK( WallsParser("-:4").inclination(Angle::Degrees) == UAngle(-4 / 60.0, Angle::Degrees) );
    }
    SECTION( "plus sign works" ) {
        CHECK( WallsParser("+::2.5").inclination(Angle::Degrees) == UAngle(2.5 / 3600.0, Angle::Degrees) );
    }
    SECTION( "misc invalid values throw" ) {
        CHECK_THROWS( WallsParser("").inclination(Angle::Degrees) );
        CHECK_THROWS( WallsParser("-").inclination(Angle::Degrees) );
        CHECK_THROWS( WallsParser("+").inclination(Angle::Degrees) );
        CHECK_THROWS( WallsParser("-.").inclination(Angle::Degrees) );
        CHECK_THROWS( WallsParser(".").inclination(Angle::Degrees) );
        CHECK_THROWS( WallsParser(" ").inclination(Angle::Degrees) );
    }
    SECTION( "out of range values throw" ) {
        CHECK_THROWS( WallsParser("90.0001").inclination(Angle::Degrees) );
        CHECK_THROWS( WallsParser("-90.00001").inclination(Angle::Degrees) );
        CHECK_THROWS( WallsParser("100.0001g").inclination(Angle::Degrees) );
        CHECK_THROWS( WallsParser("-100.00001g").inclination(Angle::Degrees) );
    }
}
