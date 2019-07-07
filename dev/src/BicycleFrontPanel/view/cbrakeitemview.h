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

protected slots:
#if QT_VERSION < 0x00050000
    void dataChanged(const QModelIndex &topLeft,
                     const QModelIndex &bottomRight,
                     const QVector<int> &roles = QVector<int>()) override;
#endif


protected:
    QLabel* mLabel;
};


#endif // CBRAKEITEMVIEW_H
