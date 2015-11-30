#include "wallsprojectparser.h"

#include <iostream>

#include <QDir>
#include <QDebug>

#include "segment.h"
#include "segmentparseexception.h"

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

WpjEntry::WpjEntry(WpjBookPtr parent, QString title)
    : Parent(parent), Title(title), Name(), Path(), Status(0), Options(), Reference()
{

}

WpjBook::WpjBook(WpjBookPtr parent, QString title)
    : WpjEntry(parent, title)
{
}

const int WpjEntry::NameDefinesSegmentBit = 1 << 3;
const int WpjEntry::FeetBit = 1 << 4;
const int WpjEntry::ReferenceUnspecifiedBit = 1 << 6;
const int WpjEntry::DontDeriveDeclBit = 1 << 8;
const int WpjEntry::DeriveDeclBit = 1 << 9;
const int WpjEntry::NotGridRelativeBit = 1 << 10;
const int WpjEntry::GridRelativeBit = 1 << 11;
const int WpjEntry::DontPreserveVertShotOrientationBit = 1 << 12;
const int WpjEntry::PreserveVertShotOrientationBit = 1 << 13;
const int WpjEntry::DontPreserveVertShotLengthBit = 1 << 14;
const int WpjEntry::PreserveVertShotLengthBit = 1 << 15;
const int WpjEntry::OtherTypeBit = 1 << 16;
const int WpjEntry::EditOnLaunchBit = 1 << 17;
const int WpjEntry::OpenOnLaunchBit = 1 << 18;
const int WpjEntry::DefaultViewAfterCompilationMask = 7 << 19;
const int WpjEntry::NorthOrEastViewBits = 1 << 19;
const int WpjEntry::NorthOrWestViewBits = 2 << 19;
const int WpjEntry::NorthViewBits = 3 << 19;
const int WpjEntry::EastViewBits = 4 << 19;
const int WpjEntry::WestViewBits = 5 << 19;

bool WpjEntry::isOther() const {
    return Status & OtherTypeBit;
}

bool WpjEntry::isSurvey() const {
    return !isBook() && !isOther();
}

bool WpjEntry::nameDefinesSegment() const {
    return Status & NameDefinesSegmentBit;
}

GeoReferencePtr WpjEntry::reference() const {
    if (Status & ReferenceUnspecifiedBit) {
        return GeoReferencePtr();
    }
    if (!Reference.isNull()) {
        return Reference;
    }
    if (!Parent.isNull()) {
        return Parent->reference();
    }
    return GeoReferencePtr();
}

WpjEntry::ReviewUnits WpjEntry::reviewUnits() const {
    return Status & FeetBit ? Feet : Meters;
}

bool WpjEntry::deriveDeclFromDate() const {
    if (Status & DontDeriveDeclBit) {
        return false;
    }
    if (Status & DeriveDeclBit) {
        return true;
    }
    if (!Parent.isNull()) {
        return Parent->deriveDeclFromDate();
    }
    return false;
}

bool WpjEntry::gridRelative() const {
    if (Status & NotGridRelativeBit) {
        return false;
    }
    if (Status & GridRelativeBit) {
        return true;
    }
    if (!Parent.isNull()) {
        return Parent->gridRelative();
    }
    return false;
}

bool WpjEntry::preserveVertShotOrientation() const {
    if (Status & DontPreserveVertShotOrientationBit) {
        return false;
    }
    if (Status & PreserveVertShotOrientationBit) {
        return true;
    }
    if (!Parent.isNull()) {
        return Parent->preserveVertShotOrientation();
    }
    return false;
}

bool WpjEntry::preserveVertShotLength() const {
    if (Status & DontPreserveVertShotLengthBit) {
        return false;
    }
    if (Status & PreserveVertShotLengthBit) {
        return true;
    }
    if (!Parent.isNull()) {
        return Parent->preserveVertShotLength();
    }
    return false;
}

WpjEntry::LaunchOptions WpjEntry::launchOptions() const {
    if (Status & EditOnLaunchBit) {
        return Edit;
    }
    if (Status & OpenOnLaunchBit) {
        return Open;
    }
    return Properties;
}

WpjEntry::View WpjEntry::defaultViewAfterCompilation() const {
    switch (Status & DefaultViewAfterCompilationMask) {
    case NorthOrEastViewBits:
        return NorthOrEast;
    case NorthOrWestViewBits:
        return NorthOrWest;
    case NorthViewBits:
        return North;
    case EastViewBits:
        return East;
    case WestViewBits:
        return West;
    default:
        if (!Parent.isNull()) {
            return Parent->defaultViewAfterCompilation();
        }
        return NorthOrEast;
    }
}

QDir WpjEntry::dir() const {
    if (Parent.isNull()) {
        return QDir(Path);
    }
    QDir dir = Parent->dir();
    dir.cd(Path);
    return dir;
}

QString WpjEntry::absolutePath() const {
    return QDir::cleanPath(dir().absoluteFilePath(Name + ".SRV"));
}

void WallsProjectParser::parseLine(QString line) {
    parseLine(Segment(line));
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
    WpjBookPtr parent = currentBook();
    WpjBookPtr newEntry(new WpjBook(parent, title));
    if (!parent.isNull()) {
        parent->Children << newEntry;
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
    CurrentEntry = currentBook()->Parent;
}

void WallsProjectParser::nameLine() {
    expect(".NAME", Qt::CaseInsensitive);
    whitespace();
    CurrentEntry->Name = remaining().value();
}

void WallsProjectParser::pathLine() {
    expect(".PATH", Qt::CaseInsensitive);
    whitespace();
    CurrentEntry->Path = remaining().value();
}

void WallsProjectParser::surveyLine() {
    expect(".SURVEY", Qt::CaseInsensitive);
    whitespace();
    QString title = remaining().value();
    WpjBookPtr book = currentBook();
    WpjEntryPtr newEntry(new WpjEntry(book, title));
    book->Children << newEntry;
    CurrentEntry = newEntry;
}

void WallsProjectParser::statusLine() {
    expect(".STATUS", Qt::CaseInsensitive);
    whitespace();
    CurrentEntry->Status = unsignedIntLiteral();
}

void WallsProjectParser::optionsLine() {
    expect(".OPTIONS", Qt::CaseInsensitive);
    whitespace();
    CurrentEntry->Options = remaining().value();
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
    CurrentEntry->Reference = GeoReferencePtr(new GeoReference);
    *(CurrentEntry->Reference) = ref;
}

WpjBookPtr WallsProjectParser::parseFile(QString fileName) {
    QFile file(fileName);

    if (!file.open(QFile::ReadOnly))
    {
        QString msg = QString("I couldn't open %1").arg(fileName);
        emit message("error", msg);
        return WpjBookPtr();
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
            emit message("error", msg);
            return WpjBookPtr();
        }

        try {
            parseLine(Segment(line, fileName, lineNumber, 0));
        }
        catch (const SegmentParseExpectedException& ex) {
            emitMessage(ex);
            return WpjBookPtr();
        }
        catch (const SegmentParseException& ex) {
            emitMessage(ex);
            return WpjBookPtr();
        }

        lineNumber++;
    }

    file.close();

    if (!ProjectRoot.isNull()) {
        ProjectRoot->Path = QFileInfo(fileName).dir().canonicalPath();
    }

    return ProjectRoot;
}

} // namespace dewalls

