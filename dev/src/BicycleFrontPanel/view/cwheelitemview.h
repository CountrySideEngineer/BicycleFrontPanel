#ifndef CWHEELITEMVIEW_H
#define CWHEELITEMVIEW_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include "view/cbicycleitemview.h"

class CWheelItemView : public CBicycleItemView
{
    Q_OBJECT

public:
    explicit CWheelItemView(QWidget* parent = nullptr);

public: //Getter/Setter
    void SetAvailableColumnIndex(int columnIndex) { this->mAvailableColumnIndex = columnIndex; }

public:

    void updateView(const QModelIndex &index, const QVariant &data) override;

protected:
    int mAvailableColumnIndex;

    QLabel* mLabel;


};

#endif // CWHEELITEMVIEW_H
