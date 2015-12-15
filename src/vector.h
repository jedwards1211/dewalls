#ifndef DEWALLS_VECTOR_H
#define DEWALLS_VECTOR_H

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
#include "dewallsexport.h"

namespace dewalls {

class DEWALLS_LIB_EXPORT VectorData : public QSharedData
{
public:
    typedef UnitizedDouble<Length> ULength;
    typedef UnitizedDouble<Angle> UAngle;
    typedef QSharedPointer<VarianceOverride> VarianceOverridePtr;

    inline VectorData()
        : QSharedData(),
          from(),
          to(),
          distance(),
          frontAzimuth(),
          backAzimuth(),
          frontInclination(),
          backInclination(),
          instHeight(),
          targetHeight(),
          north(),
          east(),
          rectUp(),
          horizVariance(),
          vertVariance(),
          left(),
          right(),
          up(),
          down(),
          lrudAngle(),
          cFlag(false),
          segment(),
          comment(),
          date(),
          units() { }

    QString from;
    QString to;
    ULength distance;
    UAngle frontAzimuth;
    UAngle backAzimuth;
    UAngle frontInclination;
    UAngle backInclination;
    ULength instHeight;
    ULength targetHeight;
    ULength north;
    ULength east;
    ULength rectUp;
    VarianceOverridePtr horizVariance;
    VarianceOverridePtr vertVariance;
    ULength left;
    ULength right;
    ULength up;
    ULength down;
    UAngle lrudAngle;
    bool cFlag;
    QStringList segment;
    QString comment;
    QDate date;
    WallsUnits units;
};

class Vector
{
public:
    typedef UnitizedDouble<Length> ULength;
    typedef UnitizedDouble<Angle> UAngle;
    typedef QSharedPointer<VarianceOverride> VarianceOverridePtr;

    inline Vector() { d = new VectorData; }
    inline Vector(const Vector& other)
        : d(other.d) { }

    inline QString from() const { return d->from; }
    inline QString to() const { return d->to; }
    inline ULength distance() const { return d->distance; }
    inline UAngle frontAzimuth() const { return d->frontAzimuth; }
    inline UAngle backAzimuth() const { return d->backAzimuth; }
    inline UAngle frontInclination() const { return d->frontInclination; }
    inline UAngle backInclination() const { return d->backInclination; }
    inline ULength instHeight() const { return d->instHeight; }
    inline ULength targetHeight() const { return d->targetHeight; }
    inline ULength north() const { return d->north; }
    inline ULength east() const { return d->east; }
    inline ULength rectUp() const { return d->rectUp; }
    inline VarianceOverridePtr horizVariance() const { return d->horizVariance; }
    inline VarianceOverridePtr vertVariance() const { return d->vertVariance; }
    inline ULength left() const { return d->left; }
    inline ULength right() const { return d->right; }
    inline ULength up() const { return d->up; }
    inline ULength down() const { return d->down; }
    inline UAngle lrudAngle() const { return d->lrudAngle; }
    inline bool cFlag() const { return d->cFlag; }
    inline QStringList segment() const { return d->segment; }
    inline QString comment() const { return d->comment; }
    inline QDate date() const { return d->date; }
    inline WallsUnits units() const { return d->units; }

    inline void setFrom(QString from) { d->from = from; }
    inline void setTo(QString to) { d->to = to; }
    inline void setDistance(ULength distance) { d->distance = distance; }
    inline void setFrontAzimuth(UAngle frontAzimuth) { d->frontAzimuth = frontAzimuth; }
    inline void setBackAzimuth(UAngle backAzimuth) { d->backAzimuth = backAzimuth; }
    inline void setFrontInclination(UAngle frontInclination) { d->frontInclination = frontInclination; }
    inline void setBackInclination(UAngle backInclination) { d->backInclination = backInclination; }
    inline void setInstHeight(ULength instHeight) { d->instHeight = instHeight; }
    inline void setTargetHeight(ULength targetHeight) { d->targetHeight = targetHeight; }
    inline void setNorth(ULength north) { d->north = north; }
    inline void setEast(ULength east) { d->east = east; }
    inline void setRectUp(ULength rectUp) { d->rectUp = rectUp; }
    inline void setHorizVariance(VarianceOverridePtr horizVariance) { d->horizVariance = horizVariance; }
    inline void setVertVariance(VarianceOverridePtr vertVariance) { d->vertVariance = vertVariance; }
    inline void setLeft(ULength left) { d->left = left; }
    inline void setRight(ULength right) { d->right = right; }
    inline void setUp(ULength up) { d->up = up; }
    inline void setDown(ULength down) { d->down = down; }
    inline void setLrudAngle(UAngle lrudAngle) { d->lrudAngle = lrudAngle; }
    inline void setCFlag(bool cFlag) { d->cFlag = cFlag; }
    inline void setSegment(QStringList segment) { d->segment = segment; }
    inline void setComment(QString comment) { d->comment = comment; }
    inline void setDate(QDate date) { d->date = date; }
    inline void setUnits(WallsUnits units) { d->units = units; }

    ///
    /// \brief derives compass-and-tape measurements from the rect measurements.
    ///
    void deriveCtFromRect();
    ///
    /// \brief calculates the vector offset including instrument and target heights and Walls INCH
    /// correction, rederives the distance and inclinations from that offset, and clears the instrument
    /// and target heights.  This is for programs that don't store instrument and target heights or
    /// something like INCH internally.
    ///
    void applyHeightCorrections();
    bool isVertical();

private:
    QSharedDataPointer<VectorData> d;
};

} // namespace dewalls

#endif // DEWALLS_VECTOR_H
