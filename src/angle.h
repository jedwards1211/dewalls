#ifndef DEWALLS_ANGLE_H
#define DEWALLS_ANGLE_H

#include <QString>
#include <QSharedPointer>
#include "unittype.h"

namespace dewalls {

class Angle : public UnitType<Angle>
{
public:
    static void init();

    static const Angle * 	   const type() { init(); return _type.data(); }
    static const Unit<Angle> * const degrees() { init(); return _type->_degrees; }
    static const Unit<Angle> * const deg() { init(); return _type->_degrees; }
    static const Unit<Angle> * const radians() { init(); return _type->_radians; }
    static const Unit<Angle> * const rad() { init(); return _type->_radians; }
    static const Unit<Angle> * const gradians() { init(); return _type->_gradians; }
    static const Unit<Angle> * const grad() { init(); return _type->_gradians; }
    static const Unit<Angle> * const milsNATO() { init(); return _type->_milsNATO; }
    static const Unit<Angle> * const mil() { init(); return _type->_milsNATO; }
    static const Unit<Angle> * const percentGrade() { init(); return _type->_percentGrade; }
    static const Unit<Angle> * const percent() { init(); return _type->_percentGrade; }

private:
    Angle();

    static QSharedPointer<Angle> _type;
    const Unit<Angle> * _degrees;
    const Unit<Angle> * _radians;
    const Unit<Angle> * _gradians;
    const Unit<Angle> * _milsNATO;
    const Unit<Angle> * _percentGrade;
};

} // namespace dewalls

#endif // DEWALLS_ANGLE_H
