#include "wallsprojectparser.h"

#include <QDir>
#include <QDebug>

#include "segment.h"
#include "segmentparseexception.h"
#include "genericexception.h"

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

// REFERENCE FIELDS
//  	northing	easting	   zn conv   el ? d  m  s  lat d  m  s long index/name
// .REF	2308552.729 324501.432 16 -0.601 27 6 20 52 14.229 88 41 13.085 0 "Adindan"

WallsProjectParser::WallsProjectParser()
    : AbstractParser(), LineParser()
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

void WallsProjectParser::parseStatus(int status, WpjEntryPtr destEntry) {
    if (status & BookTypeBit) {
        destEntry->type = WpjEntry::Book;
    }
    else if (status & OtherTypeBit) {
        destEntry->type = WpjEntry::Other;

        if (status & EditOnLaunchBit) {
            destEntry->launchOptions = WpjEntry::Edit;
        }
        else if (status & OpenOnLaunchBit) {
            destEntry->launchOptions = WpjEntry::Open;
        }
        else {
            destEntry->launchOptions = WpjEntry::Properties;
        }
    }
    if (destEntry->type != WpjEntry::Other) {
        destEntry->defaultViewAfterCompilationInherited = false;

        switch (status & DefaultViewAfterCompilationMask) {
        case NorthOrEastViewBits:
            destEntry->defaultViewAfterCompilation = WpjEntry::NorthOrEast;
            break;
        case NorthOrWestViewBits:
            destEntry->defaultViewAfterCompilation = WpjEntry::NorthOrWest;
            break;
        case NorthViewBits:
            destEntry->defaultViewAfterCompilation = WpjEntry::North;
            break;
        case EastViewBits:
            destEntry->defaultViewAfterCompilation = WpjEntry::East;
            break;
        case WestViewBits:
            destEntry->defaultViewAfterCompilation = WpjEntry::West;
            break;
        default:
            if (!destEntry->parent.isNull()) {
                destEntry->defaultViewAfterCompilationInherited = true;
            }
            break;
        }

        if (status & NameDefinesSegmentBit) {
            destEntry->nameDefinesSegment = true;
        }

        if (status & FeetBit) {
            destEntry->reviewUnits = WpjEntry::Feet;
        } else {
            destEntry->reviewUnits = WpjEntry::Meters;
        }
    }

    if (status & ReferenceUnspecifiedBit) {
        destEntry->reference.clear();
    } else if (!destEntry->parent.isNull()) {
        destEntry->reference = destEntry->parent->reference;
    }

    if (status & DontDeriveDeclBit) {
        destEntry->deriveDeclFromDate = false;
        destEntry->deriveDeclFromDateInherited = false;
    } else if (status & DeriveDeclBit) {
        destEntry->deriveDeclFromDate = true;
        destEntry->deriveDeclFromDateInherited = false;
    } else if (!destEntry->parent.isNull()) {
        destEntry->deriveDeclFromDateInherited = true;
    }

    if (status & NotGridRelativeBit) {
        destEntry->gridRelative = false;
        destEntry->gridRelativeInherited = false;
    } else if (status & GridRelativeBit) {
        destEntry->gridRelative = true;
        destEntry->gridRelativeInherited = false;
    } else if (!destEntry->parent.isNull()) {
        destEntry->gridRelativeInherited = true;
    }

    if (status & DontPreserveVertShotOrientationBit) {
        destEntry->preserveVertShotOrientation = false;
        destEntry->preserveVertShotOrientationInherited = false;
    } else if (status & PreserveVertShotOrientationBit) {
        destEntry->preserveVertShotOrientation = true;
        destEntry->preserveVertShotOrientationInherited = false;
    } else if (!destEntry->parent.isNull()) {
        destEntry->preserveVertShotOrientationInherited = true;
    }

    if (status & DontPreserveVertShotLengthBit) {
        destEntry->preserveVertShotLength = false;
        destEntry->preserveVertShotLengthInherited = false;
    } else if (status & PreserveVertShotLengthBit) {
        destEntry->preserveVertShotLength = true;
        destEntry->preserveVertShotLengthInherited = false;
    } else if (!destEntry->parent.isNull()) {
        destEntry->preserveVertShotLengthInherited = true;
    }
}

void WallsProjectParser::parseLine(Segment line) {
    reset(line);

    maybeWhitespace();

    if (ProjectRoot.isNull()) {
        oneOf([&]() { this->bookLine(); },
        [&]() { this->commentLine(); },
        [&]() { this->emptyLine(); });
    }
    else if (CurrentEntry.isNull()) {
        oneOf([&]() { this->commentLine(); },
        [&]() { this->emptyLine(); });
    }
    else {
        oneOf([&]() { this->bookLine(); },
        [&]() { this->endbookLine(); },
        [&]() { this->surveyLine(); },
        [&]() { this->nameLine(); },
        [&]() { this->pathLine(); },
        [&]() { this->refLine(); },
        [&]() { this->optionsLine(); },
        [&]() { this->statusLine(); },
        [&]() { this->commentLine(); },
        [&]() { this->emptyLine(); });
    }
}

void WallsProjectParser::emptyLine() {
    maybeWhitespace();
    endOfLine();
}

