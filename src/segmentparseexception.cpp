#include "segmentparseexception.h"
#include "wallsmessage.h"

namespace dewalls {

QString SegmentParseException::message() const
{
    return WallsMessage(*this).toString();
}

} // namespace dewalls

