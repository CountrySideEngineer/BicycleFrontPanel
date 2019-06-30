#ifndef CBRAKEITEMVIEW_H
#define CBRAKEITEMVIEW_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include "view/cbicycleitemview.h"

class CBrakeItemView : public CBicycleItemView
{
    Q_OBJECT

public:
    explicit CBrakeItemView(QWidget* parent = nullptr);

    void updateView(const QModelIndex &index, const QVariant &data) override;

protected:
    QLabel* mLabel;
};


#endif // CBRAKEITEMVIEW_H
