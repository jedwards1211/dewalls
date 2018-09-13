#include "catch.hpp"
#include "../src/wallssurveyparser.h"
#include "approx.h"
#include "unitizedmath.h"
#include "segmentparseexception.h"

#include <iostream>

using namespace dewalls;

typedef UnitizedDouble<Length> ULength;
typedef UnitizedDouble<Angle> UAngle;

TEST_CASE( "general tests", "[dewalls]" ) {
    WallsSurveyParser parser;

    REQUIRE( parser.units().vectorType() == VectorType::CT );
    REQUIRE( parser.units().dUnit() == Length::Meters );
    REQUIRE( parser.units().sUnit() == Length::Meters );
    REQUIRE( parser.units().aUnit() == Angle::Degrees );
    REQUIRE( parser.units().abUnit() == Angle::Degrees );
    REQUIRE( parser.units().vUnit() == Angle::Degrees );
    REQUIRE( parser.units().vbUnit() == Angle::Degrees );
    REQUIRE( parser.units().lrud() == LrudType::From );
    REQUIRE( parser.units().prefix().isEmpty() );
    REQUIRE( parser.units().typeabCorrected() == false );
    REQUIRE( parser.units().typevbTolerance() == UAngle(2.0, Angle::Degrees) );
    REQUIRE( parser.units().typevbCorrected() == false );
    REQUIRE( parser.units().typevbTolerance() == UAngle(2.0, Angle::Degrees) );
    REQUIRE( parser.units().rect().isZero() );
    REQUIRE( parser.units().decl().isZero() );
    REQUIRE( parser.units().grid().isZero() );
    REQUIRE( parser.units().incd().isZero() );
    REQUIRE( parser.units().incs().isZero() );
    REQUIRE( parser.units().inca().isZero() );
    REQUIRE( parser.units().incab().isZero() );
    REQUIRE( parser.units().incv().isZero() );
    REQUIRE( parser.units().incvb().isZero() );
    REQUIRE( parser.units().inch().isZero() );
    REQUIRE( parser.units().tape().size() == 2);
    REQUIRE( parser.units().tape()[0] == TapingMethodMeasurement::InstrumentHeight );
    REQUIRE( parser.units().tape()[1] == TapingMethodMeasurement::TargetHeight );
    REQUIRE( parser.date().isNull() );
    REQUIRE( parser.units().case_() == CaseType::Mixed );
    REQUIRE( parser.units().flag().isNull() );
    REQUIRE( parser.segment().isEmpty() );
    REQUIRE( parser.units().uvh() == 1.0 );
    REQUIRE( parser.units().uvv() == 1.0 );

    Vector vector;
    FixStation station;
    QList<WallsMessage> messages;
    QString comment;

    QObject::connect(&parser, &WallsSurveyParser::parsedVector, [&](Vector v) { vector = v; });
    QObject::connect(&parser, &WallsSurveyParser::parsedFixStation, [&](FixStation s) { station = s; });
    QObject::connect(&parser, &WallsSurveyParser::message, [&](WallsMessage m) { messages << m; });
    QObject::connect(&parser, &WallsSurveyParser::parsedComment, [&](QString c) { comment = c; });

    SECTION( "vector line parsing tests" ) {
        parser.parseLine("A1 A2 2.5 350 2.3");

        REQUIRE( vector.from() == "A1" );
        REQUIRE( vector.to() == "A2" );
        REQUIRE( vector.distance() == ULength(2.5, Length::Meters) );
        REQUIRE( vector.frontAzimuth() == UAngle(350, Angle::Degrees) );
        REQUIRE( !vector.backAzimuth().isValid() );
        REQUIRE( vector.frontInclination() == UAngle(2.3, Angle::Degrees) );
        REQUIRE( !vector.backInclination().isValid() );

        SECTION( "station prefixes" ) {
            parser.parseLine(":A1 A2 2.5 350 2.3");
            REQUIRE( vector.from() == ":A1" );

            parser.parseLine("::A1 A2 2.5 350 2.3");
            REQUIRE( vector.from() == "::A1" );

            parser.parseLine(":::A1 A2 2.5 350 2.3");
            REQUIRE( vector.from() == ":::A1" );

            parser.parseLine("q:::A1 A2 2.5 350 2.3");
            REQUIRE( vector.from() == "q:::A1" );

            parser.parseLine(":q::A1 A2 2.5 350 2.3");
            REQUIRE( vector.from() == ":q::A1" );

            parser.parseLine("::q:A1 A2 2.5 350 2.3");
            REQUIRE( vector.from() == "::q:A1" );

            parser.parseLine(":q:q:A1 A2 2.5 350 2.3");
            REQUIRE( vector.from() == ":q:q:A1" );

            parser.parseLine("q::q:A1 A2 2.5 350 2.3");
            REQUIRE( vector.from() == "q::q:A1" );

            parser.parseLine("q:q:q:A1 A2 2.5 350 2.3");
            REQUIRE( vector.from() == "q:q:q:A1" );

            REQUIRE_THROWS( parser.parseLine("::::A1 A2 2.5 350 2.3") );
        }

        SECTION( "backsights" ) {
            parser.parseLine("A1 A2 2.5 350/349 2.3/2.4");

            REQUIRE( vector.from() == "A1" );
            REQUIRE( vector.to() == "A2" );
            REQUIRE( vector.distance() == ULength(2.5, Length::Meters) );
            REQUIRE( vector.frontAzimuth() == UAngle(350, Angle::Degrees) );
            REQUIRE( vector.backAzimuth() == UAngle(349, Angle::Degrees) );
            REQUIRE( vector.frontInclination() == UAngle(2.3, Angle::Degrees) );
            REQUIRE( vector.backInclination() == UAngle(2.4, Angle::Degrees) );
         }

        SECTION( "frontsights/backsights can be omitted" ) {
            parser.parseLine("A1 A2 2.5 350/-- --/2.4");

            REQUIRE( vector.frontAzimuth() == UAngle(350, Angle::Degrees) );
            REQUIRE( !vector.backAzimuth().isValid() );
            REQUIRE( !vector.frontInclination().isValid() );
            REQUIRE( vector.backInclination() == UAngle(2.4, Angle::Degrees) );
        }

        SECTION( "distance" ) {
            SECTION( "distance can't be omitted" ) {
                CHECK_THROWS( parser.parseLine("A B -- 350 2.5") );
            }

            SECTION( "can't be negative" ) {
                CHECK_THROWS( parser.parseLine("A B -0.001 350 4") );
            }

            SECTION( "dUnit affects distance" ) {
                parser.parseLine("#units d=feet");
                parser.parseLine("A B 2.5 350 4");
                REQUIRE( vector.distance() == ULength(2.5, Length::Feet) );
            }

            SECTION( "sUnit doesn't affect distance" ) {
                parser.parseLine("#units s=feet");
                parser.parseLine("A B 2.5 350 4");
                REQUIRE( vector.distance() == ULength(2.5, Length::Meters) );
            }
        }

        SECTION( "incd" ) {
            parser.parseLine("#units incd=-2");
            SECTION( "error when incd changes measurement sign" ) {
                // zero-length shots should not be affected
                parser.parseLine("A B 0 350 4");
                // shots long enough should be OK
                parser.parseLine("A B 2.1 350 4");
                CHECK_THROWS( parser.parseLine("A B 1 350 4") );
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
            }

            SECTION( "frontsight azimuth can be omitted without dashes" ) {
                parser.parseLine("A B 2.5 /235 0");
            }

            SECTION( "azimuth can't be omitted for non-vertical shots" ) {
                CHECK_THROWS( parser.parseLine("A B 2.5 - 45") );
                CHECK_THROWS( parser.parseLine("A B 2.5 -/-- 45") );
            }

            SECTION( "aUnit" ) {
                parser.parseLine("#units a=grads");
                parser.parseLine("A B 1 2/3 4");
                REQUIRE( vector.frontAzimuth() == UAngle(2, Angle::Gradians) );
                REQUIRE( vector.backAzimuth() == UAngle(3, Angle::Degrees) );
            }

            SECTION( "abUnit" ) {
                parser.parseLine("#units ab=grads");
                parser.parseLine("A B 1 2/3 4");
                CHECK( vector.frontAzimuth() == UAngle(2, Angle::Degrees) );
                CHECK( vector.backAzimuth() == UAngle(3, Angle::Gradians) );
            }

            SECTION( "a/ab unit" ) {
                parser.parseLine("#units a/ab=grads");
                parser.parseLine("A B 1 2/3 4");
                CHECK( vector.frontAzimuth() == UAngle(2, Angle::Gradians) );
                CHECK( vector.backAzimuth() == UAngle(3, Angle::Gradians) );
            }

            SECTION( "parser warns if fs/bs difference exceeds tolerance" ) {
                messages.clear();
                parser.parseLine("A B 1 1/179 4");
                CHECK( messages.size() == 0 );

                messages.clear();
                parser.parseLine("A B 1 1/183 4");
                CHECK( messages.size() == 0 );

                messages.clear();
                parser.parseLine("A B 1 1/184 4");
                REQUIRE( messages.size() == 1 );
                CHECK( messages[0].message().contains("exceeds") );

                messages.clear();
                parser.parseLine("#units typeab=c");
                parser.parseLine("A B 1 1/3 4");
                CHECK( messages.size() == 0 );

                messages.clear();
                parser.parseLine("A B 1 1/359 4");
                CHECK( messages.size() == 0 );

                messages.clear();
                parser.parseLine("A B 1 359/1 4");
                CHECK( messages.size() == 0 );

                messages.clear();
                parser.parseLine("#units typeab=c,5");
                parser.parseLine("A B 1 1/6 4");
                CHECK( messages.size() == 0 );

                messages.clear();
                parser.parseLine("A B 1 1/7 4");
                CHECK( messages.size() == 1 );
            }
        }

        SECTION( "inclination" ) {
            SECTION( "vUnit" ) {
                parser.parseLine("#units v=grads");
                parser.parseLine("A B 1 2 3/4");
                CHECK( vector.frontInclination() == UAngle(3, Angle::Gradians) );
                CHECK( vector.backInclination() == UAngle(4, Angle::Degrees) );
            }

            SECTION( "backsight inclination can be omitted without dashes" ) {
                parser.parseLine("A B 1 2 /3");
            }

            SECTION( "vbUnit" ) {
                parser.parseLine("#units vb=grads");
                parser.parseLine("A B 1 2 3/4");
                CHECK( vector.frontInclination() == UAngle(3, Angle::Degrees) );
                CHECK( vector.backInclination() == UAngle(4, Angle::Gradians) );
            }

            SECTION( "v/vb unit" ) {
                parser.parseLine("#units v/vb=grads");
                parser.parseLine("A B 1 2 3/4");
                CHECK( vector.frontInclination() == UAngle(3, Angle::Gradians) );
                CHECK( vector.backInclination() == UAngle(4, Angle::Gradians) );
            }

            SECTION( "parser warns if fs/bs difference exceeds tolerance" ) {
                messages.clear();
                parser.parseLine("A B 1 2 4/-6");
                CHECK( messages.size() == 0 );

                messages.clear();
                parser.parseLine("A B 1 2 4/-2");
                CHECK( messages.size() == 0 );

                messages.clear();
                parser.parseLine("A B 1 2 4/-7");
                REQUIRE( messages.size() == 1 );
                REQUIRE( messages[0].message().contains("exceeds") );

                messages.clear();
                parser.parseLine("#units typevb=c");
                parser.parseLine("A B 1 2 1/3");
                CHECK( messages.size() == 0 );

                messages.clear();
                parser.parseLine("A B 1 2 1/-1");
                CHECK( messages.size() == 0 );

                messages.clear();
                parser.parseLine("#units typevb=c,5");
                parser.parseLine("A B 1 2 1/6");
                CHECK( messages.size() == 0 );

                messages.clear();
                parser.parseLine("A B 1 2 1/7");
                CHECK( messages.size() == 1 );
            }
        }

        SECTION( "rect data lines" ) {
            parser.parseLine("#units rect");
            parser.parseLine("A B 1 2 3");
            REQUIRE( vector.east() == ULength(1, Length::Meters) );
            REQUIRE( vector.north() == ULength(2, Length::Meters) );
            REQUIRE( vector.rectUp() == ULength(3, Length::Meters) );

            REQUIRE_THROWS( parser.parseLine("A B 1 2") );

            SECTION( "measurements can be reordered" ) {
                parser.parseLine("#units order=nue");
                parser.parseLine("A B 1 2 3");
                REQUIRE( vector.north() == ULength(1, Length::Meters) );
                REQUIRE( vector.rectUp() == ULength(2, Length::Meters) );
                REQUIRE( vector.east() == ULength(3, Length::Meters) );
            }

            SECTION( "up can be omitted" ) {
                parser.parseLine("#units order=ne");
                parser.parseLine("A B 1 2");
                REQUIRE( vector.north() == ULength(1, Length::Meters) );
                REQUIRE( vector.east() == ULength(2, Length::Meters) );
            }

            SECTION( "LRUD-only lines can be parsed" ) {
                parser.parseLine("A *1 2 3 4*");
            }

            SECTION( "LRUD/to station name ambiguity" ) {
                parser.parseLine("A <1 2 3 4");
                CHECK( vector.to() == "<1" );
                CHECK( vector.east() == ULength(2, Length::Meters) );
                CHECK( vector.north() == ULength(3, Length::Meters) );
                CHECK( vector.rectUp() == ULength(4, Length::Meters) );
                CHECK( vector.left() == ULength() );
                CHECK( vector.right() == ULength() );
                CHECK( vector.up() == ULength() );
                CHECK( vector.down() == ULength() );

                parser.parseLine("A *1 2 3 4 *5,6,7,8*");
                CHECK( vector.to() == "*1" );
                CHECK( vector.east() == ULength(2, Length::Meters) );
                CHECK( vector.north() == ULength(3, Length::Meters) );
                CHECK( vector.rectUp() == ULength(4, Length::Meters) );
                CHECK( vector.left() == ULength(5, Length::Meters) );
                CHECK( vector.right() == ULength(6, Length::Meters) );
                CHECK( vector.up() == ULength(7, Length::Meters) );
                CHECK( vector.down() == ULength(8, Length::Meters) );
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
            REQUIRE( vector.frontAzimuth() == UAngle(1, Angle::Degrees) );
            REQUIRE( vector.frontInclination() == UAngle(2, Angle::Degrees) );
            REQUIRE( vector.distance() == ULength(3, Length::Meters) );
        }

        SECTION( "inclination can be omitted from order" ) {
            parser.parseLine("#units order=da");
            parser.parseLine("A B 1 2");
            REQUIRE( vector.distance() == ULength(1, Length::Meters) );
            REQUIRE( vector.frontAzimuth() == UAngle(2, Angle::Degrees) );
            REQUIRE( !vector.frontInclination().isValid() );
        }

        SECTION( "instrument and target heights" ) {
            parser.parseLine("A B 1 2 3 4 5");
            REQUIRE( vector.instHeight() == ULength(4, Length::Meters) );
            REQUIRE( vector.targetHeight() == ULength(5, Length::Meters) );

            SECTION( "are affected by sUnit" ) {
                parser.parseLine("#units s=feet");
                parser.parseLine("A B 1 2 3 4 5");
                REQUIRE( vector.instHeight() == ULength(4, Length::Feet) );
                REQUIRE( vector.targetHeight() == ULength(5, Length::Feet) );
            }

            SECTION( "are not affected by dUnit" ) {
                parser.parseLine("#units d=feet");
                parser.parseLine("A B 1 2 3 4 5");
                REQUIRE( vector.instHeight() == ULength(4, Length::Meters) );
                REQUIRE( vector.targetHeight() == ULength(5, Length::Meters) );
            }

            SECTION( "with inclination omitted" ) {
                parser.parseLine("A B 1 2 -- 4 5");
                REQUIRE( vector.instHeight() == ULength(4, Length::Meters) );
                REQUIRE( vector.targetHeight() == ULength(5, Length::Meters) );
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
                CHECK( vector.horizVariance() == VarianceOverride::FLOATED );
                CHECK( vector.vertVariance().isNull() );

                parser.parseLine("A B 1 2 3 (,?)");
                CHECK( vector.horizVariance().isNull() );
                CHECK( vector.vertVariance() == VarianceOverride::FLOATED );
            }

            SECTION( "various types" ) {
                parser.parseLine("A B 1 2 3 (?,*)");
                CHECK( vector.horizVariance() == VarianceOverride::FLOATED );
                CHECK( vector.vertVariance() == VarianceOverride::FLOATED_TRAVERSE );

                parser.parseLine("A B 1 2 3 (1000f,r4.5f)");
                REQUIRE( vector.horizVariance()->type() == VarianceOverride::Type::LENGTH_OVERRIDE );
                CHECK( vector.horizVariance().staticCast<LengthOverride>()->lengthOverride() == ULength(1000, Length::Feet) );
                REQUIRE( vector.vertVariance()->type() == VarianceOverride::Type::RMS_ERROR );
                CHECK( vector.vertVariance().staticCast<RMSError>()->error() == ULength(4.5, Length::Feet) );
            }
        }

        SECTION( "lruds" ) {
            parser.parseLine("A B 1 2 3 *4,5,6,7*");
            CHECK( vector.left() == ULength(4, Length::Meters) );
            CHECK( vector.right() == ULength(5, Length::Meters) );
            CHECK( vector.up() == ULength(6, Length::Meters) );
            CHECK( vector.down() == ULength(7, Length::Meters) );

            parser.parseLine("A B 1 2 3 *4 5 6 7*");
            CHECK( vector.left() == ULength(4, Length::Meters) );
            CHECK( vector.right() == ULength(5, Length::Meters) );
            CHECK( vector.up() == ULength(6, Length::Meters) );
            CHECK( vector.down() == ULength(7, Length::Meters) );

            parser.parseLine("A B 1 2 3 <4 5 6 7>");
            CHECK( vector.left() == ULength(4, Length::Meters) );
            CHECK( vector.right() == ULength(5, Length::Meters) );
            CHECK( vector.up() == ULength(6, Length::Meters) );
            CHECK( vector.down() == ULength(7, Length::Meters) );

            parser.parseLine("A B 1 2 3 <4,5,6,7>");
            CHECK( vector.left() == ULength(4, Length::Meters) );
            CHECK( vector.right() == ULength(5, Length::Meters) );
            CHECK( vector.up() == ULength(6, Length::Meters) );
            CHECK( vector.down() == ULength(7, Length::Meters) );

            SECTION( "can omit lruds" ) {
                parser.parseLine("A B 1 2 3 <4,-,6,-->");
                CHECK( vector.left() == ULength(4, Length::Meters) );
                CHECK( !vector.right().isValid() );
                CHECK( vector.up() == ULength(6, Length::Meters) );
                CHECK( !vector.down().isValid() );
            }

            SECTION( "negative numbers allowed" ) {
                messages.clear();
                parser.parseLine("A B 1 2 3 *-4,5,-6f,7*");
                CHECK( vector.left() == ULength(-4, Length::Meters) );
                CHECK( vector.right() == ULength(5, Length::Meters) );
                CHECK( vector.up() == ULength(-6, Length::Feet) );
                CHECK( vector.down() == ULength(7, Length::Meters) );
                CHECK( messages.size() == 2 );
            }

            SECTION( "can unitize individual lruds" ) {
                parser.parseLine("A B 1 2 3 *4f,5m,6i3,i7*");
                CHECK( vector.left() == ULength(4, Length::Feet) );
                CHECK( vector.right() == ULength(5, Length::Meters) );
                CHECK( vector.up() == ULength(6 * 12 + 3, Length::Inches) );
                CHECK( vector.down() == ULength(7, Length::Inches) );
            }

            SECTION( "sUnit affects lruds" ) {
                parser.parseLine("#units s=feet");
                parser.parseLine("A B 1 2 3 *4,5,6,7*");
                CHECK( vector.left() == ULength(4, Length::Feet) );
                CHECK( vector.right() == ULength(5, Length::Feet) );
                CHECK( vector.up() == ULength(6, Length::Feet) );
                CHECK( vector.down() == ULength(7, Length::Feet) );
            }

            SECTION( "dUnit doesn't affect lruds" ) {
                parser.parseLine("#units d=feet");
                parser.parseLine("A B 1 2 3 *4,5,6,7*");
                CHECK( vector.left() == ULength(4, Length::Meters) );
                CHECK( vector.right() == ULength(5, Length::Meters) );
                CHECK( vector.up() == ULength(6, Length::Meters) );
                CHECK( vector.down() == ULength(7, Length::Meters) );
            }

            SECTION( "malformed lruds" ) {
                CHECK_THROWS( parser.parseLine("A B 1 2 3 *4,5,6,7>") );
                CHECK_THROWS( parser.parseLine("A B 1 2 3 <4,5,6,7*") );
            }

            SECTION( "malformed lruds that Walls accepts in contradition of its documentation" ) {
                parser.parseLine("A B 1 2 3 *4,5 6,7*");
                parser.parseLine("A B 1 2 3 <4,5 6,7>");

                messages.clear();
                parser.parseLine("A B 1 2 3 <4,5,6,>");
                CHECK( !messages.isEmpty() );

                messages.clear();
                parser.parseLine("A B 1 2 3 <4,5,6>");
                CHECK( !messages.isEmpty() );

                messages.clear();
                parser.parseLine("A B 1 2 3 <1>");
                CHECK( !messages.isEmpty() );

                messages.clear();
                parser.parseLine("A B 1 2 3 <>");
                CHECK( !messages.isEmpty() );

                messages.clear();
                parser.parseLine("A B 1 2 3 < >");
                CHECK( !messages.isEmpty() );

                messages.clear();
                parser.parseLine("A B 1 2 3 <,>");
                CHECK( !messages.isEmpty() );
            }

            SECTION( "can change lrud order" ) {
                parser.parseLine("#units lrud=from:urld");
                parser.parseLine("A B 1 2 3 *4,5,6,7*");
                REQUIRE( vector.up() == ULength(4, Length::Meters) );
                REQUIRE( vector.right() == ULength(5, Length::Meters) );
                REQUIRE( vector.left() == ULength(6, Length::Meters) );
                REQUIRE( vector.down() == ULength(7, Length::Meters) );
            }
        }

        SECTION( "lrud/station name ambiguity" ) {
            parser.parseLine("A *1 2 3 4");
            CHECK( vector.to() == "*1" );
            CHECK( vector.distance() == ULength(2, Length::Meters) );
            CHECK( vector.frontAzimuth() == UAngle(3, Angle::Degrees) );
            CHECK( vector.frontInclination() == UAngle(4, Angle::Degrees) );
            CHECK( vector.left() == ULength() );
            CHECK( vector.right() == ULength() );
            CHECK( vector.up() == ULength() );
            CHECK( vector.down() == ULength() );

            parser.parseLine("A <1 2 3 4");
            CHECK( vector.to() == "<1" );
            CHECK( vector.distance() == ULength(2, Length::Meters) );
            CHECK( vector.frontAzimuth() == UAngle(3, Angle::Degrees) );
            CHECK( vector.frontInclination() == UAngle(4, Angle::Degrees) );
            CHECK( vector.left() == ULength() );
            CHECK( vector.right() == ULength() );
            CHECK( vector.up() == ULength() );
            CHECK( vector.down() == ULength() );

            parser.parseLine("A <1 2 3 4 (?, ?)");
            CHECK( vector.to() == "<1" );
            CHECK( vector.distance() == ULength(2, Length::Meters) );
            CHECK( vector.frontAzimuth() == UAngle(3, Angle::Degrees) );
            CHECK( vector.frontInclination() == UAngle(4, Angle::Degrees) );
            CHECK( vector.left() == ULength() );
            CHECK( vector.right() == ULength() );
            CHECK( vector.up() == ULength() );
            CHECK( vector.down() == ULength() );

            parser.parseLine("A *1 2 3 4 *5,6,7,8*");
            CHECK( vector.to() == "*1" );
            CHECK( vector.distance() == ULength(2, Length::Meters) );
            CHECK( vector.frontAzimuth() == UAngle(3, Angle::Degrees) );
            CHECK( vector.frontInclination() == UAngle(4, Angle::Degrees) );
            CHECK( vector.left() == ULength(5, Length::Meters) );
            CHECK( vector.right() == ULength(6, Length::Meters) );
            CHECK( vector.up() == ULength(7, Length::Meters) );
            CHECK( vector.down() == ULength(8, Length::Meters) );

            parser.parseLine("A *1 2 3 4 4.5 *5,6,7,8*");
            CHECK( vector.to() == "*1" );
            CHECK( vector.distance() == ULength(2, Length::Meters) );
            CHECK( vector.frontAzimuth() == UAngle(3, Angle::Degrees) );
            CHECK( vector.frontInclination() == UAngle(4, Angle::Degrees) );
            CHECK( vector.instHeight() == ULength(4.5, Length::Meters) );
            CHECK( vector.left() == ULength(5, Length::Meters) );
            CHECK( vector.right() == ULength(6, Length::Meters) );
            CHECK( vector.up() == ULength(7, Length::Meters) );
            CHECK( vector.down() == ULength(8, Length::Meters) );

            parser.parseLine("A *1 2 3 4 4.5 4.6 *5,6,7,8,9*");
            CHECK( vector.to() == "*1" );
            CHECK( vector.distance() == ULength(2, Length::Meters) );
            CHECK( vector.frontAzimuth() == UAngle(3, Angle::Degrees) );
            CHECK( vector.frontInclination() == UAngle(4, Angle::Degrees) );
            CHECK( vector.instHeight() == ULength(4.5, Length::Meters) );
            CHECK( vector.targetHeight() == ULength(4.6, Length::Meters) );
            CHECK( vector.left() == ULength(5, Length::Meters) );
            CHECK( vector.right() == ULength(6, Length::Meters) );
            CHECK( vector.up() == ULength(7, Length::Meters) );
            CHECK( vector.down() == ULength(8, Length::Meters) );
            CHECK( vector.lrudAngle() == UAngle(9, Angle::Degrees) );

            parser.parseLine("A *1 2 3 4 *");
            CHECK( vector.to() == QString() );
            CHECK( vector.distance() == ULength() );
            CHECK( vector.frontAzimuth() == UAngle() );
            CHECK( vector.frontInclination() == UAngle() );
            CHECK( vector.left() == ULength(1, Length::Meters) );
            CHECK( vector.right() == ULength(2, Length::Meters) );
            CHECK( vector.up() == ULength(3, Length::Meters) );
            CHECK( vector.down() == ULength(4, Length::Meters) );

            parser.parseLine("A *1 2 3 4 5*");
            CHECK( vector.to() == QString() );
            CHECK( vector.distance() == ULength() );
            CHECK( vector.frontAzimuth() == UAngle() );
            CHECK( vector.frontInclination() == UAngle() );
            CHECK( vector.left() == ULength(1, Length::Meters) );
            CHECK( vector.right() == ULength(2, Length::Meters) );
            CHECK( vector.up() == ULength(3, Length::Meters) );
            CHECK( vector.down() == ULength(4, Length::Meters) );
            CHECK( vector.lrudAngle() == UAngle(5, Angle::Degrees) );

            parser.parseLine("A *1 2 3 4 5 C*");
            CHECK( vector.to() == QString() );
            CHECK( vector.distance() == ULength() );
            CHECK( vector.frontAzimuth() == UAngle() );
            CHECK( vector.frontInclination() == UAngle() );
            CHECK( vector.left() == ULength(1, Length::Meters) );
            CHECK( vector.right() == ULength(2, Length::Meters) );
            CHECK( vector.up() == ULength(3, Length::Meters) );
            CHECK( vector.down() == ULength(4, Length::Meters) );
            CHECK( vector.lrudAngle() == UAngle(5, Angle::Degrees) );
            CHECK( vector.cFlag() );

            parser.parseLine("A <1 2 3 4 <5,6,7,8>");
            CHECK( vector.to() == "<1" );
            CHECK( vector.distance() == ULength(2, Length::Meters) );
            CHECK( vector.frontAzimuth() == UAngle(3, Angle::Degrees) );
            CHECK( vector.frontInclination() == UAngle(4, Angle::Degrees) );
            CHECK( vector.left() == ULength(5, Length::Meters) );
            CHECK( vector.right() == ULength(6, Length::Meters) );
            CHECK( vector.up() == ULength(7, Length::Meters) );
            CHECK( vector.down() == ULength(8, Length::Meters) );

            parser.parseLine("A <1 2 3 4 >");
            CHECK( vector.to() == QString() );
            CHECK( vector.distance() == ULength() );
            CHECK( vector.frontAzimuth() == UAngle() );
            CHECK( vector.frontInclination() == UAngle() );
            CHECK( vector.left() == ULength(1, Length::Meters) );
            CHECK( vector.right() == ULength(2, Length::Meters) );
            CHECK( vector.up() == ULength(3, Length::Meters) );
            CHECK( vector.down() == ULength(4, Length::Meters) );

            CHECK_THROWS( parser.parseLine("A *1 2 3 4 *1 2 3 4") );
            CHECK_THROWS( parser.parseLine("A *-- -- -- --") );
        }

    }

    SECTION( "invalid spacing" ) {
        CHECK_THROWS( parser.parseLine("   A B 1 2 3(?,?)") );
        CHECK_THROWS( parser.parseLine("   A B 1 2 3 (?,?)*4,5,6,7*") );
        CHECK_THROWS( parser.parseLine("   A B 1 2 3*4,5,6,7*") );
    }

    SECTION( "valid spacing" ) {
        parser.parseLine("   A B 1 2 3#s blah;test");
        parser.parseLine("   A B 1 2 3 *4,5,6,7*#s blah;test");
        parser.parseLine("   A B 1 2 3 (?,?)#s blah;test");
        parser.parseLine("   A B 1 2 3 (?,?) *4,5,6,7*#s blah;test");
    }

    SECTION( "prefixes" ) {
        parser.parseLine("#units prefix=a");
        CHECK( parser.units().processStationName("b") == "a:b" );
        CHECK( parser.units().processStationName("d:b") == "d:b" );
        CHECK( parser.units().processStationName(":b") == "b" );

        parser.parseLine("#units prefix2=c");
        CHECK( parser.units().processStationName("b") == "c:a:b" );
        CHECK( parser.units().processStationName(":b") == "c::b" );
        CHECK( parser.units().processStationName("d:b") == "c:d:b" );
        CHECK( parser.units().processStationName("::b") == "b" );
        CHECK( parser.units().processStationName(":::::b") == "b" );

        parser.parseLine("#units prefix1");
        CHECK( parser.units().processStationName("b") == "c::b" );
    }

    SECTION( "fixed stations" ) {
        parser.parseLine("#FIX A1 W97:43:52.5 N31:16:45 323f (?,*) /Entrance #s blah ;dms with ft elevations");
        REQUIRE( station.name() == "A1" );
        REQUIRE( station.longitude() == UAngle(-97 - (43 + 52.5 / 60.0) / 60.0, Angle::Degrees) );
        REQUIRE( station.latitude() == UAngle(31 + (16 + 45 / 60.0) / 60.0, Angle::Degrees) );
        REQUIRE( station.rectUp() == ULength(323, Length::Feet) );
        REQUIRE( station.horizVariance() == VarianceOverride::FLOATED );
        REQUIRE( station.vertVariance() == VarianceOverride::FLOATED_TRAVERSE );
        REQUIRE( station.note() == "Entrance" );
        REQUIRE( station.segment().size() == 1 );
        REQUIRE( station.segment()[0] == "blah" );
        REQUIRE( station.comment() == "dms with ft elevations");

        parser.parseLine("#FIX A4 620775.38 3461050.67 98.45");
        REQUIRE( station.name() == "A4" );
        REQUIRE( station.east() == ULength(620775.38, Length::Meters) );
        REQUIRE( station.north() == ULength(3461050.67, Length::Meters) );
        REQUIRE( station.rectUp() == ULength(98.45, Length::Meters) );

        SECTION( "measurements can be reordered" ) {
            parser.parseLine("#units order=nue");
            parser.parseLine("#fix a 1 2 3");
            REQUIRE( station.north() == ULength(1, Length::Meters) );
            REQUIRE( station.rectUp() == ULength(2, Length::Meters) );
            REQUIRE( station.east() == ULength(3, Length::Meters) );
        }

        SECTION( "affected by dUnit" ) {
            parser.parseLine("#units d=feet");
            parser.parseLine("#fix a 1 2 3");
            REQUIRE( station.east() == ULength(1, Length::Feet) );
            REQUIRE( station.north() == ULength(2, Length::Feet) );
            REQUIRE( station.rectUp() == ULength(3, Length::Feet) );
        }

        SECTION( "valid spacing" ) {
            parser.parseLine("#FIX A1 W97:43:52.5 N31:16:45 323f(?,*)/Entrance#s blah;dms with ft elevations");
        }
    }

    SECTION( "save, restore, and reset" ) {
        parser.parseLine("#units lrud=from:urld");
        parser.parseLine("#units save");
        parser.parseLine("#units lrud=from:rldu");
        parser.parseLine("#units save");
        parser.parseLine("#units reset");
        REQUIRE( parser.units().lrudOrderString() == "LRUD" );
        parser.parseLine("#units restore");
        REQUIRE( parser.units().lrudOrderString() == "RLDU" );
        parser.parseLine("#units restore");
        REQUIRE( parser.units().lrudOrderString() == "URLD" );
    }

    SECTION( "can't do more than 10 saves" ) {
        for (int i = 0; i < 10; i++) {
            parser.parseLine("#units save");
        }
        REQUIRE_THROWS( parser.parseLine("#units save") );
    }

    SECTION( "can't restore more than number of times saved" ) {
        for (int i = 0; i < 4; i++) {
            parser.parseLine("#units save");
        }
        for (int i = 0; i < 4; i++) {
            parser.parseLine("#units restore");
        }
        REQUIRE_THROWS( parser.parseLine("#units restore") );
    }

    SECTION( "Walls' crazy macros" ) {
        parser.parseLine("#units $hello=\"der=vad pre\" $world=\"fix1=hello feet\"");
        REQUIRE( parser.macros()["hello"] == "der=vad pre" );
        REQUIRE( parser.macros()["world"] == "fix1=hello feet" );

        parser.parseLine("#units or$(hello)$(world)");

        REQUIRE( parser.units().ctOrder().size() == 3 );
        REQUIRE( parser.units().prefix().size() >= 1 );

        CHECK( parser.units().ctOrder()[0] == CtMeasurement::V );
        CHECK( parser.units().ctOrder()[1] == CtMeasurement::A );
        CHECK( parser.units().ctOrder()[2] == CtMeasurement::D );

        CHECK( parser.units().prefix()[0] == "hello" );
        CHECK( parser.units().dUnit() == Length::Feet );
        CHECK( parser.units().sUnit() == Length::Feet );

        parser.parseLine("#units $hello $world");
        CHECK( parser.macros()["hello"] == "" );
        CHECK( parser.macros()["world"] == "" );

        CHECK_THROWS( parser.parseLine("#units $(undefined)") );
    }

    SECTION( "Comment lines" ) {
        parser.parseLine(";#units invalid=hello");
        CHECK( comment == "#units invalid=hello" );
        vector = Vector();
        parser.parseLine(";a b 1 2 3");
        CHECK( comment == "a b 1 2 3" );
        CHECK( vector.from().isEmpty() );
        CHECK( vector.to().isEmpty() );
    }

    SECTION( "Block comments" ) {
        vector = Vector();
        parser.parseLine("#[");
        parser.parseLine("a b 1 2 3");
        CHECK( comment == "a b 1 2 3" );
        CHECK( vector.from().isEmpty() );
        CHECK( vector.to().isEmpty() );
        parser.parseLine("#units f");
        CHECK( parser.units().dUnit() == Length::Meters );
        parser.parseLine("#]");
        parser.parseLine("a b 1 2 3");
        CHECK( vector.from() == "a" );
        CHECK( vector.to() == "b" );
        CHECK( vector.distance() == ULength(1, Length::Meters) );
        CHECK( vector.frontAzimuth() == UAngle(2, Angle::Degrees) );
        CHECK( vector.frontInclination() == UAngle(3, Angle::Degrees) );
    }
}

TEST_CASE( "WallsUnits tests", "[dewalls]" ) {
    WallsUnits units;

    SECTION( "avgInc" ) {
        units.setTypevbCorrected(true);
        CHECK( units.avgInc(UAngle(3, Angle::Degrees), UAngle(1, Angle::Degrees)) == UAngle(2, Angle::Degrees) );
        CHECK( units.avgInc(UAngle(1, Angle::Degrees), UAngle(3, Angle::Degrees)) == UAngle(2, Angle::Degrees) );
        CHECK( units.avgInc(UAngle(3, Angle::Degrees), UAngle()) == UAngle(3, Angle::Degrees) );
        CHECK( units.avgInc(UAngle(), UAngle(3, Angle::Degrees)) == UAngle(3, Angle::Degrees) );

        units.setTypevbCorrected(false);
        CHECK( units.avgInc(UAngle(3, Angle::Degrees), UAngle(-1, Angle::Degrees)) == UAngle(2, Angle::Degrees) );
        CHECK( units.avgInc(UAngle(1, Angle::Degrees), UAngle(-3, Angle::Degrees)) == UAngle(2, Angle::Degrees) );
        CHECK( units.avgInc(UAngle(), UAngle(3, Angle::Degrees)) == UAngle(-3, Angle::Degrees) );
    }
}
