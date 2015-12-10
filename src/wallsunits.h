#ifndef DEWALLS_WALLSUNITS_H
#define DEWALLS_WALLSUNITS_H

#include <QSharedData>
#include <QSharedDataPointer>
#include <QString>
#include <QStringList>
#include <QList>
#include <QDateTime>
#include "unitizeddouble.h"
#include "length.h"
#include "angle.h"
#include "wallstypes.h"
#include "math.h"

namespace dewalls {

class WallsUnitsData : public QSharedData
{
public:
    typedef UnitizedDouble<Length> ULength;
    typedef UnitizedDouble<Angle> UAngle;

    WallsUnitsData();

    VectorType vectorType;
    QList<CtMeasurement> ctOrder;
    QList<RectMeasurement> rectOrder;
    Length::Unit dUnit;
    Length::Unit sUnit;
    Angle::Unit aUnit;
    Angle::Unit abUnit;
    Angle::Unit vUnit;
    Angle::Unit vbUnit;
    UAngle decl;
    UAngle grid;
    UAngle rect;
    ULength incd;
    UAngle inca;
    UAngle incab;
    UAngle incv;
    UAngle incvb;
    ULength incs;
    ULength inch;
    bool typeabCorrected;
    UAngle typeabTolerance;
    bool typeabNoAverage;
    bool typevbCorrected;
    UAngle typevbTolerance;
    bool typevbNoAverage;
    CaseType case_;
    LrudType lrud;
    QList<LrudMeasurement> lrudOrder;
    QList<TapingMethodMeasurement> tape;
    QString flag;
    QStringList prefix;
    double uvh;
    double uvv;
};

///
/// \brief Variables that are controlled through the #units directive (except for those
/// that aren't affected by #units save/restore/reset, like macros).
///
class WallsUnits
{
public:
    typedef UnitizedDouble<Length> ULength;
    typedef UnitizedDouble<Angle> UAngle;

    WallsUnits();

    ///
    /// \brief the current vector type - CT (compass and tape) or RECT
    ///
    inline VectorType vectorType() const { return d->vectorType; }
    ///
    /// \brief the order of measurements for CT (compass and tape) vectors.
    ///
    inline QList<CtMeasurement> ctOrder() const { return d->ctOrder; }
    ///
    /// \brief the order of measurements for RECT vectors.
    ///
    inline QList<RectMeasurement> rectOrder() const { return d->rectOrder; }
    ///
    /// \brief the unit for distances
    ///
    inline Length::Unit dUnit() const { return d->dUnit; }
    ///
    /// \brief the unit for short distances like LRUDs, inst/target heights
    ///
    inline Length::Unit sUnit() const { return d->sUnit; }
    ///
    /// \brief the frontsight azimuth unit
    ///
    inline Angle::Unit aUnit() const { return d->aUnit; }
    ///
    /// \brief the backsight azimuth unit
    ///
    inline Angle::Unit abUnit() const { return d->abUnit; }
    ///
    /// \brief the frontsight inclination unit
    ///
    inline Angle::Unit vUnit() const { return d->vUnit; }
    ///
    /// \brief the backsight inclination unit
    ///
    inline Angle::Unit vbUnit() const { return d->vbUnit; }
    ///
    /// \brief magnetic declination
    ///
    inline UAngle decl() const { return d->decl; }
    ///
    /// \brief grid correction (true north minus magnetic north)
    ///
    inline UAngle grid() const { return d->grid; }
    ///
    /// \brief rect correction (true north minus RECT north)
    ///
    inline UAngle rect() const { return d->rect; }
    ///
    /// \brief distance correction
    ///
    inline ULength incd() const { return d->incd; }
    ///
    /// \brief frontsight azimuth correction
    ///
    inline UAngle inca() const { return d->inca; }
    ///
    /// \brief backsight azimuth correction
    ///
    inline UAngle incab() const { return d->incab; }
    ///
    /// \brief frontsight inclination correction
    ///
    inline UAngle incv() const { return d->incv; }
    ///
    /// \brief backsight inclination correction
    ///
    inline UAngle incvb() const { return d->incvb; }
    ///
    /// \brief short distance corretion (e.g. LRUDs, inst/target heights)
    ///
    inline ULength incs() const { return d->incs; }
    ///
    /// \brief height correction (added to vertical offset of vectors)
    ///
    inline ULength inch() const { return d->inch; }
    ///
    /// \brief whether backsight azimuth is corrected
    ///
    inline bool typeabCorrected() const { return d->typeabCorrected; }
    ///
    /// \brief warning threshold for frontsight/backsight azimuth discrepancy
    ///
    inline UAngle typeabTolerance() const { return d->typeabTolerance; }
    ///
    /// \brief if true, use frontsight azimuth only in calculations
    ///
    inline bool typeabNoAverage() const { return d->typeabNoAverage; }
    ///
    /// \brief whether backsight inclination is corrected
    ///
    inline bool typevbCorrected() const { return d->typevbCorrected; }
    ///
    /// \brief warning threshold for frontsight/backsight inclination discrepancy
    ///
    inline UAngle typevbTolerance() const { return d->typevbTolerance; }
    ///
    /// \brief if true, use frontsight inclination only in calculations
    ///
    inline bool typevbNoAverage() const { return d->typevbNoAverage; }
    ///
    /// \brief station name case modification
    ///
    inline CaseType case_() const { return d->case_; }
    ///
    /// \brief LRUD station association/orientation
    ///
    inline LrudType lrud() const { return d->lrud; }
    ///
    /// \brief order of LRUD measurements
    ///
    inline QList<LrudMeasurement> lrudOrder() const { return d->lrudOrder; }
    ///
    /// \brief whether instrument and target heights may be present after dist/azm/inc
    ///
    inline QList<TapingMethodMeasurement> tape() const { return d->tape; }
    ///
    /// \brief walls station flag
    ///
    inline QString flag() const { return d->flag; }
    ///
    /// \brief prefixes to prepend to station, joined by colons
    ///
    inline QStringList prefix() const { return d->prefix; }
    ///
    /// \brief assumed horizontal variance of all vectors
    ///
    inline double uvh() const { return d->uvh; }
    ///
    /// \brief assumed vertical variance of all vectors
    ///
    inline double uvv() const { return d->uvv; }

