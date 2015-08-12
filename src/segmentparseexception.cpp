#include "segmentparseexception.h"

namespace dewalls {

QString SegmentParseException::message() const
{
    return QString("(in %1, line %2, column %3): %4\n%5").arg(
                _segment.source(),
                QString::number(_segment.startLine() + 1),
                QString::number(_segment.startCol() + 1),
                detailMessage(),
                _segment.underlineInContext());
}

} // namespace dewalls

