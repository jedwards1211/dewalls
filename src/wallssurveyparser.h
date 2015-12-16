#ifndef DEWALLS_WALLSPARSER_H
#define DEWALLS_WALLSPARSER_H

#include <QObject>
#include <QString>
#include <QHash>
#include <QSharedPointer>
#include <QStack>

#include "lineparser.h"
#include "unitizeddouble.h"
#include "length.h"
#include "angle.h"
#include "cardinaldirection.h"
#include "varianceoverride.h"
#include "wallstypes.h"
#include "wallsunits.h"
#include "vector.h"
#include "fixstation.h"
#include "wallsmessage.h"
#include "dewallsexport.h"

namespace dewalls {

///
/// \brief parses Walls .SRV files
/// You pass in lines frme the .SRV file to parse to parseLine(), and the parser will emit
/// signals for the various types of data it parses.  The parsed data must be interpreted
/// in the context of the current units(), date(), and segment().
///
class DEWALLS_LIB_EXPORT WallsSurveyParser : public QObject, public LineParser
{
    Q_OBJECT

public:
    typedef UnitizedDouble<Length> ULength;
    typedef UnitizedDouble<Angle>  UAngle;
    typedef QSharedPointer<VarianceOverride> VarianceOverridePtr;
    typedef void (WallsSurveyParser::*OwnProduction)();

    WallsSurveyParser();
    WallsSurveyParser(QString line);
    WallsSurveyParser(Segment segment);

    ///
    /// \brief parses the current line (which must be set by calling reset(line))
    ///
    void parseLine();
    ///
    /// \brief parses a line from a .SRV file.  This method is designed for testing
    /// and doesn't allow you to pass in the source file and line number; prefer
    /// using parseLine(Segment) instead.
    ///
    void parseLine(QString line);
    ///
    /// \brief this is the ideal method to call to parse a line of a .SRV file.
    ///
    void parseLine(Segment line);
    ///
    /// \brief parses units options that come after "#units"
    /// this can be used to parse options given by a .OPTIONS line in
    /// a .WPJ file
    ///
    void parseUnitsOptions(Segment options);

    ///
    /// \return the current Walls units (from #units directives)
    ///
    WallsUnits units() const;
    ///
    /// \return the current date associated with the data (from #date directives)
    ///
    QDate date() const;
    ///
    /// \return the current segment associated with the data (from #segment directives)
    ///
    QStringList segment() const;
    ///
    /// \return the segment that a #segment path starting with \ or / will switch back to
    ///
    QStringList rootSegment() const;
    QHash<QString, QString> macros() const;

    ///
    /// \brief sets the walls #segment.  Used for passing in name-defined segments from
    /// project files.
    ///
    void setSegment(QStringList segment);
    ///
    /// \brief sets the root #segment, which should be set to a name-defined segment
    /// from a project file.
    ///
    void setRootSegment(QStringList rootSegment);

    ULength unsignedLengthInches();
    ULength unsignedLengthNonInches(Length::Unit defaultUnit);
    ULength unsignedLength(Length::Unit defaultUnit);
    ULength length(Length::Unit defaultUnit);

    UAngle unsignedAngle(QHash<QChar, Angle::Unit> unitSuffixes, Angle::Unit defaultUnit);
    UAngle unsignedDmsAngle();

    UAngle latitude();
    UAngle longitude();

    UAngle nonQuadrantAzimuth(Angle::Unit defaultUnit);
    UAngle quadrantAzimuth();
    UAngle azimuth(Angle::Unit defaultUnit);
    UAngle azimuthOffset(Angle::Unit defaultUnit);

    UAngle unsignedInclination(Angle::Unit defaultUnit);
    UAngle inclination(Angle::Unit defaultUnit);

    VarianceOverridePtr varianceOverride(Length::Unit defaultUnit);
    VarianceOverridePtr floatedVectorVarianceOverride();
    VarianceOverridePtr floatedTraverseVarianceOverride();
    VarianceOverridePtr lengthVarianceOverride(Length::Unit defaultUnit);
    VarianceOverridePtr rmsErrorVarianceOverride(Length::Unit defaultUnit);

    template<typename F>
    QChar escapedChar(F charPredicate, std::initializer_list<QString> expectedItems);
    template<typename F>
    QString escapedText(F charPredicate, std::initializer_list<QString> expectedItems);
    QString quotedTextOrNonwhitespace();
    QString quotedText();

    template<typename R, typename F>
    bool optional(R& result, F production);

    template<typename T>
    QList<T> elementChars(QHash<QChar, T> elements, QSet<T> requiredElements);

    void beginBlockCommentLine();
    void endBlockCommentLine();
    void insideBlockCommentLine();

