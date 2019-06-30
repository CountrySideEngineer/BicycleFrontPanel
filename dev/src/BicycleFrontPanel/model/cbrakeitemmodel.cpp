#include "cbrakeitemmodel.h"

/**
 * @brief CBrakeItemModel::CBrakeItemModel  Constructs a CBrakeItemModel with the given parent.
 * @param parent    Pointer to QObject which manages the instance of this class.
 */
CBrakeItemModel::CBrakeItemModel(QObject* parent)
    : CBicycleItemModel (parent)
{}

/**
 * @brief CBrakeItemModel::setPinData   Set brake state of front, rear, and integrated state.
 *                                      The "integrated" value is supposed to be used to determine
 *                                      which image to be shown in the view.
 * @param pin   GPIO pin number.
 * @param state State of brake, when the brake is on, the value is true, otherwise false.
 */
void CBrakeItemModel::setData(const int pin, const bool state)
{
    int rowIndex = this->Pin2RowIndex(pin);
    QModelIndex modelIndex = this->index(rowIndex, 0);
    CBicycleItemModel::setData(modelIndex, QVariant(state));

    QModelIndex integratedModelIndex = this->index(MODEL_ROW_INDEX_INTEGRATED_BRAKE_STATE, 0);
    QVariant variant = this->mData[integratedModelIndex];
    int currentState = variant.toInt();
    if (false == state) {
        //Convert specified bit into 0.
        currentState &= ~(1 << rowIndex);
    } else {
        currentState |= (1 << rowIndex);
    }
    CBicycleItemModel::setData(integratedModelIndex, QVariant(currentState));
}
