#include "cbicycleitemview.h"

/**
 * @brief CBicycleItemView::CBicycleItemView    Constructs CBicycleItemView object with argument.
 * @param parent    Pointer to parent QWidget object.
 */
CBicycleItemView::CBicycleItemView(QWidget* parent)
    : QAbstractItemView(parent)
{}

/**
 * @brief CBicycleItemView::visualRect  Returns the rectangle on the view point occupied
 *                                      by the item at index.
 * @param index
 * @return  The rectangle on the view point.
 */
QRect CBicycleItemView::visualRect(const QModelIndex &index) const
{
    Q_UNUSED(index);

    return QRect(0, 0, 300, 20);
}

/**
 * @brief CBicycleItemView::scrollTo    Scroll the view if necessary to ensure that
 *                                      the item at index is visible. THe view will
 *                                      try to positon the item according the given
 *                                      hint.
 * @param index Index of model.
 * @param hint  Hint.
 */
void CBicycleItemView::scrollTo(const QModelIndex &index, ScrollHint hint)
{
    Q_UNUSED(index);
    Q_UNUSED(hint);
}

/**
 * @brief CBicycleItemView::indexAt Returns the model index of the item at the viewport
 *                                  corrdinates point.
 * @param point
 * @return
 */
QModelIndex CBicycleItemView::indexAt(const QPoint &point) const
{
    Q_UNUSED(point);

    return QModelIndex();
}

/**
 * @brief CBicycleItemView::moveCursor  Returns a QModelIndex obkect pointing to the
 *                                      next object in the view, based on the given cursorAction
 *                                      and keyboard modifiers by modifiers.
 * @param cursorAction
 * @param modifiers
 * @return
 */
QModelIndex CBicycleItemView::moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
    Q_UNUSED(cursorAction);
    Q_UNUSED(modifiers);

    return QModelIndex();
}

/**
 * @brief CBicycleItemView::horizontalOffset    Returns the horizontal offset of the view.
 * @return Horizontal offset of the view.
 */
int CBicycleItemView::horizontalOffset() const
{
    return 0;
}

/**
 * @brief CBicycleItemView::verticalOffset  Returns the vertical offset of the view.
 * @return
 */
int CBicycleItemView::verticalOffset() const
{
    return 0;
}

bool CBicycleItemView::isIndexHidden(const QModelIndex &index) const
{
    Q_UNUSED(index);

    return false;
}

/**
 * @brief CBicycleItemView::setSelection    Applies the selection flags to the items in or
 *                                          touched by the rectangle rect.
 * @param rect
 * @param command
 */
void CBicycleItemView::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command)
{
    Q_UNUSED(rect);
    Q_UNUSED(command);
}

/**
 * @brief CBicycleItemView::visualRegionForSelection    Returs the region from the viewport of the items
 *                                                      in the given selection.
 * @param selection
 * @return
 */
QRegion CBicycleItemView::visualRegionForSelection(const QItemSelection &selection) const
{
    Q_UNUSED(selection);

    return QRegion();
}

/**
 * @brief CBicycleItemView::dataChanged This slots is called when the items with the given roles are
 *                                      are changed in the model. The changed items are those from topLeft
 *                                      to bottomRight inclusive. If just one item is changed, topLeft == bottomRight.
 * @param topLeft
 * @param bottomRight
 * @param roles
 */
void CBicycleItemView::dataChanged(
        const QModelIndex &topLeft,
        const QModelIndex &bottomRight,
        const QVector<int> &roles)
{
    Q_UNUSED(bottomRight);
    Q_UNUSED(roles);

    printf("CBicycleItemView::dataChanged() called\r\n");

    this->updateView(topLeft, this->model()->data(topLeft));
}
