#include "../lib/catch.hpp"

#include "tostrings.h"
#include "../src/wallsprojectparser.h"

using namespace dewalls;

typedef UnitizedDouble<Length> ULength;
typedef UnitizedDouble<Angle> UAngle;

TEST_CASE( "WallsProjectParser parses example file", "[WallsProjectParser]" ) {
    WallsProjectParser parser;

    QObject::connect(&parser, &WallsProjectParser::message,
                     [=](WallsMessage message) {
        std::cout << message.toString().toStdString() << std::endl;
    });

    WpjBookPtr projectRoot;

    projectRoot = parser.parseFile(":/test/Kaua North Maze.wpj");

    REQUIRE( !projectRoot.isNull() );
    CHECK( projectRoot->Title == "Actun Kaua - North Maze" );
    CHECK( projectRoot->Name.value() == "KAUA-NM" );
    CHECK( projectRoot->defaultViewAfterCompilation() == WpjEntry::NorthOrEast);
    CHECK( !projectRoot->preserveVertShotLength() );
    CHECK( !projectRoot->preserveVertShotOrientation() );
    CHECK( projectRoot->deriveDeclFromDate() );
    CHECK( projectRoot->gridRelative() );

    REQUIRE( !projectRoot->reference().isNull() );
    CHECK( projectRoot->reference()->northing == ULength(2308521.655, Length::meters()) );
    CHECK( projectRoot->reference()->easting == ULength(324341.706, Length::meters()) );
    CHECK( projectRoot->reference()->zone == 16 );
    CHECK( projectRoot->reference()->gridConvergence == UAngle(-0.602, Angle::degrees()) );
    CHECK( projectRoot->reference()->elevation == ULength(27, Length::meters()) );

    REQUIRE( projectRoot->Children.size() == 5 );

    WpjEntryPtr child0 = projectRoot->Children[0];
    CHECK( child0->Title == "SVG Map Info - Please Read First!" );
    CHECK( child0->Name.value() == "SVGINFO.TXT" );
    CHECK( child0->isOther() );
    CHECK( child0->launchOptions() == WpjEntry::Edit );

    WpjEntryPtr child1 = projectRoot->Children[1];
    CHECK( child1->Title == "Flags and Notes" );
    CHECK( child1->Name.value() == "NOTES" );
    CHECK( child1->isSurvey() );

    WpjEntryPtr child2 = projectRoot->Children[2];
    CHECK( child2->Title == "SVG Sources" );
    CHECK( child2->Name.value() == QString() );
    CHECK( child2->isBook() );

    WpjEntryPtr child3 = projectRoot->Children[3];
    CHECK( child3->Title == "Fixed Points" );
    CHECK( child3->Name.value() == QString() );
    CHECK( child3->isBook() );

    WpjEntryPtr child4 = projectRoot->Children[4];
    CHECK( child4->Title == "North Maze Surveys" );
    CHECK( child4->Name.value() == "NORTH" );
    CHECK( child4->isBook() );
}

namespace dewalls {
namespace PathTests {

WpjEntryPtr entryAt(WpjBookPtr book, QString titlePath) {
    QStringList titleParts = titlePath.split("/");
    for (int i = 0; i < titleParts.size() && !book.isNull(); i++) {
        WpjEntryPtr nextChild;
        foreach(WpjEntryPtr child, book->Children) {
            if (child->Title == titleParts[i]) {
                nextChild = child;
                break;
            }
        }
        if (i == titleParts.size() - 1) {
            return nextChild;
        }
        else if (!nextChild.isNull() && nextChild->isBook()) {
            book = nextChild.staticCast<WpjBook>();
        }
        else {
            return WpjEntryPtr();
        }
    }
    return WpjEntryPtr();
}

QString absolutePath(WpjEntryPtr entry) {
    return entry.isNull() ? QString() : entry->absolutePath();
}

void checkEntryPath(WpjEntryPtr entry, QString path) {
    REQUIRE( !entry.isNull() );
    CHECK( entry->absolutePath() == QDir::cleanPath(path) );
}

} // namespace PathTests
} // namespace dewalls

TEST_CASE( "WallsProjectParser handles paths correctly", "[WallsProjectParser, .PATH]") {
    using namespace dewalls::PathTests;

    WallsProjectParser parser;

    parser.parseLine(".book root");
    parser.parseLine(".path /rootdir");
    parser.parseLine(".book a");
    parser.parseLine(".book b");
    parser.parseLine(".path bdir");
    parser.parseLine(".book c");
    parser.parseLine(".endbook");
    parser.parseLine(".book d");
    parser.parseLine(".path ddir");
    parser.parseLine(".book e");
    parser.parseLine(".endbook"); //e
    parser.parseLine(".endbook"); //d
    parser.parseLine(".endbook"); //b
    parser.parseLine(".book f");
    parser.parseLine(".path fdir");
    parser.parseLine(".survey g");
    parser.parseLine(".name gsurvey");
    parser.parseLine(".endbook"); // f
    parser.parseLine(".book h");
    parser.parseLine(".path /hdir");
    parser.parseLine(".endbook"); // h
    parser.parseLine(".survey i");
    parser.parseLine(".endbook"); // a
    parser.parseLine(".endbook"); // root

    WpjBookPtr root = parser.result();

    REQUIRE( !root.isNull() );
    CHECK( "/rootdir" == absolutePath(root) );
    CHECK( "/rootdir" == absolutePath(entryAt(root, "a")) );
    CHECK( "/rootdir/bdir" == absolutePath(entryAt(root, "a/b")) );
    CHECK( "/rootdir/bdir" == absolutePath(entryAt(root, "a/b/c")) );
    CHECK( "/rootdir/bdir/ddir" == absolutePath(entryAt(root, "a/b/d")) );
    CHECK( "/rootdir/bdir/ddir" == absolutePath(entryAt(root, "a/b/d/e")) );
    CHECK( "/rootdir/fdir" == absolutePath(entryAt(root, "a/f")) );
    CHECK( "/rootdir/fdir/gsurvey.SRV" == absolutePath(entryAt(root, "a/f/g")) );
    CHECK( "/hdir" == absolutePath(entryAt(root, "a/h")) );
    CHECK( QString() == absolutePath(entryAt(root, "a/i")) );
}
