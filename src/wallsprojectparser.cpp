#include "wallsprojectparser.h"

namespace dewalls {

// STATUS BITS
//   2^0 : Type = Book
//   2^1 : ?
//   2^2 : ?
//   2^3 : Name defines segment
//   2^4 : 1 = Feet, 0 = Meters
//   2^5 : ?
//   2^6 : reference unspecified
//   2^7 : ?
// Derive decl from date:
//   2^8 : 1 = no
//   2^9 : 1 = yes
// UTM/GPS Grid-relative:
//   2^10: 1 = no
//   2^11: 1 = yes
// Preserve vertical shot orientation:
//   2^12: 1 = no
//   2^13: 1 = yes
// Preserve vertical shot length:
//   2^14: 1 = no
//   2^15: 1 = yes
// Other type
//   2^16: 1 = type is other
//   2^17: edit on launch
//   2^18: open on launch
// Default view after compilation (bits 21-19):
//      1: North or East
// 	   10: North or West
//     11: North
//    100: East
//    101: West
//  	northing	easting	   zn conv   el ? d  m  s  lat d  m  s long index/name
// .REF	2308552.729 324501.432 16 -0.601 27 6 20 52 14.229 88 41 13.085 0 "Adindan"


WallsProjectParser::WallsProjectParser()
{

}

const int WallsProjectParser::BookTypeBit = 1;
const int WallsProjectParser::NameDefinesSegmentBit = 1 << 3;
const int WallsProjectParser::FeetBit = 1 << 4;
const int WallsProjectParser::ReferenceUnspecifiedBit = 1 << 6;
const int WallsProjectParser::DontDeriveDeclBit = 1 << 8;
const int WallsProjectParser::DeriveDeclBit = 1 << 9;
const int WallsProjectParser::NotGridRelativeBit = 1 << 10;
const int WallsProjectParser::GridRelativeBit = 1 << 11;
const int WallsProjectParser::DontPreserveVertShotOrientationBit = 1 << 12;
const int WallsProjectParser::PreserveVertShotOrientationBit = 1 << 13;
const int WallsProjectParser::DontPreserveVertShotLengthBit = 1 << 14;
const int WallsProjectParser::PreserveVertShotLengthBit = 1 << 15;
const int WallsProjectParser::OtherTypeBit = 1 << 16;
const int WallsProjectParser::EditOnLaunchBit = 1 << 17;
const int WallsProjectParser::OpenOnLaunchBit = 1 << 18;
const int WallsProjectParser::DefaultViewAfterCompilationMask = 7 << 19;
const int WallsProjectParser::NorthOrEastViewBits = 1 << 19;
const int WallsProjectParser::NorthOrWestViewBits = 2 << 19;
const int WallsProjectParser::NorthViewBits = 3 << 19;
const int WallsProjectParser::EastViewBits = 4 << 19;
const int WallsProjectParser::WestViewBits = 5 << 19;

void WallsProjectParser::parseStatus(int status, WpjEntry destEntry, WpjEntry parentEntry) {
    if (status & BookTypeBit) {
        destEntry.type = WpjEntry::Book;
    }
    else if (status & OtherTypeBit) {
        destEntry.type = WpjEntry::Other;
        destEntry.defaultViewAfterCompilationInherited = false;

        switch (status & DefaultViewAfterCompilationMask) {
        case NorthOrEastViewBits:
            destEntry.defaultViewAfterCompilation = WpjEntry::NorthOrEast;
            break;
        case NorthOrWestViewBits:
            destEntry.defaultViewAfterCompilation = WpjEntry::NorthOrWest;
            break;
        case NorthViewBits:
            destEntry.defaultViewAfterCompilation = WpjEntry::North;
            break;
        case EastViewBits:
            destEntry.defaultViewAfterCompilation = WpjEntry::East;
            break;
        case WestViewBits:
            destEntry.defaultViewAfterCompilation = WpjEntry::West;
            break;
        default:
            destEntry.defaultViewAfterCompilationInherited = true;
            destEntry.defaultViewAfterCompilation = parentEntry.defaultViewAfterCompilation;
            break;
        }
    }

    if (status & NameDefinesSegmentBit) {
        destEntry.nameDefinesSegment = true;
    }

    if (status & FeetBit) {
        destEntry.reviewUnits = WpjEntry::Feet;
    } else {
        destEntry.reviewUnits = WpjEntry::Meters;
    }

    if (status & ReferenceUnspecifiedBit) {
        destEntry.referenceUnspecified = true;
    } else {
        destEntry.referenceUnspecified = false;
    }

    if (status & DontDeriveDeclBit) {
        destEntry.deriveDeclFromDate = false;
        destEntry.deriveDeclFromDateInherited = false;
    } else if (status & DeriveDeclBit) {
        destEntry.deriveDeclFromDate = true;
        destEntry.deriveDeclFromDateInherited = false;
    } else {
        destEntry.deriveDeclFromDate = parentEntry.deriveDeclFromDate;
        destEntry.deriveDeclFromDateInherited = true;
    }

    if (status & NotGridRelativeBit) {
        destEntry.gridRelative = false;
        destEntry.gridRelativeInherited = false;
    } else if (status & GridRelativeBit) {
        destEntry.gridRelative = true;
        destEntry.gridRelativeInherited = false;
    } else {
        destEntry.gridRelative = parentEntry.gridRelative;
        destEntry.gridRelativeInherited = true;
    }

    if (status & DontPreserveVertShotOrientationBit) {
        destEntry.preserveVertShotOrientation = false;
        destEntry.preserveVertShotOrientationInherited = false;
    } else if (status & PreserveVertShotOrientationBit) {
        destEntry.preserveVertShotOrientation = true;
        destEntry.preserveVertShotOrientationInherited = false;
    } else {
        destEntry.preserveVertShotOrientation = parentEntry.preserveVertShotOrientation;
        destEntry.preserveVertShotOrientationInherited = true;
    }

    if (status & DontPreserveVertShotLengthBit) {
        destEntry.preserveVertShotLength = false;
        destEntry.preserveVertShotLengthInherited = false;
    } else if (status & PreserveVertShotLengthBit) {
        destEntry.preserveVertShotLength = true;
        destEntry.preserveVertShotLengthInherited = false;
    } else {
        destEntry.preserveVertShotLength = parentEntry.preserveVertShotLength;
        destEntry.preserveVertShotLengthInherited = true;
    }
}

} // namespace dewalls

