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
        if (!inc.isValid()) inc = UAngle(0, Angle::Degrees);
        double sini = usin(inc);
        double cosi = ucos(inc);

        ULength tapeDist = distance() + units().incd();
        ULength tapeFromHeight = units().tape()[0] == TapingMethodMeasurement::Station ? ULength(0, tapeDist.unit()) : instHeight();
        ULength tapeToHeight   = units().tape()[1] == TapingMethodMeasurement::Station ? ULength(0, tapeDist.unit()) : targetHeight();
        ULength delta = tapeFromHeight - tapeToHeight;

        ULength distAlongInc = usqrt(usq(tapeDist) - usq(delta) + usq(delta * sini * sini)) - delta * sini;

        ULength totalDelta = instHeight() - targetHeight() + units().inch();

        ULength stationToStationDist = usqrt(usq(distAlongInc) + 2 * sini * umul(totalDelta, distAlongInc) + usq(totalDelta));
        UAngle  stationToStationInc  = inc + uatan(totalDelta * cosi / (distAlongInc + totalDelta * sini));

        setDistance(stationToStationDist - units().incd());

        UAngle dInc = stationToStationInc - inc;

        if (!frontInclination().isValid() && !backInclination().isValid())
        {
            setFrontInclination(stationToStationInc);
        }
        else
        {
            setFrontInclination(frontInclination() - dInc);
            setBackInclination (backInclination () - dInc);
        }
    }
}


} // namespace dewalls
