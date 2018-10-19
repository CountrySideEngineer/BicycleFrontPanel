#ifndef CDATETIMEBUILDER_H
#define CDATETIMEBUILDER_H

#include <QObject>
#include "cdatetime.h"

class CDateTimeBuilder
{
public:
    enum DATE_FORMAT {
        DATE_YYYY_MM_DD = 0,
        DATE_YYYY_M_D,
        DATE_YY_MM_DD,
        DATE_YY_M_D,
    };

    enum TIME_FORMAT {
        TIME_HH_MM_AP = 0,
        TIME_H_M_AP,
        TIME_HH_MM,
        TIME_H_M,
    };

public:
    CDateTimeBuilder();
    virtual ~CDateTimeBuilder();

    void setDateFormat(DATE_FORMAT format) { this->mDateFormat = format; }
    void setTimeFormat(TIME_FORMAT format) { this->mTimeFormat = format; }

    CDateTime* createDate() const;
    CDateTime* createTime() const;

protected:
    DATE_FORMAT mDateFormat;
    TIME_FORMAT mTimeFormat;

    CDateTime* mDateTime;
};

#endif // CDATETIMEBUILDER_H
