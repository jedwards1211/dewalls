#include "../lib/catch.hpp"
#include "../src/wallsparser.h"
#include "approx.h"

using namespace dewalls;

typedef UnitizedDouble<Angle> UAngle;

TEST_CASE( "azimuth parsing tests" , "[dewalls, azimuth]" ) {
    SECTION( "defaultUnit works") {
        CHECK( WallsParser("2.5").azimuth(Angle::Degrees) == UAngle(2.5, Angle::Degrees) );
        CHECK( WallsParser("2.5").azimuth(Angle::Gradians) == UAngle(2.5, Angle::Gradians) );
    }
    SECTION( "units suffixes work" ) {
        CHECK( WallsParser("2.5g").azimuth(Angle::Degrees) == UAngle(2.5, Angle::Gradians) );
        CHECK( WallsParser("2.5m").azimuth(Angle::Degrees) == UAngle(2.5, Angle::MilsNATO) );
    }
    SECTION( "percentGrade doesn't work" ) {
        WallsParser parser("2.5p");
        REQUIRE( parser.azimuth(Angle::Degrees) == UAngle(2.5, Angle::Degrees) );
        REQUIRE_THROWS( parser.endOfLine() );
    }
    SECTION( "negative numbers work for azimuthOffset" ) {
        CHECK( WallsParser("-2.5").azimuthOffset(Angle::Degrees) == UAngle(-2.5, Angle::Degrees) );
    }
    SECTION( "negative numbers throw for azimuth") {
        CHECK_THROWS( WallsParser("-2.5").azimuth(Angle::Degrees));
    }
    SECTION( "degrees:minutes:seconds work" ) {
        CHECK( WallsParser("5:4:23").azimuth(Angle::Degrees) == UAngle(5 + (4 + 23 / 60.0) / 60.0, Angle::Degrees) );
        CHECK( WallsParser("5:4").azimuth(Angle::Degrees) == UAngle(5 + 4 / 60.0, Angle::Degrees) );
        CHECK( WallsParser("5::23").azimuth(Angle::Degrees) == UAngle(5 + (23 / 60.0) / 60.0, Angle::Degrees) );
        CHECK( approxEquals(WallsParser("::23.5").azimuth(Angle::Degrees), UAngle((23.5 / 60.0) / 60.0, Angle::Degrees)) );
        CHECK( WallsParser(":4").azimuth(Angle::Degrees) == UAngle(4 / 60.0, Angle::Degrees) );
    }
    SECTION( "basic quadrants work" ) {
        CHECK( WallsParser("N30E").azimuth(Angle::Gradians) == UAngle(30, Angle::Degrees) );
        CHECK( WallsParser("N30W").azimuth(Angle::Degrees) == UAngle(330, Angle::Degrees) );
        CHECK( WallsParser("E30N").azimuth(Angle::Degrees) == UAngle(60, Angle::Degrees) );
        CHECK( WallsParser("E30S").azimuth(Angle::Degrees) == UAngle(120, Angle::Degrees) );
        CHECK( WallsParser("S30E").azimuth(Angle::Degrees) == UAngle(150, Angle::Degrees) );
        CHECK( WallsParser("S30W").azimuth(Angle::Degrees) == UAngle(210, Angle::Degrees) );
        CHECK( WallsParser("W30S").azimuth(Angle::Degrees) == UAngle(240, Angle::Degrees) );
        CHECK( WallsParser("W30N").azimuth(Angle::Degrees) == UAngle(300, Angle::Degrees) );
    }
    SECTION( "advanced quadrants work" ) {
        CHECK( WallsParser("N30.5E").azimuth(Angle::Degrees) == UAngle(30.5, Angle::Degrees) );
        CHECK( WallsParser("N30:30E").azimuth(Angle::Degrees) == UAngle(30.5, Angle::Degrees) );
        CHECK( WallsParser("N30:30:30E").azimuth(Angle::Degrees) == UAngle(30.5 + 30 / 3600.0, Angle::Degrees) );
        CHECK( WallsParser("N30::30E").azimuth(Angle::Degrees) == UAngle(30 + 30 / 3600.0, Angle::Degrees) );
        CHECK( approxEquals(WallsParser("N::30.5E").azimuth(Angle::Degrees), UAngle(30.5 / 3600.0, Angle::Degrees)) );
        CHECK( WallsParser("N:30E").azimuth(Angle::Degrees) == UAngle(.5, Angle::Degrees) );
        CHECK( WallsParser("N30gE").azimuth(Angle::Degrees) == UAngle(30, Angle::Gradians) );
    }
    SECTION( "misc invalid values throw" ) {
        CHECK_THROWS( WallsParser("").azimuth(Angle::Degrees) );
        CHECK_THROWS( WallsParser("-").azimuth(Angle::Degrees) );
        CHECK_THROWS( WallsParser("-.").azimuth(Angle::Degrees) );
        CHECK_THROWS( WallsParser(".").azimuth(Angle::Degrees) );
        CHECK_THROWS( WallsParser("+2").azimuth(Angle::Degrees) );
        CHECK_THROWS( WallsParser(" ").azimuth(Angle::Degrees) );
    }
    SECTION( "out of range values throw" ) {
        CHECK_THROWS( WallsParser("360").azimuth(Angle::Degrees) );
        CHECK_THROWS( WallsParser("-0.00001").azimuth(Angle::Degrees) );
        CHECK_THROWS( WallsParser("400g").azimuth(Angle::Degrees) );
        CHECK_THROWS( WallsParser("-0.00001g").azimuth(Angle::Degrees) );
        CHECK_THROWS( WallsParser("N90E").azimuth(Angle::Degrees) );
        CHECK_THROWS( WallsParser("N100gE").azimuth(Angle::Degrees) );
    }
}
