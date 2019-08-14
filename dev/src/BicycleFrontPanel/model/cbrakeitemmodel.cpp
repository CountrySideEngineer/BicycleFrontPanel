#include <iostream>
#include <exception>
#include "cbrakeitemmodel.h"
#include "model/cimageresourcemanager.h"
#include "model/cimageresource.h"

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
        int colIndex = this->Pin2ColIndex(pin);
        QModelIndex modelIndex = this->index(rowIndex, colIndex);
        CBicycleItemModel::setData(modelIndex, QVariant(state), false);

        QModelIndex integratedModelIndex = this->index(
                    MODEL_ROW_INDEX_BRAKE_STATE,
                    MODEL_COL_INDEX_INTEGRATED_BRAKE_STATE);
        QVariant variant = this->data(integratedModelIndex);
        int currentState = variant.toInt();
        if (false == state) {
            //Convert specified bit into 0.
            currentState &= ~(1 << colIndex);
        } else {
            currentState |= (1 << colIndex);
        }
        CBicycleItemModel::setData(integratedModelIndex, QVariant(currentState), false);

        this->updateImageData();
        this->updatePart();
    } catch (std::invalid_argument &ex) {
        std::cout << ex.what() << std::endl;
        std::cout << "pin : " << pin << " is invalid" << std::endl;
    }
}

void CBrakeItemModel::setData(const int pin, const uint32_t state)
{
    int rowIndex = this->Pin2RowIndex(pin);
    int colIndex = this->Pin2ColIndex(pin);
    QModelIndex modelIndex = this->index(rowIndex, colIndex);
    CBicycleItemModel::setData(modelIndex, QVariant(state), false);

    this->UpdateLight();
    this->updateImageData();
    this->updatePart();
}


/**
 * @brief CBrakeItemModel::setMode  Set "mode" value into model.
 * @param mode  Mode to set.
 */
void CBrakeItemModel::setMode(const int mode)
{
    auto modelIndex = this->index(MODEL_ROW_INDEX_LIGHT_STATE,
                                  MODEL_COL_INDEX_LIGHT_TURN_ON_CONFIG);
    CBicycleItemModel::setData(modelIndex, QVariant(mode), false);

    this->UpdateLight();
    this->updateImageData();
}

/**
 * @brief CBrakeItemModel::setState Set "State" value into model.
 * @param state State to set.
 */
void CBrakeItemModel::setState(const int state)
{
    auto modelIndex = this->index(MODEL_ROW_INDEX_LIGHT_STATE,
                                  MODEL_COL_INDEX_LIGHT_MANUAL_TURN_ON_CONFIG);
    CBicycleItemModel::setData(modelIndex, QVariant(state), false);

    this->UpdateLight();
    this->updateImageData();
}

/**
 * @brief CBrakeItemModel::setRequest   Set "Request" value into model.
 *                                      The value is assumed to be sent from otehr device.
 * @param request   A request sent from other deivce showing whether the light should be
 *                  turned on or not.
 */
void CBrakeItemModel::setRequest(const int request)
{
    auto modelIndex = this->index(MODEL_ROW_INDEX_LIGHT_STATE,
                                  MODEL_COL_INDEX_LIGHT_TURN_ON_REQUEST);
    CBicycleItemModel::setData(modelIndex, QVariant(request), false);

    this->UpdateLight();
    this->updateImageData();
}


/**
 * @brief CBrakeItemModel::UpdateLight  Update light directin value in model.
 */
void CBrakeItemModel::UpdateLight(const bool isUpdateView)
{
    auto turnOnConfigModelIndex = this->index(MODEL_ROW_INDEX_LIGHT_STATE,
                                              MODEL_COL_INDEX_LIGHT_TURN_ON_CONFIG);
    auto turnOnConfigVariant = this->data(turnOnConfigModelIndex);
    int turnOnConfigValue = turnOnConfigVariant.toInt();

    int turnOnDirection = 0;
    if (0 == turnOnConfigValue) {
        //Turn on the liht automatically
        auto turnOnReqModelIndex = this->index(MODEL_ROW_INDEX_LIGHT_STATE,
                                               MODEL_COL_INDEX_LIGHT_TURN_ON_REQUEST);
        auto turnOnReqVariant = this->data(turnOnReqModelIndex);
        int turnOnReqValue = turnOnReqVariant.toInt();
        if (1 == turnOnReqValue) {
            turnOnDirection = 1;
        } else {
            turnOnDirection = 0;
        }
    } else if (1 == turnOnConfigValue){
        //Tunr on the light manually.
        auto manualTurnOnConfigModelIndex = this->index(MODEL_ROW_INDEX_LIGHT_STATE,
                                                        MODEL_COL_INDEX_LIGHT_MANUAL_TURN_ON_CONFIG);
        auto manualTurnOnConfigVariant = this->data(manualTurnOnConfigModelIndex);
        int manualTurnOnConfigValue = manualTurnOnConfigVariant.toInt();
        if (1 == manualTurnOnConfigValue) {
            turnOnDirection = 1;
        } else {
            turnOnDirection = 0;
        }
    }

    auto turnOnDirectionModelIndex = this->index(MODEL_ROW_INDEX_LIGHT_STATE,
                                                 MODEL_COL_INDEX_LIGHT_TURN_ON_DIRECTION);
    CBicycleItemModel::setData(turnOnDirectionModelIndex, QVariant(turnOnDirection), isUpdateView);
}

