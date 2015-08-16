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

    static const Length * 	   const type() { init(); return _type.data(); }
    static const Unit<Length> * const meters() { init(); return _type->_meters; }
    static const Unit<Length> * const m() { init(); return _type->_meters; }
    static const Unit<Length> * const centimeters() { init(); return _type->_centimeters; }
    static const Unit<Length> * const cm() { init(); return _type->_centimeters; }
    static const Unit<Length> * const kilometers() { init(); return _type->_kilometers; }
    static const Unit<Length> * const km() { init(); return _type->_kilometers; }
    static const Unit<Length> * const feet() { init(); return _type->_feet; }
    static const Unit<Length> * const ft() { init(); return _type->_feet; }
    static const Unit<Length> * const yards() { init(); return _type->_yards; }
    static const Unit<Length> * const yd() { init(); return _type->_yards; }
    static const Unit<Length> * const inches() { init(); return _type->_inches; }
    static const Unit<Length> * const in() { init(); return _type->_inches; }

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
