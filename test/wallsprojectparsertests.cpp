#include "../lib/catch.hpp"

#include "../src/wallsprojectparser.h"

#include <iostream>

using namespace dewalls;

TEST_CASE( "WallsProjectParserTests", "[WallsProjectParser]" ) {
   WallsProjectParser parser;

   QObject::connect(&parser,
                    static_cast<void(WallsProjectParser::*)(Severity,QString)>(&WallsProjectParser::message),
                    [=](const Severity& severity, const QString& message) {
       std::cout << message.toStdString() << std::endl;
   });
   QObject::connect(&parser,
                    static_cast<void(WallsProjectParser::*)(Severity,QString,QString)>(&WallsProjectParser::message),
                    [=](const Severity& severity, const QString& message, const QString& source) {
       std::cout << "In file: " << source.toStdString() << std::endl;
       std::cout << message.toStdString() << std::endl;
   });
   QObject::connect(&parser,
                    static_cast<void(WallsProjectParser::*)(Severity,QString,QString,int,int,int,int)>(
                        &WallsProjectParser::message),
                    [=](const Severity& severity, const QString& message, const QString& source,
                        int startLine, int startColumn, int endLine, int endColumn) {
       std::cout << "In file: " << source.toStdString() << std::endl;
       std::cout << "Line " << startLine << std::endl;
       std::cout << message.toStdString() << std::endl;
   });

   WpjEntryPtr projectRoot;

   projectRoot = parser.parseFile(":/test/Kaua North Maze.wpj");

   REQUIRE( !projectRoot.isNull() );
   CHECK( projectRoot->title == "Actun Kaua - North Maze" );
   CHECK( projectRoot->name == "KAUA-NM" );

   CHECK( !projectRoot->referenceInherited );
   REQUIRE( !projectRoot->reference.isNull() );
   CHECK( projectRoot->reference->northing == 2308521.655 );
   CHECK( projectRoot->reference->easting == 324341.706 );
   CHECK( projectRoot->reference->zone == 16 );
   CHECK( projectRoot->reference->gridConvergence == -0.602 );
   CHECK( projectRoot->reference->elevation == 27 );

   REQUIRE( projectRoot->children.size() == 5 );

   WpjEntryPtr child0 = projectRoot->children[0];
   CHECK( child0->title == "SVG Map Info - Please Read First!" );
   CHECK( child0->name == "SVGINFO.TXT" );
   CHECK( child0->type == WpjEntry::Other );

   WpjEntryPtr child1 = projectRoot->children[1];
   CHECK( child1->title == "Flags and Notes" );
   CHECK( child1->name == "NOTES" );
   CHECK( child1->type == WpjEntry::Survey );

   WpjEntryPtr child2 = projectRoot->children[2];
   CHECK( child2->title == "SVG Sources" );
   CHECK( child2->name == QString() );
   CHECK( child2->type == WpjEntry::Book );

   WpjEntryPtr child3 = projectRoot->children[3];
   CHECK( child3->title == "Fixed Points" );
   CHECK( child3->name == QString() );
   CHECK( child3->type == WpjEntry::Book );

   WpjEntryPtr child4 = projectRoot->children[4];
   CHECK( child4->title == "North Maze Surveys" );
   CHECK( child4->name == "NORTH" );
   CHECK( child4->type == WpjEntry::Book );
}
