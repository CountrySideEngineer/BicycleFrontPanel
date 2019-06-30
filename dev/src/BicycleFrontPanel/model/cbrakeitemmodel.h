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

protected:
    const int MODEL_ROW_INDEX_INTEGRATED_BRAKE_STATE = 2;
};

#endif // CBRAKEITEMMODEL_H
