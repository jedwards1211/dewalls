#ifndef DEWALLS_WALLSPARSER_H
#define DEWALLS_WALLSPARSER_H

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
#include "wallsvisitor.h"
#include "vector.h"
#include "fixstation.h"

namespace dewalls {

class WallsParser : public LineParser
{
public:
    typedef UnitizedDouble<Length> ULength;
    typedef UnitizedDouble<Angle>  UAngle;
    typedef const Unit<Length> * LengthUnit;
    typedef const Unit<Angle>  * AngleUnit;
    typedef QSharedPointer<VarianceOverride> VarianceOverridePtr;
    typedef void (WallsParser::*OwnProduction)();

    WallsParser();
    WallsParser(QString line);
    WallsParser(Segment segment);

    WallsVisitor* visitor() const;
    void setVisitor(WallsVisitor* visitor);

    WallsUnits units() const;
    QHash<QString, QString> macros() const;
    QDate date() const;
    QString segment() const;

    ULength unsignedLengthInches();
    ULength unsignedLengthNonInches(LengthUnit defaultUnit);
    ULength unsignedLength(LengthUnit defaultUnit);
    ULength length(LengthUnit defaultUnit);

    UAngle unsignedAngle(QHash<QChar, AngleUnit> unitSuffixes, AngleUnit defaultUnit);
    UAngle unsignedDmsAngle();

    UAngle latitude();
    UAngle longitude();

    UAngle nonQuadrantAzimuth(AngleUnit defaultUnit);
    UAngle quadrantAzimuth();
    UAngle azimuth(AngleUnit defaultUnit);
    UAngle azimuthOffset(AngleUnit defaultUnit);

    UAngle unsignedInclination(AngleUnit defaultUnit);
    UAngle inclination(AngleUnit defaultUnit);

    VarianceOverridePtr varianceOverride(LengthUnit defaultUnit);
    VarianceOverridePtr floatedVectorVarianceOverride();
    VarianceOverridePtr floatedTraverseVarianceOverride();
    VarianceOverridePtr lengthVarianceOverride(LengthUnit defaultUnit);
    VarianceOverridePtr rmsErrorVarianceOverride(LengthUnit defaultUnit);

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

private:
    static double approx(double val);

    static QHash<QString, LengthUnit> createLengthUnits();
    static QHash<QString, AngleUnit> createAzmUnits();
    static QHash<QString, AngleUnit> createIncUnits();
    static QHash<QChar, LengthUnit> createLengthUnitSuffixes();
    static QHash<QChar, AngleUnit> createAzmUnitSuffixes();
    static QHash<QChar, AngleUnit> createIncUnitSuffixes();
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

    static const QHash<QString, LengthUnit> lengthUnits;
    static const QHash<QString, AngleUnit> azmUnits;
    static const QHash<QString, AngleUnit> incUnits;
    static const QHash<QChar, LengthUnit> lengthUnitSuffixes;
    static const QHash<QChar, AngleUnit> azmUnitSuffixes;
    static const QHash<QChar, AngleUnit> incUnitSuffixes;
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
    void varianceOverrides();
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
    void inlineSegmentDirective();
    void fixLine();
    void fixedStation();
    void afterFixedStation();
    void fixRectElement(RectElement elem);
    void fixEast();
    void fixNorth();
    void fixUp();
    void afterFixMeasurements();
    void afterFixVarianceOverrides();
    void inlineNote();
    void afterFixInlineNote();
    void comment();

    void inlineCommentOrEndOfLine();
    void inlineComment();

    WallsVisitor* _visitor;
    bool _inBlockComment;
    WallsUnits _units;
    QStack<WallsUnits> _stack;
    QHash<QString, QString> _macros;
    QString _segment;
    QDate _date;

    Segment _fromStationSegment;
    QString _fromStation;
    Segment _toStationSegment;
    QString _toStation;

    Segment _azmSegment;
    UAngle  _azmFs;
    UAngle  _azmBs;

    Segment _incSegment;
    UAngle  _incFs;
    UAngle  _incBs;
};

inline WallsVisitor* WallsParser::visitor() const
{
    return _visitor;
}

inline void WallsParser::setVisitor(WallsVisitor* visitor)
{
    _visitor = visitor;
}

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
