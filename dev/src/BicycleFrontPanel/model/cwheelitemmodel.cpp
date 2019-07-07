#include "cwheelitemmodel.h"

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

    QModelIndex rotateModelIndex = this->index(rowIndex, CWheelItemModel::MODEL_COLUMN_INDEX_ROTATE);
    CBicycleItemModel::setData(rotateModelIndex, QVariant(rotate));

    QModelIndex velocityModelIndex = this->index(rowIndex, CWheelItemModel::MODEL_COLUMN_INDEX_VELOCITY);
    CBicycleItemModel::setData(velocityModelIndex, QVariant(velocity));

    this->updateData(CWheelItemModel::MODEL_COLUMN_INDEX_ROTATE);
    this->updateData(CWheelItemModel::MODEL_COLUMN_INDEX_VELOCITY);
}

#define AVERAGE(value1, value2)     (((value1) >> 1) + (value2 >> 1))

/**
 * @brief CWheelItemModel::updateData   Update model data of integrated index.
 * @param columnIndex   Column index of
 */
void CWheelItemModel::updateData(int columnIndex)
{
    QModelIndex frontWheelModelIndex =
            this->index(MODEL_ROW_INDEX_FRONT_WHEEL_MODEL, columnIndex);
    QVariant frontWheelModel = this->data(frontWheelModelIndex);
    auto frontWheelModelValue = frontWheelModel.toUInt();

    QModelIndex rearWheelModelIndex =
            this->index(MODEL_ROW_INDEX_REAR_WHEEL_MODEL, columnIndex);
    QVariant rearWheelModel = this->data(rearWheelModelIndex);
    auto rearWheelModelValue = rearWheelModel.toUInt();

    auto wheelModelValue = AVERAGE(frontWheelModelValue, rearWheelModelValue);

    QModelIndex wheelModelIndex =
            this->index(MODEL_ROW_INDEX_INTEGRATED_WHEEL_MODEL, columnIndex);
    CBicycleItemModel::setData(wheelModelIndex, QVariant(wheelModelValue));
}

