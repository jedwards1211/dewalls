#include "abstractparser.h"

#include "segmentparseexception.h"
#include "segmentparseexpectedexception.h"

namespace dewalls {

AbstractParser::AbstractParser(QObject *parent) : QObject(parent)
{

}

void AbstractParser::emitMessage(const SegmentParseException &ex) {
    emit message(Severity::Error, ex.detailMessage(), ex.segment().source(),
                 ex.segment().startLine(), ex.segment().startCol(),
                 ex.segment().endLine(), ex.segment().endCol());
}

void AbstractParser::emitMessage(const SegmentParseExpectedException &ex) {
    emit message(Severity::Error, ex.detailMessage() + '\n' + ex.segment().underlineInContext(),
                 ex.segment().source(),
                 ex.segment().startLine(), ex.segment().startCol(),
                 ex.segment().endLine(), ex.segment().endCol());
}


} // namespace dewalls

