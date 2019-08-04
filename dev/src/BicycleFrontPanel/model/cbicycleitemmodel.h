#ifndef CBICYCLEITEMMODEL_H
#define CBICYCLEITEMMODEL_H

#include <QObject>
#include <QAbstractItemModel>

class CBicycleItemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit CBicycleItemModel(QObject* parent = nullptr);
    ~CBicycleItemModel() override {}

public:
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool setData(const QModelIndex &index, const QVariant &value, bool updates, int role = Qt::EditRole);
    virtual void setData(const int pin, const bool value);
    virtual void setData(const int pin, const uint32_t rotate, const uint32_t velocity);

    virtual void setModelRowWithPin(const int modelRow, const int pin);
    virtual void setModelColWithPin(const int modelCol, const int pin);

protected:
    virtual int Pin2RowIndex(int pin);
    virtual int Pin2ColIndex(int pin);
    virtual int Pin2Index(int pin, QMap<int, int> &modelMap);
    virtual void setModelMapWithPin(const int index, const int pin, QMap<int, int>& modelMap);

protected:
    QMap<QModelIndex, QVariant> mData;
    QMap<int, int> mRowIndexPinData;
    QMap<int, int> mColIndexPinData;
};

#endif // CBICYCLEITEMMODEL_H
