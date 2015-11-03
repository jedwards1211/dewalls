#include "wallstypes.h"

namespace dewalls
{

QString applyCaseType(CaseType type, const QString& s)
{
    switch (type)
    {
    case Lower:
        return s.toLower();
    case Upper:
        return s.toUpper();
    default:
        break;
    }
    return s;
}

}
