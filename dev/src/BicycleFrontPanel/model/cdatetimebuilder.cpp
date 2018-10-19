#include "cdatetimebuilder.h"

/**
 * @brief Constructor of CDateTimeBuilder class.
 */
CDateTimeBuilder::CDateTimeBuilder() : mDateTime(nullptr)
{
    this->mDateFormat = DATE_FORMAT::DATE_YYYY_MM_DD;
    this->mTimeFormat = TIME_FORMAT::TIME_HH_MM_AP;
    this->mDateTime = new CDateTime(QString("yyyy/MM/dd"));
}

/**
 * @brief Destructor of CDateTimeBuilder.
 */
CDateTimeBuilder::~CDateTimeBuilder()
{
    if (nullptr != this->mDateTime) {
        delete this->mDateTime;
    }
}

/**
 * @brief Returns pointer to CDateTime object.The object contains the format
 *        of date time.
 * @return Pointer to CDateTime object which contains date time format.
 */
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

/**
 * @brief Returns pointer to CDateTime object.The object contains the format
 *        of time of day.
 * @return Pointer to CDateTime object which contains time of day.
 */
CDateTime* CDateTimeBuilder::createTime() const
{
    QString timeFormat = QString();
    switch (this->mTimeFormat) {
    case TIME_HH_MM: timeFormat = QString("hh:mm"); break;
    case TIME_HH_MM_AP: timeFormat = QString("hh:mm(AP)"); break;
    case TIME_H_M: timeFormat = QString("h:m"); break;
    case TIME_H_M_AP: timeFormat = QString("h:m(AP)"); break;
    }

    this->mDateTime->setFormat(timeFormat);
    return this->mDateTime;
}
