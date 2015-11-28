#ifndef DEWALLS_GENERICEXCEPTION_H
#define DEWALLS_GENERICEXCEPTION_H

#include <QException>
#include <QString>

namespace dewalls {

class GenericException : public QException
{
public:
    inline GenericException(QString message) : Message(message) {}
    inline virtual void raise() { throw *this; }
    inline virtual GenericException *clone() { return new GenericException(*this); }

    inline QString message() const { return Message; }

private:
    QString Message;
};

} // namespace dewalls

#endif // DEWALLS_GENERICEXCEPTION_H
