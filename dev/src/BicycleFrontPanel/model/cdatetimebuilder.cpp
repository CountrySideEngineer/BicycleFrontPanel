#include "cdatetimebuilder.h"

CDateTimeBuilder::CDateTimeBuilder() : mDateTime(nullptr) {}
CDateTimeBuilder::~CDateTimeBuilder()
{
    if (nullptr != this->mDateTime) {
        delete this->mDateTime;
    }
}

CDateTime* CDateTimeBuilder::createDate() const
{
    QString dateFormat = QString();
    switch (this->mDateFormat) {
    case DATE_YYYY_MM_DD: dateFormat = QString("yyyy/MM/dd"); break;
    case DATE_YYYY_M_D: dateFormat = QString("yyyy/M/d"); break;
    case DATE_YY_MM_DD: dateFormat = QString("yy/MM/dd"); break;
    case DATE_YY_M_D: dateFormat = QString("yy/M/d"); break;
    }

    this->mDateTime->setFormat(dateFormat);
    return this->mDateTime;
}

CDateTime* CDateTimeBuilder::createTime() const
{
    QString timeFormat = QString();
    switch (this->mTimeFormat) {
    case TIME_HH_MM: timeFormat = QString("hh:mm"); break;
    case TIME_HH_MM_AP: timeFormat = QString("hh:mm(AP)");  break;
    case TIME_H_M: timeFormat = QString("h:m"); break;
    case TIME_H_M_AP: timeFormat = QString("h:m(AP)"); break;
    }

    this->mDateTime->setFormat(timeFormat);
    return this->mDateTime;
}
