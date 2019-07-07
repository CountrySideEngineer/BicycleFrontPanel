#ifndef CWHEELITEMMODEL_H
#define CWHEELITEMMODEL_H

#include <QObject>
#include "model/cbicycleitemmodel.h"

class CWheelItemModel : public CBicycleItemModel
{
    Q_OBJECT

protected:
    /**
     * @brief The AValueConverter class
     */
    class IValueConverter
    {
    public:
        virtual ~IValueConverter() {}

    public:
        virtual QString Value2String(uint32_t value) = 0;

    protected:
        virtual QString getUnit() = 0;
    };

    /**
     * @brief The VelocityValueConverter class
     */
    class VelocityValueConverter : public IValueConverter
    {
    public:
        explicit VelocityValueConverter() {}

    public:
        QString Value2String(uint32_t value) override;

    protected:
        QString getUnit() override;
    };

    /**
     * @brief The RotateValueConverter class
     */
    class RotateValueConverter : public IValueConverter
    {
    public:
        explicit RotateValueConverter() {}

    public:
        QString Value2String(uint32_t value) override;

    protected:
        QString getUnit() override;
    };

public:
    explicit CWheelItemModel(QObject* parent = nullptr);
    ~CWheelItemModel() override {}

public:
    void setData(const int pin, const uint32_t rotate, const uint32_t velocity) override;
    virtual void setData(const int rowIndex, const int columnIndex, const uint32_t value, IValueConverter &converter);
    virtual void setData(const int columnIndex, IValueConverter &converter);

protected:

public:
    static const int MODEL_ROW_INDEX_FRONT_WHEEL_MODEL = 0;
    static const int MODEL_ROW_INDEX_REAR_WHEEL_MODEL = 1;
    static const int MODEL_ROW_INDEX_INTEGRATED_WHEEL_MODEL = 2;
    static const int MODEL_COLUMN_INDEX_ROTATE = 0;
    static const int MODEL_COLUMN_INDEX_VELOCITY = 1;
    static const int MODEL_COLUMN_INDEX_ROTATE_TO_SHOW = 2;
    static const int MODEL_COLUMN_INDEX_VELOCITY_TO_SHOW = 3;

};

#endif // CWHEELITEMMODEL_H
