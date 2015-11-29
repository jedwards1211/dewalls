#ifndef DEWALLS_ABSTRACTPARSER_H
#define DEWALLS_ABSTRACTPARSER_H

#include <QObject>

#include "severity.h"

namespace dewalls {

class SegmentParseException;

class AbstractParser : public QObject
{
    Q_OBJECT

public:
    explicit AbstractParser(QObject *parent = 0);

signals:
    void message(Severity severity, QString message, QString source = QString(),
                 int startLine = -1, int startCol = -1,
                 int endLine = -1, int endCol = -1);

protected:
    void emitMessage(const SegmentParseException& exception);
};

} // namespace dewalls

#endif // DEWALLS_ABSTRACTPARSER_H
