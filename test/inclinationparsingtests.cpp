#include "../lib/catch.hpp"
#include "../src/wallsparser.h"
#include "approx.h"

using namespace dewalls;

typedef UnitizedDouble<Angle> UAngle;

TEST_CASE( "inclination parsing tests" , "[dewalls, inclination]" ) {
    SECTION( "defaultUnit works") {
        CHECK( WallsParser("2.5").inclination(Angle::deg()) == UAngle(2.5, Angle::deg()) );
        CHECK( WallsParser("2.5").inclination(Angle::grad()) == UAngle(2.5, Angle::grad()) );
    }
    SECTION( "units suffixes work" ) {
        CHECK( WallsParser("2.5g").inclination(Angle::deg()) == UAngle(2.5, Angle::grad()) );
        CHECK( WallsParser("2.5m").inclination(Angle::deg()) == UAngle(2.5, Angle::mil()) );
        CHECK( WallsParser("2.5p").inclination(Angle::deg()) == UAngle(2.5, Angle::percent()) );
    }
    SECTION( "negative numbers work" ) {
        CHECK( WallsParser("-2.5").inclination(Angle::deg()) == UAngle(-2.5, Angle::deg()) );
    }
    SECTION( "degrees:minutes:seconds work" ) {
        CHECK( WallsParser("5:4:23").inclination(Angle::deg()) == UAngle(5 + (4 + 23 / 60.0) / 60.0, Angle::deg()) );
        CHECK( WallsParser("5:4").inclination(Angle::deg()) == UAngle(5 + 4 / 60.0, Angle::deg()) );
        CHECK( WallsParser("-5::23").inclination(Angle::deg()) == UAngle(-5 - (23 / 60.0) / 60.0, Angle::deg()) );
        CHECK( approxEquals(WallsParser("::23.5").inclination(Angle::deg()), UAngle((23.5 / 60.0) / 60.0, Angle::deg())) );
        CHECK( WallsParser("-:4").inclination(Angle::deg()) == UAngle(-4 / 60.0, Angle::deg()) );
    }
    SECTION( "plus sign works" ) {
        CHECK( WallsParser("+::2.5").inclination(Angle::deg()) == UAngle(2.5 / 3600.0, Angle::deg()) );
    }
    SECTION( "misc invalid values throw" ) {
        CHECK_THROWS( WallsParser("").inclination(Angle::deg()) );
        CHECK_THROWS( WallsParser("-").inclination(Angle::deg()) );
        CHECK_THROWS( WallsParser("+").inclination(Angle::deg()) );
        CHECK_THROWS( WallsParser("-.").inclination(Angle::deg()) );
        CHECK_THROWS( WallsParser(".").inclination(Angle::deg()) );
        CHECK_THROWS( WallsParser(" ").inclination(Angle::deg()) );
    }
    SECTION( "out of range values throw" ) {
        CHECK_THROWS( WallsParser("90.0001").inclination(Angle::deg()) );
        CHECK_THROWS( WallsParser("-90.00001").inclination(Angle::deg()) );
        CHECK_THROWS( WallsParser("100.0001g").inclination(Angle::deg()) );
        CHECK_THROWS( WallsParser("-100.00001g").inclination(Angle::deg()) );
    }
}
