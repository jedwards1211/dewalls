#ifndef DEWALLS_AMBIGUOUSVECTOREXCEPTION_H
#define DEWALLS_AMBIGUOUSVECTOREXCEPTION_H

#include <QException>

#include "dewallsexport.h"

namespace dewalls {

class DEWALLS_LIB_EXPORT AmbiguousVectorException : public QException
{
public:
    inline AmbiguousVectorException() : QException() {}
    virtual void raise() const { throw *this; }
    virtual AmbiguousVectorException *clone() const { return new AmbiguousVectorException(*this); }
};

} // namespace dewalls

#endif // DEWALLS_AMBIGUOUSVECTOREXCEPTION_H
