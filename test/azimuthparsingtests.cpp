#include "../lib/catch.hpp"
#include "../src/wallsparser.h"

using namespace dewalls;

typedef UnitizedDouble<Angle> UAngle;

double approx(double val)
{
    return floor(val * 1e6) * 1e-6;
}

template<class T>
bool approxEquals(UnitizedDouble<T> a, UnitizedDouble<T> b)
{
    return floor(a.get(a.unit()) * 1e6) == floor(b.get(b.unit()) * 1e6);
}

TEST_CASE( "azimuth parsing tests" , "[dewalls, azimuth]" ) {
    SECTION( "defaultUnit works") {
        CHECK( WallsParser("2.5").azimuth(Angle::deg()) == UAngle(2.5, Angle::deg()) );
        CHECK( WallsParser("2.5").azimuth(Angle::grad()) == UAngle(2.5, Angle::grad()) );
    }
    SECTION( "units suffixes work" ) {
        CHECK( WallsParser("2.5g").azimuth(Angle::deg()) == UAngle(2.5, Angle::grad()) );
        CHECK( WallsParser("2.5m").azimuth(Angle::deg()) == UAngle(2.5, Angle::mil()) );
    }
    SECTION( "percentGrade doesn't work" ) {
        WallsParser parser("2.5p");
        REQUIRE( parser.azimuth(Angle::deg()) == UAngle(2.5, Angle::deg()) );
        REQUIRE_THROWS( parser.endOfLine() );
    }
    SECTION( "negative numbers work for azimuthOffset" ) {
        CHECK( WallsParser("-2.5").azimuthOffset(Angle::deg()) == UAngle(-2.5, Angle::deg()) );
    }
    SECTION( "negative numbers throw for azimuth") {
        CHECK_THROWS( WallsParser("-2.5").azimuth(Angle::deg()));
    }
    SECTION( "degrees:minutes:seconds work" ) {
        CHECK( WallsParser("5:4:23").azimuth(Angle::deg()) == UAngle(5 + (4 + 23 / 60.0) / 60.0, Angle::deg()) );
        CHECK( WallsParser("5:4").azimuth(Angle::deg()) == UAngle(5 + 4 / 60.0, Angle::deg()) );
        CHECK( WallsParser("5::23").azimuth(Angle::deg()) == UAngle(5 + (23 / 60.0) / 60.0, Angle::deg()) );
        CHECK( approxEquals(WallsParser("::23.5").azimuth(Angle::deg()), UAngle((23.5 / 60.0) / 60.0, Angle::deg())) );
        CHECK( WallsParser(":4").azimuth(Angle::deg()) == UAngle(4 / 60.0, Angle::deg()) );
    }
    SECTION( "basic quadrants work" ) {
        CHECK( WallsParser("N30E").azimuth(Angle::grad()) == UAngle(30, Angle::deg()) );
        CHECK( WallsParser("N30W").azimuth(Angle::deg()) == UAngle(330, Angle::deg()) );
        CHECK( WallsParser("E30N").azimuth(Angle::deg()) == UAngle(60, Angle::deg()) );
        CHECK( WallsParser("E30S").azimuth(Angle::deg()) == UAngle(120, Angle::deg()) );
        CHECK( WallsParser("S30E").azimuth(Angle::deg()) == UAngle(150, Angle::deg()) );
        CHECK( WallsParser("S30W").azimuth(Angle::deg()) == UAngle(210, Angle::deg()) );
        CHECK( WallsParser("W30S").azimuth(Angle::deg()) == UAngle(240, Angle::deg()) );
        CHECK( WallsParser("W30N").azimuth(Angle::deg()) == UAngle(300, Angle::deg()) );
    }
    SECTION( "advanced quadrants work" ) {
        CHECK( WallsParser("N30.5E").azimuth(Angle::deg()) == UAngle(30.5, Angle::deg()) );
        CHECK( WallsParser("N30:30E").azimuth(Angle::deg()) == UAngle(30.5, Angle::deg()) );
        CHECK( WallsParser("N30:30:30E").azimuth(Angle::deg()) == UAngle(30.5 + 30 / 3600.0, Angle::deg()) );
        CHECK( WallsParser("N30::30E").azimuth(Angle::deg()) == UAngle(30 + 30 / 3600.0, Angle::deg()) );
        CHECK( approxEquals(WallsParser("N::30.5E").azimuth(Angle::deg()), UAngle(30.5 / 3600.0, Angle::deg())) );
        CHECK( WallsParser("N:30E").azimuth(Angle::deg()) == UAngle(.5, Angle::deg()) );
        CHECK( WallsParser("N30gE").azimuth(Angle::deg()) == UAngle(30, Angle::grad()) );
    }
    SECTION( "misc invalid values throw" ) {
        CHECK_THROWS( WallsParser("").azimuth(Angle::deg()) );
        CHECK_THROWS( WallsParser("-").azimuth(Angle::deg()) );
        CHECK_THROWS( WallsParser("-.").azimuth(Angle::deg()) );
        CHECK_THROWS( WallsParser(".").azimuth(Angle::deg()) );
        CHECK_THROWS( WallsParser("+2").azimuth(Angle::deg()) );
        CHECK_THROWS( WallsParser(" ").azimuth(Angle::deg()) );
    }
}
