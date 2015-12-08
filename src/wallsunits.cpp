#include "wallsunits.h"
#include "unitizedmath.h"

namespace dewalls {

typedef UnitizedDouble<Length> ULength;
typedef UnitizedDouble<Angle> UAngle;

WallsUnitsData::WallsUnitsData()
    : QSharedData(),
      vectorType(VectorType::CT),
      ctOrder(QList<CtMeasurement>({CtMeasurement::D, CtMeasurement::A, CtMeasurement::V})),
      rectOrder(QList<RectMeasurement>({RectMeasurement::E, RectMeasurement::N, RectMeasurement::U})),
      dUnit(Length::Meters),
      sUnit(Length::Meters),
      aUnit(Angle::Degrees),
      abUnit(Angle::Degrees),
      vUnit(Angle::Degrees),
      vbUnit(Angle::Degrees),
      decl(UAngle(0.0, Angle::Degrees)),
      grid(UAngle(0.0, Angle::Degrees)),
      rect(UAngle(0.0, Angle::Degrees)),
      incd(ULength(0.0, Length::Meters)),
      inca(UAngle(0.0, Angle::Degrees)),
      incab(UAngle(0.0, Angle::Degrees)),
      incv(UAngle(0.0, Angle::Degrees)),
      incvb(UAngle(0.0, Angle::Degrees)),
      incs(ULength(0.0, Length::Meters)),
      inch(ULength(0.0, Length::Meters)),
      typeabCorrected(false),
      typeabTolerance(UAngle(2, Angle::Degrees)),
      typeabNoAverage(false),
      typevbCorrected(false),
      typevbTolerance(UAngle(2, Angle::Degrees)),
      typevbNoAverage(false),
      case_(CaseType::Mixed),
      lrud(LrudType::From),
      lrudOrder(QList<LrudMeasurement>({LrudMeasurement::L, LrudMeasurement::R, LrudMeasurement::U, LrudMeasurement::D})),
      tape(QList<TapingMethodMeasurement>({TapingMethodMeasurement::InstrumentHeight, TapingMethodMeasurement::TargetHeight})),
      flag(QString()),
      prefix(QStringList()),
      uvh(1.0),
      uvv(1.0)
{

}

WallsUnits::WallsUnits()
{
    d = new WallsUnitsData;
}

void WallsUnits::setPrefix(int index, QString newPrefix)
{
    if (index < 0 || index > 2)
    {
        throw std::invalid_argument("prefix index out of range");
    }

    d.detach();

    while (d->prefix.size() <= index)
    {
        d->prefix << QString();
    }
    d->prefix[index] = newPrefix;

    while (!d->prefix.isEmpty() && d->prefix.last().isNull())
    {
        d->prefix.removeLast();
    }
}

QString WallsUnits::processStationName(QString name) const
{
    if (name.isNull())
    {
        return QString();
    }
    name = applyCaseType(d->case_, name);
    int explicitPrefixCount = name.count(':');
    for (int i = explicitPrefixCount; i < d->prefix.size(); i++)
    {
        name.prepend(':').prepend(d->prefix[i]);
    }
    return name.replace(QRegExp("^:+"), "");
}

ULength WallsUnits::correctLength(ULength length, ULength correction)
{
    return length.isNonzero() && correction.isNonzero() ? length + correction : length;
}

void WallsUnits::applyHeightCorrections(ULength& dist, UAngle& fsInc, UAngle& bsInc, ULength ih, ULength th) const
{
    if (inch().isNonzero() || ih.isNonzero() || th.isNonzero())
    {
        UAngle inc = avgInc(fsInc + incv(), bsInc + incvb());
        if (inc.isValid() && !isVertical(fsInc, bsInc))
        {
            // horizontal offset before height correction
            ULength ne = ucos(inc) * correctLength(dist, incd());
            // vertical offset before height correction
            ULength u = usin(inc) * correctLength(dist, incd());

            u += inch();
            if (ih.isValid()) u += correctLength(ih, incs());
            if (th.isValid()) u -= correctLength(th, incs());

            // adjust fsInc/bsInc so that their new avg
            // is the corrected inclination
            UAngle dinc = uatan2(u, ne) - inc;
            fsInc += dinc;
            bsInc += (typevbCorrected() ? dinc : -dinc);

            dist = usqrt(usq(ne) + usq(u)) - incd();
        }
    }
}

UAngle WallsUnits::avgInc(UAngle fsInc, UAngle bsInc) const
{
    if (!typevbCorrected())
    {
        bsInc = -bsInc;
    }
    if (!fsInc.isValid())
    {
        return bsInc;
    }
    else if (!bsInc.isValid())
    {
        return fsInc;
    }

    return (fsInc + bsInc) * 0.5;
}

bool WallsUnits::isVertical(UAngle angle)
{
    return fabs(fabs(angle.get(Angle::Degrees)) - 90.0) < 1e-6;
}

bool WallsUnits::isVertical(UAngle fsInc, UAngle bsInc)
{
    if (fsInc.isValid() && !isVertical(fsInc)) {
        return false;
    }
    if (bsInc.isValid() && !isVertical(bsInc)) {
        return false;
    }
    return fsInc.isValid() || bsInc.isValid();
}

QString WallsUnits::lrudOrderString() const
{
    QString result;
    foreach(LrudMeasurement elem, lrudOrder()) {
        result += char(elem);
    }
    return result;
}

} // namespace dewalls