/**
 * @brief CBrakeItemModel::getLightDirection    Returns the direction of light, turn on or off.
 * @return  The direciont of light. The value 1 means that the direction is "ON",
 *          otherwise it means "OFF".
 */
int CBrakeItemModel::getLightDirection()
{
    auto directionModelIndex = this->index(MODEL_ROW_INDEX_LIGHT_STATE,
                                           MODEL_COL_INDEX_LIGHT_TURN_ON_DIRECTION);
    auto directionVariant = this->data(directionModelIndex);

    return directionVariant.toInt();
}


/**
 * @brief CBrakeItemModel::updateImageData  Update image data.
 */
void CBrakeItemModel::updateImageData()
{
    auto turnOnConfigModelIndex = this->index(MODEL_ROW_INDEX_LIGHT_STATE,
                                              MODEL_COL_INDEX_LIGHT_TURN_ON_CONFIG);
    int turnOnConfigInt = this->data(turnOnConfigModelIndex).toInt();
    int lightState = 0;

    if (LIGHT_AUTO_MANUAL_MODE_AUTO == turnOnConfigInt) {
        auto turnOnRequestModelIndex = this->index(MODEL_ROW_INDEX_LIGHT_STATE,
                                                   MODEL_COL_INDEX_LIGHT_TURN_ON_REQUEST);
        int turnOnRequest = this->data(turnOnRequestModelIndex).toInt();
        if (0 == turnOnRequest) {
            lightState = 0;
        } else {
            lightState = 1;
        }
    } else {
        auto manualTurnOnConfigModelIndex = this->index(MODEL_ROW_INDEX_LIGHT_STATE,
                                                        MODEL_COL_INDEX_LIGHT_MANUAL_TURN_ON_CONFIG);
        auto manualTurnOnConfig = this->data(manualTurnOnConfigModelIndex).toInt();
        if (0 == manualTurnOnConfig) {
            lightState = 3;
        } else {
            lightState = 2;
        }
    }

    auto brakeModelIndex = this->index(MODEL_ROW_INDEX_BRAKE_STATE,
                                       MODEL_COL_INDEX_INTEGRATED_BRAKE_STATE);
    auto brakeVariant = this->data(brakeModelIndex);
    int brakeState = brakeVariant.toInt();

    this->setImageData(lightState, brakeState);
}

/**
 * @brief CBrakeItemModel::setImageData Set image corresponding to state of light and brake.
 * @param light     State of light
 * @param brake     State of brake.
 */
void CBrakeItemModel::setImageData(const int light, const int brake)
{
    try {
        CImageResourceManager resourceManager;
        QString imagePath = resourceManager.getImageResourcePath(light, brake);
        QModelIndex modelIndex = this->index(MODEL_ROW_INDEX_IMAGE_PATH,
                                             MODEL_COL_INDEX_BRAKE_STATE_IMAGE);
        CBicycleItemModel::setData(modelIndex, QVariant(imagePath));
    }
    catch (std::invalid_argument &ex) {
        std::cout << ex.what() << std::endl;
        std::cout << "light = " << light << " brake = " << brake << std::endl;
    }
}

/**
 * @brief CBrakeItemModel::changeLightAutoManMode   Change model of light, "auto" or "manual".
 * @param mode  Model to set. The value 0 is "auto", 1 is "manual", and otherwise "auto".
 */
void CBrakeItemModel::changeLightAutoManMode(int mode)
{
    int modeToSet = 0;
    if (0 == mode) {
        modeToSet = LIGHT_AUTO_MANUAL_MODE_AUTO;
    } else if (1 == mode) {
        modeToSet = LIGHT_AUTO_MANUAL_MODE_MANUAL;
    } else {
        /*
         * If the mode to set is invalid, user can not control light on or off.
         * To avoid such a situation, invalid "mode" valud set to corresponding to
         * auto mode.
         */
        modeToSet = LIGHT_AUTO_MANUAL_MODE_AUTO;
    }
    this->setMode(modeToSet);
    this->updatePart();
}

void CBrakeItemModel::changeLightManOnOffState(int state)
{
    int stateToSet = LIGHT_MANUAL_SWITCH_STATE_OFF;
    if (0 == state) {
        stateToSet = LIGHT_MANUAL_SWITCH_STATE_OFF;
    } else {
        stateToSet = LIGHT_MANUAL_SWITCH_STATE_ON;
    }
    this->setState(stateToSet);
    this->updatePart();
}

void CBrakeItemModel::updatePart()
{
    try {
        auto directionIndex = this->index(MODEL_ROW_INDEX_LIGHT_STATE,
                                          MODEL_COL_INDEX_LIGHT_TURN_ON_DIRECTION);
        int32_t lightLevel = this->data(directionIndex).toInt();
        this->mLight->UpdateState(lightLevel);
    } catch (...) {
        std::cout << "An exception occurred while update light." << std::endl;
    }
}
