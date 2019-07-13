#include "cwheelitemview.h"

CWheelItemView::CWheelItemView(QWidget* parent)
 : CBicycleItemView(parent)
{
    this->mLabel = new QLabel(this);
    this->mLabel->resize(200, 30);
    this->mLabel->setAlignment(Qt::AlignmentFlag::AlignRight | Qt::AlignmentFlag::AlignVCenter);
}

CWheelItemView::~CWheelItemView()
{
    delete this->mLabel;
}


void CWheelItemView::updateView(const QModelIndex &index, const QVariant &data)
{
    if (this->mAvailableColumnIndex == index.column()) {
        this->mLabel->setText(data.toString());
    } else {
        //Nothing to do!
    }
}

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

    this->updateView(topLeft, this->model()->data(topLeft));
}
