#include <QString>
#include "cwheelitemmodel.h"

/**
 * @brief CWheelItemModel::VelocityValueConverter::Value2String
 * @param value
 * @return
 */
QString CWheelItemModel::VelocityValueConverter::Value2String(uint32_t value)
{
    uint32_t integerPart = value / 100;
    uint32_t decadePart = value % 100;
    char decadeArray[3] = { 0 };
    snprintf(decadeArray, 3, "%02d", decadePart);

    QString valueString =
            QString(integerPart) + QString(".") + QString(decadeArray) + this->getUnit();
    return valueString;
}

/**
 * @brief CWheelItemModel::VelocityValueConverter::getUnit
 * @return
 */
QString CWheelItemModel::VelocityValueConverter::getUnit() { return QString("km/h"); }

/**
 * @brief CWheelItemModel::RotateValueConverter::Value2String
 * @param value
 * @return
 */
QString CWheelItemModel::RotateValueConverter::Value2String(uint32_t value)
{
    QString valueString = QString(value) + this->getUnit();

    return valueString;
}

/**
 * @brief CWheelItemModel::RotateValueConverter::getUnit
 * @return
 */
QString CWheelItemModel::RotateValueConverter::getUnit() { return QString("[RPM]"); }

/**
 * @brief CWheelItemModel::CWheelItemModel  Constructs a CWheelItemModel object
 *                                          with given parent.
 * @param parent    Pointer to parent.
 */
CWheelItemModel::CWheelItemModel(QObject* parent)
    : CBicycleItemModel (parent)
{}

/**
 * @brief CWheelItemModel::setPinData   Set data of wheel, velocity and rotate into model.
 * @param pin   GPIO pin number.
 * @param velocity  Velocity of wheel to set
 * @param rotate
 */
void CWheelItemModel::setData(const int pin, const uint32_t rotate, const uint32_t velocity)
{
    int rowIndex = this->Pin2RowIndex(pin);

    RotateValueConverter rotateConverter;
    VelocityValueConverter velocityConverter;
    this->setData(rowIndex, MODEL_COLUMN_INDEX_ROTATE_TO_SHOW, rotate, rotateConverter);
    this->setData(rowIndex, MODEL_COLUMN_INDEX_VELOCITY_TO_SHOW, velocity, velocityConverter);
}

/**
 * @brief CWheelItemModel::setData
 * @param rowIndex
 * @param columnIndex
 * @param value
 * @param converter
 */
void CWheelItemModel::setData(
        const int rowIndex,
        const int columnIndex,
        const uint32_t value,
        IValueConverter &converter)
{
    QModelIndex modelIndex = this->index(rowIndex, columnIndex);
    CBicycleItemModel::setData(modelIndex, QVariant(value));

    this->setData(columnIndex, converter);
}

#define AVERAGE(value1, value2)     (((value1) >> 1) + (value2 >> 1))
/**
 * @brief CWheelItemModel::updateData
 * @param columnIndex
 * @param converter
 */
void CWheelItemModel::setData(const int columnIndex, IValueConverter &converter)
{
    QModelIndex frontModelIndex = this->index(MODEL_ROW_INDEX_FRONT_WHEEL_MODEL, columnIndex);
    QVariant frontVariant = this->data(frontModelIndex);
    uint32_t frontValue = frontVariant.toUInt();

    QModelIndex rearModelIndex = this->index(MODEL_ROW_INDEX_REAR_WHEEL_MODEL, columnIndex);
    QVariant rearVariant = this->data(rearModelIndex);
    uint32_t rearValue = rearVariant.toUInt();

    uint32_t average = AVERAGE(frontValue, rearValue);
    QModelIndex averageModelIndex = this->index(MODEL_ROW_INDEX_INTEGRATED_WHEEL_MODEL, columnIndex);
    CBicycleItemModel::setData(averageModelIndex, QVariant(average), false);

    averageModelIndex = this->index(MODEL_ROW_INDEX_INTEGRATED_WHEEL_MODEL, columnIndex + 2);
    QString stringAverage = converter.Value2String(average);
    CBicycleItemModel::setData(averageModelIndex, QVariant(average));
}

