#ifndef DEWALLS_WALLSPROJECTPARSER_H
#define DEWALLS_WALLSPROJECTPARSER_H

#include <QObject>
#include <QList>
#include <QString>
#include <QDir>
#include <QSharedPointer>
#include <QFile>
#include "unitizeddouble.h"
#include "angle.h"
#include "length.h"

#include "wallsmessage.h"
#include "lineparser.h"

namespace dewalls {

class Segment;

struct GeoReference {
    typedef UnitizedDouble<Length> ULength;
    typedef UnitizedDouble<Angle> UAngle;

    // positive for north, negative for south
    int zone;
    ULength northing;
    ULength easting;
    UAngle gridConvergence;
    ULength elevation;
    UAngle latitude;
    UAngle longitude;
    int wallsDatumIndex;
    QString datumName;
};

typedef QSharedPointer<GeoReference> GeoReferencePtr;

class WpjEntry;
class WpjBook;

typedef QSharedPointer<WpjEntry> WpjEntryPtr;
typedef QSharedPointer<WpjBook> WpjBookPtr;

class WpjEntry {
public:
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
        NorthOrEast = 0,
        NorthOrWest = 1,
        North = 2,
        East = 3,
        West = 4
    };

    WpjEntry(WpjBookPtr parent, QString title);
    virtual ~WpjEntry() {}

    const WpjBookPtr Parent;
    // display name
    const QString Title;
    // the file name (relative to path if given)
    Segment Name;
    // the path of the directory in which the file is found relative to the parent
    QString Path;
    int Status;
    // extra #units options
    Segment Options;
    GeoReferencePtr Reference;

    bool referenceInherited() const;
    GeoReferencePtr reference() const;

    virtual bool isBook() const { return false; }
    bool isOther() const;
    bool isSurvey() const;
    bool nameDefinesSegment() const;
    ReviewUnits reviewUnits() const;
    bool deriveDeclFromDate() const;
    bool gridRelative() const;
    bool preserveVertShotOrientation() const;
    bool preserveVertShotLength() const;
    LaunchOptions launchOptions() const;
    View defaultViewAfterCompilation() const;
    /**
     * @return the directory where this entry's file (or this book's subentries' files)
     * is located
     */
    QDir dir() const;
    /**
     * @return the absolute path to this entry's file (or this book's directory)
     */
    QString absolutePath() const;
    /**
     * @return the inherited and own #units options for this entry (or this book's subentries)
     */
    QList<Segment> allOptions() const;

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
};

class WpjBook : public WpjEntry {
public:
    WpjBook(WpjBookPtr parent, QString title);
    virtual ~WpjBook() {}

    virtual bool isBook() const { return true; }

    QList<WpjEntryPtr> Children;
};

class WallsProjectParser : public QObject, public LineParser
{
    Q_OBJECT
public:
    WallsProjectParser(QObject* parent = 0);

    void parseLine(QString line);
    void parseLine(Segment line);
    WpjBookPtr parseFile(QString file);

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

    inline WpjBookPtr result() const {
        return ProjectRoot;
    }

    inline WpjBookPtr currentBook() const {
        if (CurrentEntry.isNull()) {
            return WpjBookPtr();
        }
        if (CurrentEntry->isBook()) {
            return CurrentEntry.dynamicCast<WpjBook>();
        }
        return CurrentEntry->Parent;
    }

signals:
    void message(WallsMessage message);

private:
    WpjEntryPtr CurrentEntry;
    WpjBookPtr ProjectRoot;
};

} // namespace dewalls

#endif // DEWALLS_WALLSPROJECTPARSER_H
