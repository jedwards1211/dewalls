#include "wallsparser.h"
#include "unitizedmath.h"

namespace dewalls {

typedef UnitizedDouble<Length> ULength;
typedef UnitizedDouble<Angle>  UAngle;
typedef const Unit<Length> * LengthUnit;
typedef const Unit<Angle>  * AngleUnit;
typedef QSharedPointer<VarianceOverride> VarianceOverridePtr;
typedef void (WallsParser::*OwnProduction)();

QHash<QString, LengthUnit> createLengthUnits()
{
    QHash<QString, LengthUnit> result;
    result["meters"] = result["meter"] = result["m"] = Length::meters();
    result["feet"] = result["foot"] = result["ft"] = result["f"] = Length::feet();
    return result;
}

QHash<QString, AngleUnit> createAzmUnits()
{
    QHash<QString, AngleUnit> result;
    result["degree"] = result["degree"] = result["deg"] = result["d"] = Angle::degrees();
    result["mills"] = result["mils"] = result["mil"] = result["m"] = Angle::milsNATO();
    result["grads"] = result["grad"] = result["g"] = Angle::gradians();
    return result;
}

QHash<QString, AngleUnit> createIncUnits()
{
    QHash<QString, AngleUnit> result;
    result["degrees"] = result["degree"] = result["deg"] = result["d"] = Angle::degrees();
    result["mills"] = result["mils"] = result["mil"] = result["m"] = Angle::milsNATO();
    result["grads"] = result["grad"] = result["g"] = Angle::gradians();
    result["percent"] = result["p"] = Angle::percentGrade();
    return result;
}

QHash<QChar, LengthUnit> createLengthUnitSuffixes()
{
    QHash<QChar, LengthUnit> result;
    result['m'] = result['M'] = Length::meters();
    result['f'] = result['F'] = Length::feet();
    result['i'] = result['I'] = Length::inches();
    return result;
}

QHash<QChar, AngleUnit> createAzmUnitSuffixes()
{
    QHash<QChar, AngleUnit> result;
    result['d'] = result['D'] = Angle::degrees();
    result['g'] = result['G'] = Angle::gradians();
    result['m'] = result['M'] = Angle::milsNATO();
    return result;
}

QHash<QChar, AngleUnit> createIncUnitSuffixes()
{
    QHash<QChar, AngleUnit> result;
    result['d'] = result['D'] = Angle::degrees();
    result['g'] = result['G'] = Angle::gradians();
    result['m'] = result['M'] = Angle::milsNATO();
    result['p'] = result['P'] = Angle::percentGrade();
    return result;
}

QHash<QChar, CardinalDirection> createCardinalDirections()
{
    QHash<QChar, CardinalDirection> result;
    result['n'] = result['N'] = CardinalDirection::North;
    result['s'] = result['S'] = CardinalDirection::South;
    result['e'] = result['E'] = CardinalDirection::East;
    result['w'] = result['W'] = CardinalDirection::West;
    return result;
}


QHash<QChar, CardinalDirection> createNorthSouth()
{
    QHash<QChar, CardinalDirection> result;
    result['n'] = result['N'] = CardinalDirection::North;
    result['s'] = result['S'] = CardinalDirection::South;
    return result;
}

QHash<QChar, CardinalDirection> createEastWest()
{
    QHash<QChar, CardinalDirection> result;
    result['e'] = result['E'] = CardinalDirection::East;
    result['w'] = result['W'] = CardinalDirection::West;
    return result;
}

QHash<QChar, QChar> createEscapedChars()
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

QHash<QChar, CtElement> createCtElements()
{
    QHash<QChar, CtElement> result;
    result['d'] = result['D'] = CtElement::D;
    result['a'] = result['A'] = CtElement::A;
    result['v'] = result['V'] = CtElement::V;
    return result;
}

QHash<QChar, RectElement> createRectElements()
{
    QHash<QChar, RectElement> result;
    result['e'] = result['E'] = RectElement::E;
    result['n'] = result['N'] = RectElement::N;
    result['u'] = result['U'] = RectElement::U;
    return result;
}

QHash<QChar, LrudElement> createLrudElements()
{
    QHash<QChar, LrudElement> result;
    result['l'] = result['L'] = LrudElement::L;
    result['r'] = result['R'] = LrudElement::R;
    result['u'] = result['U'] = LrudElement::U;
    result['d'] = result['D'] = LrudElement::D;
    return result;
}

QHash<QString, bool> createCorrectedValues()
{
    QHash<QString, bool> result;
    result["corrected"] = result["c"] = true;
    result["normal"] = result["n"] = false;
    return result;
}

QHash<QString, CaseType> createCaseTypes()
{
    QHash<QString, CaseType> result;
    result["upper"] = result["u"] = CaseType::Upper;
    result["lower"] = result["l"] = CaseType::Lower;
    result["mixed"] = result["m"] = CaseType::Mixed;
    return result;
}

QHash<QString, LrudType> createLrudTypes()
{
    QHash<QString, LrudType> result;
    result["from"] = result["f"] = LrudType::From;
    result["fb"] = LrudType::FB;
    result["to"] = result["t"] = LrudType::To;
    result["tb"] = LrudType::TB;
    return result;
}

QHash<QString, QList<TapingMethodElement>> createTapingMethods()
{
    typedef QList<TapingMethodElement> Method;
    QHash<QString, QList<TapingMethodElement>> result;
    result["it"] = Method({TapingMethodElement::InstrumentHeight, TapingMethodElement::TargetHeight});
    result["is"] = Method({TapingMethodElement::InstrumentHeight});
    result["st"] = Method({TapingMethodElement::TargetHeight});
    result["ss"] = Method();
    return result;
}

QHash<QString, int> createPrefixDirectives()
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

const UAngle WallsParser::oneEighty = UAngle(180.0, Angle::degrees());

double WallsParser::approx(double val)
{
    return floor(val * 1e6) * 1e-6;
}

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
      lengthUnits(createLengthUnits()),
      azmUnits(createAzmUnits()),
      incUnits(createIncUnits()),
      lengthUnitSuffixes(createLengthUnitSuffixes()),
      azmUnitSuffixes(createAzmUnitSuffixes()),
      incUnitSuffixes(createIncUnitSuffixes()),
      cardinalDirections(createCardinalDirections()),
      northSouth(createNorthSouth()),
      eastWest(createEastWest()),
      escapedChars(createEscapedChars()),
      ctElements(createCtElements()),
      requiredCtElements({CtElement::D, CtElement::A}),
      rectElements(createRectElements()),
      requiredRectElements({RectElement::E, RectElement::N}),
      lrudElements(createLrudElements()),
      requiredLrudElements({LrudElement::L, LrudElement::R, LrudElement::U, LrudElement::D}),
      correctedValues(createCorrectedValues()),
      caseTypes(createCaseTypes()),
      lrudTypes(createLrudTypes()),
      tapingMethods(createTapingMethods()),
      prefixDirectives(createPrefixDirectives()),
      wordRx(QRegExp("\\w+")),
      notSemicolonRx(QRegExp("[^;]+")),
      unitsOptionRx(QRegExp("[a-zA-Z_0-9/]*")),
      directiveRx(QRegExp("#([][]|[a-zA-Z0-9]+)")),
      macroNameRx(QRegExp("[^()=,,# \t]*")),
      stationRx(QRegExp("[^<*,,#/ \t][^,,#/ \t]{0,7}")),
      prefixRx(QRegExp("[^:,,#/ \t]+")),
      optionalRx(QRegExp("--+")),
      optionalStationRx(QRegExp("-+")),
      isoDateRx(QRegExp("\\d{4}-\\d{2}-\\d{2}")),
      usDateRx1(QRegExp("\\d{2}-\\d{2}-\\d{2,4}")),
      usDateRx2(QRegExp("\\d{2}/\\d{2}/\\d{2,4}")),
      usDateRx3(QRegExp("\\d{4}-\\d{1,2}-\\d{1,2}")),
      unitsOptionMap(createUnitsOptionMap()),
      directivesMap(createDirectivesMap()),
      _visitor(NULL),
      _inBlockComment(false),
      _units(QSharedPointer<WallsUnits>(new WallsUnits())),
      _stack(QStack<QSharedPointer<WallsUnits>>()),
      _segment(),
      _date()
{

}

WallsParser& WallsParser::operator =(const WallsParser& rhs)
{
    _visitor = rhs._visitor;
    _inBlockComment = rhs._inBlockComment;
    _units = QSharedPointer<WallsUnits>(new WallsUnits(*rhs._units));
    _stack.clear();
    foreach (QSharedPointer<WallsUnits> stackItem, rhs._stack)
    {
        _stack.push(QSharedPointer<WallsUnits>(new WallsUnits(*stackItem)));
    }
    _segment = rhs._segment;
    _date = rhs._date;
    return *this;
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
          [&]() { return this->unsignedLengthNonInches(defaultUnit); },
          [&]() { return this->unsignedLengthInches(); });
    return result;
}

