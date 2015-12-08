#include "wallssurveyparser.h"
#include "unitizedmath.h"

namespace dewalls {

typedef UnitizedDouble<Length> ULength;
typedef UnitizedDouble<Angle>  UAngle;
typedef QSharedPointer<VarianceOverride> VarianceOverridePtr;
typedef void (WallsSurveyParser::*OwnProduction)();

QHash<QString, Length::Unit> WallsSurveyParser::createLengthUnits()
{
    QHash<QString, Length::Unit> result;
    result["meters"] = result["meter"] = result["m"] = Length::Meters;
    result["feet"] = result["foot"] = result["ft"] = result["f"] = Length::Feet;
    return result;
}

QHash<QString, Angle::Unit> WallsSurveyParser::createAzmUnits()
{
    QHash<QString, Angle::Unit> result;
    result["degree"] = result["degree"] = result["deg"] = result["d"] = Angle::Degrees;
    result["mills"] = result["mils"] = result["mil"] = result["m"] = Angle::MilsNATO;
    result["grads"] = result["grad"] = result["g"] = Angle::Gradians;
    return result;
}

QHash<QString, Angle::Unit> WallsSurveyParser::createIncUnits()
{
    QHash<QString, Angle::Unit> result;
    result["degrees"] = result["degree"] = result["deg"] = result["d"] = Angle::Degrees;
    result["mills"] = result["mils"] = result["mil"] = result["m"] = Angle::MilsNATO;
    result["grads"] = result["grad"] = result["g"] = Angle::Gradians;
    result["percent"] = result["p"] = Angle::PercentGrade;
    return result;
}

QHash<QChar, Length::Unit> WallsSurveyParser::createLengthUnitSuffixes()
{
    QHash<QChar, Length::Unit> result;
    result['m'] = result['M'] = Length::Meters;
    result['f'] = result['F'] = Length::Feet;
    result['i'] = result['I'] = Length::Inches;
    return result;
}

QHash<QChar, Angle::Unit> WallsSurveyParser::createAzmUnitSuffixes()
{
    QHash<QChar, Angle::Unit> result;
    result['d'] = result['D'] = Angle::Degrees;
    result['g'] = result['G'] = Angle::Gradians;
    result['m'] = result['M'] = Angle::MilsNATO;
    return result;
}

QHash<QChar, Angle::Unit> WallsSurveyParser::createIncUnitSuffixes()
{
    QHash<QChar, Angle::Unit> result;
    result['d'] = result['D'] = Angle::Degrees;
    result['g'] = result['G'] = Angle::Gradians;
    result['m'] = result['M'] = Angle::MilsNATO;
    result['p'] = result['P'] = Angle::PercentGrade;
    return result;
}

QHash<QChar, CardinalDirection> WallsSurveyParser::createCardinalDirections()
{
    QHash<QChar, CardinalDirection> result;
    result['n'] = result['N'] = CardinalDirection::North;
    result['s'] = result['S'] = CardinalDirection::South;
    result['e'] = result['E'] = CardinalDirection::East;
    result['w'] = result['W'] = CardinalDirection::West;
    return result;
}


QHash<QChar, CardinalDirection> WallsSurveyParser::createNorthSouth()
{
    QHash<QChar, CardinalDirection> result;
    result['n'] = result['N'] = CardinalDirection::North;
    result['s'] = result['S'] = CardinalDirection::South;
    return result;
}

QHash<QChar, CardinalDirection> WallsSurveyParser::createEastWest()
{
    QHash<QChar, CardinalDirection> result;
    result['e'] = result['E'] = CardinalDirection::East;
    result['w'] = result['W'] = CardinalDirection::West;
    return result;
}

QHash<QChar, QChar> WallsSurveyParser::createEscapedChars()
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

QHash<QChar, CtMeasurement> WallsSurveyParser::createCtElements()
{
    QHash<QChar, CtMeasurement> result;
    result['d'] = result['D'] = CtMeasurement::D;
    result['a'] = result['A'] = CtMeasurement::A;
    result['v'] = result['V'] = CtMeasurement::V;
    return result;
}

QHash<QChar, RectMeasurement> WallsSurveyParser::createRectElements()
{
    QHash<QChar, RectMeasurement> result;
    result['e'] = result['E'] = RectMeasurement::E;
    result['n'] = result['N'] = RectMeasurement::N;
    result['u'] = result['U'] = RectMeasurement::U;
    return result;
}

QHash<QChar, LrudMeasurement> WallsSurveyParser::createLrudElements()
{
    QHash<QChar, LrudMeasurement> result;
    result['l'] = result['L'] = LrudMeasurement::L;
    result['r'] = result['R'] = LrudMeasurement::R;
    result['u'] = result['U'] = LrudMeasurement::U;
    result['d'] = result['D'] = LrudMeasurement::D;
    return result;
}

QHash<QString, bool> WallsSurveyParser::createCorrectedValues()
{
    QHash<QString, bool> result;
    result["corrected"] = result["c"] = true;
    result["normal"] = result["n"] = false;
    return result;
}

QHash<QString, CaseType> WallsSurveyParser::createCaseTypes()
{
    QHash<QString, CaseType> result;
    result["upper"] = result["u"] = CaseType::Upper;
    result["lower"] = result["l"] = CaseType::Lower;
    result["mixed"] = result["m"] = CaseType::Mixed;
    return result;
}

QHash<QString, LrudType> WallsSurveyParser::createLrudTypes()
{
    QHash<QString, LrudType> result;
    result["from"] = result["f"] = LrudType::From;
    result["fb"] = LrudType::FB;
    result["to"] = result["t"] = LrudType::To;
    result["tb"] = LrudType::TB;
    return result;
}

QHash<QString, QList<TapingMethodMeasurement>> WallsSurveyParser::createTapingMethods()
{
    typedef QList<TapingMethodMeasurement> Method;
    QHash<QString, QList<TapingMethodMeasurement>> result;
    result["it"] = Method({TapingMethodMeasurement::InstrumentHeight, TapingMethodMeasurement::TargetHeight});
    result["is"] = Method({TapingMethodMeasurement::InstrumentHeight});
    result["st"] = Method({TapingMethodMeasurement::TargetHeight});
    result["ss"] = Method();
    return result;
}

QHash<QString, int> WallsSurveyParser::createPrefixDirectives()
{
    QHash<QString, int> result;
    result["#prefix1"] = result["#prefix"] = 0;
    result["#prefix2"] = 1;
    result["#prefix3"] = 2;
    return result;
}

QHash<QString, OwnProduction> WallsSurveyParser::createUnitsOptionMap()
{
    QHash<QString, OwnProduction> result;
    result["save"] = &WallsSurveyParser::save;
    result["restore"] = &WallsSurveyParser::restore;
    result["reset"] = &WallsSurveyParser::reset_;
    result["m"] = &WallsSurveyParser::meters;
    result["meters"] = &WallsSurveyParser::meters;
    result["f"] = &WallsSurveyParser::feet;
    result["feet"] = &WallsSurveyParser::feet;
    result["ct"] = &WallsSurveyParser::ct;
    result["d"] = &WallsSurveyParser::d;
    result["s"] = &WallsSurveyParser::s;
    result["a"] = &WallsSurveyParser::a;
    result["ab"] = &WallsSurveyParser::ab;
    result["a/ab"] = &WallsSurveyParser::a_ab;
    result["v"] = &WallsSurveyParser::v;
    result["vb"] = &WallsSurveyParser::vb;
    result["v/vb"] = &WallsSurveyParser::v_vb;
    result["o"] = &WallsSurveyParser::order;
    result["order"] = &WallsSurveyParser::order;
    result["decl"] = &WallsSurveyParser::decl;
    result["grid"] = &WallsSurveyParser::grid;
    result["rect"] = &WallsSurveyParser::rect;
    result["incd"] = &WallsSurveyParser::incd;
    result["inch"] = &WallsSurveyParser::inch;
    result["incs"] = &WallsSurveyParser::incs;
    result["inca"] = &WallsSurveyParser::inca;
    result["incab"] = &WallsSurveyParser::incab;
    result["incv"] = &WallsSurveyParser::incv;
    result["incvb"] = &WallsSurveyParser::incvb;
    result["typeab"] = &WallsSurveyParser::typeab;
    result["typevb"] = &WallsSurveyParser::typevb;
    result["case"] = &WallsSurveyParser::case_;
    result["lrud"] = &WallsSurveyParser::lrud;
    result["tape"] = &WallsSurveyParser::tape;
    result["prefix"] = &WallsSurveyParser::prefix1;
    result["prefix1"] = &WallsSurveyParser::prefix1;
    result["prefix2"] = &WallsSurveyParser::prefix2;
    result["prefix3"] = &WallsSurveyParser::prefix3;
    result["uvh"] = &WallsSurveyParser::uvh;
    result["uvv"] = &WallsSurveyParser::uvv;
    result["uv"] = &WallsSurveyParser::uv;
    result["flag"] = &WallsSurveyParser::flag;

    return result;
}

QHash<QString, OwnProduction> WallsSurveyParser::createDirectivesMap()
{
    QHash<QString, OwnProduction> result;
    result["#units"] = result["#u"] = &WallsSurveyParser::unitsLine;
    result["#flag"] = result["#f"] = &WallsSurveyParser::flagLine;
    result["#fix"] = &WallsSurveyParser::fixLine;
    result["#note"] = &WallsSurveyParser::noteLine;
    result["#symbol"] = result["#sym"] = &WallsSurveyParser::symbolLine;
    result["#segment"] = result["#seg"] = result["#s"] = &WallsSurveyParser::segmentLine;
    result["#date"] = &WallsSurveyParser::dateLine;
    result["#["] = &WallsSurveyParser::beginBlockCommentLine;
    result["#]"] = &WallsSurveyParser::endBlockCommentLine;
    result["#prefix1"] = result["#prefix2"] =
            result["#prefix3"] = result["#prefix"] = &WallsSurveyParser::prefixLine;
    return result;
}

double WallsSurveyParser::approx(double val)
{
    return floor(val * 1e6) * 1e-6;
}


const QHash<QString, Length::Unit> WallsSurveyParser::lengthUnits = WallsSurveyParser::createLengthUnits();
const QHash<QString, Angle::Unit> WallsSurveyParser::azmUnits = WallsSurveyParser::createAzmUnits();
const QHash<QString, Angle::Unit> WallsSurveyParser::incUnits = WallsSurveyParser::createIncUnits();
const QHash<QChar, Length::Unit> WallsSurveyParser::lengthUnitSuffixes = WallsSurveyParser::createLengthUnitSuffixes();
const QHash<QChar, Angle::Unit> WallsSurveyParser::azmUnitSuffixes = WallsSurveyParser::createAzmUnitSuffixes();
const QHash<QChar, Angle::Unit> WallsSurveyParser::incUnitSuffixes = WallsSurveyParser::createIncUnitSuffixes();
const QHash<QChar, CardinalDirection> WallsSurveyParser::cardinalDirections = WallsSurveyParser::createCardinalDirections();
const QHash<QChar, CardinalDirection> WallsSurveyParser::northSouth = WallsSurveyParser::createNorthSouth();
const QHash<QChar, CardinalDirection> WallsSurveyParser::eastWest = WallsSurveyParser::createEastWest();
const QHash<QChar, QChar> WallsSurveyParser::escapedChars = WallsSurveyParser::createEscapedChars();
const QHash<QChar, CtMeasurement> WallsSurveyParser::ctElements = WallsSurveyParser::createCtElements();
const QSet<CtMeasurement> WallsSurveyParser::requiredCtElements({CtMeasurement::D, CtMeasurement::A});
const QHash<QChar, RectMeasurement> WallsSurveyParser::rectElements = WallsSurveyParser::createRectElements();
const QSet<RectMeasurement> WallsSurveyParser::requiredRectElements({RectMeasurement::E, RectMeasurement::N});
const QHash<QChar, LrudMeasurement> WallsSurveyParser::lrudElements = WallsSurveyParser::createLrudElements();
const QSet<LrudMeasurement> WallsSurveyParser::requiredLrudElements({LrudMeasurement::L, LrudMeasurement::R, LrudMeasurement::U, LrudMeasurement::D});
const QHash<QString, bool> WallsSurveyParser::correctedValues = WallsSurveyParser::createCorrectedValues();
const QHash<QString, CaseType> WallsSurveyParser::caseTypes = WallsSurveyParser::createCaseTypes();
const QHash<QString, LrudType> WallsSurveyParser::lrudTypes = WallsSurveyParser::createLrudTypes();
const QHash<QString, QList<TapingMethodMeasurement>> WallsSurveyParser::tapingMethods = WallsSurveyParser::createTapingMethods();
const QHash<QString, int> WallsSurveyParser::prefixDirectives = WallsSurveyParser::createPrefixDirectives();

const QRegExp WallsSurveyParser::wordRx("\\w+");
const QRegExp WallsSurveyParser::notSemicolonRx("[^;]+");
const QRegExp WallsSurveyParser::unitsOptionRx("[a-zA-Z_0-9/]*");
const QRegExp WallsSurveyParser::directiveRx("#([][]|[a-zA-Z0-9]+)");
const QRegExp WallsSurveyParser::macroNameRx("[^()=,,# \t]*");
const QRegExp WallsSurveyParser::stationRx("[^;,,#/ \t]{0,8}");
const QRegExp WallsSurveyParser::prefixRx("[^:;,,#/ \t]+");

const QRegExp WallsSurveyParser::optionalRx("--+");
const QRegExp WallsSurveyParser::optionalStationRx("-+");

const QRegExp WallsSurveyParser::isoDateRx("\\d{4}-\\d{2}-\\d{2}");
const QRegExp WallsSurveyParser::usDateRx1("\\d{2}-\\d{2}-\\d{2,4}");
const QRegExp WallsSurveyParser::usDateRx2("\\d{2}/\\d{2}/\\d{2,4}");
const QRegExp WallsSurveyParser::usDateRx3("\\d{4}-\\d{1,2}-\\d{1,2}");

const QRegExp WallsSurveyParser::segmentPartRx("[^./\\;][^/\\;]+");

const QHash<QString, OwnProduction> WallsSurveyParser::unitsOptionMap = WallsSurveyParser::createUnitsOptionMap();
const QHash<QString, OwnProduction> WallsSurveyParser::directivesMap = WallsSurveyParser::createDirectivesMap();

const UAngle WallsSurveyParser::oneEighty = UAngle(180.0, Angle::Degrees);

WallsSurveyParser::WallsSurveyParser()
    : WallsSurveyParser(Segment())
{

}

WallsSurveyParser::WallsSurveyParser(QString line)
    : WallsSurveyParser(Segment(line))
{

}

WallsSurveyParser::WallsSurveyParser(Segment segment)
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

ULength WallsSurveyParser::unsignedLengthInches()
{
    expect('i', Qt::CaseInsensitive);
    double inches = unsignedDoubleLiteral();
    return ULength(inches, Length::Inches);
}

ULength WallsSurveyParser::unsignedLengthNonInches(Length::Unit defaultUnit)
{
    double value = unsignedDoubleLiteral();
    Length::Unit unit = oneOfMap(lengthUnitSuffixes, defaultUnit);
    if (unit == Length::Inches)
    {
        double inches = unsignedDoubleLiteral();
        return ULength(value * 12 + inches, Length::Inches);
    }
    return ULength(value, unit);
}

ULength WallsSurveyParser::unsignedLength(Length::Unit defaultUnit)
{
    ULength result;
    oneOfR(result,
           [&]() { return unsignedLengthNonInches(defaultUnit); },
    [&]() { return unsignedLengthInches(); });
    return result;
}

ULength WallsSurveyParser::length(Length::Unit defaultUnit)
{
    bool negate = maybe( [&]() { return expect('-'); } );
    ULength length = unsignedLength(defaultUnit);
    return negate ? -length : length;
}

UAngle WallsSurveyParser::unsignedAngle(QHash<QChar, Angle::Unit> unitSuffixes, Angle::Unit defaultUnit)
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
                      (hasSeconds ? seconds / 3600.0 : 0), Angle::Degrees);
    }
    else if (!hasValue)
    {
        throwAllExpected();
    }
    return UAngle(value, oneOfMap(unitSuffixes, defaultUnit));
}

