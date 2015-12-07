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

namespace dewalls {

class WallsParser : public QObject, public LineParser
{
    Q_OBJECT

public:
    typedef UnitizedDouble<Length> ULength;
    typedef UnitizedDouble<Angle>  UAngle;
    typedef QSharedPointer<VarianceOverride> VarianceOverridePtr;
    typedef void (WallsParser::*OwnProduction)();

    WallsParser();
    WallsParser(QString line);
    WallsParser(Segment segment);

    WallsUnits units() const;
    QHash<QString, QString> macros() const;
    QDate date() const;
    QString segment() const;

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

    void parseLine();
    void parseLine(QString line);
    void parseLine(Segment line);
    void parseUnitsOptions(Segment options);

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

    QString combineSegments(QString base, Segment offset);

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
    static QHash<QChar, CtElement> createCtElements();
    static QHash<QChar, RectElement> createRectElements();
    static QHash<QChar, LrudElement> createLrudElements();
    static QHash<QString, bool> createCorrectedValues();
    static QHash<QString, CaseType> createCaseTypes();
    static QHash<QString, LrudType> createLrudTypes();
    static QHash<QString, QList<TapingMethodElement>> createTapingMethods();
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
    static const QHash<QChar, CtElement> ctElements;
    static const QSet<CtElement> requiredCtElements;
    static const QHash<QChar, RectElement> rectElements;
    static const QSet<RectElement> requiredRectElements;
    static const QHash<QChar, LrudElement> lrudElements;
    static const QSet<LrudElement> requiredLrudElements;
    static const QHash<QString, bool> correctedValues;
    static const QHash<QString, CaseType> caseTypes;
    static const QHash<QString, LrudType> lrudTypes;
    static const QHash<QString, QList<TapingMethodElement>> tapingMethods;
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

    static const QHash<QString, OwnProduction> unitsOptionMap;
    static const QHash<QString, OwnProduction> directivesMap;

    static const UAngle oneEighty;

    UAngle azmDifference(UAngle fs, UAngle bs);
    UAngle incDifference(UAngle fs, UAngle bs);

    void replaceMacros();
    QString movePastEndQuote();
    QString replaceMacro();

    Segment untilComment(std::initializer_list<QString> expectedItems);

    Segment segmentDirective();
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
    void rectElement(RectElement elem);
    void east();
    void north();
    void rectUp();
    void ctElement(CtElement elem);
    void distance();
    void azimuth();
    void inclination();
    void tapingMethodElements();
    void tapingMethodElement(TapingMethodElement elem);
    void instrumentHeight();
    void targetHeight();
    void lrudElement(LrudElement elem);
    void left();
    void right();
    void up();
    void down();
    void afterVectorMeasurements();
    template<class T> void varianceOverrides(T& target);
    void afterVectorVarianceOverrides();
    void lruds();
    void lrudContent();
    void commaDelimLrudContent();
    void whitespaceDelimLrudContent();
    void afterRequiredCommaDelimLrudMeasurements();
    void afterRequiredWhitespaceDelimLrudMeasurements();
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
    void fixRectElement(RectElement elem);
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
    QString _segment;
    QDate _date;

    Segment _fromStationSegment;
    Segment _toStationSegment;
    Segment _azmSegment;
    Segment _incSegment;

    Vector _vector;
    FixStation _fixStation;
};

inline WallsUnits WallsParser::units() const
{
    return _units;
}

inline QHash<QString, QString> WallsParser::macros() const
{
    return _macros;
}

inline QDate WallsParser::date() const
{
    return _date;
}

inline QString WallsParser::segment() const
{
    return _segment;
}

template<typename F>
QChar WallsParser::escapedChar(F charPredicate, std::initializer_list<QString> expectedItems)
{
    QChar c = expectChar(charPredicate, expectedItems);
    return c == '\\' ? oneOfMap(escapedChars) : c;
}

template<typename F>
QString WallsParser::escapedText(F charPredicate, std::initializer_list<QString> expectedItems)
{
    QString result;
    while (maybe([&]() { result.append(escapedChar(charPredicate, expectedItems)); } ));
    return result;
}

template<typename R, typename F>
bool WallsParser::optional(R& result, F production)
{
    if (maybe([&]() { return expect(optionalRx, {"-", "--"}); }))
    {
        return false;
    }
    result = production();
    return true;
}

template<typename T>
QList<T> WallsParser::elementChars(QHash<QChar, T> elements, QSet<T> requiredElements)
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
