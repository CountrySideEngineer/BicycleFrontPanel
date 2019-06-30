#include "cwheelitemview.h"

CWheelItemView::CWheelItemView(QWidget* parent)
 : CBicycleItemView (parent) {}

void CWheelItemView::updateView(const QModelIndex &index, const QVariant &data)
{
    if (this->mAvailableColumnIndex == index.column()) {
        this->mLabel->setText(QString::number(data.toInt()));
    }
}