ULength WallsParser::length(LengthUnit defaultUnit)
{
    bool negate = maybe( [&]() { return this->expect('-'); } );
    ULength length = unsignedLength(defaultUnit);
    return negate ? -length : length;
}

UAngle WallsParser::unsignedAngle(QHash<QChar, AngleUnit> unitSuffixes, AngleUnit defaultUnit)
{
    auto this_expectColon = [&]() { this->expect(':'); };

    double value;
    bool hasValue = maybeOwn(value, &WallsParser::unsignedDoubleLiteral);
    if (maybe(this_expectColon))
    {
        double minutes, seconds;
        bool hasMinutes = maybeOwn(minutes, &WallsParser::unsignedDoubleLiteral);
        bool hasSeconds = false;
        if (maybe(this_expectColon))
        {
            hasSeconds = maybeOwn(seconds, &WallsParser::unsignedDoubleLiteral);
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
    auto this_expectColon = [&]() { this->expect(':'); };

    double degrees, minutes, seconds;
    bool hasDegrees = maybeOwn(degrees, &WallsParser::unsignedDoubleLiteral);
    expect(':');
    bool hasMinutes = maybeOwn(minutes, &WallsParser::unsignedDoubleLiteral);
    bool hasSeconds = false;
    if (maybe(this_expectColon))
    {
        hasSeconds = maybeOwn(seconds, &WallsParser::unsignedDoubleLiteral);
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
    if (maybe(angle, [&]() { return this->nonQuadrantAzimuth(Angle::degrees()); }))
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
    oneOfR(result, [&]() { return this->quadrantAzimuth(); },
                   [&]() { return this->nonQuadrantAzimuth(defaultUnit); });
    return result;
}

UAngle WallsParser::azimuthOffset(AngleUnit defaultUnit)
{
    double signum;
    if (!maybe(signum, [this]() { return this->oneOfMap(signSignums); } ))
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
    bool hasSignum = maybe(signum, [this]() { return this->oneOfMap(signSignums); } );
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
           [&]() { return this->floatedVectorVarianceOverride(); },
           [&]() { return this->floatedTraverseVarianceOverride(); },
           [&]() { return this->lengthVarianceOverride(defaultUnit); },
           [&]() { return this->rmsErrorVarianceOverride(defaultUnit); },
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
    oneOfR(result, [&]() { return this->quotedText(); },
    [&]() { return this->nonwhitespace().value(); });
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
            this->oneOfWithLookahead([&]() { this->endBlockCommentLine(); },
            [&]() { this->insideBlockCommentLine(); });
        });
    }
    else
    {
        throwAllExpected([&]() { this->oneOf([&]() { this->comment(); },
            [&]() { this->directiveLine(); },
            [&]() { this->vectorLine(); });
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
            if (!_units->macros.contains(macroName.value()))
            {
                throw SegmentParseException(macroName, "macro not defined");
            }
            return _units->macros[macroName.value()];
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
    _visitor->visitBlockCommentLine(remaining().value());
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
    _visitor->visitBlockCommentLine(remaining().value());
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
    oneOf([&]() { this->expect("#segment", Qt::CaseInsensitive); },
    [&]() { this->expect("#seg", Qt::CaseInsensitive); },
    [&]() { this->expect("#s", Qt::CaseInsensitive); } );

    if (maybeWhitespace())
    {
        Segment result;
        maybe(result, [&]() { return this->untilComment({"<SEGMENT>"}); });
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
            if (baseParts.isEmpty())
            {
                throw SegmentParseException(part, "already at root segment, can't go up");
            }
            baseParts.removeLast();
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
        if (maybe(segment, [&]() { return this->expect(prefixRx, {"<PREFIX>"}); }))
        {
            prefix = segment.value();
        }
    }

    _units->setPrefix(prefixIndex, prefix);
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
    oneOf([&]() { this->expect("#note", Qt::CaseInsensitive); },
    [&]() { this->expect("#n", Qt::CaseInsensitive); });

    whitespace();
    QString station = expect(stationRx, {"<STATION NAME>"}).value();
    whitespace();
    QString note = escapedText([](QChar c) { return c != ';'; }, {"<NOTE>"});

    _visitor->visitNoteLine(station, note);
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
    oneOf([&]() { this->expect("#flag", Qt::CaseInsensitive); },
    [&]() { this->expect("#f", Qt::CaseInsensitive); });

    QStringList stations;

    maybeWhitespace();

    while(true)
    {
        Segment station;
        if (!maybe([&]() { return this->expect(stationRx, {"<STATION NAME>"}); }))
        {
            break;
        }
        stations << station.value();

        maybeWhitespace();
        maybeChar(',');
        maybeWhitespace();
    }

    QString flag;
    bool hasFlag = maybe(flag, [&]() { return this->slashPrefixedFlag(); });
    maybeWhitespace();

    if (stations.isEmpty())
    {
        _units->flag = flag;
    }
    else
    {
        if (!hasFlag)
        {
            throwAllExpected();
        }
        _visitor->visitFlaggedStations(flag, stations);
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

    oneOf([&]() { this->expect("#symbol", Qt::CaseInsensitive); },
    [&]() { this->expect("#sym", Qt::CaseInsensitive); });

    // ignore the rest for now
    remaining();
}

void WallsParser::dateLine()
{
    maybeWhitespace();
    _visitor->visitDateLine(dateDirective());
    maybeWhitespace();
    inlineCommentOrEndOfLine();
}

QDate WallsParser::dateDirective()
{
    expect("#date", Qt::CaseInsensitive);
    whitespace();
    oneOfR(_date,
    [&]() { return this->isoDate(); },
    [&]() { return this->usDate1(); },
    [&]() { return this->usDate2(); },
    [&]() { return this->usDate3(); });
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
    oneOf([&]() { this->expect("#units", Qt::CaseInsensitive); },
    [&]() { this->expect("#u", Qt::CaseInsensitive); });

    _visitor->beginUnitsLine();

    try
    {
        unitsOptions();
        _visitor->endUnitsLine();
    }
    catch (const SegmentParseExpectedException& ex)
    {
        _visitor->abortUnitsLine();
        throw ex;
    }
    catch (const SegmentParseException& ex)
    {
        _visitor->abortUnitsLine();
        throw ex;
    }
}

void WallsParser::unitsOptions()
{
    while(!maybe([&]() { this->inlineCommentOrEndOfLine(); } ))
    {
        whitespace();
        maybe([&]() {
            this->oneOf(
                        [&]() { this->unitsOption(); },
                        [&]() { this->macroOption(); }
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
    _units->macros[macroName] = macroValue;
}

void WallsParser::save()
{
    if (_stack.size() > 10)
    {
        throw SegmentParseException(_line.mid(_i - 4, _i), "units stack is full");
    }
    _stack.push(QSharedPointer<WallsUnits>(new WallsUnits(*_units)));
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
    _units = QSharedPointer<WallsUnits>(new WallsUnits());
}

void WallsParser::meters()
{
    _units->d_unit = _units->s_unit = Length::meters();
}

void WallsParser::feet()
{
    _units->d_unit = _units->s_unit = Length::feet();
}

void WallsParser::ct()
{
    _units->vectorType = VectorType::CT;
}

void WallsParser::d()
{
    expect('=');
    _units->d_unit = oneOfMapLowercase(nonwhitespaceRx, lengthUnits);
}

void WallsParser::s()
{
    expect('=');
    _units->s_unit = oneOfMapLowercase(nonwhitespaceRx, lengthUnits);
}

void WallsParser::a()
{
    expect('=');
    _units->a_unit = oneOfMapLowercase(nonwhitespaceRx, azmUnits);
}

void WallsParser::ab()
{
    expect('=');
    _units->ab_unit = oneOfMapLowercase(nonwhitespaceRx, azmUnits);
}

void WallsParser::a_ab()
{
    expect('=');
    _units->a_unit = _units->ab_unit = oneOfMapLowercase(nonwhitespaceRx, azmUnits);
}

void WallsParser::v()
{
    expect('=');
    _units->v_unit = _units->ab_unit = oneOfMapLowercase(nonwhitespaceRx, incUnits);
}

void WallsParser::vb()
{
    expect('=');
    _units->vb_unit = oneOfMapLowercase(nonwhitespaceRx, incUnits);
}

void WallsParser::v_vb()
{
    expect('=');
    _units->v_unit = _units->vb_unit = oneOfMapLowercase(nonwhitespaceRx, incUnits);
}

void WallsParser::order()
{
    expect('=');
    oneOf([&]() { this->ctOrder(); },
    [&]() { this->rectOrder(); });
}

void WallsParser::ctOrder()
{
    _units->ctOrder = elementChars(ctElements, requiredCtElements);
}

void WallsParser::rectOrder()
{
    _units->rectOrder = elementChars(rectElements, requiredRectElements);
}

void WallsParser::decl()
{
    expect('=');
    _units->decl = azimuthOffset(_units->a_unit);
}

void WallsParser::grid()
{
    expect('=');
    _units->grid = azimuthOffset(_units->a_unit);
}

void WallsParser::rect()
{
    if (maybeChar('='))
    {
        _units->rect = azimuthOffset(_units->a_unit);
    }
    else
    {
        _units->vectorType = VectorType::RECT;
    }
}

void WallsParser::incd()
{
    expect('=');
    _units->incd = length(_units->d_unit);
}

void WallsParser::inch()
{
    expect('=');
    _units->inch = length(_units->s_unit);
}

void WallsParser::incs()
{
    expect('=');
    _units->incs = length(_units->s_unit);
}

void WallsParser::inca()
{
    expect('=');
    _units->inca = azimuthOffset(_units->a_unit);
}

void WallsParser::incab()
{
    expect('=');
    _units->incab = azimuthOffset(_units->ab_unit);
}

void WallsParser::incv()
{
    expect('=');
    _units->incv = inclination(_units->v_unit);
}

void WallsParser::incvb()
{
    expect('=');
    _units->incvb = inclination(_units->vb_unit);
}

void WallsParser::typeab()
{
    expect('=');
    _units->typeab_corrected = oneOfMapLowercase(wordRx, correctedValues);
    if (maybeChar(','))
    {
        _units->typeab_tolerance = UAngle(unsignedDoubleLiteral(), Angle::degrees());
        if (maybeChar(','))
        {
            expect('x', Qt::CaseInsensitive);
            _units->typeab_no_average = true;
        }
        else
        {
            _units->typeab_no_average = false;
        }
    }
    else
    {
        _units->typeab_tolerance = UAngle(2.0, Angle::degrees());
    }
}

void WallsParser::typevb()
{
    expect('=');
    _units->typevb_corrected = oneOfMapLowercase(wordRx, correctedValues);
    if (maybeChar(','))
    {
        _units->typevb_tolerance = UAngle(unsignedDoubleLiteral(), Angle::degrees());
        if (maybeChar(','))
        {
            expect('x', Qt::CaseInsensitive);
            _units->typevb_no_average = true;
        }
        else
        {
            _units->typevb_no_average = false;
        }
    }
    else
    {
        _units->typevb_tolerance = UAngle(2.0, Angle::degrees());
    }
}

void WallsParser::case_()
{
    expect('=');
    _units->case_ = oneOfMapLowercase(nonwhitespaceRx, caseTypes);
}

void WallsParser::lrud()
{
    expect('=');
    _units->lrud = oneOfMapLowercase(wordRx, lrudTypes);
    if (maybeChar(':'))
    {
        lrudOrder();
    }
    else
    {
        _units->lrud_order = QList<LrudElement>({LrudElement::L, LrudElement::R, LrudElement::U, LrudElement::D});
    }
}

void WallsParser::lrudOrder()
{
    _units->lrud_order = elementChars(lrudElements, requiredLrudElements);
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
    _units->setPrefix(index, prefix);
}

void WallsParser::tape()
{
    expect('=');
    _units->tape = oneOfMapLowercase(nonwhitespaceRx, tapingMethods);
}

void WallsParser::uvh()
{
    expect('=');
    _units->uvh = unsignedDoubleLiteral();
}

void WallsParser::uvv()
{
    expect('=');
    _units->uvv = unsignedDoubleLiteral();
}

void WallsParser::uv()
{
    expect('=');
    _units->uvv = _units->uvh = unsignedDoubleLiteral();
}

void WallsParser::flag()
{
    QString flag;
    if (maybeChar('='))
    {
        flag = quotedTextOrNonwhitespace();
    }
    _units->flag = flag;
}

void WallsParser::vectorLine()
{
    maybeWhitespace();
    fromStation();
    try
    {
        whitespace();
        afterFromStation();
        maybeWhitespace();
        endOfLine();
        _visitor->endVectorLine();
    }
    catch (const SegmentParseExpectedException& ex)
    {
        _visitor->abortVectorLine();
        throw ex;
    }
    catch (const SegmentParseException& ex)
    {
        _visitor->abortVectorLine();
        throw ex;
    }
}

void WallsParser::fromStation()
{
    _fromStationSegment = expect(stationRx, {"<STATION NAME>"}).value();
    _fromStation = _fromStationSegment.value();
    if (optionalStationRx.indexIn(_fromStation) >= 0) {
        _fromStation.clear();
    }
    _visitor->beginVectorLine();
    _visitor->visitFrom(_fromStation);
}

void WallsParser::afterFromStation()
{
    oneOf([&]() {
        this->toStation();
        this->whitespace();
        this->afterToStation();
    }, [&]() {
        this->lruds();
        this->afterLruds();
    });
}

void WallsParser::toStation()
{
    _toStationSegment = expect(stationRx, {"<STATION NAME>"});
    _toStation = _toStationSegment.value();
    if (optionalStationRx.indexIn(_toStation) >= 0)
    {
        _toStation.clear();
    }
    if (_fromStation.isEmpty() && _toStation.isEmpty()) {
        throw SegmentParseException(_toStationSegment, "from and to station can't both be omitted");
    }
    _visitor->visitTo(_toStation);
}

void WallsParser::afterToStation()
{
    int k = 0;
    if (_units->vectorType == VectorType::RECT)
    {
        foreach(RectElement elem, _units->rectOrder)
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
        foreach(CtElement elem, _units->ctOrder)
        {
            if (k++ > 0)
            {
                whitespace();
            }
            ctElement(elem);
        }
    }

    using namespace std;

    if (_units->vectorType == VectorType::CT)
    {
        if (!_azmFs.isValid() && !_azmBs.isValid() && !WallsUnits::isVertical(_units->avgInc(_incFs, _incBs)))
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
    _visitor->visitEast(length(_units->d_unit));
}

void WallsParser::north()
{
    _visitor->visitNorth(length(_units->d_unit));
}

void WallsParser::rectUp()
{
    _visitor->visitRectUp(length(_units->d_unit));
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

void WallsParser::distance()
{
    _visitor->visitDistance(unsignedLength(_units->d_unit));
}

UAngle WallsParser::azmDifference(UAngle fs, UAngle bs) {
    if (!_units->typeab_corrected) {
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
    _azmFs.clear();
    _azmBs.clear();
    if (optional(_azmFs, [&]() { return this->azimuth(_units->a_unit); }))
    {
        _visitor->visitFrontsightAzimuth(_azmFs);
    }
    if (maybeChar('/'))
    {
        if (optional(_azmBs, [&]() { return this->azimuth(_units->ab_unit); }))
        {
            _visitor->visitBacksightAzimuth(_azmBs);
        }
    }
    _azmSegment = _line.mid(start, _i - start);

    if (_azmFs.isValid() && _azmBs.isValid())
    {
        UAngle diff = azmDifference(_azmFs, _azmBs);
        if (diff > _units->typeab_tolerance)
        {
            _visitor->message({QString("azimuth fs/bs difference (%1) exceeds tolerance (%2)")
                               .arg(diff.toString())
                               .arg(_units->typevb_tolerance.toString()),
                               WallsMessage::Warning,
                               _azmSegment.source(),
                               _azmSegment.startLine(),
                               _azmSegment.startCol(),
                               _azmSegment.endLine(),
                               _azmSegment.endCol()});
        }
    }
}

UAngle WallsParser::incDifference(UAngle fs, UAngle bs) {
    return _units->typevb_corrected ? uabs(fs - bs) : uabs(fs + bs);
}

void WallsParser::inclination()
{
    _incFs.clear();
    _incBs.clear();
    int start = _i;
    optional(_incFs, [&]() { return this->inclination(_units->v_unit); });
    if (maybeChar('/'))
    {
        optional(_incBs, [&]() { return this->inclination(_units->vb_unit); });
    }
    _incSegment = _line.mid(start, _i - start);

    if (!_incFs.isValid() && !_incBs.isValid()) {
        _incFs = UAngle(0, _units->v_unit);
        _visitor->visitFrontsightInclination(_incFs);
    }
    else {
        if (_incFs.isValid()) {
            _visitor->visitFrontsightInclination(_incFs);
        }
        if (_incBs.isValid()) {
            _visitor->visitBacksightInclination(_incBs);
        }
        if (_incFs.isValid() && _incBs.isValid())
        {
            UAngle diff = incDifference(_incFs, _incBs);
            if (diff > _units->typevb_tolerance)
            {
                _visitor->message({QString("inclination fs/bs difference (%1) exceeds tolerance (%2)")
                                   .arg(diff.toString())
                                   .arg(_units->typevb_tolerance.toString()),
                                   WallsMessage::Warning,
                                   _incSegment.source(),
                                   _incSegment.startLine(),
                                   _incSegment.startCol(),
                                   _incSegment.endLine(),
                                   _incSegment.endCol()});
            }
        }
    }
}

void WallsParser::tapingMethodElements()
{
    foreach(TapingMethodElement elem, _units->tape)
    {
        if (!maybeWhitespace() ||
            !maybe([&]() { this->tapingMethodElement(elem); }))
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
    ULength ih;
    if (optional(ih, [&]() { return this->length(_units->s_unit); }))
    {
        _visitor->visitInstrumentHeight(ih);
    }
}

void WallsParser::targetHeight()
{
    ULength th;
    if (optional(th, [&]() { return this->length(_units->s_unit); }))
    {
        _visitor->visitTargetHeight(th);
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
    ULength left;
    if (optional(left, [&]() { return this->unsignedLength(_units->s_unit); }))
    {
        _visitor->visitLeft(left);
    }
}

void WallsParser::right()
{
    ULength right;
    if (optional(right, [&]() { return this->unsignedLength(_units->s_unit); }))
    {
        _visitor->visitRight(right);
    }
}

void WallsParser::up()
{
    ULength up;
    if (optional(up, [&]() { return this->unsignedLength(_units->s_unit); }))
    {
        _visitor->visitUp(up);
    }
}

void WallsParser::down()
{
    ULength down;
    if (optional(down, [&]() { return this->unsignedLength(_units->s_unit); }))
    {
        _visitor->visitDown(down);
    }
}

void WallsParser::afterVectorMeasurements()
{
    if (maybe([&]() { return this->varianceOverrides(); }))
    {
        maybeWhitespace();
    }
    afterVectorVarianceOverrides();
}

void WallsParser::varianceOverrides()
{
    expect('(');
    maybeWhitespace();
    VarianceOverridePtr horizontal = varianceOverride(_units->d_unit);
    if (!horizontal.isNull())
    {
        _visitor->visitHorizontalVarianceOverride(horizontal);
    }
    maybeWhitespace();
    if (maybeChar(','))
    {
        maybeWhitespace();
        VarianceOverridePtr vertical = varianceOverride(_units->d_unit);
        if (horizontal.isNull() && vertical.isNull())
        {
            throw allExpected();
        }
        if (!vertical.isNull()) {
            _visitor->visitVerticalVarianceOverride(vertical);
        }
    }
    else if (!horizontal.isNull())
    {
        _visitor->visitVerticalVarianceOverride(horizontal);
    }
    expect(')');
}

void WallsParser::afterVectorVarianceOverrides()
{
    if (maybe([&]() { this->lruds(); }))
    {
        maybeWhitespace();
    }
    afterLruds();
}

void WallsParser::lruds()
{
    oneOfWithLookahead([&]() {
        this->expect('<');
        this->lrudContent();
        this->expect('>');
    }, [&]() {
        this->expect('*');
        this->lrudContent();
        this->expect('*');
    });
}

void WallsParser::lrudContent()
{
    oneOfWithLookahead([&]() {
        this->commaDelimLrudContent();
    }, [&]() {
        this->whitespaceDelimLrudContent();
    });
}

void WallsParser::commaDelimLrudContent()
{
    maybeWhitespace();
    int m = 0;
    foreach(LrudElement elem, _units->lrud_order)
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
    foreach(LrudElement elem, _units->lrud_order)
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
            this->lrudFacingAngle();
            this->maybeWhitespace();
            if (this->maybeChar(','))
            {
                this->maybeWhitespace();
                this->lrudCFlag();
            }
        }, [&]() { this->lrudCFlag(); });
    }
}

void WallsParser::afterRequiredWhitespaceDelimLrudMeasurements()
{
    maybe([&]() {
        this->oneOf([&]() {
            this->lrudFacingAngle();
            if (this->maybeWhitespace())
            {
                maybe([&]() { this->lrudCFlag(); });
            }
        },
        [&]() { this->lrudCFlag(); });
    });
}

void WallsParser::lrudFacingAngle()
{
    _visitor->visitLrudFacingAngle(azimuth(_units->a_unit));
}

void WallsParser::lrudCFlag()
{
    expect('c', Qt::CaseInsensitive);
}

void WallsParser::afterLruds()
{
    if (maybe([&]() { this->inlineDirective(); }))
    {
        maybeWhitespace();
    }
    inlineCommentOrEndOfLine();
}

void WallsParser::inlineDirective()
{
    // currently this is the only directive that can be on a vector line
    inlineSegmentDirective();
}

void WallsParser::inlineSegmentDirective()
{
    _visitor->visitInlineSegment(segmentDirective().value());
}

void WallsParser::fixLine()
{
    maybeWhitespace();
    expect("#fix", Qt::CaseInsensitive);
    whitespace();
    fixedStation();
    try
    {
        whitespace();
        afterFixedStation();
        maybeWhitespace();
        endOfLine();
        _visitor->endFixLine();
    }
    catch (const SegmentParseExpectedException& ex)
    {
        _visitor->abortFixLine();
        throw ex;
    }
    catch (const SegmentParseException& ex)
    {
        _visitor->abortFixLine();
        throw ex;
    }
}

void WallsParser::fixedStation()
{
    QString fixed = expect(stationRx, {"<STATION NAME>"}).value();
    _visitor->beginFixLine();
    _visitor->visitFixedStation(fixed);
}

void WallsParser::afterFixedStation()
{
    int k = 0;
    foreach(RectElement elem, _units->rectOrder)
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
    oneOf([&]() { this->_visitor->visitEast(this->length(this->_units->d_unit)); },
    [&]() { this->_visitor->visitLongitude(this->longitude()); });
}

void WallsParser::fixNorth()
{
    oneOf([&]() { this->_visitor->visitNorth(this->length(this->_units->d_unit)); },
    [&]() { this->_visitor->visitLatitude(this->latitude()); });
}

void WallsParser::fixUp()
{
    _visitor->visitRectUp(length(_units->d_unit));
}

void WallsParser::afterFixMeasurements()
{
    if (maybe([&]() { this->varianceOverrides(); }))
    {
        maybeWhitespace();
    }
    afterFixVarianceOverrides();
}

void WallsParser::afterFixVarianceOverrides()
{
    if (maybe([&]() { this->inlineNote(); }))
    {
        maybeWhitespace();
    }
    afterFixInlineNote();
}

void WallsParser::inlineNote()
{
    expect('/');
    _visitor->visitInlineNote(escapedText([](QChar c) { return c != ';' && c != '#'; }, {"<NOTE>"}).trimmed());
}

void WallsParser::afterFixInlineNote()
{
    if (maybe([&]() { this->inlineDirective(); }))
    {
        maybeWhitespace();
    }
    inlineCommentOrEndOfLine();
}

void WallsParser::inlineCommentOrEndOfLine()
{
    oneOf([&]() { this->inlineComment(); },
    [&]() { this->endOfLine(); });
}

void WallsParser::comment()
{
    expect(';');
    _visitor->visitCommentLine(remaining().value());
}

void WallsParser::inlineComment()
{
    expect(';');
    _visitor->visitInlineComment(remaining().value());
}

} // namespace dewalls
