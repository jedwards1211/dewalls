#ifndef DEWALLS_FIXSTATION_H
#define DEWALLS_FIXSTATION_H

#include "unitizeddouble.h"
#include "length.h"
#include "angle.h"
#include "varianceoverride.h"
#include <QSharedData>
#include <QSharedDataPointer>
#include <QSharedPointer>
#include <QString>
#include <QDate>
#include "wallsunits.h"

namespace dewalls {

class FixStationData : public QSharedData
{
public:
    typedef UnitizedDouble<Length> ULength;
    typedef UnitizedDouble<Angle> UAngle;
    typedef QSharedPointer<VarianceOverride> VarianceOverridePtr;

    inline FixStationData()
        : QSharedData(),
          name(),
          north(),
          east(),
          rectUp(),
          latitude(),
          longitude(),
          horizVariance(),
          vertVariance(),
          note(),
          segment(),
          comment(),
          date(),
          units() { }

    QString name;
    ULength north;
    ULength east;
    ULength rectUp;
    UAngle latitude;
    UAngle longitude;
    VarianceOverridePtr horizVariance;
    VarianceOverridePtr vertVariance;
    QString note;
    QStringList segment;
    QString comment;
    QDate date;
    WallsUnits units;
};

class FixStation
{
public:
    typedef UnitizedDouble<Length> ULength;
    typedef UnitizedDouble<Angle> UAngle;
    typedef QSharedPointer<VarianceOverride> VarianceOverridePtr;

    inline FixStation() { d = new FixStationData; }
    inline FixStation(const FixStation& other)
        : d(other.d) { }

    inline QString name() { return d->name; }
    inline ULength north() { return d->north; }
    inline ULength east() { return d->east; }
    inline ULength rectUp() { return d->rectUp; }
    inline UAngle latitude() { return d->latitude; }
    inline UAngle longitude() { return d->longitude; }
    inline VarianceOverridePtr horizVariance() { return d->horizVariance; }
    inline VarianceOverridePtr vertVariance() { return d->vertVariance; }
    inline QString note() { return d->note; }
    inline QStringList segment() { return d->segment; }
    inline QString comment() { return d->comment; }
    inline QDate date() const { return d->date; }
    inline WallsUnits units() const { return d->units; }

    inline void setName(QString name) { d->name = name; }
    inline void setNorth(ULength north) { d->north = north; }
    inline void setEast(ULength east) { d->east = east; }
    inline void setRectUp(ULength rectUp) { d->rectUp = rectUp; }
    inline void setLatitude(UAngle latitude) { d->latitude = latitude; }
    inline void setLongitude(UAngle longitude) { d->longitude = longitude; }
    inline void setHorizVariance(VarianceOverridePtr horizVariance) { d->horizVariance = horizVariance; }
    inline void setVertVariance(VarianceOverridePtr vertVariance) { d->vertVariance = vertVariance; }
    inline void setNote(QString note) { d->note = note; }
    inline void setSegment(QStringList segment) { d->segment = segment; }
    inline void setComment(QString comment) { d->comment = comment; }
    inline void setDate(QDate date) { d->date = date; }
    inline void setUnits(WallsUnits units) { d->units = units; }

private:
    QSharedDataPointer<FixStationData> d;
};

} // namespace dewalls

#endif // DEWALLS_FIXSTATION_H
