#include "wallsmessage.h"

namespace dewalls {

WallsMessage::WallsMessage(QString severity, QString message, QString source,
             int startLine, int startColumn, int endLine, int endColumn)
    : Severity(severity), Message(message), Source(source),
      StartLine(startLine), StartColumn(startColumn),
      EndLine(endLine), EndColumn(endColumn)
{
}

WallsMessage::WallsMessage(QString severity, QString message, Segment segment)
    : Severity(severity), Message(message), Source(segment.source()),
      StartLine(segment.startLine()), StartColumn(segment.startCol()),
      EndLine(segment.endLine()), EndColumn(segment.endCol())
{
}

WallsMessage::WallsMessage(const SegmentParseException& ex)
    : Severity("error"),
      Message(ex.detailMessage() + "\n" + ex.segment().underlineInContext()),
      Source(ex.segment().source()),
      StartLine(ex.segment().startLine()), StartColumn(ex.segment().startCol()),
      EndLine(ex.segment().endLine()), EndColumn(ex.segment().endCol())
{
}

QString WallsMessage::toString()
{
    QString result = QString("%1: %2").arg(Severity, Message);
    if (!Source.isNull()) {
        result += "\n(" + Source;
        if (StartLine >= 0) {
            result += QString(", line %1").arg(StartLine + 1);
            if (StartColumn >= 0) {
                result += QString(", column %1").arg(StartColumn + 1);
            }
        }
        result += ")";
    }
    return result;
}

} // namespace dewalls

