#include "../lib/catch.hpp"
#include "../src/wallsparser.h"
#include "wallsvisitor.h"
#include "approx.h"

using namespace dewalls;

typedef UnitizedDouble<Length> ULength;
typedef UnitizedDouble<Angle> UAngle;

TEST_CASE( "general tests", "[dewalls]" ) {
    WallsParser parser;

    REQUIRE( parser.units()->vectorType == VectorType::CT );
    REQUIRE( parser.units()->d_unit == Length::meters() );
    REQUIRE( parser.units()->s_unit == Length::meters() );
    REQUIRE( parser.units()->a_unit == Angle::degrees() );
    REQUIRE( parser.units()->ab_unit == Angle::degrees() );
    REQUIRE( parser.units()->v_unit == Angle::degrees() );
    REQUIRE( parser.units()->vb_unit == Angle::degrees() );
    REQUIRE( parser.units()->lrud == LrudType::From );
    REQUIRE( parser.units()->prefix.isEmpty() );
    REQUIRE( parser.units()->typeab_corrected == false );
    REQUIRE( parser.units()->typevb_corrected == false );
    REQUIRE( parser.units()->rect.isZero() );
    REQUIRE( parser.units()->decl.isZero() );
    REQUIRE( parser.units()->grid.isZero() );
    REQUIRE( parser.units()->incd.isZero() );
    REQUIRE( parser.units()->incs.isZero() );
    REQUIRE( parser.units()->inca.isZero() );
    REQUIRE( parser.units()->incab.isZero() );
    REQUIRE( parser.units()->incv.isZero() );
    REQUIRE( parser.units()->incvb.isZero() );
    REQUIRE( parser.units()->inch.isZero() );
    REQUIRE( parser.units()->date.isNull() );
    REQUIRE( parser.units()->case_ == CaseType::Mixed );
    REQUIRE( parser.units()->flag.isNull() );
    REQUIRE( parser.units()->segment.isNull() );
    REQUIRE( parser.units()->uvh == 1.0 );
    REQUIRE( parser.units()->uvv == 1.0 );

    CapturingWallsVisitor visitor;

    parser.setVisitor(&visitor);

    SECTION( "vector line parsing tests" ) {
        parser.parseLine("A1 A2 2.5 350 2.3");

        REQUIRE( visitor.from == "A1" );
        REQUIRE( visitor.to == "A2" );
        REQUIRE( visitor.distance == ULength(2.5, Length::meters()) );
        REQUIRE( visitor.frontsightAzimuth == UAngle(350, Angle::degrees()) );
        REQUIRE( !visitor.backsightAzimuth.isValid() );
        REQUIRE( visitor.frontsightInclination == UAngle(2.3, Angle::degrees()) );
        REQUIRE( !visitor.backsightInclination.isValid() );

        SECTION( "backsights" ) {
            parser.parseLine("A1 A2 2.5 350/349 2.3/2.4");

            REQUIRE( visitor.from == "A1" );
            REQUIRE( visitor.to == "A2" );
            REQUIRE( visitor.distance == ULength(2.5, Length::meters()) );
            REQUIRE( visitor.frontsightAzimuth == UAngle(350, Angle::degrees()) );
            REQUIRE( visitor.backsightAzimuth == UAngle(349, Angle::degrees()) );
            REQUIRE( visitor.frontsightInclination == UAngle(2.3, Angle::degrees()) );
            REQUIRE( visitor.backsightInclination == UAngle(2.4, Angle::degrees()) );
         }

        SECTION( "frontsights/backsights can be omitted" ) {
            parser.parseLine("A1 A2 2.5 350/-- --/2.4");

            REQUIRE( visitor.frontsightAzimuth == UAngle(350, Angle::degrees()) );
            REQUIRE( !visitor.backsightAzimuth.isValid() );
            REQUIRE( !visitor.frontsightInclination.isValid() );
            REQUIRE( visitor.backsightInclination == UAngle(2.4, Angle::degrees()) );
        }

        SECTION( "distance" ) {
            SECTION( "distance can't be omitted" ) {
                CHECK_THROWS( parser.parseLine("A B -- 350 2.5") );
            }

            SECTION( "can't be negative" ) {
                CHECK_THROWS( parser.parseLine("A B -0.001 350 4") );
            }

            SECTION( "d_unit affects distance" ) {
                parser.parseLine("#units d=feet");
                parser.parseLine("A B 2.5 350 4");
                REQUIRE( visitor.distance == ULength(2.5, Length::feet()) );
            }

            SECTION( "s_unit doesn't affect distance" ) {
                parser.parseLine("#units s=feet");
                parser.parseLine("A B 2.5 350 4");
                REQUIRE( visitor.distance == ULength(2.5, Length::meters()) );
            }
        }

        SECTION( "azimuth" ) {
            SECTION( "azimuth can be omitted for vertical shots" ) {
                parser.parseLine("A B 2.5 -- 90");
                parser.parseLine("A B 2.5 -- 90/-90");
                parser.parseLine("A B 2.5 -- --/90");
                parser.parseLine("A B 2.5 -- --/-90");
                parser.parseLine("A B 2.5 -- 90/-90");
                parser.parseLine("A B 2.5 --/-- 90");
                parser.parseLine("A B 2.5 -- -90");
                parser.parseLine("A B 2.5 -- 100g");
                parser.parseLine("A B 2.5 -- -100g");

                parser.parseLine("#units typevb=c");
                parser.parseLine("A B 2.5 -- 90/90");
                CHECK_THROWS( parser.parseLine("A B 2.5 -- 90/-90") );
            }

            SECTION( "azimuth can't be omitted for non-vertical shots" ) {
                CHECK_THROWS( parser.parseLine("A B 2.5 -- 45") );
                CHECK_THROWS( parser.parseLine("A B 2.5 --/-- 45") );
            }

            SECTION( "a_unit" ) {
                parser.parseLine("#units a=grads");
                parser.parseLine("A B 1 2/3 4");
                REQUIRE( visitor.frontsightAzimuth == UAngle(2, Angle::gradians()) );
                REQUIRE( visitor.backsightAzimuth == UAngle(3, Angle::degrees()) );
            }

            SECTION( "ab_unit" ) {
                parser.parseLine("#units ab=grads");
                parser.parseLine("A B 1 2/3 4");
                REQUIRE( visitor.frontsightAzimuth == UAngle(2, Angle::degrees()) );
                REQUIRE( visitor.backsightAzimuth == UAngle(3, Angle::gradians()) );
            }

            SECTION( "a/ab unit" ) {
                parser.parseLine("#units a/ab=grads");
                parser.parseLine("A B 1 2/3 4");
                REQUIRE( visitor.frontsightAzimuth == UAngle(2, Angle::gradians()) );
                REQUIRE( visitor.backsightAzimuth == UAngle(3, Angle::gradians()) );
            }
        }

        SECTION( "inclination" ) {
            SECTION( "inclination can't be omitted" ) {
                CHECK_THROWS( parser.parseLine("A B 2.5 350 --") );
                CHECK_THROWS( parser.parseLine("A B 2.5 350 --/--") );
            }

            SECTION( "v_unit" ) {
                parser.parseLine("#units v=grads");
                parser.parseLine("A B 1 2 3/4");
                REQUIRE( visitor.frontsightInclination == UAngle(3, Angle::gradians()) );
                REQUIRE( visitor.backsightInclination == UAngle(4, Angle::degrees()) );
            }

            SECTION( "vb_unit" ) {
                parser.parseLine("#units vb=grads");
                parser.parseLine("A B 1 2 3/4");
                REQUIRE( visitor.frontsightInclination == UAngle(3, Angle::degrees()) );
                REQUIRE( visitor.backsightInclination == UAngle(4, Angle::gradians()) );
            }

            SECTION( "v/vb unit" ) {
                parser.parseLine("#units v/vb=grads");
                parser.parseLine("A B 1 2 3/4");
                REQUIRE( visitor.frontsightInclination == UAngle(3, Angle::gradians()) );
                REQUIRE( visitor.backsightInclination == UAngle(4, Angle::gradians()) );
            }
        }

        SECTION( "rect data lines" ) {
            parser.parseLine("#units rect");
            parser.parseLine("A B 1 2 3");
            REQUIRE( visitor.east == ULength(1, Length::meters()) );
            REQUIRE( visitor.north == ULength(2, Length::meters()) );
            REQUIRE( visitor.rectUp == ULength(3, Length::meters()) );

            REQUIRE_THROWS( parser.parseLine("A B 1 2") );

            SECTION( "measurements can be reordered" ) {
                parser.parseLine("#units order=nue");
                parser.parseLine("A B 1 2 3");
                REQUIRE( visitor.north == ULength(1, Length::meters()) );
                REQUIRE( visitor.rectUp == ULength(2, Length::meters()) );
                REQUIRE( visitor.east == ULength(3, Length::meters()) );
            }

            SECTION( "up can be omitted" ) {
                parser.parseLine("#units order=ne");
                parser.parseLine("A B 1 2");
                REQUIRE( visitor.north == ULength(1, Length::meters()) );
                REQUIRE( visitor.east == ULength(2, Length::meters()) );
            }
        }

        SECTION( "splay shots" ) {
            parser.parseLine("A - 2.5 350 5");
            parser.parseLine("- B 2.5 350 5");
            REQUIRE_THROWS( parser.parseLine("- - 2.5 350 5") );
        }

        SECTION( "compass/tape measurements can be reordered" ) {
            parser.parseLine("#units order=avd");
            parser.parseLine("A B 1 2 3");
            REQUIRE( visitor.frontsightAzimuth == UAngle(1, Angle::degrees()) );
            REQUIRE( visitor.frontsightInclination == UAngle(2, Angle::degrees()) );
            REQUIRE( visitor.distance == ULength(3, Length::meters()) );
        }

        SECTION( "inclination can be omitted from order" ) {
            parser.parseLine("#units order=da");
            parser.parseLine("A B 1 2");
            REQUIRE( visitor.distance == ULength(1, Length::meters()) );
            REQUIRE( visitor.frontsightAzimuth == UAngle(2, Angle::degrees()) );
            REQUIRE( !visitor.frontsightInclination.isValid() );
        }

        SECTION( "instrument and target heights" ) {
            parser.parseLine("A B 1 2 3 4 5");
            REQUIRE( visitor.instrumentHeight == ULength(4, Length::meters()) );
            REQUIRE( visitor.targetHeight == ULength(5, Length::meters()) );

            SECTION( "are affected by s_unit" ) {
                parser.parseLine("#units s=feet");
                parser.parseLine("A B 1 2 3 4 5");
                REQUIRE( visitor.instrumentHeight == ULength(4, Length::feet()) );
                REQUIRE( visitor.targetHeight == ULength(5, Length::feet()) );
            }

            SECTION( "are not affected by d_unit" ) {
                parser.parseLine("#units d=feet");
                parser.parseLine("A B 1 2 3 4 5");
                REQUIRE( visitor.instrumentHeight == ULength(4, Length::meters()) );
                REQUIRE( visitor.targetHeight == ULength(5, Length::meters()) );
            }

            SECTION( "target only" ) {
                parser.parseLine("#units tape=st");
                parser.parseLine("A B 1 2 3 4");
                REQUIRE( !visitor.instrumentHeight.isValid() );
                REQUIRE( visitor.targetHeight == ULength(4, Length::meters()) );
                REQUIRE_THROWS( parser.parseLine("A B 1 2 3 4 5") );
            }

            SECTION( "disabled" ) {
                parser.parseLine("#units tape=ss");
                REQUIRE_THROWS( parser.parseLine("A B 1 2 3 4") );
            }

            SECTION( "aren't allowed for rect data lines" ) {
                parser.parseLine("#units rect");
                REQUIRE_THROWS( parser.parseLine("A B 1 2 3 4 5") );
            }
        }

        SECTION( "variance overrides" ) {
            SECTION( "both can't be omitted" )  {
               REQUIRE_THROWS( parser.parseLine("A B 1 2 3 (,)") );
            }

            SECTION( "one can be omitted" ) {
                parser.parseLine("A B 1 2 3 (?,)");
                CHECK( visitor.horizontalVarianceOverride == VarianceOverride::FLOATED );
                CHECK( visitor.verticalVarianceOverride.isNull() );

                parser.parseLine("A B 1 2 3 (,?)");
                CHECK( visitor.horizontalVarianceOverride.isNull() );
                CHECK( visitor.verticalVarianceOverride == VarianceOverride::FLOATED );
            }

            SECTION( "various types" ) {
                parser.parseLine("A B 1 2 3 (?,*)");
                CHECK( visitor.horizontalVarianceOverride == VarianceOverride::FLOATED );
                CHECK( visitor.verticalVarianceOverride == VarianceOverride::FLOATED_TRAVERSE );

                parser.parseLine("A B 1 2 3 (1000f,r4.5f)");
                REQUIRE( visitor.horizontalVarianceOverride->type() == VarianceOverride::Type::LENGTH_OVERRIDE );
                CHECK( visitor.horizontalVarianceOverride.staticCast<LengthOverride>()->lengthOverride() == ULength(1000, Length::feet()) );
                REQUIRE( visitor.verticalVarianceOverride->type() == VarianceOverride::Type::RMS_ERROR );
                CHECK( visitor.verticalVarianceOverride.staticCast<RMSError>()->error() == ULength(4.5, Length::feet()) );
            }
        }

        SECTION( "lruds" ) {
            parser.parseLine("A B 1 2 3 *4,5,6,7*");
            REQUIRE( visitor.left == ULength(4, Length::meters()) );
            REQUIRE( visitor.right == ULength(5, Length::meters()) );
            REQUIRE( visitor.up == ULength(6, Length::meters()) );
            REQUIRE( visitor.down == ULength(7, Length::meters()) );

            parser.parseLine("A B 1 2 3 *4 5 6 7*");
            REQUIRE( visitor.left == ULength(4, Length::meters()) );
            REQUIRE( visitor.right == ULength(5, Length::meters()) );
            REQUIRE( visitor.up == ULength(6, Length::meters()) );
            REQUIRE( visitor.down == ULength(7, Length::meters()) );

            parser.parseLine("A B 1 2 3 <4 5 6 7>");
            REQUIRE( visitor.left == ULength(4, Length::meters()) );
            REQUIRE( visitor.right == ULength(5, Length::meters()) );
            REQUIRE( visitor.up == ULength(6, Length::meters()) );
            REQUIRE( visitor.down == ULength(7, Length::meters()) );

            parser.parseLine("A B 1 2 3 <4,5,6,7>");
            REQUIRE( visitor.left == ULength(4, Length::meters()) );
            REQUIRE( visitor.right == ULength(5, Length::meters()) );
            REQUIRE( visitor.up == ULength(6, Length::meters()) );
            REQUIRE( visitor.down == ULength(7, Length::meters()) );

            SECTION( "can omit lruds" ) {
                parser.parseLine("A B 1 2 3 <4,--,6,-->");
                REQUIRE( visitor.left == ULength(4, Length::meters()) );
                REQUIRE( !visitor.right.isValid() );
                REQUIRE( visitor.up == ULength(6, Length::meters()) );
                REQUIRE( !visitor.down.isValid() );
            }

            SECTION( "negative numbers not allowed" ) {
                CHECK_THROWS( parser.parseLine("A B 1 2 3 *-4,5,6,7*") );
            }

            SECTION( "can unitize individual lruds" ) {
                parser.parseLine("A B 1 2 3 *4f,5m,6i3,i7*");
                REQUIRE( visitor.left == ULength(4, Length::feet()) );
                REQUIRE( visitor.right == ULength(5, Length::meters()) );
                REQUIRE( visitor.up == ULength(6 * 12 + 3, Length::inches()) );
                REQUIRE( visitor.down == ULength(7, Length::inches()) );
            }

            SECTION( "s_unit affects lruds" ) {
                parser.parseLine("#units s=feet");
                parser.parseLine("A B 1 2 3 *4,5,6,7*");
                REQUIRE( visitor.left == ULength(4, Length::feet()) );
                REQUIRE( visitor.right == ULength(5, Length::feet()) );
                REQUIRE( visitor.up == ULength(6, Length::feet()) );
                REQUIRE( visitor.down == ULength(7, Length::feet()) );
            }

            SECTION( "d_unit doesn't affect lruds" ) {
                parser.parseLine("#units d=feet");
                parser.parseLine("A B 1 2 3 *4,5,6,7*");
                REQUIRE( visitor.left == ULength(4, Length::meters()) );
                REQUIRE( visitor.right == ULength(5, Length::meters()) );
                REQUIRE( visitor.up == ULength(6, Length::meters()) );
                REQUIRE( visitor.down == ULength(7, Length::meters()) );
            }

            SECTION( "malformed lruds" ) {
                CHECK_THROWS( parser.parseLine("A B 1 2 3 *4,5,6,7>") );
                CHECK_THROWS( parser.parseLine("A B 1 2 3 <4,5,6,7*") );
                CHECK_THROWS( parser.parseLine("A B 1 2 3 *4,5 6,7*") );
                CHECK_THROWS( parser.parseLine("A B 1 2 3 <4,5 6,7>") );
                CHECK_THROWS( parser.parseLine("A B 1 2 3 <4,5,6,>") );
                CHECK_THROWS( parser.parseLine("A B 1 2 3 <4,5,6>") );
                CHECK_THROWS( parser.parseLine("A B 1 2 3 <>") );
                CHECK_THROWS( parser.parseLine("A B 1 2 3 < >") );
                CHECK_THROWS( parser.parseLine("A B 1 2 3 <,>") );
            }

            SECTION( "can change lrud order" ) {
                parser.parseLine("#units lrud=from:urld");
                parser.parseLine("A B 1 2 3 *4,5,6,7*");
                REQUIRE( visitor.up == ULength(4, Length::meters()) );
                REQUIRE( visitor.right == ULength(5, Length::meters()) );
                REQUIRE( visitor.left == ULength(6, Length::meters()) );
                REQUIRE( visitor.down == ULength(7, Length::meters()) );
            }
        }
    }

    SECTION( "prefixes" ) {
        parser.parseLine("#units prefix=a");
        CHECK( parser.units()->processStationName("b") == "a:b" );
        CHECK( parser.units()->processStationName("d:b") == "d:b" );
        CHECK( parser.units()->processStationName(":b") == "b" );

        parser.parseLine("#units prefix2=c");
        CHECK( parser.units()->processStationName("b") == "c:a:b" );
        CHECK( parser.units()->processStationName(":b") == "c::b" );
        CHECK( parser.units()->processStationName("d:b") == "c:d:b" );
        CHECK( parser.units()->processStationName("::b") == "b" );
        CHECK( parser.units()->processStationName(":::::b") == "b" );

        parser.parseLine("#units prefix1");
        CHECK( parser.units()->processStationName("b") == "c::b" );
    }
}
