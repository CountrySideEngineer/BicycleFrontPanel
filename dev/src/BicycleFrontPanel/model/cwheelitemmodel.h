#ifndef CWHEELITEMMODEL_H
#define CWHEELITEMMODEL_H

#include <QObject>
#include "model/cbicycleitemmodel.h"

class CWheelItemModel : public CBicycleItemModel
{
    Q_OBJECT

public:
    explicit CWheelItemModel(QObject* parent = nullptr);
    ~CWheelItemModel() override {}

public:
    void setData(const int pin, const uint32_t rotate, const uint32_t velocity) override;

protected:
    virtual void updateData(int columnIndex);

    const int MODEL_ROW_INDEX_FRONT_WHEEL_MODEL = 0;
    const int MODEL_ROW_INDEX_REAR_WHEEL_MODEL = 1;
    const int MODEL_ROW_INDEX_INTEGRATED_WHEEL_MODEL = 2;
    const int MODEL_COLUMN_INDEX_ROTATE = 0;
    const int MODEL_COLUMN_INDEX_VELOCITY = 1;
};

#endif // CWHEELITEMMODEL_H
