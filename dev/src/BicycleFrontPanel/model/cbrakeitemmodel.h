#ifndef CBRAKEITEMMODEL_H
#define CBRAKEITEMMODEL_H

#include <QObject>
#include <QLabel>
#include "model/cbicycleitemmodel.h"
#include "model/clight.h"

class CBrakeItemModel : public CBicycleItemModel
{
    Q_OBJECT

public:
    explicit CBrakeItemModel(QObject* parent = nullptr);
    ~CBrakeItemModel() override {}

public:
    void setData(const int pin, const bool value) override;
    void setData(const int pin, const uint32_t value) override;
    void setMode(const int mode);
    void setState(const int state);
    void setRequest(const int request);
    void UpdateLight(const bool isUpdateView = false);
    void setImageData(const int light, const int brake);
    void changeLightAutoManMode(int mode);
    void changeLightManOnOffState(int state);

    int getLightDirection();

    void setLightPtr(CLight* light) { this->mLight = light; }

protected:
    void updateImageData();
    void updatePart();


public:
    //Model table row index.
    static const int MODEL_ROW_INDEX_BRAKE_STATE = 0;
    static const int MODEL_ROW_INDEX_BRAKE_GPIO_PIN = 1;
    static const int MODEL_ROW_INDEX_LIGHT_STATE = 2;
    static const int MODEL_ROW_INDEX_LIGHT_GPIO_PIN = 3;
    static const int MODEL_ROW_INDEX_IMAGE_PATH = 4;

    //Model table column index.
    static const int MODEL_COL_INDEX_FRONT_BRAKE_STATE = 0;
    static const int MODEL_COL_INDEX_REAR_BRAKE_STATE = 1;
    static const int MODEL_COL_INDEX_INTEGRATED_BRAKE_STATE = 2;
    static const int MODEL_COL_INDEX_LIGHT_TURN_ON_REQUEST = 0;
    static const int MODEL_COL_INDEX_LIGHT_TURN_ON_CONFIG = 1;
    static const int MODEL_COL_INDEX_LIGHT_MANUAL_TURN_ON_CONFIG = 2;
    static const int MODEL_COL_INDEX_LIGHT_TURN_ON_DIRECTION = 3;
    static const int MODEL_COL_INDEX_BRAKE_STATE_IMAGE = 0;

    static const int LIGHT_AUTO_MANUAL_MODE_AUTO = 0;
    static const int LIGHT_AUTO_MANUAL_MODE_MANUAL = 1;
    static const int LIGHT_MANUAL_SWITCH_STATE_OFF = 0;
    static const int LIGHT_MANUAL_SWITCH_STATE_ON = 1;

protected:
    CLight* mLight;
};

#endif // CBRAKEITEMMODEL_H
