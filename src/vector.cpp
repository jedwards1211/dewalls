#include "vector.h"
#include "unitizedmath.h"
#include "segmentparseexception.h"
#include <iostream>

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
        // get corrected average inclination (default to zero)
        UAngle inc = units().avgInc(frontInclination() + units().incv(), backInclination() + units().incvb());

        // get corrected distance
        ULength tapeDist = distance() + units().incd();

        // get corrected instrument and target heights (default to zero)
        ULength _instHeight = instHeight() + units().incs();
        if (!_instHeight.isValid()) _instHeight = ULength(0, tapeDist.unit());
        ULength _targetHeight = targetHeight() + units().incs();
        if (!_targetHeight.isValid()) _targetHeight = ULength(0, tapeDist.unit());

        ULength stationToStationDist;
        UAngle stationToStationInc;

        if (units().tape()[0] == TapingMethodMeasurement::Station &&
                units().tape()[1] == TapingMethodMeasurement::Station &&
                !inc.isValid() || inc.isZero()) {

            // this is a dive-style shot
            stationToStationDist = tapeDist;

            ULength heightOffset = _instHeight - _targetHeight;

            if (uabs(heightOffset) > tapeDist * (1 + 1e-6)) {
                throw SegmentParseException(sourceSegment(), "instrument and target height difference is greater than tape distance; this is impossible");
            }

            if (uabs(uabs(heightOffset) - tapeDist) < tapeDist * 1e-8) {
                // vertical shot
                stationToStationInc = heightOffset.isPositive() ?
                            UAngle(90.0, Angle::Degrees) :
                            UAngle(-90.0, Angle::Degrees);
                stationToStationDist = uabs(heightOffset + units().inch());
            }
            else {
                if (units().inch().isNonzero()) {
                    ULength horizDistance = usqrt(usq(tapeDist) - usq(heightOffset));
                    ULength totalHeightOffset = heightOffset + units().inch();
                    stationToStationDist = usqrt(usq(horizDistance) + usq(totalHeightOffset));
                    stationToStationInc = uatan2(totalHeightOffset, horizDistance);
                }
                else {
                    stationToStationInc = uasin(heightOffset / tapeDist);
                }
            }
        }
        if (!stationToStationInc.isValid()){
            if (!inc.isValid()) inc = UAngle(0, Angle::Degrees);

            // compute height of tape ends above stations
            ULength tapeFromHeight = units().tape()[0] == TapingMethodMeasurement::Station ? ULength(0, tapeDist.unit()) : _instHeight;
            ULength tapeToHeight   = units().tape()[1] == TapingMethodMeasurement::Station ? ULength(0, tapeDist.unit()) : _targetHeight;

            // compute height of instrument and target above tape ends
            ULength instHeightAboveTape = _instHeight - tapeFromHeight;
            ULength targetHeightAboveTape = _targetHeight - tapeToHeight;

            // height change between tape vector and instrument to target vector
            ULength delta = instHeightAboveTape - targetHeightAboveTape;

            if (uabs(delta) > tapeDist) {
                throw SegmentParseException(sourceSegment(), "vector is ambiguous because abs(instrument height above tape - target height above tape) > distance.  In this case, there are two possible vectors that fulfill the constraints imposed by the measurements.  Split this shot into two shots (one vertical) to make it unambiguous.");
            }

            // compute instrument to target distance
            // it's difficult to justify this equation in pure text, it requires a geometric proof
            ULength instToTargetDist = usqrt(usq(tapeDist) - usq(delta * ucos(inc))) - delta * usin(inc);

            // height change between inst to target vector and final corrected vector
            ULength totalDelta = _instHeight - _targetHeight + units().inch();

            // compute station to station distance and inclination
            stationToStationDist = usqrt(usq(instToTargetDist * usin(inc) + totalDelta) + usq(instToTargetDist * ucos(inc)));
            stationToStationInc = uatan2(instToTargetDist * usin(inc) + totalDelta, instToTargetDist * ucos(inc));
        }

        // make sure to subtract corrections so that when they are applied later,
        // they will produce the same vector calculated here
        setDistance(stationToStationDist - units().incd());

        if (!frontInclination().isValid() && !backInclination().isValid())
        {
            setFrontInclination(stationToStationInc - units().incv());
        }
        else
        {
            UAngle dInc = stationToStationInc - inc;
            // since we are moving the original vectors by the difference, we don't need to subtract the
            // correction factors -- they're already present
            setFrontInclination(frontInclination() + dInc);
            setBackInclination (backInclination () + dInc);
        }

        // clear out the instrument and target heights, since the vector is now fully determined by the
        // distance and inclination
        setInstHeight(ULength());
        setTargetHeight(ULength());

        return true;
    }
    return false;
}


} // namespace dewalls