void WallsProjectParser::bookLine() {
    expect(".BOOK", Qt::CaseInsensitive);
    whitespace();
    QString title = remaining().value();
    WpjEntryPtr newEntry(new WpjEntry);
    newEntry->type = WpjEntry::Book;
    newEntry->title = title;
    if (!CurrentEntry.isNull()) {
        WpjEntryPtr book = bookAncestor(CurrentEntry);
        newEntry->parent = book;
        book->children << newEntry;
    }
    else {
        ProjectRoot = newEntry;
    }
    CurrentEntry = newEntry;
}

void WallsProjectParser::endbookLine() {
    expect(".ENDBOOK", Qt::CaseInsensitive);
    maybeWhitespace();
    endOfLine();
    if (bookAncestor(CurrentEntry) == ProjectRoot) {
        applyInheritedProps(ProjectRoot);
    }
    CurrentEntry = bookAncestor(CurrentEntry)->parent;
}

void WallsProjectParser::nameLine() {
    expect(".NAME", Qt::CaseInsensitive);
    whitespace();
    CurrentEntry->name = remaining().value();
}

void WallsProjectParser::pathLine() {
    expect(".PATH", Qt::CaseInsensitive);
    whitespace();
    CurrentEntry->path = QDir(remaining().value());
}

void WallsProjectParser::surveyLine() {
    expect(".SURVEY", Qt::CaseInsensitive);
    whitespace();
    QString title = remaining().value();
    WpjEntryPtr newEntry(new WpjEntry);
    newEntry->type = WpjEntry::Survey;
    newEntry->title = title;
    WpjEntryPtr book = bookAncestor(CurrentEntry);
    newEntry->parent = book;
    book->children << newEntry;
    CurrentEntry = newEntry;
}

void WallsProjectParser::statusLine() {
    expect(".STATUS", Qt::CaseInsensitive);
    whitespace();
    int status = unsignedIntLiteral();
    parseStatus(status, CurrentEntry);
}

void WallsProjectParser::optionsLine() {
    expect(".OPTIONS", Qt::CaseInsensitive);
    whitespace();
    CurrentEntry->options = remaining().value();
}

void WallsProjectParser::commentLine() {
    expect(";");
    remaining();
}

void WallsProjectParser::refLine() {
    expect(".REF", Qt::CaseInsensitive);
    whitespace();
    GeoReference ref;
    ref.northing = doubleLiteral();
    whitespace();
    ref.easting = doubleLiteral();
    whitespace();
    ref.zone = intLiteral();
    whitespace();
    ref.gridConvergence = doubleLiteral();
    whitespace();
    ref.elevation = doubleLiteral();
    whitespace();
    doubleLiteral(); // don't know what this field represents
    whitespace();
    ref.latitude.degrees = intLiteral();
    whitespace();
    ref.latitude.minutes = unsignedIntLiteral();
    whitespace();
    ref.latitude.seconds = unsignedDoubleLiteral();
    whitespace();
    ref.longitude.degrees = intLiteral();
    whitespace();
    ref.longitude.minutes = unsignedIntLiteral();
    whitespace();
    ref.longitude.seconds = unsignedDoubleLiteral();
    whitespace();
    ref.wallsDatumIndex = unsignedIntLiteral();
    whitespace();
    expect('"');
    ref.datumName = expect(QRegExp("[^\"]+"), {"<DATUM_NAME>"}).value();
    expect('"');
    maybeWhitespace();
    endOfLine();
    CurrentEntry->reference = GeoReferencePtr(new GeoReference);
    *(CurrentEntry->reference) = ref;
}

void WallsProjectParser::applyInheritedProps(WpjEntryPtr entry) {
    foreach (WpjEntryPtr child, entry->children) {
        if (child->referenceInherited) {
            child->reference = entry->reference;
        }
        if (child->defaultViewAfterCompilationInherited) {
            child->defaultViewAfterCompilation = entry->defaultViewAfterCompilation;
        }
        if (child->deriveDeclFromDateInherited) {
            child->deriveDeclFromDate = entry->deriveDeclFromDate;
        }
        if (child->gridRelativeInherited) {
            child->gridRelative = entry->gridRelative;
        }
        if (child->preserveVertShotOrientationInherited) {
            child->preserveVertShotOrientation = entry->preserveVertShotOrientation;
        }
        if (child->preserveVertShotLengthInherited) {
            child->preserveVertShotLength = entry->preserveVertShotLength;
        }
        applyInheritedProps(child);
    }
}

WpjEntryPtr WallsProjectParser::parseFile(QString fileName) {
    QFile file(fileName);
    QDir dir(fileName);
    dir.cdUp();

    if (!file.open(QFile::ReadOnly))
    {
        QString msg = QString("I couldn't open %1").arg(fileName);
        emit message(Severity::Error, msg);
        throw GenericException(msg);
    }

    int lineNumber = 0;
    while (!file.atEnd())
    {
        QString line = file.readLine();
        line = line.trimmed();
        if (file.error() != QFile::NoError)
        {
            QString msg = QString("Error reading from file %1 at line %2: %3")
                    .arg(fileName)
                    .arg(lineNumber)
                    .arg(file.errorString());
            emit message(Severity::Error, msg);
            throw GenericException(msg);
        }

        try {
            parseLine(Segment(line, fileName, lineNumber, 0));
        }
        catch (const SegmentParseExpectedException& ex) {
            emitMessage(ex);
            throw;
        }
        catch (const SegmentParseException& ex) {
            emitMessage(ex);
            throw;
        }

        lineNumber++;
    }

    file.close();

    return ProjectRoot;
}

} // namespace dewalls

