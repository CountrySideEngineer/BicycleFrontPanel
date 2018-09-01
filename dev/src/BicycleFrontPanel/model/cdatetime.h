#ifndef CDATETIME_H
#define CDATETIME_H
#include <QObject>
#include <QString>

/**
 * @brief The CDateTime class.
 *        Handle date and time string.
 */
class CDateTime
{
public:
    CDateTime(QString format) { this->setFormat(format); }
    virtual ~CDateTime() {}

public:
    QString getTime();

    void setFormat(QString format) { this->mFormat = format; }

protected:
    QString mFormat;
};

#endif // CDATETIME_H