    inline void setVectorType(VectorType vectorType) { d->vectorType = vectorType; }
    inline void setCtOrder(QList<CtMeasurement> ctOrder) { d->ctOrder = ctOrder; }
    inline void setRectOrder(QList<RectMeasurement> rectOrder) { d->rectOrder = rectOrder; }
    inline void setDUnit(Length::Unit dUnit) { d->dUnit = dUnit; }
    inline void setSUnit(Length::Unit sUnit) { d->sUnit = sUnit; }
    inline void setAUnit(Angle::Unit aUnit) { d->aUnit = aUnit; }
    inline void setAbUnit(Angle::Unit abUnit) { d->abUnit = abUnit; }
    inline void setVUnit(Angle::Unit vUnit) { d->vUnit = vUnit; }
    inline void setVbUnit(Angle::Unit vbUnit) { d->vbUnit = vbUnit; }
    inline void setDecl(UAngle decl) { d->decl = decl; }
    inline void setGrid(UAngle grid) { d->grid = grid; }
    inline void setRect(UAngle rect) { d->rect = rect; }
    inline void setIncd(ULength incd) { d->incd = incd; }
    inline void setInca(UAngle inca) { d->inca = inca; }
    inline void setIncab(UAngle incab) { d->incab = incab; }
    inline void setIncv(UAngle incv) { d->incv = incv; }
    inline void setIncvb(UAngle incvb) { d->incvb = incvb; }
    inline void setIncs(ULength incs) { d->incs = incs; }
    inline void setInch(ULength inch) { d->inch = inch; }
    inline void setTypeabCorrected(bool typeabCorrected) { d->typeabCorrected = typeabCorrected; }
    inline void setTypeabTolerance(UAngle typeabTolerance) { d->typeabTolerance = typeabTolerance; }
    inline void setTypeabNoAverage(bool typeabNoAverage) { d->typeabNoAverage = typeabNoAverage; }
    inline void setTypevbCorrected(bool typevbCorrected) { d->typevbCorrected = typevbCorrected; }
    inline void setTypevbTolerance(UAngle typevbTolerance) { d->typevbTolerance = typevbTolerance; }
    inline void setTypevbNoAverage(bool typevbNoAverage) { d->typevbNoAverage = typevbNoAverage; }
    inline void setCase(CaseType case_) { d->case_ = case_; }
    inline void setLrud(LrudType lrud) { d->lrud = lrud; }
    inline void setLrudOrder(QList<LrudMeasurement> lrudOrder) { d->lrudOrder = lrudOrder; }
    inline void setTape(QList<TapingMethodMeasurement> tape) { d->tape = tape; }
    inline void setFlag(QString flag) { d->flag = flag; }
    inline void setPrefix(QStringList prefix) { d->prefix = prefix; }
    inline void setUvh(double uvh) { d->uvh = uvh; }
    inline void setUvv(double uvv) { d->uvv = uvv; }

    ///
    /// \brief sets the prefix at the given index [1-3]
    ///
    void setPrefix(int index, QString newPrefix);
    ///
    /// \brief applies case modification and prefixes to a station name
    ///
    QString processStationName(QString name) const;

    ///
    /// \return QString representation of LRUD order
    ///
    QString lrudOrderString() const;

    ///
    /// \brief corrects a length measurement if it's nonzero by adding the given correction
    ///
    static ULength correctLength(ULength dist, ULength correction);
    ///
    /// \brief gets the average inclination of the given frontsight/backsight, which may be invalid
    ///
    UAngle avgInc(UAngle fsInc, UAngle bsInc) const;
    ///
    /// \brief determines if the given inclination is almost vertical
    ///
    static bool isVertical(UAngle angle);
    ///
    /// \brief determines if the given frontsight/backsight inclination are almost vertical
    ///
    static bool isVertical(UAngle fsInc, UAngle bsInc);

private:
    QSharedDataPointer<WallsUnitsData> d;
};

} // namespace dewalls

#endif // DEWALLS_WALLSUNITS_H
