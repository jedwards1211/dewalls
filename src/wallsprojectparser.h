#ifndef DEWALLS_WALLSPROJECTPARSER_H
#define DEWALLS_WALLSPROJECTPARSER_H

#include <QObject>
#include <QList>
#include <QString>
#include <QDir>

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

struct WpjEntry {
    enum Type {
        Book = 0,
        SurveyFile = 1,
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
    GeoReference reference;
    bool referenceUnspecified;
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
    QList<WpjEntry> children;
};

class WallsProjectParser
{
public:
    WallsProjectParser();

    void parseLine(Segment line);

    inline WpjEntry result() const {
        return ProjectRoot;
    }

    static void parseStatus(int status, WpjEntry destEntry, WpjEntry parentEntry);

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
    WpjEntry CurrentEntry;
    WpjEntry ProjectRoot;
};

} // namespace dewalls

#endif // DEWALLS_WALLSPROJECTPARSER_H
