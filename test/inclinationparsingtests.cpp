#include "../lib/catch.hpp"
#include "../src/wallssurveyparser.h"
#include "approx.h"

using namespace dewalls;

typedef UnitizedDouble<Angle> UAngle;

TEST_CASE( "inclination parsing tests" , "[dewalls, inclination]" ) {
    SECTION( "defaultUnit works") {
        CHECK( WallsSurveyParser("2.5").inclination(Angle::Degrees) == UAngle(2.5, Angle::Degrees) );
        CHECK( WallsSurveyParser("2.5").inclination(Angle::Gradians) == UAngle(2.5, Angle::Gradians) );
    }
    SECTION( "units suffixes work" ) {
        CHECK( WallsSurveyParser("2.5g").inclination(Angle::Degrees) == UAngle(2.5, Angle::Gradians) );
        CHECK( WallsSurveyParser("2.5m").inclination(Angle::Degrees) == UAngle(2.5, Angle::MilsNATO) );
        CHECK( WallsSurveyParser("2.5p").inclination(Angle::Degrees) == UAngle(2.5, Angle::PercentGrade) );
    }
    SECTION( "negative numbers work" ) {
        CHECK( WallsSurveyParser("-2.5").inclination(Angle::Degrees) == UAngle(-2.5, Angle::Degrees) );
    }
    SECTION( "degrees:minutes:seconds work" ) {
        CHECK( WallsSurveyParser("5:4:23").inclination(Angle::Degrees) == UAngle(5 + (4 + 23 / 60.0) / 60.0, Angle::Degrees) );
        CHECK( WallsSurveyParser("5:4").inclination(Angle::Degrees) == UAngle(5 + 4 / 60.0, Angle::Degrees) );
        CHECK( WallsSurveyParser("-5::23").inclination(Angle::Degrees) == UAngle(-5 - (23 / 60.0) / 60.0, Angle::Degrees) );
        CHECK( approxEquals(WallsSurveyParser("::23.5").inclination(Angle::Degrees), UAngle((23.5 / 60.0) / 60.0, Angle::Degrees)) );
        CHECK( WallsSurveyParser("-:4").inclination(Angle::Degrees) == UAngle(-4 / 60.0, Angle::Degrees) );
    }
    SECTION( "plus sign works" ) {
        CHECK( WallsSurveyParser("+::2.5").inclination(Angle::Degrees) == UAngle(2.5 / 3600.0, Angle::Degrees) );
    }
    SECTION( "misc invalid values throw" ) {
        CHECK_THROWS( WallsSurveyParser("").inclination(Angle::Degrees) );
        CHECK_THROWS( WallsSurveyParser("-").inclination(Angle::Degrees) );
        CHECK_THROWS( WallsSurveyParser("+").inclination(Angle::Degrees) );
        CHECK_THROWS( WallsSurveyParser("-.").inclination(Angle::Degrees) );
        CHECK_THROWS( WallsSurveyParser(".").inclination(Angle::Degrees) );
        CHECK_THROWS( WallsSurveyParser(" ").inclination(Angle::Degrees) );
    }
    SECTION( "out of range values throw" ) {
        CHECK_THROWS( WallsSurveyParser("90.0001").inclination(Angle::Degrees) );
        CHECK_THROWS( WallsSurveyParser("-90.00001").inclination(Angle::Degrees) );
        CHECK_THROWS( WallsSurveyParser("100.0001g").inclination(Angle::Degrees) );
        CHECK_THROWS( WallsSurveyParser("-100.00001g").inclination(Angle::Degrees) );
    }
}
