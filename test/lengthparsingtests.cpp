#include "catch.hpp"
#include "../src/wallssurveyparser.h"

using namespace dewalls;

typedef UnitizedDouble<Length> ULength;

TEST_CASE( "length parsing tests" , "[dewalls, length]" ) {
    SECTION( "defaultUnit works") {
        CHECK( WallsSurveyParser("2.5").length(Length::Meters) == ULength(2.5, Length::Meters) );
        CHECK( WallsSurveyParser("2.5").length(Length::Feet) == ULength(2.5, Length::Feet) );
    }
    SECTION( "Units suffixes work" ) {
        CHECK( WallsSurveyParser("2.5f").length(Length::Meters) == ULength(2.5, Length::Feet) );
        CHECK( WallsSurveyParser("i2.5").length(Length::Meters) == ULength(2.5, Length::Inches) );
        CHECK( WallsSurveyParser("2.5i4").length(Length::Meters) == ULength(4, Length::Inches) + ULength(2.5, Length::Feet) );
    }
    SECTION( "negative numbers work" ) {
        CHECK( WallsSurveyParser("-2.5").length(Length::Meters) == ULength(-2.5, Length::Meters) );
    }
    SECTION( "negative numbers throw for unsignedLength") {
        CHECK_THROWS( WallsSurveyParser("-2.5").unsignedLength(Length::Meters));
    }
    SECTION( "misc invalid values throw" ) {
        CHECK_THROWS( WallsSurveyParser("").length(Length::Meters) );
        CHECK_THROWS( WallsSurveyParser("-").length(Length::Meters) );
        CHECK_THROWS( WallsSurveyParser("-.").length(Length::Meters) );
        CHECK_THROWS( WallsSurveyParser(".").length(Length::Meters) );
        CHECK_THROWS( WallsSurveyParser("+2").length(Length::Meters) );
        CHECK_THROWS( WallsSurveyParser(" ").length(Length::Meters) );
    }
}
