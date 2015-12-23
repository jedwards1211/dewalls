#include "vector.h"
#include "unitizedmath.h"
#include "segmentparseexception.h"

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

bool Vector::applyHeightCorrections()
{
    if (!isVertical() && (units().inch().isNonzero() || instHeight().isNonzero() || targetHeight().isNonzero()))
    {
        UAngle inc = units().avgInc(frontInclination() + units().incv(), backInclination() + units().incvb());
        if (!inc.isValid()) inc = UAngle(0, Angle::Degrees);
        double sini = usin(inc);
        double cosi = ucos(inc);

        ULength tapeDist = distance() + units().incd();
        ULength _instHeight = instHeight() + units().incs();
        if (!_instHeight.isValid()) _instHeight = ULength(0, tapeDist.unit());
        ULength _targetHeight = targetHeight() + units().incs();
        if (!_targetHeight.isValid()) _targetHeight = ULength(0, tapeDist.unit());
        ULength tapeFromHeight = units().tape()[0] == TapingMethodMeasurement::Station ? ULength(0, tapeDist.unit()) : _instHeight;
        ULength tapeToHeight   = units().tape()[1] == TapingMethodMeasurement::Station ? ULength(0, tapeDist.unit()) : _targetHeight;
        ULength delta = (tapeToHeight - tapeFromHeight) - (_targetHeight - _instHeight);

        ULength unAdjusted = usqrt(usq(tapeDist) - usq(delta * cosi));
        if ((-unAdjusted - delta * sini).isPositive())
        {
            throw SegmentParseException(sourceSegment(), "vector is ambiguous; there are two possible vectors that satisfy the constraints imposed by the instrument/target heights, INCH and taping method");
        }

        ULength distAlongInc = unAdjusted - delta * sini;

        ULength totalDelta = _instHeight - _targetHeight + units().inch();

        ULength stationToStationDist = usqrt(usq(distAlongInc) + 2 * sini * umul(totalDelta, distAlongInc) + usq(totalDelta));
        unAdjusted = usqrt(usq(stationToStationDist) - usq(totalDelta * cosi));
        if ((-unAdjusted - totalDelta * sini).isPositive())
        {
            throw SegmentParseException(sourceSegment(), "vector is ambiguous; there are two possible vectors that satisfy the constraints imposed by the instrument/target heights, INCH and taping method");
        }

        UAngle  stationToStationInc  = inc + uatan(totalDelta * cosi / (distAlongInc + totalDelta * sini));

        setDistance(stationToStationDist - units().incd());

        if (!frontInclination().isValid() && !backInclination().isValid())
        {
            setFrontInclination(stationToStationInc - units().incv());
        }
        else
        {
            UAngle dInc = stationToStationInc - inc;
            setFrontInclination(frontInclination() + dInc);
            setBackInclination (backInclination () + dInc);
        }

        return true;
    }
    return false;
}


} // namespace dewalls
