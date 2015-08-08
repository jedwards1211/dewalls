#include "../lib/catch.hpp"
#include "../src/wallsparser.h"

using namespace dewalls;

typedef UnitizedDouble<Length> ULength;

TEST_CASE( "length parsing tests" , "[dewalls, length]" ) {
    SECTION( "defaultUnit works") {
        CHECK( WallsParser("2.5").length(Length::m()) == ULength(2.5, Length::m()) );
        CHECK( WallsParser("2.5").length(Length::ft()) == ULength(2.5, Length::ft()) );
    }
    SECTION( "Units suffixes work" ) {
        CHECK( WallsParser("2.5f").length(Length::m()) == ULength(2.5, Length::ft()) );
        CHECK( WallsParser("i2.5").length(Length::m()) == ULength(2.5, Length::in()) );
        CHECK( WallsParser("2.5i4").length(Length::m()) == ULength(4, Length::in()) + ULength(2.5, Length::ft()) );
    }
    SECTION( "negative numbers work" ) {
        CHECK( WallsParser("-2.5").length(Length::m()) == ULength(-2.5, Length::m()) );
    }
    SECTION( "negative numbers throw for unsignedLength") {
        CHECK_THROWS( WallsParser("-2.5").unsignedLength(Length::m()));
    }
    SECTION( "misc invalid values throw" ) {
        CHECK_THROWS( WallsParser("").length(Length::m()) );
        CHECK_THROWS( WallsParser("-").length(Length::m()) );
        CHECK_THROWS( WallsParser("-.").length(Length::m()) );
        CHECK_THROWS( WallsParser(".").length(Length::m()) );
        CHECK_THROWS( WallsParser("+2").length(Length::m()) );
        CHECK_THROWS( WallsParser(" ").length(Length::m()) );
    }
}