UAngle WallsSurveyParser::unsignedDmsAngle()
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
                  (hasSeconds ? seconds / 3600.0 : 0), Angle::Degrees);
}

UAngle WallsSurveyParser::latitude()
{
    int start = _i;
    CardinalDirection side = oneOfMap(northSouth);
    UAngle latitude = unsignedDmsAngle();

    if (approx(latitude.get(Angle::Degrees)) > 90.0)
    {
        throw SegmentParseException(_line.mid(start, _i), "latitude out of range");
    }

    if (side == CardinalDirection::South)
    {
        return -latitude;
    }
    return latitude;
}

UAngle WallsSurveyParser::longitude()
{
    int start = _i;
    CardinalDirection side = oneOfMap(eastWest);
    UAngle longitude = unsignedDmsAngle();

    if (approx(longitude.get(Angle::Degrees)) > 180.0)
    {
        throw SegmentParseException(_line.mid(start, _i), "longitude out of range");
    }

    if (side == CardinalDirection::West)
    {
        return -longitude;
    }
    return longitude;
}

UAngle WallsSurveyParser::nonQuadrantAzimuth(Angle::Unit defaultUnit)
{
    int start = _i;

    UAngle result = unsignedAngle(azmUnitSuffixes, defaultUnit);

    if (approx(result.get(Angle::Degrees)) >= 360.0)
    {
        throw SegmentParseException(_line.mid(start, _i), "azimuth out of range");
    }

    return result;
}

