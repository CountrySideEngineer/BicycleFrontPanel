#ifndef CBICYCLEITEMVIEW_H
#define CBICYCLEITEMVIEW_H

#include <QObject>
#include <QAbstractItemView>

class CBicycleItemView : public QAbstractItemView
{
    Q_OBJECT
public:
    explicit CBicycleItemView(QWidget* parent = nullptr);

    QRect visualRect(const QModelIndex &index) const override;
    void scrollTo(const QModelIndex &index, ScrollHint hint) override;
    QModelIndex indexAt(const QPoint &point) const override;

    virtual void updateView(const QModelIndex &index, const QVariant &data) = 0;

protected slots:
#if QT_VERSION >= 0x00050000
    void dataChanged(const QModelIndex &topLeft,
                     const QModelIndex &bottomRight,
                     const QVector<int> &roles) override;
#else
    void dataChanged(const QModelIndex &topLeft,
                     const QModelIndex &bottomRight,
                     const QVector<int> &roles);
#endif

protected:
    QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers) override;
    int horizontalOffset() const override;
    int verticalOffset() const override;
    bool isIndexHidden(const QModelIndex &index) const override;
    void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command) override;
    QRegion visualRegionForSelection(const QItemSelection &selection) const override;
};

#endif // CBICYCLEITEMVIEW_H
