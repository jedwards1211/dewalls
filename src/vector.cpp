#include "vector.h"
#include "unitizedmath.h"

namespace dewalls {

void Vector::deriveCtFromRect()
{
    ULength ne2 = usq(north()) + usq(east());
    ULength ne = usqrt(ne2); // horizontal offset
    ULength up = rectUp();
    if (!up.isValid()) up = ULength(0, Length::Meters);

    setDistance(usqrt(ne2 + usq(up)).in(units().dUnit()) - units().incd());
    UAngle azm = uatan2(east(), north()).in(units().aUnit()) - units().inca();
    if (azm < UAngle(0, Angle::Degrees))
    {
        azm += UAngle(360.0, Angle::Degrees);
    }
    setFrontAzimuth(azm);
    setFrontInclination(uatan2(up, ne).in(units().vUnit()) - units().incv());
}

bool Vector::isVertical()
{
    return units().isVertical(frontInclination(), frontAzimuth());
}

void Vector::applyHeightCorrections()
{
    if (!isVertical() && (units().inch().isNonzero() || instHeight().isNonzero() || targetHeight().isNonzero()))
    {
        UAngle inc = units().avgInc(frontInclination() + units().incv(), backInclination() + units().incvb());
        if (inc.isValid())
        {
            // horizontal offset before height correction
            ULength ne = ucos(inc) * WallsUnits::correctLength(distance(), units().incd());
            // vertical offset before height correction
            ULength u = usin(inc) * WallsUnits::correctLength(distance(), units().incd());

            u += units().inch();
            if (instHeight().isValid()) u += WallsUnits::correctLength(instHeight(), units().incs());
            if (targetHeight().isValid()) u -= WallsUnits::correctLength(targetHeight(), units().incs());

            // adjust fsInc/bsInc so that their new avg
            // is the corrected inclination
            UAngle dinc = uatan2(u, ne) - inc;

            d.detach();
            d->frontInclination += dinc;
            d->backInclination += (units().typevbCorrected() ? dinc : -dinc);

            d->distance = usqrt(usq(ne) + usq(u)) - units().incd();
            d->instHeight.clear();
            d->targetHeight.clear();
        }
    }
}


} // namespace dewalls
