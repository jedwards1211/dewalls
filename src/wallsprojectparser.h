#ifndef DEWALLS_WALLSPROJECTPARSER_H
#define DEWALLS_WALLSPROJECTPARSER_H

#include <QList>
#include <QString>
#include <QDir>
#include <QSharedPointer>
#include <QFile>

#include "abstractparser.h"
#include "lineparser.h"

namespace dewalls {

class Segment;

struct DmsAngle {
    double degrees;
    double minutes;
    double seconds;
};

struct GeoReference {
    // positive for north, negative for south
    int zone;
    double northing;
    double easting;
    double gridConvergence;
    double elevation;
    DmsAngle latitude;
    DmsAngle longitude;
    int wallsDatumIndex;
    QString datumName;
};

typedef QSharedPointer<GeoReference> GeoReferencePtr;

struct WpjEntry;

typedef QSharedPointer<WpjEntry> WpjEntryPtr;

struct WpjEntry {
    enum Type {
        Book = 0,
        Survey = 1,
        Other = 2
    };

    enum ReviewUnits {
        Meters = 0,
        Feet = 1
    };

    enum LaunchOptions {
        Properties = 0,
        Edit = 1,
        Open = 2
    };

    enum View {
        NorthOrEast = 1,
        NorthOrWest = 2,
        North = 3,
        East = 4,
        West = 5
    };

    // display name for the file
    QString title;
    // the file name (relative to path if given)
    QString name;
    // the path in which the file is found
    QDir path;
    Type type;
    ReviewUnits reviewUnits;
    LaunchOptions launchOptions;
    bool nameDefinesSegment;
    // extra #units options
    QString options;
    GeoReferencePtr reference;
    bool referenceInherited;
    bool deriveDeclFromDate;
    bool deriveDeclFromDateInherited;
    bool gridRelative;
    bool gridRelativeInherited;
    bool preserveVertShotOrientation;
    bool preserveVertShotOrientationInherited;
    bool preserveVertShotLength;
    bool preserveVertShotLengthInherited;
    View defaultViewAfterCompilation;
    bool defaultViewAfterCompilationInherited;
    WpjEntryPtr parent;
    QList<WpjEntryPtr> children;
};

class WallsProjectParser : public AbstractParser, public LineParser
{
    Q_OBJECT
public:
    WallsProjectParser();

    void parseLine(Segment line);
    WpjEntryPtr parseFile(QString file);

    void emptyLine();
    void bookLine();
    void endbookLine();
    void surveyLine();
    void nameLine();
    void refLine();
    void optionsLine();
    void statusLine();
    void pathLine();
    void commentLine();

    void applyInheritedProps(WpjEntryPtr entry);

    inline WpjEntryPtr result() const {
        return ProjectRoot;
    }

    static void parseStatus(int status, WpjEntryPtr destEntry);

    static const int BookTypeBit;
    static const int NameDefinesSegmentBit;
    static const int FeetBit;
    static const int ReferenceUnspecifiedBit;
    static const int DontDeriveDeclBit;
    static const int DeriveDeclBit;
    static const int NotGridRelativeBit;
    static const int GridRelativeBit;
    static const int DontPreserveVertShotOrientationBit;
    static const int PreserveVertShotOrientationBit;
    static const int DontPreserveVertShotLengthBit;
    static const int PreserveVertShotLengthBit;
    static const int OtherTypeBit;
    static const int EditOnLaunchBit;
    static const int OpenOnLaunchBit;
    static const int DefaultViewAfterCompilationMask;
    static const int NorthOrEastViewBits;
    static const int NorthOrWestViewBits;
    static const int NorthViewBits;
    static const int EastViewBits;
    static const int WestViewBits;

private:
    WpjEntryPtr CurrentEntry;
    WpjEntryPtr ProjectRoot;

    inline WpjEntryPtr bookAncestor(WpjEntryPtr entry) {
        return (entry.isNull() || entry->type == WpjEntry::Book) ? entry :
                                                                   bookAncestor(entry->parent);
    }
};

} // namespace dewalls

#endif // DEWALLS_WALLSPROJECTPARSER_H
