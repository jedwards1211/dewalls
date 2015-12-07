#include "wallsparser.h"
#include "unitizedmath.h"

namespace dewalls {

typedef UnitizedDouble<Length> ULength;
typedef UnitizedDouble<Angle>  UAngle;
typedef const Unit<Length> * LengthUnit;
typedef const Unit<Angle>  * AngleUnit;
typedef QSharedPointer<VarianceOverride> VarianceOverridePtr;
typedef void (WallsParser::*OwnProduction)();

QHash<QString, LengthUnit> WallsParser::createLengthUnits()
{
    QHash<QString, LengthUnit> result;
    result["meters"] = result["meter"] = result["m"] = Length::meters();
    result["feet"] = result["foot"] = result["ft"] = result["f"] = Length::feet();
    return result;
}

QHash<QString, AngleUnit> WallsParser::createAzmUnits()
{
    QHash<QString, AngleUnit> result;
    result["degree"] = result["degree"] = result["deg"] = result["d"] = Angle::degrees();
    result["mills"] = result["mils"] = result["mil"] = result["m"] = Angle::milsNATO();
    result["grads"] = result["grad"] = result["g"] = Angle::gradians();
    return result;
}

QHash<QString, AngleUnit> WallsParser::createIncUnits()
{
    QHash<QString, AngleUnit> result;
    result["degrees"] = result["degree"] = result["deg"] = result["d"] = Angle::degrees();
    result["mills"] = result["mils"] = result["mil"] = result["m"] = Angle::milsNATO();
    result["grads"] = result["grad"] = result["g"] = Angle::gradians();
    result["percent"] = result["p"] = Angle::percentGrade();
    return result;
}

QHash<QChar, LengthUnit> WallsParser::createLengthUnitSuffixes()
{
    QHash<QChar, LengthUnit> result;
    result['m'] = result['M'] = Length::meters();
    result['f'] = result['F'] = Length::feet();
    result['i'] = result['I'] = Length::inches();
    return result;
}

QHash<QChar, AngleUnit> WallsParser::createAzmUnitSuffixes()
{
    QHash<QChar, AngleUnit> result;
    result['d'] = result['D'] = Angle::degrees();
    result['g'] = result['G'] = Angle::gradians();
    result['m'] = result['M'] = Angle::milsNATO();
    return result;
}

QHash<QChar, AngleUnit> WallsParser::createIncUnitSuffixes()
{
    QHash<QChar, AngleUnit> result;
    result['d'] = result['D'] = Angle::degrees();
    result['g'] = result['G'] = Angle::gradians();
    result['m'] = result['M'] = Angle::milsNATO();
    result['p'] = result['P'] = Angle::percentGrade();
    return result;
}

QHash<QChar, CardinalDirection> WallsParser::createCardinalDirections()
{
    QHash<QChar, CardinalDirection> result;
    result['n'] = result['N'] = CardinalDirection::North;
    result['s'] = result['S'] = CardinalDirection::South;
    result['e'] = result['E'] = CardinalDirection::East;
    result['w'] = result['W'] = CardinalDirection::West;
    return result;
}


QHash<QChar, CardinalDirection> WallsParser::createNorthSouth()
{
    QHash<QChar, CardinalDirection> result;
    result['n'] = result['N'] = CardinalDirection::North;
    result['s'] = result['S'] = CardinalDirection::South;
    return result;
}

QHash<QChar, CardinalDirection> WallsParser::createEastWest()
{
    QHash<QChar, CardinalDirection> result;
    result['e'] = result['E'] = CardinalDirection::East;
    result['w'] = result['W'] = CardinalDirection::West;
    return result;
}

QHash<QChar, QChar> WallsParser::createEscapedChars()
{
    QHash<QChar, QChar> result;
    result['r'] = 'r';
    result['n'] = 'n';
    result['f'] = 'f';
    result['t'] = 't';
    result['"'] = '"';
    result['\\'] = '\\';
    return result;
}

QHash<QChar, CtElement> WallsParser::createCtElements()
{
    QHash<QChar, CtElement> result;
    result['d'] = result['D'] = CtElement::D;
    result['a'] = result['A'] = CtElement::A;
    result['v'] = result['V'] = CtElement::V;
    return result;
}

QHash<QChar, RectElement> WallsParser::createRectElements()
{
    QHash<QChar, RectElement> result;
    result['e'] = result['E'] = RectElement::E;
    result['n'] = result['N'] = RectElement::N;
    result['u'] = result['U'] = RectElement::U;
    return result;
}

QHash<QChar, LrudElement> WallsParser::createLrudElements()
{
    QHash<QChar, LrudElement> result;
    result['l'] = result['L'] = LrudElement::L;
    result['r'] = result['R'] = LrudElement::R;
    result['u'] = result['U'] = LrudElement::U;
    result['d'] = result['D'] = LrudElement::D;
    return result;
}

QHash<QString, bool> WallsParser::createCorrectedValues()
{
    QHash<QString, bool> result;
    result["corrected"] = result["c"] = true;
    result["normal"] = result["n"] = false;
    return result;
}

QHash<QString, CaseType> WallsParser::createCaseTypes()
{
    QHash<QString, CaseType> result;
    result["upper"] = result["u"] = CaseType::Upper;
    result["lower"] = result["l"] = CaseType::Lower;
    result["mixed"] = result["m"] = CaseType::Mixed;
    return result;
}

QHash<QString, LrudType> WallsParser::createLrudTypes()
{
    QHash<QString, LrudType> result;
    result["from"] = result["f"] = LrudType::From;
    result["fb"] = LrudType::FB;
    result["to"] = result["t"] = LrudType::To;
    result["tb"] = LrudType::TB;
    return result;
}

QHash<QString, QList<TapingMethodElement>> WallsParser::createTapingMethods()
{
    typedef QList<TapingMethodElement> Method;
    QHash<QString, QList<TapingMethodElement>> result;
    result["it"] = Method({TapingMethodElement::InstrumentHeight, TapingMethodElement::TargetHeight});
    result["is"] = Method({TapingMethodElement::InstrumentHeight});
    result["st"] = Method({TapingMethodElement::TargetHeight});
    result["ss"] = Method();
    return result;
}

QHash<QString, int> WallsParser::createPrefixDirectives()
{
    QHash<QString, int> result;
    result["#prefix1"] = result["#prefix"] = 0;
    result["#prefix2"] = 1;
    result["#prefix3"] = 2;
    return result;
}

QHash<QString, OwnProduction> WallsParser::createUnitsOptionMap()
{
    QHash<QString, OwnProduction> result;
    result["save"] = &WallsParser::save;
    result["restore"] = &WallsParser::restore;
    result["reset"] = &WallsParser::reset_;
    result["m"] = &WallsParser::meters;
    result["meters"] = &WallsParser::meters;
    result["f"] = &WallsParser::feet;
    result["feet"] = &WallsParser::feet;
    result["ct"] = &WallsParser::ct;
    result["d"] = &WallsParser::d;
    result["s"] = &WallsParser::s;
    result["a"] = &WallsParser::a;
    result["ab"] = &WallsParser::ab;
    result["a/ab"] = &WallsParser::a_ab;
    result["v"] = &WallsParser::v;
    result["vb"] = &WallsParser::vb;
    result["v/vb"] = &WallsParser::v_vb;
    result["o"] = &WallsParser::order;
    result["order"] = &WallsParser::order;
    result["decl"] = &WallsParser::decl;
    result["grid"] = &WallsParser::grid;
    result["rect"] = &WallsParser::rect;
    result["incd"] = &WallsParser::incd;
    result["inch"] = &WallsParser::inch;
    result["incs"] = &WallsParser::incs;
    result["inca"] = &WallsParser::inca;
    result["incab"] = &WallsParser::incab;
    result["incv"] = &WallsParser::incv;
    result["incvb"] = &WallsParser::incvb;
    result["typeab"] = &WallsParser::typeab;
    result["typevb"] = &WallsParser::typevb;
    result["case"] = &WallsParser::case_;
    result["lrud"] = &WallsParser::lrud;
    result["tape"] = &WallsParser::tape;
    result["prefix"] = &WallsParser::prefix1;
    result["prefix1"] = &WallsParser::prefix1;
    result["prefix2"] = &WallsParser::prefix2;
    result["prefix3"] = &WallsParser::prefix3;
    result["uvh"] = &WallsParser::uvh;
    result["uvv"] = &WallsParser::uvv;
    result["uv"] = &WallsParser::uv;
    result["flag"] = &WallsParser::flag;

    return result;
}

QHash<QString, OwnProduction> WallsParser::createDirectivesMap()
{
    QHash<QString, OwnProduction> result;
    result["#units"] = result["#u"] = &WallsParser::unitsLine;
    result["#flag"] = result["#f"] = &WallsParser::flagLine;
    result["#fix"] = &WallsParser::fixLine;
    result["#note"] = &WallsParser::noteLine;
    result["#symbol"] = result["#sym"] = &WallsParser::symbolLine;
    result["#segment"] = result["#seg"] = result["#s"] = &WallsParser::segmentLine;
    result["#date"] = &WallsParser::dateLine;
    result["#["] = &WallsParser::beginBlockCommentLine;
    result["#]"] = &WallsParser::endBlockCommentLine;
    result["#prefix1"] = result["#prefix2"] =
            result["#prefix3"] = result["#prefix"] = &WallsParser::prefixLine;
    return result;
}

double WallsParser::approx(double val)
{
    return floor(val * 1e6) * 1e-6;
}


const QHash<QString, LengthUnit> WallsParser::lengthUnits = WallsParser::createLengthUnits();
const QHash<QString, AngleUnit> WallsParser::azmUnits = WallsParser::createAzmUnits();
const QHash<QString, AngleUnit> WallsParser::incUnits = WallsParser::createIncUnits();
const QHash<QChar, LengthUnit> WallsParser::lengthUnitSuffixes = WallsParser::createLengthUnitSuffixes();
const QHash<QChar, AngleUnit> WallsParser::azmUnitSuffixes = WallsParser::createAzmUnitSuffixes();
const QHash<QChar, AngleUnit> WallsParser::incUnitSuffixes = WallsParser::createIncUnitSuffixes();
const QHash<QChar, CardinalDirection> WallsParser::cardinalDirections = WallsParser::createCardinalDirections();
const QHash<QChar, CardinalDirection> WallsParser::northSouth = WallsParser::createNorthSouth();
const QHash<QChar, CardinalDirection> WallsParser::eastWest = WallsParser::createEastWest();
const QHash<QChar, QChar> WallsParser::escapedChars = WallsParser::createEscapedChars();
const QHash<QChar, CtElement> WallsParser::ctElements = WallsParser::createCtElements();
const QSet<CtElement> WallsParser::requiredCtElements({CtElement::D, CtElement::A});
const QHash<QChar, RectElement> WallsParser::rectElements = WallsParser::createRectElements();
const QSet<RectElement> WallsParser::requiredRectElements({RectElement::E, RectElement::N});
const QHash<QChar, LrudElement> WallsParser::lrudElements = WallsParser::createLrudElements();
const QSet<LrudElement> WallsParser::requiredLrudElements({LrudElement::L, LrudElement::R, LrudElement::U, LrudElement::D});
const QHash<QString, bool> WallsParser::correctedValues = WallsParser::createCorrectedValues();
const QHash<QString, CaseType> WallsParser::caseTypes = WallsParser::createCaseTypes();
const QHash<QString, LrudType> WallsParser::lrudTypes = WallsParser::createLrudTypes();
const QHash<QString, QList<TapingMethodElement>> WallsParser::tapingMethods = WallsParser::createTapingMethods();
const QHash<QString, int> WallsParser::prefixDirectives = WallsParser::createPrefixDirectives();

const QRegExp WallsParser::wordRx("\\w+");
const QRegExp WallsParser::notSemicolonRx("[^;]+");
const QRegExp WallsParser::unitsOptionRx("[a-zA-Z_0-9/]*");
const QRegExp WallsParser::directiveRx("#([][]|[a-zA-Z0-9]+)");
const QRegExp WallsParser::macroNameRx("[^()=,,# \t]*");
const QRegExp WallsParser::stationRx("[^;,,#/ \t]{0,8}");
const QRegExp WallsParser::prefixRx("[^:;,,#/ \t]+");

const QRegExp WallsParser::optionalRx("--+");
const QRegExp WallsParser::optionalStationRx("-+");

const QRegExp WallsParser::isoDateRx("\\d{4}-\\d{2}-\\d{2}");
const QRegExp WallsParser::usDateRx1("\\d{2}-\\d{2}-\\d{2,4}");
const QRegExp WallsParser::usDateRx2("\\d{2}/\\d{2}/\\d{2,4}");
const QRegExp WallsParser::usDateRx3("\\d{4}-\\d{1,2}-\\d{1,2}");

const QHash<QString, OwnProduction> WallsParser::unitsOptionMap = WallsParser::createUnitsOptionMap();
const QHash<QString, OwnProduction> WallsParser::directivesMap = WallsParser::createDirectivesMap();

const UAngle WallsParser::oneEighty = UAngle(180.0, Angle::degrees());

WallsParser::WallsParser()
    : WallsParser(Segment())
{

}

WallsParser::WallsParser(QString line)
    : WallsParser(Segment(line))
{

}

WallsParser::WallsParser(Segment segment)
    : LineParser(segment),
      _inBlockComment(false),
      _units(),
      _stack(),
      _macros(),
      _segment(),
      _date(),
      _fromStationSegment(),
      _toStationSegment(),
      _azmSegment(),
      _incSegment(),
      _vector(),
      _fixStation()
{

}

ULength WallsParser::unsignedLengthInches()
{
    expect('i', Qt::CaseInsensitive);
    double inches = unsignedDoubleLiteral();
    return ULength(inches, Length::in());
}

ULength WallsParser::unsignedLengthNonInches(LengthUnit defaultUnit)
{
    double value = unsignedDoubleLiteral();
    LengthUnit unit = oneOfMap(lengthUnitSuffixes, defaultUnit);
    if (unit == Length::inches())
    {
        double inches = unsignedDoubleLiteral();
        return ULength(value * 12 + inches, Length::inches());
    }
    return ULength(value, unit);
}

ULength WallsParser::unsignedLength(LengthUnit defaultUnit)
{
    ULength result;
    oneOfR(result,
           [&]() { return unsignedLengthNonInches(defaultUnit); },
    [&]() { return unsignedLengthInches(); });
    return result;
}

ULength WallsParser::length(LengthUnit defaultUnit)
{
    bool negate = maybe( [&]() { return expect('-'); } );
    ULength length = unsignedLength(defaultUnit);
    return negate ? -length : length;
}

UAngle WallsParser::unsignedAngle(QHash<QChar, AngleUnit> unitSuffixes, AngleUnit defaultUnit)
{
    auto expectColon = [&]() { expect(':'); };
    auto _unsignedDoubleLiteral = [&]{ return unsignedDoubleLiteral(); };

    double value;
    bool hasValue = maybe(value, _unsignedDoubleLiteral);
    if (maybe(expectColon))
    {
        double minutes, seconds;
        bool hasMinutes = maybe(minutes, _unsignedDoubleLiteral);
        bool hasSeconds = false;
        if (maybe(expectColon))
        {
            hasSeconds = maybe(seconds, _unsignedDoubleLiteral);
        }
        if (!(hasValue || hasMinutes || hasSeconds))
        {
            throwAllExpected();
        }
        return UAngle((hasValue   ? value 		     : 0) +
                      (hasMinutes ? minutes / 60.0   : 0) +
                      (hasSeconds ? seconds / 3600.0 : 0), Angle::degrees());
    }
    else if (!hasValue)
    {
        throwAllExpected();
    }
    return UAngle(value, oneOfMap(unitSuffixes, defaultUnit));
}

UAngle WallsParser::unsignedDmsAngle()
{
    auto expectColon = [&]() { expect(':'); };
    auto _unsignedDoubleLiteral = [&]{ return unsignedDoubleLiteral(); };

    double degrees, minutes, seconds;
    bool hasDegrees = maybe(degrees, _unsignedDoubleLiteral);
    expect(':');
    bool hasMinutes = maybe(minutes, _unsignedDoubleLiteral);
    bool hasSeconds = false;
    if (maybe(expectColon))
    {
        hasSeconds = maybe(seconds, _unsignedDoubleLiteral);
    }
    if (!(hasDegrees || hasMinutes || hasSeconds))
    {
        throwAllExpected();
    }
    return UAngle((hasDegrees ? degrees 	     : 0) +
                  (hasMinutes ? minutes / 60.0   : 0) +
                  (hasSeconds ? seconds / 3600.0 : 0), Angle::degrees());
}

UAngle WallsParser::latitude()
{
    int start = _i;
    CardinalDirection side = oneOfMap(northSouth);
    UAngle latitude = unsignedDmsAngle();

    if (approx(latitude.get(Angle::degrees())) > 90.0)
    {
        throw SegmentParseException(_line.mid(start, _i), "latitude out of range");
    }

    if (side == CardinalDirection::South)
    {
        return -latitude;
    }
    return latitude;
}

UAngle WallsParser::longitude()
{
    int start = _i;
    CardinalDirection side = oneOfMap(eastWest);
    UAngle longitude = unsignedDmsAngle();

    if (approx(longitude.get(Angle::degrees())) > 180.0)
    {
        throw SegmentParseException(_line.mid(start, _i), "longitude out of range");
    }

    if (side == CardinalDirection::West)
    {
        return -longitude;
    }
    return longitude;
}

UAngle WallsParser::nonQuadrantAzimuth(AngleUnit defaultUnit)
{
    int start = _i;

    UAngle result = unsignedAngle(azmUnitSuffixes, defaultUnit);

    if (approx(result.get(Angle::degrees())) >= 360.0)
    {
        throw SegmentParseException(_line.mid(start, _i), "azimuth out of range");
    }

    return result;
}

UAngle WallsParser::quadrantAzimuth()
{
    CardinalDirection from = oneOfMap(cardinalDirections);

    int start = _i;
    UAngle angle;
    if (maybe(angle, [&]() { return nonQuadrantAzimuth(Angle::degrees()); }))
    {
        if (approx(angle.get(Angle::degrees())) >= 90.0)
        {
            throw SegmentParseException(_line.mid(start, _i), "azimuth out of range");
        }

        CardinalDirection to = oneOfMap(
                    from == CardinalDirection::North ||
                    from == CardinalDirection::South ?
                        eastWest : northSouth);

        return from.quadrant(to, angle);
    }
    return from.angle();
}

UAngle WallsParser::azimuth(AngleUnit defaultUnit)
{
    UAngle result;
    oneOfR(result, [&]() { return quadrantAzimuth(); },
    [&]() { return nonQuadrantAzimuth(defaultUnit); });
    return result;
}

UAngle WallsParser::azimuthOffset(AngleUnit defaultUnit)
{
    double signum;
    if (!maybe(signum, [this]() { return oneOfMap(signSignums); } ))
    {
        signum = 1.0;
    }
    return nonQuadrantAzimuth(defaultUnit) * signum;
}

UAngle WallsParser::unsignedInclination(AngleUnit defaultUnit)
{
    int start = _i;
    UAngle result = unsignedAngle(incUnitSuffixes, defaultUnit);

    if (approx(result.get(Angle::degrees())) > 90.0)
    {
        throw SegmentParseException(_line.mid(start, _i), "inclination out of range");
    }

    return result;
}

UAngle WallsParser::inclination(AngleUnit defaultUnit)
{
    int start = _i;
    double signum;
    bool hasSignum = maybe(signum, [this]() { return oneOfMap(signSignums); } );
    UAngle angle = unsignedInclination(defaultUnit);

    if (hasSignum)
    {
        if (angle.get(angle.unit()) == 0.0)
        {
            throw SegmentParseException(_line.mid(start, _i), "zero inclinations must not be preceded by a sign");
        }
        return angle * signum;
    }
    return angle;
}

VarianceOverridePtr WallsParser::varianceOverride(LengthUnit defaultUnit)
{
    VarianceOverridePtr result;
    oneOfR(result,
           [&]() { return floatedVectorVarianceOverride(); },
    [&]() { return floatedTraverseVarianceOverride(); },
    [&]() { return lengthVarianceOverride(defaultUnit); },
    [&]() { return rmsErrorVarianceOverride(defaultUnit); },
    [&]() { return VarianceOverridePtr(NULL); });
    return result;
}

VarianceOverridePtr WallsParser::floatedVectorVarianceOverride()
{
    expect('?');
    return VarianceOverride::FLOATED;
}

VarianceOverridePtr WallsParser::floatedTraverseVarianceOverride()
{
    expect('*');
    return VarianceOverride::FLOATED_TRAVERSE;
}

VarianceOverridePtr WallsParser::lengthVarianceOverride(LengthUnit defaultUnit)
{
    return VarianceOverridePtr(new LengthOverride(unsignedLength(defaultUnit)));
}

VarianceOverridePtr WallsParser::rmsErrorVarianceOverride(LengthUnit defaultUnit)
{
    expect('r', Qt::CaseInsensitive);
    return VarianceOverridePtr(new RMSError(unsignedLength(defaultUnit)));
}

QString WallsParser::quotedTextOrNonwhitespace()
{
    QString result;
    oneOfR(result, [&]() { return quotedText(); },
    [&]() { return nonwhitespace().value(); });
    return result;
}

QString WallsParser::quotedText()
{
    expect('"');
    QString result = escapedText([](QChar c) { return c != '"'; }, {"<QUOTED TEXT>"});
    expect('"');
    return result;
}

QString WallsParser::movePastEndQuote()
{
    int start = _i++;
    while (_i < _line.length())
    {
        QChar c = _line.at(_i++);
        if (c == '\\')
        {
            if (_i < _line.length()) _i++;
        }
        else if (c == '"')
        {
            break;
        }
    }

    return _line.value().mid(start, _i - start);
}

void WallsParser::parseLine(QString line)
{
    reset(line);
    parseLine();
}

void WallsParser::parseLine(Segment line)
{
    reset(line);
    parseLine();
}

void WallsParser::parseLine()
{
    maybeWhitespace();

    if (isAtEnd())
    {
        return;
    }

    if (_inBlockComment)
    {
        throwAllExpected([&]() {
            oneOfWithLookahead([&]() { endBlockCommentLine(); },
            [&]() { insideBlockCommentLine(); });
        });
    }
    else
    {
        throwAllExpected([&]() { oneOf([&]() { comment(); },
            [&]() { directiveLine(); },
            [&]() { vectorLine(); });
        });
    }
}

void WallsParser::directiveLine()
{
    int start = _i;
    OwnProduction directive = oneOfMapLowercase(directiveRx, directivesMap);
    _i = start;
    if (directive != &WallsParser::fixLine) replaceMacros();
    (this->*directive)();
}

QString WallsParser::replaceMacro()
{
    _i += 2;
    int start = _i;
    while (_i < _line.length())
    {
        QChar c = _line.at(_i++);
        if (c == ')')
        {
            Segment macroName = _line.mid(start, _i - 1 - start);
            if (!_macros.contains(macroName.value()))
            {
                throw SegmentParseException(macroName, "macro not defined");
            }
            return _macros[macroName.value()];
        }
        else if (c.isSpace())
        {
            throw SegmentParseExpectedException(_line.atAsSegment(_i - 1), "<NONWHITESPACE>");
        }
    }
    throw SegmentParseExpectedException(_line.atAsSegment(_i), std::initializer_list<QString>{"<NON_WHITESPACE>", ")"});
}

void WallsParser::replaceMacros()
{
    QString newLine;

    bool replaced = false;

    while (_i < _line.length())
    {
        QChar c = _line.at(_i);
        switch(c.toLatin1())
        {
        case '"':
            newLine += movePastEndQuote();
            break;
        case '$':
            if (_i + 1 < _line.length() && _line.at(_i + 1) == '(')
            {
                replaced = true;
                newLine += replaceMacro();
                break;
            }
        default:
            newLine += c;
            _i++;
            break;
        }
    }

    _i = 0;

    if (replaced)
    {
        _line = Segment(newLine, _line.source(), _line.startLine(), _line.startCol());
    }
}

void WallsParser::beginBlockCommentLine()
{
    maybeWhitespace();
    expect("#[");
    _inBlockComment = true;
}

void WallsParser::endBlockCommentLine()
{
    maybeWhitespace();
    expect("#]");
    remaining();
    _inBlockComment = false;
}

void WallsParser::insideBlockCommentLine()
{
    emit parsedComment(remaining().value());
}

Segment WallsParser::untilComment(std::initializer_list<QString> expectedItems)
{
    return expect(notSemicolonRx, expectedItems);
}

void WallsParser::segmentLine()
{
    maybeWhitespace();
    _segment = combineSegments(_segment, segmentDirective());
    maybeWhitespace();
    inlineCommentOrEndOfLine();
}

Segment WallsParser::segmentDirective()
{
    oneOf([&]() { expect("#segment", Qt::CaseInsensitive); },
    [&]() { expect("#seg", Qt::CaseInsensitive); },
    [&]() { expect("#s", Qt::CaseInsensitive); } );

    if (maybeWhitespace())
    {
        Segment result;
        maybe(result, [&]() { return untilComment({"<SEGMENT>"}); });
        return result.trimmed();
    }
    return _line.mid(_i, 0);
}

QString WallsParser::combineSegments(QString base, Segment offset)
{
    if (offset.value().startsWith('/'))
    {
        return offset.value();
    }

    QStringList baseParts = base.split("/", QString::SkipEmptyParts);
    QList<Segment> offsetParts = offset.split("/", QString::SkipEmptyParts);

    foreach (Segment part, offsetParts)
    {
        if (part.value() == "..")
        {
            if (!baseParts.isEmpty())
            {
                baseParts.removeLast();
            }
        }
        else if (part.value() != ".")
        {
            baseParts << part.value();
        }
    }

    return '/' + baseParts.join('/');
}

void WallsParser::prefixLine()
{
    maybeWhitespace();
    prefixDirective();
    maybeWhitespace();
    inlineCommentOrEndOfLine();
}

void WallsParser::prefixDirective()
{
    int prefixIndex = oneOfMapLowercase(nonwhitespaceRx, prefixDirectives);

    QString prefix;

    if (maybeWhitespace())
    {
        Segment segment;
        if (maybe(segment, [&]() { return expect(prefixRx, {"<PREFIX>"}); }))
        {
            prefix = segment.value();
        }
    }

    _units.setPrefix(prefixIndex, prefix);
}

void WallsParser::noteLine()
{
    maybeWhitespace();
    noteDirective();
    maybeWhitespace();
    inlineCommentOrEndOfLine();
}

void WallsParser::noteDirective()
{
    oneOf([&]() { expect("#note", Qt::CaseInsensitive); },
    [&]() { expect("#n", Qt::CaseInsensitive); });

    whitespace();
    QString station = expect(stationRx, {"<STATION NAME>"}).value();
    whitespace();
    QString _note = escapedText([](QChar c) { return c != ';'; }, {"<NOTE>"});

    emit parsedNote(station, _note);
}

void WallsParser::flagLine()
{
    maybeWhitespace();
    flagDirective();
    maybeWhitespace();
    inlineCommentOrEndOfLine();
}

void WallsParser::flagDirective()
{
    oneOf([&]() { expect("#flag", Qt::CaseInsensitive); },
    [&]() { expect("#f", Qt::CaseInsensitive); });

    QStringList stations;

    maybeWhitespace();

    while(true)
    {
        Segment station;
        if (!maybe([&]() { return expect(stationRx, {"<STATION NAME>"}); }))
        {
            break;
        }
        stations << station.value();

        maybeWhitespace();
        maybeChar(',');
        maybeWhitespace();
    }

    QString _flag;
    bool hasFlag = maybe(_flag, [&]() { return slashPrefixedFlag(); });
    maybeWhitespace();

    if (stations.isEmpty())
    {
        _units.flag() = _flag;
    }
    else
    {
        if (!hasFlag)
        {
            throwAllExpected();
        }
        emit parsedFlag(stations, _flag);
    }

    inlineCommentOrEndOfLine();
}

QString WallsParser::slashPrefixedFlag()
{
    expect('/');
    return expect(notSemicolonRx, {"<FLAG>"}).value();
}

void WallsParser::symbolLine()
{
    maybeWhitespace();

    oneOf([&]() { expect("#symbol", Qt::CaseInsensitive); },
    [&]() { expect("#sym", Qt::CaseInsensitive); });

    // ignore the rest for now
    remaining();
}

void WallsParser::dateLine()
{
    maybeWhitespace();
    emit parsedDate(dateDirective());
    maybeWhitespace();
    inlineCommentOrEndOfLine();
}

QDate WallsParser::dateDirective()
{
    expect("#date", Qt::CaseInsensitive);
    whitespace();
    oneOfR(_date,
           [&]() { return isoDate(); },
    [&]() { return usDate1(); },
    [&]() { return usDate2(); },
    [&]() { return usDate3(); });
    return _date;
}

QDate WallsParser::isoDate()
{
    Segment dateSegment = expect(isoDateRx, {"<DATE>"});
    return QDate::fromString(dateSegment.value(), Qt::ISODate);
}

QDate WallsParser::usDate1()
{
    QString str = expect(usDateRx1, {"<DATE>"}).value();
    return QDate::fromString(str, str.length() > 8 ? "MM-dd-yyyy" : "MM-dd-yy");
}

QDate WallsParser::usDate2()
{
    QString str = expect(usDateRx2, {"<DATE>"}).value();
    return QDate::fromString(str, str.length() > 8 ? "MM/dd/yyyy" : "MM/dd/yy");
}

QDate WallsParser::usDate3()
{
    QString str = expect(usDateRx3, {"<DATE>"}).value();
    return QDate::fromString(str, "yyyy-MM-dd");
}
void WallsParser::unitsLine()
{
    maybeWhitespace();
    oneOf([&]() { expect("#units", Qt::CaseInsensitive); },
    [&]() { expect("#u", Qt::CaseInsensitive); });

    emit willParseUnits();

    if (maybeWhitespace())
    {
        unitsOptions();
        emit parsedUnits();
    }
}

void WallsParser::parseUnitsOptions(Segment options)
{
    reset(options);
    unitsOptions();
    emit parsedUnits();
}

void WallsParser::unitsOptions()
{
    bool gotOne = false;
    while(!maybe([&]() { inlineCommentOrEndOfLine(); } ))
    {
        if (gotOne)
        {
            whitespace();
        }
        else
        {
            gotOne = true;
        }

        maybe([&]() {
            oneOf(
                        [&]() { unitsOption(); },
            [&]() { macroOption(); }
            );
        });
    }
}

void WallsParser::unitsOption()
{
    void (WallsParser::*option)() = oneOfMapLowercase(unitsOptionRx, unitsOptionMap);
    (this->*option)();
}

void WallsParser::macroOption()
{
    expect('$');
    QString macroName = expect(macroNameRx, {"<MACRO NAME>"}).value();
    QString macroValue;
    if (maybeChar('='))
    {
        macroValue = quotedTextOrNonwhitespace();
    }
    _macros[macroName] = macroValue;
}

void WallsParser::save()
{
    if (_stack.size() >= 10)
    {
        throw SegmentParseException(_line.mid(_i - 4, _i), "units stack is full");
    }
    _stack.push(_units);
}

void WallsParser::restore()
{
    if (_stack.isEmpty())
    {
        throw SegmentParseException(_line.mid(_i - 7, _i), "units stack is empty");
    }
    _units = _stack.pop();
}

void WallsParser::reset_()
{
    _units = WallsUnits();
}

void WallsParser::meters()
{
    _units.setDUnit(Length::meters());
    _units.setSUnit(Length::meters());
}

void WallsParser::feet()
{
    _units.setDUnit(Length::feet());
    _units.setSUnit(Length::feet());
}

void WallsParser::ct()
{
    _units.setVectorType(VectorType::CT);
}

void WallsParser::d()
{
    expect('=');
    _units.setDUnit(oneOfMapLowercase(nonwhitespaceRx, lengthUnits));
}

void WallsParser::s()
{
    expect('=');
    _units.setSUnit(oneOfMapLowercase(nonwhitespaceRx, lengthUnits));
}

void WallsParser::a()
{
    expect('=');
    _units.setAUnit(oneOfMapLowercase(nonwhitespaceRx, azmUnits));
}

void WallsParser::ab()
{
    expect('=');
    _units.setAbUnit(oneOfMapLowercase(nonwhitespaceRx, azmUnits));
}

void WallsParser::a_ab()
{
    expect('=');
    AngleUnit unit = oneOfMapLowercase(nonwhitespaceRx, azmUnits);
    _units.setAUnit(unit);
    _units.setAbUnit(unit);
}

void WallsParser::v()
{
    expect('=');
    _units.setVUnit(oneOfMapLowercase(nonwhitespaceRx, incUnits));
}

void WallsParser::vb()
{
    expect('=');
    _units.setVbUnit(oneOfMapLowercase(nonwhitespaceRx, incUnits));
}

void WallsParser::v_vb()
{
    expect('=');
    AngleUnit unit = oneOfMapLowercase(nonwhitespaceRx, incUnits);
    _units.setVUnit(unit);
    _units.setVbUnit(unit);
}

void WallsParser::order()
{
    expect('=');
    oneOf([&]() { ctOrder(); },
    [&]() { rectOrder(); });
}

void WallsParser::ctOrder()
{
    _units.setCtOrder(elementChars(ctElements, requiredCtElements));
}

void WallsParser::rectOrder()
{
    _units.setRectOrder(elementChars(rectElements, requiredRectElements));
}

void WallsParser::decl()
{
    expect('=');
    _units.setDecl(azimuthOffset(_units.aUnit()));
}

void WallsParser::grid()
{
    expect('=');
    _units.setGrid(azimuthOffset(_units.aUnit()));
}

void WallsParser::rect()
{
    if (maybeChar('='))
    {
        _units.setRect(azimuthOffset(_units.aUnit()));
    }
    else
    {
        _units.setVectorType(VectorType::RECT);
    }
}

void WallsParser::incd()
{
    expect('=');
    _units.setIncd(length(_units.dUnit()));
}

void WallsParser::inch()
{
    expect('=');
    _units.setInch(length(_units.sUnit()));
}

void WallsParser::incs()
{
    expect('=');
    _units.setIncs(length(_units.sUnit()));
}

void WallsParser::inca()
{
    expect('=');
    _units.setInca(azimuthOffset(_units.aUnit()));
}

void WallsParser::incab()
{
    expect('=');
    _units.setIncab(azimuthOffset(_units.abUnit()));
}

void WallsParser::incv()
{
    expect('=');
    _units.setIncv(inclination(_units.vUnit()));
}

void WallsParser::incvb()
{
    expect('=');
    _units.setIncvb(inclination(_units.vbUnit()));
}

void WallsParser::typeab()
{
    expect('=');
    _units.setTypeabCorrected(oneOfMapLowercase(wordRx, correctedValues));
    if (maybeChar(','))
    {
        _units.setTypeabTolerance(UAngle(unsignedDoubleLiteral(), Angle::degrees()));
        if (maybeChar(','))
        {
            expect('x', Qt::CaseInsensitive);
            _units.setTypeabNoAverage(true);
        }
        else
        {
            _units.setTypeabNoAverage(false);
        }
    }
    else
    {
        _units.setTypeabTolerance(UAngle(2.0, Angle::degrees()));
    }
}

void WallsParser::typevb()
{
    expect('=');
    _units.setTypevbCorrected(oneOfMapLowercase(wordRx, correctedValues));
    if (maybeChar(','))
    {
        _units.setTypevbTolerance(UAngle(unsignedDoubleLiteral(), Angle::degrees()));
        if (maybeChar(','))
        {
            expect('x', Qt::CaseInsensitive);
            _units.setTypevbNoAverage(true);
        }
        else
        {
            _units.setTypevbNoAverage(false);
        }
    }
    else
    {
        _units.setTypevbTolerance(UAngle(2.0, Angle::degrees()));
    }
}

void WallsParser::case_()
{
    expect('=');
    _units.setCase(oneOfMapLowercase(nonwhitespaceRx, caseTypes));
}

void WallsParser::lrud()
{
    expect('=');
    _units.setLrud(oneOfMapLowercase(wordRx, lrudTypes));
    if (maybeChar(':'))
    {
        lrudOrder();
    }
    else
    {
        _units.setLrudOrder(QList<LrudElement>({LrudElement::L, LrudElement::R, LrudElement::U, LrudElement::D}));
    }
}

void WallsParser::lrudOrder()
{
    _units.setLrudOrder(elementChars(lrudElements, requiredLrudElements));
}

void WallsParser::prefix1()
{
    prefix(0);
}

void WallsParser::prefix2()
{
    prefix(1);
}

void WallsParser::prefix3()
{
    prefix(2);
}

void WallsParser::prefix(int index)
{
    QString prefix;

    if (maybeChar('='))
    {
        prefix = expect(prefixRx, {"<PREFIX>"}).value();
    }
    _units.setPrefix(index, prefix);
}

void WallsParser::tape()
{
    expect('=');
    _units.setTape(oneOfMapLowercase(nonwhitespaceRx, tapingMethods));
}

void WallsParser::uvh()
{
    expect('=');
    _units.setUvh(unsignedDoubleLiteral());
}

void WallsParser::uvv()
{
    expect('=');
    _units.setUvv(unsignedDoubleLiteral());
}

void WallsParser::uv()
{
    expect('=');
    double value = unsignedDoubleLiteral();
    _units.setUvv(value);
    _units.setUvh(value);
}

void WallsParser::flag()
{
    QString flag;
    if (maybeChar('='))
    {
        flag = quotedTextOrNonwhitespace();
    }
    _units.setFlag(flag);
}

void WallsParser::vectorLine()
{
    maybeWhitespace();
    fromStation();
    whitespace();
    afterFromStation();
    maybeWhitespace();
    endOfLine();
    emit parsedVector(_vector);
}

void WallsParser::fromStation()
{
    _fromStationSegment = expect(stationRx, {"<STATION NAME>"}).value();
    QString from = _fromStationSegment.value();
    if (optionalStationRx.exactMatch(from)) {
        from.clear();
    }
    _vector = Vector();
    _vector.setFrom(from);
}

void WallsParser::afterFromStation()
{
    oneOfWithLookahead([&]() {
        toStation();
        whitespace();
        afterToStation();
    }, [&]() {
        // clear all measurements
        QString from = _vector.from();
        _vector = Vector();
        _vector.setFrom(from);
        lruds();
        afterLruds();
    });
}

void WallsParser::toStation()
{
    _toStationSegment = expect(stationRx, {"<STATION NAME>"});
    QString to = _toStationSegment.value();
    if (optionalStationRx.exactMatch(to))
    {
        to.clear();
    }
    if (_vector.from().isEmpty() && to.isEmpty())
    {
        throw SegmentParseException(_toStationSegment, "from and to station can't both be omitted");
    }
    _vector.setTo(to);
}

void WallsParser::afterToStation()
{
    int k = 0;
    if (_units.vectorType() == VectorType::RECT)
    {
        foreach(RectElement elem, _units.rectOrder())
        {
            if (k++ > 0)
            {
                whitespace();
            }
            rectElement(elem);
        }
    }
    else
    {
        foreach(CtElement elem, _units.ctOrder())
        {
            if (k++ > 0)
            {
                whitespace();
            }
            ctElement(elem);
        }
    }

    using namespace std;

    if (_units.vectorType() == VectorType::CT)
    {
        if (!_vector.frontAzimuth().isValid() && !_vector.backAzimuth().isValid() &&
                !WallsUnits::isVertical(_vector.frontInclination(), _vector.backInclination()))
        {
            throw SegmentParseException(_azmSegment, "azimuth can only be omitted for vertical shots");
        }

        tapingMethodElements();
    }

    maybeWhitespace();
    afterVectorMeasurements();
}

void WallsParser::rectElement(RectElement elem)
{
    switch(elem) {
    case RectElement::E:
        east();
        break;
    case RectElement::N:
        north();
        break;
    case RectElement::U:
        rectUp();
        break;
    }
}

void WallsParser::east()
{
    _vector.setEast(length(_units.dUnit()));
}

void WallsParser::north()
{
    _vector.setNorth(length(_units.dUnit()));
}

void WallsParser::rectUp()
{
    _vector.setRectUp(length(_units.dUnit()));
}

void WallsParser::ctElement(CtElement elem)
{
    switch(elem)
    {
    case CtElement::D:
        distance();
        break;
    case CtElement::A:
        azimuth();
        break;
    case CtElement::V:
        inclination();
        break;
    }
}

void WallsParser::checkCorrectedSign(int segStart, ULength measurement, ULength correction) {
    if (measurement.isNonzero() && correction.isNonzero() &&
            measurement.signum() != (measurement + correction).signum()) {
        throw SegmentParseException(_segment.mid(segStart, _i - segStart), "correction changes sign of measurement");
    }
}

void WallsParser::distance()
{
    int start = _i;
    ULength dist = unsignedLength(_units.dUnit());
    checkCorrectedSign(start, dist, _units.incd());
    _vector.setDistance(dist);
}

UAngle WallsParser::azmDifference(UAngle fs, UAngle bs) {
    if (!_units.typeabCorrected()) {
        if (bs < oneEighty)
        {
            bs += oneEighty;
        }
        else
        {
            bs -= oneEighty;
        }
    }
    UAngle diff = uabs(fs - bs);
    return diff > oneEighty ? UAngle(360.0, Angle::degrees()) - diff : diff;
}

void WallsParser::azimuth()
{
    int start = _i;
    UAngle azmFs;
    UAngle azmBs;

    oneOf([&]() {
        optional(azmFs, [&]() { return azimuth(_units.aUnit()); });
        maybe([&]() {
            expect('/');
            optional(azmBs, [&]() { return azimuth(_units.abUnit()); });
        });
    },
    [&]() {
        expect('/');
        optional(azmBs, [&]() { return azimuth(_units.abUnit()); });
    });

    _vector.setFrontAzimuth(azmFs);
    _vector.setBackAzimuth(azmBs);

    _azmSegment = _line.mid(start, _i - start);

    if (azmFs.isValid() && azmBs.isValid())
    {
        UAngle diff = azmDifference(azmFs, azmBs);
        if (diff > _units.typeabTolerance())
        {
            emit message(WallsMessage("warning",
                                      QString("azimuth fs/bs difference (%1) exceeds tolerance (%2)")
                                      .arg(diff.toString())
                                      .arg(_units.typeabTolerance().toString()),
                                      _azmSegment));
        }
    }
}

UAngle WallsParser::incDifference(UAngle fs, UAngle bs) {
    return _units.typevbCorrected() ? uabs(fs - bs) : uabs(fs + bs);
}

void WallsParser::inclination()
{
    int start = _i;
    UAngle incFs;
    UAngle incBs;

    oneOf([&]() {
        optional(incFs, [&]() { return inclination(_units.vUnit()); });
        maybe([&]() {
            expect('/');
            optional(incBs, [&]() { return inclination(_units.vbUnit()); });
        });
    },
    [&]() {
        expect('/');
        optional(incBs, [&]() { return inclination(_units.vbUnit()); });
    });

    _incSegment = _line.mid(start, _i - start);

    if (!incFs.isValid() && !incBs.isValid()) {
        incFs = UAngle(0, _units.vUnit());
    }
    else if (incFs.isValid() && incBs.isValid()) {
        UAngle diff = incDifference(incFs, incBs);
        if (diff > _units.typevbTolerance())
        {
            emit message(WallsMessage("warning",
                                      QString("inclination fs/bs difference (%1) exceeds tolerance (%2)")
                                      .arg(diff.toString())
                                      .arg(_units.typevbTolerance().toString()),
                                      _incSegment));
        }
    }

    _vector.setFrontInclination(incFs);
    _vector.setBackInclination(incBs);
}

void WallsParser::tapingMethodElements()
{
    foreach(TapingMethodElement elem, _units.tape())
    {
        if (!maybeWhitespace() ||
                !maybe([&]() { tapingMethodElement(elem); }))
        {
            break;
        }
    }
}

void WallsParser::tapingMethodElement(TapingMethodElement elem)
{
    switch(elem)
    {
    case TapingMethodElement::InstrumentHeight:
        instrumentHeight();
        break;
    case TapingMethodElement::TargetHeight:
        targetHeight();
        break;
    }
}

void WallsParser::instrumentHeight()
{
    int start = _i;
    ULength ih;
    if (optional(ih, [&]() { return length(_units.sUnit()); }))
    {
        checkCorrectedSign(start, ih, _units.incs());
        _vector.setInstHeight(ih);
    }
}

void WallsParser::targetHeight()
{
    int start = _i;
    ULength th;
    if (optional(th, [&]() { return length(_units.sUnit()); }))
    {
        checkCorrectedSign(start, th, _units.incs());
        _vector.setTargetHeight(th);
    }
}

void WallsParser::lrudElement(LrudElement elem)
{
    switch(elem)
    {
    case LrudElement::L:
        left();
        break;
    case LrudElement::R:
        right();
        break;
    case LrudElement::U:
        up();
        break;
    case LrudElement::D:
        down();
        break;
    }
}

void WallsParser::left()
{
    int start = _i;
    ULength left;
    if (optional(left, [&]() { return unsignedLength(_units.sUnit()); }))
    {
        checkCorrectedSign(start, left, _units.incs());
        _vector.setLeft(left);
    }
}

void WallsParser::right()
{
    int start = _i;
    ULength right;
    if (optional(right, [&]() { return unsignedLength(_units.sUnit()); }))
    {
        checkCorrectedSign(start, right, _units.incs());
        _vector.setRight(right);
    }
}

void WallsParser::up()
{
    int start = _i;
    ULength up;
    if (optional(up, [&]() { return unsignedLength(_units.sUnit()); }))
    {
        checkCorrectedSign(start, up, _units.incs());
        _vector.setUp(up);
    }
}

void WallsParser::down()
{
    int start = _i;
    ULength down;
    if (optional(down, [&]() { return unsignedLength(_units.sUnit()); }))
    {
        checkCorrectedSign(start, down, _units.incs());
        _vector.setDown(down);
    }
}

void WallsParser::afterVectorMeasurements()
{
    if (maybe([&]() { return varianceOverrides(_vector); }))
    {
        maybeWhitespace();
    }
    afterVectorVarianceOverrides();
}

template<class T>
void WallsParser::varianceOverrides(T& target)
{
    expect('(');
    maybeWhitespace();
    VarianceOverridePtr horizontal = varianceOverride(_units.dUnit());
    target.setHorizVariance(horizontal);
    maybeWhitespace();
    if (maybeChar(','))
    {
        maybeWhitespace();
        VarianceOverridePtr vertical = varianceOverride(_units.dUnit());
        if (horizontal.isNull() && vertical.isNull())
        {
            throw allExpected();
        }
        target.setVertVariance(vertical);
    }
    else if (!horizontal.isNull())
    {
        target.setVertVariance(horizontal);
    }
    expect(')');
}

void WallsParser::afterVectorVarianceOverrides()
{
    if (maybe([&]() { lruds(); }))
    {
        maybeWhitespace();
    }
    afterLruds();
}

void WallsParser::lruds()
{
    oneOfWithLookahead([&]() {
        expect('<');
        lrudContent();
        expect('>');
    }, [&]() {
        expect('*');
        lrudContent();
        expect('*');
    });
}

void WallsParser::lrudContent()
{
    oneOfWithLookahead([&]() {
        commaDelimLrudContent();
    }, [&]() {
        whitespaceDelimLrudContent();
    });
}

void WallsParser::commaDelimLrudContent()
{
    maybeWhitespace();
    int m = 0;
    foreach(LrudElement elem, _units.lrudOrder())
    {
        if (m++ > 0)
        {
            maybeWhitespace();
            expect(',');
            maybeWhitespace();
        }
        lrudElement(elem);
    }
    maybeWhitespace();
    afterRequiredCommaDelimLrudMeasurements();
}

void WallsParser::whitespaceDelimLrudContent()
{
    maybeWhitespace();
    int m = 0;
    foreach(LrudElement elem, _units.lrudOrder())
    {
        if (m++ > 0)
        {
            whitespace();
        }
        lrudElement(elem);
    }
    maybeWhitespace();
    afterRequiredWhitespaceDelimLrudMeasurements();
}

void WallsParser::afterRequiredCommaDelimLrudMeasurements()
{
    if (maybeChar(','))
    {
        maybeWhitespace();
        oneOf([&]() {
            lrudFacingAngle();
            maybeWhitespace();
            if (maybeChar(','))
            {
                maybeWhitespace();
                lrudCFlag();
            }
        }, [&]() { lrudCFlag(); });
    }
}

void WallsParser::afterRequiredWhitespaceDelimLrudMeasurements()
{
    maybe([&]() {
        oneOf([&]() {
            lrudFacingAngle();
            if (maybeWhitespace())
            {
                maybe([&]() { lrudCFlag(); });
            }
        },
        [&]() { lrudCFlag(); });
    });
}

void WallsParser::lrudFacingAngle()
{
    _vector.setLrudAngle(azimuth(_units.aUnit()));
}

void WallsParser::lrudCFlag()
{
    expect('c', Qt::CaseInsensitive);
    _vector.setCFlag(true);
}

void WallsParser::afterLruds()
{
    if (maybe([&]() { inlineDirective(); }))
    {
        maybeWhitespace();
    }
    inlineCommentOrEndOfLine();
}

void WallsParser::inlineDirective()
{
    // currently this is the only directive that can be on a vector line
    inlineSegmentDirective(_vector);
}

void WallsParser::inlineFixDirective()
{
    // currently this is the only directive that can be on a fix station line
    inlineSegmentDirective(_fixStation);
}

template<class T>
void WallsParser::inlineSegmentDirective(T& target)
{
    target.setSegment(segmentDirective().value());
}

void WallsParser::fixLine()
{
    maybeWhitespace();
    expect("#fix", Qt::CaseInsensitive);
    whitespace();
    fixedStation();
    whitespace();
    afterFixedStation();
    maybeWhitespace();
    endOfLine();
    emit parsedFixStation(_fixStation);
}

void WallsParser::fixedStation()
{
    QString fixed = expect(stationRx, {"<STATION NAME>"}).value();
    _fixStation = FixStation();
    _fixStation.setName(fixed);
}

void WallsParser::afterFixedStation()
{
    int k = 0;
    foreach(RectElement elem, _units.rectOrder())
    {
        if (k++ > 0)
        {
            whitespace();
        }
        fixRectElement(elem);
    }
    maybeWhitespace();
    afterFixMeasurements();
}

void WallsParser::fixRectElement(RectElement elem)
{
    switch(elem)
    {
    case RectElement::E:
        fixEast();
        break;
    case RectElement::N:
        fixNorth();
        break;
    case RectElement::U:
        fixUp();
        break;
    }
}

void WallsParser::fixEast()
{
    oneOf([&]() { _fixStation.setEast(length(_units.dUnit())); },
    [&]() { _fixStation.setLongitude(longitude()); });
}

void WallsParser::fixNorth()
{
    oneOf([&]() { _fixStation.setNorth(length(_units.dUnit())); },
    [&]() { _fixStation.setLatitude(latitude()); });
}

void WallsParser::fixUp()
{
    _fixStation.setRectUp(length(_units.dUnit()));
}

void WallsParser::afterFixMeasurements()
{
    if (maybe([&]() { varianceOverrides(_fixStation); }))
    {
        maybeWhitespace();
    }
    afterFixVarianceOverrides();
}

void WallsParser::afterFixVarianceOverrides()
{
    if (maybe([&]() { inlineNote(_fixStation); }))
    {
        maybeWhitespace();
    }
    afterInlineFixNote();
}

template<class T>
void WallsParser::inlineNote(T& target)
{
    expect('/');
    target.setNote(escapedText([](QChar c) { return c != ';' && c != '#'; }, {"<NOTE>"}).trimmed());
}

void WallsParser::afterInlineFixNote()
{
    if (maybe([&]() { inlineFixDirective(); }))
    {
        maybeWhitespace();
    }
    inlineCommentOrEndOfLine(_fixStation);
}

void WallsParser::inlineCommentOrEndOfLine()
{
    oneOf([&]() { inlineComment(); },
    [&]() { endOfLine(); });
}

template<class T>
void WallsParser::inlineCommentOrEndOfLine(T& target)
{
    oneOf([&]() { inlineComment(target); },
    [&]() { endOfLine(); });
}

void WallsParser::comment()
{
    expect(';');
    emit parsedComment(remaining().value());
}

void WallsParser::inlineComment()
{
    expect(';');
    emit parsedComment(remaining().value());
}

template<class T>
void WallsParser::inlineComment(T& target)
{
    expect(';');
    target.setComment(remaining().value());
}


} // namespace dewalls
