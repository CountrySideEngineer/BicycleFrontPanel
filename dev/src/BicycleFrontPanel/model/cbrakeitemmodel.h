#ifndef CBRAKEITEMMODEL_H
#define CBRAKEITEMMODEL_H

#include <QObject>
#include <QLabel>
#include "model/cbicycleitemmodel.h"

class CBrakeItemModel : public CBicycleItemModel
{
    Q_OBJECT

public:
    explicit CBrakeItemModel(QObject* parent = nullptr);
    ~CBrakeItemModel() override {}

public:
    void setData(const int pin, const bool value) override;
    void setMode(const int mode);
    void setState(const int state);
    void setRequest(const int request);
    void UpdateLight();
    void setImageData(const int light, const int brake);

    int getLightDirection();

protected:
    void updateImageData();


public:
    //Model table row index.
    static const int MODEL_ROW_INDEX_BRAKE_STATE = 0;
    static const int MODEL_ROW_INDEX_LIGHT_STATE = 1;
    static const int MODEL_ROW_INDEX_IMAGE_PATH = 2;

    //Model table column index.
    static const int MODEL_COL_INDEX_FRONT_BRAKE_STATE = 0;
    static const int MODEL_COL_INDEX_REAR_BRAKE_STATE = 1;
    static const int MODEL_COL_INDEX_INTEGRATED_BRAKE_STATE = 2;
    static const int MODEL_COL_INDEX_LIGHT_TURN_ON_REQUEST = 0;
    static const int MODEL_COL_INDEX_LIGHT_TURN_ON_CONFIG = 1;
    static const int MODEL_COL_INDEX_LIGHT_MANUAL_TURN_ON_CONFIG = 2;
    static const int MODEL_COL_INDEX_LIGHT_TURN_ON_DIRECTION = 3;
    static const int MODEL_COL_INDEX_BRAKE_STATE_IMAGE = 0;
};

#endif // CBRAKEITEMMODEL_H
