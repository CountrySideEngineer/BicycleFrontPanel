#include <iostream>
#include <exception>
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
    try {
        int rowIndex = this->Pin2RowIndex(pin);

        QModelIndex modelIndex = this->index(rowIndex, 0);
        this->mData[modelIndex] = QVariant(state);
        if (true == CBicycleItemModel::setData(modelIndex, QVariant(state))) {
            printf("CBicycleItemModel::setData() - OK\r\n");
        } else {
            printf("CBicycleItemModel::setData() - NG\r\n");
        }

        QModelIndex integratedModelIndex = this->index(MODEL_ROW_INDEX_INTEGRATED_BRAKE_STATE, 0);
        QVariant variant = this->mData[integratedModelIndex];
        int currentState = variant.toInt();
        if (false == state) {
            //Convert specified bit into 0.
            currentState &= ~(1 << rowIndex);
        } else {
            currentState |= (1 << rowIndex);
        }

        if (true == CBicycleItemModel::setData(integratedModelIndex, QVariant(currentState))) {
            printf("CBicycleItemModel::setData() - OK\r\n");
        } else {
            printf("CBicycleItemModel::setData() - NG\r\n");
        }
    } catch (std::invalid_argument &ex) {
        std::cout << ex.what() << std::endl;
        std::cout << "pin : " << pin << " is invalid" << std::endl;
    }
}
