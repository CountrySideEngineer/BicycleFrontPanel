#include "cdatetime.h"
#include <QDateTime>

/**
 * @brief   This function returns current date and time in specified format.
 *          The format must be set before this function is called.
 * @return  Returns the date time in specified format if the operation finished
 *          successfully, otherwise returns a message "FORMAT INVALID".
 */
QString CDateTime::getTime()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QString dateString = dateTime.toString(this->mFormat);

    if (dateString.isEmpty()) {
        dateString = "FORMAT INVALID";
    }
    return dateString;
}
