#ifndef DEWALLS_LENGTH_H
#define DEWALLS_LENGTH_H

#include <QString>
#include "unittype.h"
#include "defaultunit.h"
#include <QSharedPointer>

namespace dewalls {

class Length : public UnitType<Length>
{
public:
    static void init();

    static const Length *       type() { init(); return _type.data(); }
    static const Unit<Length> * meters() { init(); return _type->_meters; }
    static const Unit<Length> * m() { init(); return _type->_meters; }
    static const Unit<Length> * centimeters() { init(); return _type->_centimeters; }
    static const Unit<Length> * cm() { init(); return _type->_centimeters; }
    static const Unit<Length> * kilometers() { init(); return _type->_kilometers; }
    static const Unit<Length> * km() { init(); return _type->_kilometers; }
    static const Unit<Length> * feet() { init(); return _type->_feet; }
    static const Unit<Length> * ft() { init(); return _type->_feet; }
    static const Unit<Length> * yards() { init(); return _type->_yards; }
    static const Unit<Length> * yd() { init(); return _type->_yards; }
    static const Unit<Length> * inches() { init(); return _type->_inches; }
    static const Unit<Length> * in() { init(); return _type->_inches; }

private:
    Length();

    static QSharedPointer<Length> _type;
    const DefaultUnit<Length>* _meters;
    const DefaultUnit<Length>* _centimeters;
    const DefaultUnit<Length>* _kilometers;
    const DefaultUnit<Length>* _feet;
    const DefaultUnit<Length>* _yards;
    const DefaultUnit<Length>* _inches;
};

} // namespace dewalls

#endif // DEWALLS_LENGTH_H
