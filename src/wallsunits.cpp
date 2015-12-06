#include "wallsunits.h"
#include "unitizedmath.h"

namespace dewalls {

typedef const Unit<Length> * LengthUnit;
typedef const Unit<Angle>  * AngleUnit;
typedef UnitizedDouble<Length> ULength;
typedef UnitizedDouble<Angle> UAngle;

WallsUnitsData::WallsUnitsData()
    : QSharedData(),
      vectorType(VectorType::CT),
      ctOrder(QList<CtElement>({CtElement::D, CtElement::A, CtElement::V})),
      rectOrder(QList<RectElement>({RectElement::E, RectElement::N, RectElement::U})),
      dUnit(Length::meters()),
      sUnit(Length::meters()),
      aUnit(Angle::degrees()),
      abUnit(Angle::degrees()),
      vUnit(Angle::degrees()),
      vbUnit(Angle::degrees()),
      decl(UAngle(0.0, Angle::degrees())),
      grid(UAngle(0.0, Angle::degrees())),
      rect(UAngle(0.0, Angle::degrees())),
      incd(ULength(0.0, Length::meters())),
      inca(UAngle(0.0, Angle::degrees())),
      incab(UAngle(0.0, Angle::degrees())),
      incv(UAngle(0.0, Angle::degrees())),
      incvb(UAngle(0.0, Angle::degrees())),
      incs(ULength(0.0, Length::meters())),
      inch(ULength(0.0, Length::meters())),
      typeabCorrected(false),
      typeabTolerance(UAngle(2, Angle::degrees())),
      typeabNoAverage(false),
      typevbCorrected(false),
      typevbTolerance(UAngle(2, Angle::degrees())),
      typevbNoAverage(false),
      case_(CaseType::Mixed),
      lrud(LrudType::From),
      lrudOrder(QList<LrudElement>({LrudElement::L, LrudElement::R, LrudElement::U, LrudElement::D})),
      tape(QList<TapingMethodElement>({TapingMethodElement::InstrumentHeight, TapingMethodElement::TargetHeight})),
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

void WallsUnits::rectToCt(ULength north, ULength east, ULength up, ULength& distance, UAngle& azm, UAngle& inc) const
{
    ULength ne2 = usq(north) + usq(east);
    ULength ne = usqrt(ne2); // horizontal offset
    if (!up.isValid()) up = ULength(0, Length::m());

    distance = usqrt(ne2 + usq(up)).in(dUnit()) - incd();
    azm = uatan2(east, north).in(aUnit()) - inca();
    if (azm < UAngle(0, Angle::degrees()))
    {
        azm += UAngle(360.0, Angle::degrees());
    }
    inc = uatan2(up, ne).in(vUnit()) - incv();
}

ULength WallsUnits::correctLength(ULength length, ULength correction) const
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
    return fabs(fabs(angle.get(Angle::degrees())) - 90.0) < 1e-6;
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
    foreach(LrudElement elem, lrudOrder()) {
        result += char(elem);
    }
    return result;
}

} // namespace dewalls

