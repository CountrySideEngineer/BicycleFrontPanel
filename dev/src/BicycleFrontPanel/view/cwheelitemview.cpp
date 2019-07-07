#include "cwheelitemview.h"

CWheelItemView::CWheelItemView(QWidget* parent)
 : CBicycleItemView(parent)
{
    this->mLabel = new QLabel(this);
    this->mLabel->resize(200, 30);
    this->mLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    this->mLabel->setText("CWHEEL");
}

void CWheelItemView::updateView(const QModelIndex &index, const QVariant &data)
{
    printf("CWheelItemView::updateView() called\r\n");

    if (this->mAvailableColumnIndex == index.column()) {
        this->mLabel->setText(QString::number(data.toInt()));
    } else {
        //Nothing to do.
    }
}

#if QT_VERSION < 0x00050000
/**
 * @brief CWheelItemView::dataChanged This slots is called when the items with the given roles are
 *                                      are changed in the model. The changed items are those from topLeft
 *                                      to bottomRight inclusive. If just one item is changed, topLeft == bottomRight.
 * @param topLeft
 * @param bottomRight
 * @param roles
 */
void CWheelItemView::dataChanged(
        const QModelIndex &topLeft,
        const QModelIndex &bottomRight,
        const QVector<int> &roles)
{
    Q_UNUSED(bottomRight);
    Q_UNUSED(roles);

    printf("CWheelItemView::dataChanged() called\r\n");

    this->updateView(topLeft, this->model()->data(topLeft));
}
#endif
