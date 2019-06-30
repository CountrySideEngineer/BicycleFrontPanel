#include "cbrakeitemview.h"
#include "model/cimageresourcemanager.h"

CBrakeItemView::CBrakeItemView(QWidget* parent)
    : CBicycleItemView (parent)
{}

/**
 * @brief CBrakeItemView::updateView    Update view with model data.
 * @param index Model data index.
 * @param data  Data to set to view.
 */
void CBrakeItemView::updateView(const QModelIndex &index, const QVariant &data)
{
    Q_UNUSED(index);

    printf("CBrakeItemView::updateView called\r\n");

    int brakeItemIndex = data.toInt();
    CImageResourceManager imageResourceManager;
    QPixmap image = imageResourceManager.getImageResourcePath(0, brakeItemIndex);

    this->mLabel->setPixmap(image);
}
