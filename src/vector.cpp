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

} // namespace dewalls