    void directiveLine();
    void segmentLine();
    void prefixLine();
    void noteLine();
    void flagLine();
    void symbolLine();
    void dateLine();
    void unitsLine();
    void vectorLine();

signals:
    void parsedVector(Vector parsedVector);
    void parsedFixStation(FixStation station);
    void parsedComment(QString parsedComment);
    void parsedNote(QString station, QString parsedNote);
    void parsedDate(QDate date);
    void parsedFlag(QStringList stations, QString flag);
    void willParseUnits();
    void parsedUnits();
    void parsedSegment(QString segment);
    void message(WallsMessage message);

private:
    static double approx(double val);

    static QHash<QString, Length::Unit> createLengthUnits();
    static QHash<QString, Angle::Unit> createAzmUnits();
    static QHash<QString, Angle::Unit> createIncUnits();
    static QHash<QChar, Length::Unit> createLengthUnitSuffixes();
    static QHash<QChar, Angle::Unit> createAzmUnitSuffixes();
    static QHash<QChar, Angle::Unit> createIncUnitSuffixes();
    static QHash<QChar, CardinalDirection> createCardinalDirections();
    static QHash<QChar, CardinalDirection> createNorthSouth();
    static QHash<QChar, CardinalDirection> createEastWest();
    static QHash<QChar, QChar> createEscapedChars();
    static QHash<QChar, CtMeasurement> createCtElements();
    static QHash<QChar, RectMeasurement> createRectElements();
    static QHash<QChar, LrudMeasurement> createLrudElements();
    static QHash<QString, bool> createCorrectedValues();
    static QHash<QString, CaseType> createCaseTypes();
    static QHash<QString, LrudType> createLrudTypes();
    static QHash<QString, QList<TapingMethodMeasurement>> createTapingMethods();
    static QHash<QString, int> createPrefixDirectives();
    static QHash<QString, OwnProduction> createUnitsOptionMap();
    static QHash<QString, OwnProduction> createDirectivesMap();

    static const QHash<QString, Length::Unit> lengthUnits;
    static const QHash<QString, Angle::Unit> azmUnits;
    static const QHash<QString, Angle::Unit> incUnits;
    static const QHash<QChar, Length::Unit> lengthUnitSuffixes;
    static const QHash<QChar, Angle::Unit> azmUnitSuffixes;
    static const QHash<QChar, Angle::Unit> incUnitSuffixes;
    static const QHash<QChar, CardinalDirection> cardinalDirections;
    static const QHash<QChar, CardinalDirection> northSouth;
    static const QHash<QChar, CardinalDirection> eastWest;
    static const QHash<QChar, QChar> escapedChars;
    static const QHash<QChar, CtMeasurement> ctElements;
    static const QSet<CtMeasurement> requiredCtElements;
    static const QHash<QChar, RectMeasurement> rectElements;
    static const QSet<RectMeasurement> requiredRectElements;
    static const QHash<QChar, LrudMeasurement> lrudElements;
    static const QSet<LrudMeasurement> requiredLrudElements;
    static const QHash<QString, bool> correctedValues;
    static const QHash<QString, CaseType> caseTypes;
    static const QHash<QString, LrudType> lrudTypes;
    static const QHash<QString, QList<TapingMethodMeasurement>> tapingMethods;
    static const QHash<QString, int> prefixDirectives;

    static const QRegExp wordRx;
    static const QRegExp notSemicolonRx;
    static const QRegExp unitsOptionRx;
    static const QRegExp directiveRx;
    static const QRegExp macroNameRx;
    static const QRegExp stationRx;
    static const QRegExp prefixRx;

    static const QRegExp optionalRx;
    static const QRegExp optionalStationRx;

    static const QRegExp isoDateRx;
    static const QRegExp usDateRx1;
    static const QRegExp usDateRx2;
    static const QRegExp usDateRx3;

    static const QRegExp segmentPartRx;

    static const QHash<QString, OwnProduction> unitsOptionMap;
    static const QHash<QString, OwnProduction> directivesMap;

    static const UAngle oneEighty;

    UAngle azmDifference(UAngle fs, UAngle bs);
    UAngle incDifference(UAngle fs, UAngle bs);

    void replaceMacros();
    QString movePastEndQuote();
    QString replaceMacro();

    Segment untilComment(std::initializer_list<QString> expectedItems);

    void segmentSeparator();
    QString initialSegmentPart();
    QString nonInitialSegmentPart();
    QStringList segmentPath();
    QStringList segmentDirective();
    void prefixDirective();
    void noteDirective();

    void flagDirective();
    QString slashPrefixedFlag();

    QDate dateDirective();
    QDate isoDate();
    QDate usDate1();
    QDate usDate2();
    QDate usDate3();

