#ifndef CBRAKEITEMMODEL_H
#define CBRAKEITEMMODEL_H

#include <QObject>
#include "model/cbicycleitemmodel.h"

class CBrakeItemModel : public CBicycleItemModel
{
    Q_OBJECT

public:
    explicit CBrakeItemModel(QObject* parent = nullptr);
    ~CBrakeItemModel() override {}

public:
    void setData(const int pin, const bool value) override;

public:
    static const int MODEL_ROW_INDEX_FRONT_BRAKE_STATE = 0;
    static const int MODEL_ROW_INDEX_REAR_BRAKE_STATE = 1;
    static const int MODEL_ROW_INDEX_INTEGRATED_BRAKE_STATE = 2;
};

#endif // CBRAKEITEMMODEL_H
