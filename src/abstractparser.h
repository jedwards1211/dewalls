#ifndef DEWALLS_ABSTRACTPARSER_H
#define DEWALLS_ABSTRACTPARSER_H

#include <QObject>

#include "severity.h"

namespace dewalls {

class SegmentParseException;
class SegmentParseExpectedException;

class AbstractParser : public QObject
{
    Q_OBJECT

public:
    explicit AbstractParser(QObject *parent = 0);

signals:
    void message(Severity severity, QString message);
    void message(Severity severity, QString message, QString source);
    void message(Severity severity, QString message, QString source,
                 int startLine, int startCol, int endLine, int endCol);

protected:
    void emitMessage(const SegmentParseException& exception);
    void emitMessage(const SegmentParseExpectedException& exception);
};

} // namespace dewalls

#endif // DEWALLS_ABSTRACTPARSER_H