    void unitsOptions();
    void unitsOption();
    void macroOption();
    void save();
    void restore();
    void reset_();
    void meters();
    void feet();
    void ct();
    void d();
    void s();
    void a();
    void ab();
    void a_ab();
    void v();
    void vb();
    void v_vb();
    void order();
    void ctOrder();
    void rectOrder();
    void decl();
    void grid();
    void rect();
    void incd();
    void inch();
    void incs();
    void inca();
    void incab();
    void incv();
    void incvb();
    void typeab();
    void typevb();
    void case_();
    void lrud();
    void lrudOrder();
    void prefix1();
    void prefix2();
    void prefix3();
    void prefix(int index);
    void tape();
    void uvh();
    void uvv();
    void uv();
    void flag();

    void checkCorrectedSign(int segStart, ULength measurement, ULength correction);

    void fromStation();
    void afterFromStation();
    void toStation();
    void afterToStation();
    void rectMeasurement(RectMeasurement elem);
    void east();
    void north();
    void rectUp();
    void ctMeasurement(CtMeasurement elem);
    void distance();
    void azimuth();
    void inclination();
    void tapingMethodMeasurement();
    void tapingMethodMeasurement(TapingMethodMeasurement elem);
    void instrumentHeight();
    void targetHeight();
    void lrudMeasurement(LrudMeasurement elem);
    void left();
    void right();
    void up();
    void down();
    void afterVectorMeasurements();
    template<class T> void varianceOverrides(T& target);
    void afterVectorVarianceOverrides();
    void lruds();
    void lrudContent();
    void afterRequiredLrudMeasurements();
    void lrudFacingAngle();
    void lrudCFlag();
    void afterLruds();
    void inlineDirective();
    template<class T> void inlineNote(T& target);
    template<class T> void inlineSegmentDirective(T& target);
    template<class T> void inlineCommentOrEndOfLine(T& target);
    template<class T> void inlineComment(T& target);

    void inlineCommentOrEndOfLine();
    void inlineComment();

    void fixLine();
    void fixedStation();
    void afterFixedStation();
    void fixRectMeasurement(RectMeasurement elem);
    void fixEast();
    void fixNorth();
    void fixUp();
    void afterFixMeasurements();
    void afterFixVarianceOverrides();
    void afterInlineFixNote();
    void inlineFixDirective();

    void comment();

    bool _inBlockComment;
    WallsUnits _units;
    QStack<WallsUnits> _stack;
    QHash<QString, QString> _macros;
    QStringList _segment;
    QStringList _rootSegment;
    QDate _date;

    bool _parsedSegmentDirective;
    Segment _fromStationSegment;
    Segment _toStationSegment;
    Segment _azmSegment;
    Segment _incSegment;

    Vector _vector;
    FixStation _fixStation;
};

inline WallsUnits WallsSurveyParser::units() const
{
    return _units;
}

inline QHash<QString, QString> WallsSurveyParser::macros() const
{
    return _macros;
}

inline QDate WallsSurveyParser::date() const
{
    return _date;
}

inline QStringList WallsSurveyParser::rootSegment() const
{
    return _rootSegment;
}

inline void WallsSurveyParser::setRootSegment(QStringList rootSegment)
{
    _rootSegment = rootSegment;
}

inline QStringList WallsSurveyParser::segment() const
{
    return _segment;
}

inline void WallsSurveyParser::setSegment(QStringList segment)
{
    _segment = segment;
}

template<typename F>
QChar WallsSurveyParser::escapedChar(F charPredicate, std::initializer_list<QString> expectedItems)
{
    QChar c = expectChar(charPredicate, expectedItems);
    return c == '\\' ? oneOfMap(escapedChars) : c;
}

template<typename F>
QString WallsSurveyParser::escapedText(F charPredicate, std::initializer_list<QString> expectedItems)
{
    QString result;
    while (maybe([&]() { result.append(escapedChar(charPredicate, expectedItems)); } ));
    return result;
}

template<typename R, typename F>
bool WallsSurveyParser::optional(R& result, F production)
{
    try
    {
        result = production();
    }
    catch (const SegmentParseExpectedException& ex)
    {
        if (maybe([&]() { return expect(optionalRx, {"-", "--"}); }))
        {
            return false;
        }
        throw;
    }

    return true;

//    if (maybe([&]() { return expect(optionalRx, {"-", "--"}); }))
//    {
//        return false;
//    }
//    result = production();
//    return true;
}

template<typename T>
QList<T> WallsSurveyParser::elementChars(QHash<QChar, T> elements, QSet<T> requiredElements)
{
    QList<T> result;
    while (!elements.isEmpty())
    {
        T element;
        if (requiredElements.isEmpty()) {
            if (!maybe(element, [&]() { return oneOfMap(elements); }))
            {
                break;
            }
        }
        else {
            element = oneOfMap(elements);
        }
        result += element;
        QChar c = _line.at(_i - 1);
        elements.remove(c.toLower());
        elements.remove(c.toUpper());
        requiredElements -= element;
    }
    return result;
}

} // namespace dewalls

#endif // DEWALLS_WALLSPARSER_H
