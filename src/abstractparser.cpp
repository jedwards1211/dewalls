#include "abstractparser.h"

#include "segmentparseexception.h"

namespace dewalls {

AbstractParser::AbstractParser(QObject *parent) : QObject(parent)
{

}

void AbstractParser::emitMessage(const SegmentParseException &ex) {
    emit message("error", ex.detailMessage(), ex.segment().source(),
                 ex.segment().startLine(), ex.segment().startCol(),
                 ex.segment().endLine(), ex.segment().endCol());
}

} // namespace dewalls