UAngle WallsSurveyParser::quadrantAzimuth()
{
    CardinalDirection from = oneOfMap(cardinalDirections);

    int start = _i;
    UAngle angle;
    if (maybe(angle, [&]() { return nonQuadrantAzimuth(Angle::Degrees); }))
    {
        if (approx(angle.get(Angle::Degrees)) >= 90.0)
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

UAngle WallsSurveyParser::azimuth(Angle::Unit defaultUnit)
{
    UAngle result;
    oneOfR(result, [&]() { return quadrantAzimuth(); },
    [&]() { return nonQuadrantAzimuth(defaultUnit); });
    return result;
}

UAngle WallsSurveyParser::azimuthOffset(Angle::Unit defaultUnit)
{
    double signum;
    if (!maybe(signum, [this]() { return oneOfMap(signSignums); } ))
    {
        signum = 1.0;
    }
    return nonQuadrantAzimuth(defaultUnit) * signum;
}

UAngle WallsSurveyParser::unsignedInclination(Angle::Unit defaultUnit)
{
    int start = _i;
    UAngle result = unsignedAngle(incUnitSuffixes, defaultUnit);

    if (approx(result.get(Angle::Degrees)) > 90.0)
    {
        throw SegmentParseException(_line.mid(start, _i), "inclination out of range");
    }

    return result;
}

UAngle WallsSurveyParser::inclination(Angle::Unit defaultUnit)
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

VarianceOverridePtr WallsSurveyParser::varianceOverride(Length::Unit defaultUnit)
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

VarianceOverridePtr WallsSurveyParser::floatedVectorVarianceOverride()
{
    expect('?');
    return VarianceOverride::FLOATED;
}

VarianceOverridePtr WallsSurveyParser::floatedTraverseVarianceOverride()
{
    expect('*');
    return VarianceOverride::FLOATED_TRAVERSE;
}

VarianceOverridePtr WallsSurveyParser::lengthVarianceOverride(Length::Unit defaultUnit)
{
    return VarianceOverridePtr(new LengthOverride(unsignedLength(defaultUnit)));
}

VarianceOverridePtr WallsSurveyParser::rmsErrorVarianceOverride(Length::Unit defaultUnit)
{
    expect('r', Qt::CaseInsensitive);
    return VarianceOverridePtr(new RMSError(unsignedLength(defaultUnit)));
}

QString WallsSurveyParser::quotedTextOrNonwhitespace()
{
    QString result;
    oneOfR(result, [&]() { return quotedText(); },
    [&]() { return nonwhitespace().value(); });
    return result;
}

QString WallsSurveyParser::quotedText()
{
    expect('"');
    QString result = escapedText([](QChar c) { return c != '"'; }, {"<QUOTED TEXT>"});
    expect('"');
    return result;
}

QString WallsSurveyParser::movePastEndQuote()
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

void WallsSurveyParser::parseLine(QString line)
{
    reset(line);
    parseLine();
}

void WallsSurveyParser::parseLine(Segment line)
{
    reset(line);
    parseLine();
}

void WallsSurveyParser::parseLine()
{
    _parsedSegmentDirective = false;
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

void WallsSurveyParser::directiveLine()
{
    int start = _i;
    OwnProduction directive = oneOfMapLowercase(directiveRx, directivesMap);
    _i = start;
    if (directive != &WallsSurveyParser::fixLine) replaceMacros();
    (this->*directive)();
}

QString WallsSurveyParser::replaceMacro()
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

void WallsSurveyParser::replaceMacros()
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

void WallsSurveyParser::beginBlockCommentLine()
{
    maybeWhitespace();
    expect("#[");
    _inBlockComment = true;
}

void WallsSurveyParser::endBlockCommentLine()
{
    maybeWhitespace();
    expect("#]");
    remaining();
    _inBlockComment = false;
}

void WallsSurveyParser::insideBlockCommentLine()
{
    emit parsedComment(remaining().value());
}

Segment WallsSurveyParser::untilComment(std::initializer_list<QString> expectedItems)
{
    return expect(notSemicolonRx, expectedItems);
}

void WallsSurveyParser::segmentLine()
{
    maybeWhitespace();
    _segment = segmentDirective();
    maybeWhitespace();
    inlineCommentOrEndOfLine();
}

void WallsSurveyParser::segmentSeparator()
{
    oneOf([&]() { expect("/"); },
    [&]() { expect("\\"); });
}

QString WallsSurveyParser::initialSegmentPart()
{
    QString result;
    oneOfR(result, [&]() { expect("."); return "."; },
    [&]() { expect(".."); return ".."; });
    return result;
}

QString WallsSurveyParser::nonInitialSegmentPart()
{
    return expect(segmentPartRx, {"<PATH ELEMENT>"}).value();
}

QStringList WallsSurveyParser::segmentPath()
{
    QStringList path;

    oneOf([&]() {
        segmentSeparator();
        path = _rootSegment;
    },
    [&]() {
        path = _segment;
        while (maybe([&] {
            if (initialSegmentPart() == ".." && !path.isEmpty()) {
                path.removeLast();
            }
        }));
    });

    int lastAddedIndex = -1;
    while (maybe([&] {
        path << nonInitialSegmentPart();
        lastAddedIndex = path.size() - 1;
    }));

    if (lastAddedIndex >= 0) {
        QString lastPart = path[lastAddedIndex];
        lastPart.truncate(QRegExp("\\s*$").indexIn(lastPart));
        path[lastAddedIndex] = lastPart;
    }

    return path;
}

QStringList WallsSurveyParser::segmentDirective()
{
    oneOf([&]() { expect("#segment", Qt::CaseInsensitive); },
    [&]() { expect("#seg", Qt::CaseInsensitive); },
    [&]() { expect("#s", Qt::CaseInsensitive); } );

    _parsedSegmentDirective = true;

    QStringList result = _segment;

    if (maybeWhitespace())
    {
        maybe(result, [&]() { return segmentPath(); });
    }

    return result;
}

void WallsSurveyParser::prefixLine()
{
    maybeWhitespace();
    prefixDirective();
    maybeWhitespace();
    inlineCommentOrEndOfLine();
}

void WallsSurveyParser::prefixDirective()
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

void WallsSurveyParser::noteLine()
{
    maybeWhitespace();
    noteDirective();
    maybeWhitespace();
    inlineCommentOrEndOfLine();
}

void WallsSurveyParser::noteDirective()
{
    oneOf([&]() { expect("#note", Qt::CaseInsensitive); },
    [&]() { expect("#n", Qt::CaseInsensitive); });

    whitespace();
    QString station = expect(stationRx, {"<STATION NAME>"}).value();
    whitespace();
    QString _note = escapedText([](QChar c) { return c != ';'; }, {"<NOTE>"});

    emit parsedNote(station, _note);
}

void WallsSurveyParser::flagLine()
{
    maybeWhitespace();
    flagDirective();
    maybeWhitespace();
    inlineCommentOrEndOfLine();
}

void WallsSurveyParser::flagDirective()
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

QString WallsSurveyParser::slashPrefixedFlag()
{
    expect('/');
    return expect(notSemicolonRx, {"<FLAG>"}).value();
}

void WallsSurveyParser::symbolLine()
{
    maybeWhitespace();

    oneOf([&]() { expect("#symbol", Qt::CaseInsensitive); },
    [&]() { expect("#sym", Qt::CaseInsensitive); });

    // ignore the rest for now
    remaining();
}

void WallsSurveyParser::dateLine()
{
    maybeWhitespace();
    emit parsedDate(dateDirective());
    maybeWhitespace();
    inlineCommentOrEndOfLine();
}

QDate WallsSurveyParser::dateDirective()
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

QDate WallsSurveyParser::isoDate()
{
    Segment dateSegment = expect(isoDateRx, {"<DATE>"});
    return QDate::fromString(dateSegment.value(), Qt::ISODate);
}

QDate WallsSurveyParser::usDate1()
{
    QString str = expect(usDateRx1, {"<DATE>"}).value();
    return QDate::fromString(str, str.length() > 8 ? "MM-dd-yyyy" : "MM-dd-yy");
}

QDate WallsSurveyParser::usDate2()
{
    QString str = expect(usDateRx2, {"<DATE>"}).value();
    return QDate::fromString(str, str.length() > 8 ? "MM/dd/yyyy" : "MM/dd/yy");
}

QDate WallsSurveyParser::usDate3()
{
    QString str = expect(usDateRx3, {"<DATE>"}).value();
    return QDate::fromString(str, "yyyy-MM-dd");
}
void WallsSurveyParser::unitsLine()
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

void WallsSurveyParser::parseUnitsOptions(Segment options)
{
    reset(options);
    unitsOptions();
    emit parsedUnits();
}

void WallsSurveyParser::unitsOptions()
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

void WallsSurveyParser::unitsOption()
{
    void (WallsSurveyParser::*option)() = oneOfMapLowercase(unitsOptionRx, unitsOptionMap);
    (this->*option)();
}

void WallsSurveyParser::macroOption()
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

void WallsSurveyParser::save()
{
    if (_stack.size() >= 10)
    {
        throw SegmentParseException(_line.mid(_i - 4, _i), "units stack is full");
    }
    _stack.push(_units);
}

void WallsSurveyParser::restore()
{
    if (_stack.isEmpty())
    {
        throw SegmentParseException(_line.mid(_i - 7, _i), "units stack is empty");
    }
    _units = _stack.pop();
}

void WallsSurveyParser::reset_()
{
    _units = WallsUnits();
}

void WallsSurveyParser::meters()
{
    _units.setDUnit(Length::Meters);
    _units.setSUnit(Length::Meters);
}

void WallsSurveyParser::feet()
{
    _units.setDUnit(Length::Feet);
    _units.setSUnit(Length::Feet);
}

void WallsSurveyParser::ct()
{
    _units.setVectorType(VectorType::CT);
}

void WallsSurveyParser::d()
{
    expect('=');
    _units.setDUnit(oneOfMapLowercase(nonwhitespaceRx, lengthUnits));
}

void WallsSurveyParser::s()
{
    expect('=');
    _units.setSUnit(oneOfMapLowercase(nonwhitespaceRx, lengthUnits));
}

void WallsSurveyParser::a()
{
    expect('=');
    _units.setAUnit(oneOfMapLowercase(nonwhitespaceRx, azmUnits));
}

void WallsSurveyParser::ab()
{
    expect('=');
    _units.setAbUnit(oneOfMapLowercase(nonwhitespaceRx, azmUnits));
}

void WallsSurveyParser::a_ab()
{
    expect('=');
    Angle::Unit unit = oneOfMapLowercase(nonwhitespaceRx, azmUnits);
    _units.setAUnit(unit);
    _units.setAbUnit(unit);
}

void WallsSurveyParser::v()
{
    expect('=');
    _units.setVUnit(oneOfMapLowercase(nonwhitespaceRx, incUnits));
}

void WallsSurveyParser::vb()
{
    expect('=');
    _units.setVbUnit(oneOfMapLowercase(nonwhitespaceRx, incUnits));
}

void WallsSurveyParser::v_vb()
{
    expect('=');
    Angle::Unit unit = oneOfMapLowercase(nonwhitespaceRx, incUnits);
    _units.setVUnit(unit);
    _units.setVbUnit(unit);
}

void WallsSurveyParser::order()
{
    expect('=');
    oneOf([&]() { ctOrder(); },
    [&]() { rectOrder(); });
}

void WallsSurveyParser::ctOrder()
{
    _units.setCtOrder(elementChars(ctElements, requiredCtElements));
}

void WallsSurveyParser::rectOrder()
{
    _units.setRectOrder(elementChars(rectElements, requiredRectElements));
}

void WallsSurveyParser::decl()
{
    expect('=');
    _units.setDecl(azimuthOffset(_units.aUnit()));
}

void WallsSurveyParser::grid()
{
    expect('=');
    _units.setGrid(azimuthOffset(_units.aUnit()));
}

void WallsSurveyParser::rect()
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

void WallsSurveyParser::incd()
{
    expect('=');
    _units.setIncd(length(_units.dUnit()));
}

void WallsSurveyParser::inch()
{
    expect('=');
    _units.setInch(length(_units.sUnit()));
}

void WallsSurveyParser::incs()
{
    expect('=');
    _units.setIncs(length(_units.sUnit()));
}

void WallsSurveyParser::inca()
{
    expect('=');
    _units.setInca(azimuthOffset(_units.aUnit()));
}

void WallsSurveyParser::incab()
{
    expect('=');
    _units.setIncab(azimuthOffset(_units.abUnit()));
}

void WallsSurveyParser::incv()
{
    expect('=');
    _units.setIncv(inclination(_units.vUnit()));
}

void WallsSurveyParser::incvb()
{
    expect('=');
    _units.setIncvb(inclination(_units.vbUnit()));
}

void WallsSurveyParser::typeab()
{
    expect('=');
    _units.setTypeabCorrected(oneOfMapLowercase(wordRx, correctedValues));
    if (maybeChar(','))
    {
        _units.setTypeabTolerance(UAngle(unsignedDoubleLiteral(), Angle::Degrees));
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
        _units.setTypeabTolerance(UAngle(2.0, Angle::Degrees));
    }
}

void WallsSurveyParser::typevb()
{
    expect('=');
    _units.setTypevbCorrected(oneOfMapLowercase(wordRx, correctedValues));
    if (maybeChar(','))
    {
        _units.setTypevbTolerance(UAngle(unsignedDoubleLiteral(), Angle::Degrees));
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
        _units.setTypevbTolerance(UAngle(2.0, Angle::Degrees));
    }
}

void WallsSurveyParser::case_()
{
    expect('=');
    _units.setCase(oneOfMapLowercase(nonwhitespaceRx, caseTypes));
}

void WallsSurveyParser::lrud()
{
    expect('=');
    _units.setLrud(oneOfMapLowercase(wordRx, lrudTypes));
    if (maybeChar(':'))
    {
        lrudOrder();
    }
    else
    {
        _units.setLrudOrder(QList<LrudMeasurement>({LrudMeasurement::L, LrudMeasurement::R, LrudMeasurement::U, LrudMeasurement::D}));
    }
}

void WallsSurveyParser::lrudOrder()
{
    _units.setLrudOrder(elementChars(lrudElements, requiredLrudElements));
}

void WallsSurveyParser::prefix1()
{
    prefix(0);
}

void WallsSurveyParser::prefix2()
{
    prefix(1);
}

void WallsSurveyParser::prefix3()
{
    prefix(2);
}

void WallsSurveyParser::prefix(int index)
{
    QString prefix;

    if (maybeChar('='))
    {
        prefix = expect(prefixRx, {"<PREFIX>"}).value();
    }
    _units.setPrefix(index, prefix);
}

void WallsSurveyParser::tape()
{
    expect('=');
    _units.setTape(oneOfMapLowercase(nonwhitespaceRx, tapingMethods));
}

void WallsSurveyParser::uvh()
{
    expect('=');
    _units.setUvh(unsignedDoubleLiteral());
}

void WallsSurveyParser::uvv()
{
    expect('=');
    _units.setUvv(unsignedDoubleLiteral());
}

void WallsSurveyParser::uv()
{
    expect('=');
    double value = unsignedDoubleLiteral();
    _units.setUvv(value);
    _units.setUvh(value);
}

void WallsSurveyParser::flag()
{
    QString flag;
    if (maybeChar('='))
    {
        flag = quotedTextOrNonwhitespace();
    }
    _units.setFlag(flag);
}

void WallsSurveyParser::vectorLine()
{
    maybeWhitespace();
    fromStation();
    _parsedSegmentDirective = false;
    whitespace();
    afterFromStation();
    maybeWhitespace();
    endOfLine();
    if (_parsedSegmentDirective) {
        _vector.setSegment(_segment);
    }
    _vector.setDate(_date);
    _vector.setUnits(_units);
    emit parsedVector(_vector);
}

void WallsSurveyParser::fromStation()
{
    _fromStationSegment = expect(stationRx, {"<STATION NAME>"}).value();
    QString from = _fromStationSegment.value();
    if (optionalStationRx.exactMatch(from)) {
        from.clear();
    }
    _vector = Vector();
    _vector.setFrom(from);
}

void WallsSurveyParser::afterFromStation()
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

void WallsSurveyParser::toStation()
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

void WallsSurveyParser::afterToStation()
{
    int k = 0;
    if (_units.vectorType() == VectorType::RECT)
    {
        foreach(RectMeasurement elem, _units.rectOrder())
        {
            if (k++ > 0)
            {
                whitespace();
            }
            rectMeasurement(elem);
        }
    }
    else
    {
        foreach(CtMeasurement elem, _units.ctOrder())
        {
            if (k++ > 0)
            {
                whitespace();
            }
            ctMeasurement(elem);
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

        tapingMethodMeasurement();
    }

//    maybeWhitespace();
    afterVectorMeasurements();
}

void WallsSurveyParser::rectMeasurement(RectMeasurement elem)
{
    switch(elem) {
    case RectMeasurement::E:
        east();
        break;
    case RectMeasurement::N:
        north();
        break;
    case RectMeasurement::U:
        rectUp();
        break;
    }
}

void WallsSurveyParser::east()
{
    _vector.setEast(length(_units.dUnit()));
}

void WallsSurveyParser::north()
{
    _vector.setNorth(length(_units.dUnit()));
}

void WallsSurveyParser::rectUp()
{
    _vector.setRectUp(length(_units.dUnit()));
}

void WallsSurveyParser::ctMeasurement(CtMeasurement elem)
{
    switch(elem)
    {
    case CtMeasurement::D:
        distance();
        break;
    case CtMeasurement::A:
        azimuth();
        break;
    case CtMeasurement::V:
        inclination();
        break;
    }
}

void WallsSurveyParser::checkCorrectedSign(int segStart, ULength measurement, ULength correction) {
    if (measurement.isNonzero() && correction.isNonzero() &&
            measurement.signum() != (measurement + correction).signum()) {
        throw SegmentParseException(_line.mid(segStart, _i - segStart), "correction changes sign of measurement");
    }
}

void WallsSurveyParser::distance()
{
    int start = _i;
    ULength dist = unsignedLength(_units.dUnit());
    checkCorrectedSign(start, dist, _units.incd());
    _vector.setDistance(dist);
}

UAngle WallsSurveyParser::azmDifference(UAngle fs, UAngle bs) {
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
    return diff > oneEighty ? UAngle(360.0, Angle::Degrees) - diff : diff;
}

void WallsSurveyParser::azimuth()
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

UAngle WallsSurveyParser::incDifference(UAngle fs, UAngle bs) {
    return _units.typevbCorrected() ? uabs(fs - bs) : uabs(fs + bs);
}

void WallsSurveyParser::inclination()
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

void WallsSurveyParser::tapingMethodMeasurement()
{
    foreach(TapingMethodMeasurement elem, _units.tape())
    {
        if (!maybeWithLookahead([&]() {
                   whitespace();
                   tapingMethodMeasurement(elem); }))
        {
            break;
        }
    }
}

void WallsSurveyParser::tapingMethodMeasurement(TapingMethodMeasurement elem)
{
    switch(elem)
    {
    case TapingMethodMeasurement::InstrumentHeight:
        instrumentHeight();
        break;
    case TapingMethodMeasurement::TargetHeight:
        targetHeight();
        break;
    }
}

void WallsSurveyParser::instrumentHeight()
{
    int start = _i;
    ULength ih;
    if (optional(ih, [&]() { return length(_units.sUnit()); }))
    {
        checkCorrectedSign(start, ih, _units.incs());
        _vector.setInstHeight(ih);
    }
}

void WallsSurveyParser::targetHeight()
{
    int start = _i;
    ULength th;
    if (optional(th, [&]() { return length(_units.sUnit()); }))
    {
        checkCorrectedSign(start, th, _units.incs());
        _vector.setTargetHeight(th);
    }
}

void WallsSurveyParser::lrudMeasurement(LrudMeasurement elem)
{
    switch(elem)
    {
    case LrudMeasurement::L:
        left();
        break;
    case LrudMeasurement::R:
        right();
        break;
    case LrudMeasurement::U:
        up();
        break;
    case LrudMeasurement::D:
        down();
        break;
    }
}

void WallsSurveyParser::left()
{
    int start = _i;
    ULength left;
    if (optional(left, [&]() { return unsignedLength(_units.sUnit()); }))
    {
        checkCorrectedSign(start, left, _units.incs());
        _vector.setLeft(left);
    }
}

void WallsSurveyParser::right()
{
    int start = _i;
    ULength right;
    if (optional(right, [&]() { return unsignedLength(_units.sUnit()); }))
    {
        checkCorrectedSign(start, right, _units.incs());
        _vector.setRight(right);
    }
}

void WallsSurveyParser::up()
{
    int start = _i;
    ULength up;
    if (optional(up, [&]() { return unsignedLength(_units.sUnit()); }))
    {
        checkCorrectedSign(start, up, _units.incs());
        _vector.setUp(up);
    }
}

void WallsSurveyParser::down()
{
    int start = _i;
    ULength down;
    if (optional(down, [&]() { return unsignedLength(_units.sUnit()); }))
    {
        checkCorrectedSign(start, down, _units.incs());
        _vector.setDown(down);
    }
}

void WallsSurveyParser::afterVectorMeasurements()
{
    maybeWithLookahead([&]() {
        whitespace();
        varianceOverrides(_vector);
    });
//    if (maybe([&]() { return varianceOverrides(_vector); }))
//    {
//        maybeWhitespace();
//    }
    afterVectorVarianceOverrides();
}

template<class T>
void WallsSurveyParser::varianceOverrides(T& target)
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

void WallsSurveyParser::afterVectorVarianceOverrides()
{
    maybeWithLookahead([&]() {
        whitespace();
        lruds();
    });
//    if (maybe([&]() { lruds(); }))
//    {
//        maybeWhitespace();
//    }
    afterLruds();
}

void WallsSurveyParser::lruds()
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

void WallsSurveyParser::lrudContent()
{
    oneOfWithLookahead([&]() {
        commaDelimLrudContent();
    }, [&]() {
        whitespaceDelimLrudContent();
    });
}

void WallsSurveyParser::commaDelimLrudContent()
{
    maybeWhitespace();
    int m = 0;
    foreach(LrudMeasurement elem, _units.lrudOrder())
    {
        if (m++ > 0)
        {
            maybeWhitespace();
            expect(',');
            maybeWhitespace();
        }
        lrudMeasurement(elem);
    }
    maybeWhitespace();
    afterRequiredCommaDelimLrudMeasurements();
}

void WallsSurveyParser::whitespaceDelimLrudContent()
{
    maybeWhitespace();
    int m = 0;
    foreach(LrudMeasurement elem, _units.lrudOrder())
    {
        if (m++ > 0)
        {
            whitespace();
        }
        lrudMeasurement(elem);
    }
    maybeWhitespace();
    afterRequiredWhitespaceDelimLrudMeasurements();
}

void WallsSurveyParser::afterRequiredCommaDelimLrudMeasurements()
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

void WallsSurveyParser::afterRequiredWhitespaceDelimLrudMeasurements()
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

void WallsSurveyParser::lrudFacingAngle()
{
    _vector.setLrudAngle(azimuth(_units.aUnit()));
}

void WallsSurveyParser::lrudCFlag()
{
    expect('c', Qt::CaseInsensitive);
    _vector.setCFlag(true);
}

void WallsSurveyParser::afterLruds()
{
    if (maybe([&]() { inlineDirective(); }))
    {
        maybeWhitespace();
    }
    inlineCommentOrEndOfLine();
}

void WallsSurveyParser::inlineDirective()
{
    // currently this is the only directive that can be on a vector line
    inlineSegmentDirective(_vector);
}

void WallsSurveyParser::inlineFixDirective()
{
    // currently this is the only directive that can be on a fix station line
    inlineSegmentDirective(_fixStation);
}

template<class T>
void WallsSurveyParser::inlineSegmentDirective(T& target)
{
    target.setSegment(segmentDirective());
}

void WallsSurveyParser::fixLine()
{
    maybeWhitespace();
    expect("#fix", Qt::CaseInsensitive);
    whitespace();
    fixedStation();
    whitespace();
    _parsedSegmentDirective = false;
    afterFixedStation();
    maybeWhitespace();
    endOfLine();
    if (!_parsedSegmentDirective)
    {
        _fixStation.setSegment(_segment);
    }
    _fixStation.setDate(_date);
    _fixStation.setUnits(_units);
    emit parsedFixStation(_fixStation);
}

void WallsSurveyParser::fixedStation()
{
    QString fixed = expect(stationRx, {"<STATION NAME>"}).value();
    _fixStation = FixStation();
    _fixStation.setName(fixed);
}

void WallsSurveyParser::afterFixedStation()
{
    int k = 0;
    foreach(RectMeasurement elem, _units.rectOrder())
    {
        if (k++ > 0)
        {
            whitespace();
        }
        fixRectMeasurement(elem);
    }
    maybeWhitespace();
    afterFixMeasurements();
}

void WallsSurveyParser::fixRectMeasurement(RectMeasurement elem)
{
    switch(elem)
    {
    case RectMeasurement::E:
        fixEast();
        break;
    case RectMeasurement::N:
        fixNorth();
        break;
    case RectMeasurement::U:
        fixUp();
        break;
    }
}

void WallsSurveyParser::fixEast()
{
    oneOf([&]() { _fixStation.setEast(length(_units.dUnit())); },
    [&]() { _fixStation.setLongitude(longitude()); });
}

void WallsSurveyParser::fixNorth()
{
    oneOf([&]() { _fixStation.setNorth(length(_units.dUnit())); },
    [&]() { _fixStation.setLatitude(latitude()); });
}

void WallsSurveyParser::fixUp()
{
    _fixStation.setRectUp(length(_units.dUnit()));
}

void WallsSurveyParser::afterFixMeasurements()
{
    if (maybe([&]() { varianceOverrides(_fixStation); }))
    {
        maybeWhitespace();
    }
    afterFixVarianceOverrides();
}

void WallsSurveyParser::afterFixVarianceOverrides()
{
    if (maybe([&]() { inlineNote(_fixStation); }))
    {
        maybeWhitespace();
    }
    afterInlineFixNote();
}

template<class T>
void WallsSurveyParser::inlineNote(T& target)
{
    expect('/');
    target.setNote(escapedText([](QChar c) { return c != ';' && c != '#'; }, {"<NOTE>"}).trimmed());
}

void WallsSurveyParser::afterInlineFixNote()
{
    if (maybe([&]() { inlineFixDirective(); }))
    {
        maybeWhitespace();
    }
    inlineCommentOrEndOfLine(_fixStation);
}

void WallsSurveyParser::inlineCommentOrEndOfLine()
{
    oneOf([&]() { inlineComment(); },
    [&]() { endOfLine(); });
}

template<class T>
void WallsSurveyParser::inlineCommentOrEndOfLine(T& target)
{
    oneOf([&]() { inlineComment(target); },
    [&]() { endOfLine(); });
}

void WallsSurveyParser::comment()
{
    expect(';');
    emit parsedComment(remaining().value());
}

void WallsSurveyParser::inlineComment()
{
    expect(';');
    emit parsedComment(remaining().value());
}

template<class T>
void WallsSurveyParser::inlineComment(T& target)
{
    expect(';');
    target.setComment(remaining().value());
}


} // namespace dewalls
