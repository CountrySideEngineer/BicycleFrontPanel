#include <QString>
#include "cwheelitemmodel.h"

/**
 * @brief CWheelItemModel::VelocityValueConverter::Value2String     Convert velocity value
 *                                                                  in uint32_t data type into QString.
 *                                                                  The format is xxxx.yyy, the decade part
 *                                                                  has 3 number.
 * @param value Value to convert.
 * @return  Convert value in QString data type.
 */
QString CWheelItemModel::VelocityValueConverter::Value2String(uint32_t value)
{
    uint32_t integerPart = value / 100;
    uint32_t decadePart = value % 100;

    QString valueString =
            QString::number(integerPart) +
            QString(".") +
            QString::number(decadePart).rightJustified(0, 2) +
            this->getUnit();
    return valueString;
}

/**
 * @brief CWheelItemModel::VelocityValueConverter::getUnit  Returns the unit of velocity.
 * @return  Unit of velocity.
 */
QString CWheelItemModel::VelocityValueConverter::getUnit() { return QString("[km/h]"); }

/**
 * @brief CWheelItemModel::RotateValueConverter::Value2String   Convert the number of rotation value
 *                                                              in uint32_t data type into QString.
 * @param value Rotation value to be converted.
 * @return  Converted value in QStirng data type.
 */
QString CWheelItemModel::RotateValueConverter::Value2String(uint32_t value)
{
    QString valueString = QString::number(value) + this->getUnit();

    return valueString;
}

/**
 * @brief CWheelItemModel::RotateValueConverter::getUnit    Returns teh unit of rotation.
 * @return  Unit of rotation.
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
    this->setData(rowIndex, MODEL_COLUMN_INDEX_ROTATE, rotate, rotateConverter);
    this->setData(rowIndex, MODEL_COLUMN_INDEX_VELOCITY, velocity, velocityConverter);
}

/**
 * @brief CWheelItemModel::setData  Set data into model specified by index of row, rowIndex, and column, columnIndex.
 *                                  The value is set both in raw number and string with format.
 * @param rowIndex  Index of row of model the value to be set.
 * @param columnIndex   Index of column of model the value to be set.
 * @param value     The value to be set to model.
 * @param converter Instance of IValueConverter's subclass to be used convert value into string.
 */
void CWheelItemModel::setData(
        const int rowIndex,
        const int columnIndex,
        const uint32_t value,
        IValueConverter &converter)
{
    QModelIndex modelIndex = this->index(rowIndex, columnIndex);
    CBicycleItemModel::setData(modelIndex, QVariant(value), false);

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
    CBicycleItemModel::setData(averageModelIndex, QVariant(stringAverage));
}

