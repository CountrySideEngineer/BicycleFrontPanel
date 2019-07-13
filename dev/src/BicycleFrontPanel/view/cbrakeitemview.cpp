#include "cbrakeitemview.h"
#include "model/cimageresourcemanager.h"

CBrakeItemView::CBrakeItemView(QWidget* parent)
    : CBicycleItemView (parent)
{
    this->mLabel = new QLabel(this);
    this->mLabel->resize(480, 260);
    this->mLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

/**
 * @brief CBrakeItemView::updateView    Update view with model data.
 * @param index Model data index.
 * @param data  Data to set to view.
 */
void CBrakeItemView::updateView(const QModelIndex &index, const QVariant &data)
{
    Q_UNUSED(index);

    QString imagePath = data.toString();

    printf("ImagePath = %s\r\n", imagePath.toStdString().c_str());

    QPixmap image = QPixmap(imagePath);

    this->mLabel->setPixmap(image);
}

/**
 * @brief CBrakeItemView::dataChanged This slots is called when the items with the given roles are
 *                                      are changed in the model. The changed items are those from topLeft
 *                                      to bottomRight inclusive. If just one item is changed, topLeft == bottomRight.
 * @param topLeft
 * @param bottomRight
 * @param roles
 */
void CBrakeItemView::dataChanged(
        const QModelIndex &topLeft,
        const QModelIndex &bottomRight,
        const QVector<int> &roles)
{
    Q_UNUSED(bottomRight);
    Q_UNUSED(roles);

    this->updateView(topLeft, this->model()->data(topLeft));
}
