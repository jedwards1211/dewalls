#include "../lib/catch.hpp"
#include "../src/wallsparser.h"

using namespace dewalls;

typedef UnitizedDouble<Length> ULength;

TEST_CASE( "length parsing tests" , "[dewalls, length]" ) {
    SECTION( "defaultUnit works") {
        CHECK( WallsParser("2.5").length(Length::Meters) == ULength(2.5, Length::Meters) );
        CHECK( WallsParser("2.5").length(Length::Feet) == ULength(2.5, Length::Feet) );
    }
    SECTION( "Units suffixes work" ) {
        CHECK( WallsParser("2.5f").length(Length::Meters) == ULength(2.5, Length::Feet) );
        CHECK( WallsParser("i2.5").length(Length::Meters) == ULength(2.5, Length::Inches) );
        CHECK( WallsParser("2.5i4").length(Length::Meters) == ULength(4, Length::Inches) + ULength(2.5, Length::Feet) );
    }
    SECTION( "negative numbers work" ) {
        CHECK( WallsParser("-2.5").length(Length::Meters) == ULength(-2.5, Length::Meters) );
    }
    SECTION( "negative numbers throw for unsignedLength") {
        CHECK_THROWS( WallsParser("-2.5").unsignedLength(Length::Meters));
    }
    SECTION( "misc invalid values throw" ) {
        CHECK_THROWS( WallsParser("").length(Length::Meters) );
        CHECK_THROWS( WallsParser("-").length(Length::Meters) );
        CHECK_THROWS( WallsParser("-.").length(Length::Meters) );
        CHECK_THROWS( WallsParser(".").length(Length::Meters) );
        CHECK_THROWS( WallsParser("+2").length(Length::Meters) );
        CHECK_THROWS( WallsParser(" ").length(Length::Meters) );
    }
}
