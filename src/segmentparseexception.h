#ifndef DEWALLS_SEGMENTPARSEEXCEPTION_H
#define DEWALLS_SEGMENTPARSEEXCEPTION_H

#include <exception>

#include "segment.h"
#include "dewallsexport.h"

namespace dewalls {

class DEWALLS_LIB_EXPORT SegmentParseException : public std::exception
{
public:
    SegmentParseException(Segment segment);
    SegmentParseException(Segment segment, QString detailMessage);
    Segment segment() const;
    virtual QString detailMessage() const;
    virtual QString message() const;
private:
    Segment _segment;
    QString _detailMessage;
};

inline SegmentParseException::SegmentParseException(Segment segment, QString detailMessage)
    : _segment(segment),
      _detailMessage(detailMessage)
{

}

inline SegmentParseException::SegmentParseException(Segment segment)
    : SegmentParseException(segment, "")
{

}

inline Segment SegmentParseException::segment() const
{
    return _segment;
}

inline QString SegmentParseException::detailMessage() const
{
    return _detailMessage;
}

} // namespace dewalls

#endif // DEWALLS_SEGMENTPARSEEXCEPTION_H
