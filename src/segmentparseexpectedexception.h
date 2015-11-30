#ifndef DEWALLS_SEGMENTPARSEEXPECTEDEXCEPTION_H
#define DEWALLS_SEGMENTPARSEEXPECTEDEXCEPTION_H

#include <QSet>
#include <QList>
#include "segmentparseexception.h"
#include <initializer_list>
#include "dewallsexport.h"

namespace dewalls {

class DEWALLS_LIB_EXPORT SegmentParseExpectedException : public SegmentParseException
{
public:
    SegmentParseExpectedException(Segment segment, QString expectedItem);
    SegmentParseExpectedException(Segment segment, QList<QString> expectedItems);
    SegmentParseExpectedException(Segment segment, std::initializer_list<QString> expectedItems);
    QList<QString> expectedItems() const;
    virtual QString detailMessage() const;
private:
    QList<QString> _expectedItems;
};

inline QList<QString> SegmentParseExpectedException::expectedItems() const
{
    return _expectedItems;
}

} // namespace dewalls

#endif // DEWALLS_SEGMENTPARSEEXPECTEDEXCEPTION_H
