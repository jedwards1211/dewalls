#ifndef DEWALLS_WALLSMESSAGE_H
#define DEWALLS_WALLSMESSAGE_H

#include <QString>
#include "segmentparseexception.h"
#include "dewallsexport.h"

namespace dewalls {

///
/// \brief a message emitted by WallsSurveyParser or WallsProjectParser, usually an error or a
/// warning
///
class DEWALLS_LIB_EXPORT WallsMessage {
public:
    WallsMessage(QString severity, QString message, QString source = QString(),
                 int startLine = -1, int startColumn = -1, int endLine = -1, int endColumn = -1);
    WallsMessage(QString severity, QString message, Segment segment);
    WallsMessage(const SegmentParseException& ex);

    inline QString severity() const { return Severity; }
    inline QString message() const { return Message; }
    inline QString source() const { return Source; }
    inline int startLine() const { return StartLine; }
    inline int startColumn() const { return StartColumn; }
    inline int endLine() const { return EndLine; }
    inline int endColumn() const { return EndColumn; }

    QString toString();

private:
    QString Severity;
    QString Message;
    QString Source;
    int StartLine;
    int StartColumn;
    int EndLine;
    int EndColumn;
};

} // namespace dewalls

#endif // DEWALLS_WALLSMESSAGE_H
