#ifndef DEWALLS_ABSTRACTPARSER_H
#define DEWALLS_ABSTRACTPARSER_H

#include <QObject>
#include "wallsmessage.h"

namespace dewalls {

class SegmentParseException;

class AbstractParser : public QObject
{
    Q_OBJECT

public:
    explicit AbstractParser(QObject *parent = 0);

signals:
    void message(WallsMessage message);
};

} // namespace dewalls

#endif // DEWALLS_ABSTRACTPARSER_H
