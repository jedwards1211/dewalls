#ifndef DEWALLS_ANGLE_H
#define DEWALLS_ANGLE_H

#include <QString>
#include <QSharedPointer>
#include "unittype.h"
#include "dewallsexport.h"

namespace dewalls {

class DEWALLS_LIB_EXPORT Angle : public UnitType<Angle>
{
public:
    static void init();

    static const Angle *       type() { init(); return _type.data(); }
    static const Unit<Angle> * degrees() { init(); return _type->_degrees; }
    static const Unit<Angle> * deg() { init(); return _type->_degrees; }
    static const Unit<Angle> * radians() { init(); return _type->_radians; }
    static const Unit<Angle> * rad() { init(); return _type->_radians; }
    static const Unit<Angle> * gradians() { init(); return _type->_gradians; }
    static const Unit<Angle> * grad() { init(); return _type->_gradians; }
    static const Unit<Angle> * milsNATO() { init(); return _type->_milsNATO; }
    static const Unit<Angle> * mil() { init(); return _type->_milsNATO; }
    static const Unit<Angle> * percentGrade() { init(); return _type->_percentGrade; }
    static const Unit<Angle> * percent() { init(); return _type->_percentGrade; }

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
