#ifndef WALLSTYPES_H
#define WALLSTYPES_H

#include <QString>
#include <QHash>
#include "dewallsexport.h"

namespace dewalls
{

///
/// \brief station name case transforms
///
enum CaseType
{
    /// convert to upper case
    Upper = 0,
    /// convert to lower case
    Lower = 1,
    /// leave in original case
    Mixed = 2
};

QString applyCaseType(CaseType, const QString& s);

///
/// \brief a type of measurement for compass-and-tape vectors
///
enum class CtMeasurement
{
    /// distance
    D = 'D',
    /// azimuth
    A = 'A',
    /// inclination
    V = 'V'
};

inline uint qHash(CtMeasurement key, uint seed = 0)
{
    return uint(static_cast<char>(key)) ^ seed;
}

///
/// \brief a type of measurement for LRUDs
///
enum class LrudMeasurement
{
    /// left
    L = 'L',
    /// right
    R = 'R',
    /// up
    U = 'U',
    /// down
    D = 'D'
};

inline uint qHash(const LrudMeasurement& key, uint seed = 0)
{
    return uint(static_cast<char>(key)) ^ seed;
}

///
/// \brief the types of LRUD station associations/orientations
///
enum class DEWALLS_LIB_EXPORT LrudType
{
    /// at from station, perpendicular to vector
    From = 0,
    /// at to station, perpendicular to vector
    To = 1,
    /// at from station, bisecting vectors
    FB = 2,
    /// at to station, bisecting vectors
    TB = 3
};

///
/// \brief a type of measurement for RECT vectors
///
enum class RectMeasurement
{
    /// east offset
    E = 'E',
    /// north offset
    N = 'N',
    /// up offset
    U = 'U'
};

inline uint qHash(const RectMeasurement& key, uint seed = 0)
{
    return uint(static_cast<char>(key)) ^ seed;
}

///
/// \brief instrument/target height measurement types
///
enum class TapingMethodMeasurement
{
    Station = 0,
    InstrumentHeight = 1,
    TargetHeight = 2
};

///
/// \brief how a vector is measured (compass and tape or cartesian component vectors)
///
enum class DEWALLS_LIB_EXPORT VectorType
{
    /// compass and tape
    CT = 0,
    /// rectangular (north, east, up)
    RECT = 1
};

}

#endif // WALLSTYPES_H

