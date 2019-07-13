#include <exception>
#include "cbicycleitemmodel.h"
#include <QMap>

/**
 * @brief CBicycleItemModel::CBicycleItemModel  Constructor
 * @param parent    Pointer to parental QObejct.
 */
CBicycleItemModel::CBicycleItemModel(QObject* parent)
    : QAbstractItemModel(parent)
{}

/**
 * @brief CBicycleItemModel::index  Return index of model.
 * @param row       Model row index.
 * @param column    Model column index
 * @param parent    Parent of model.
 * @return  Model index of QModelIndex object.
 */
QModelIndex CBicycleItemModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return createIndex(row, column);
}

/**
 * @brief CBicycleItemModel::parent Return parent obhect of child.
 * @param child QModelIndex object to return parent.
 * @return Parent QModelIndex of child.
 */
QModelIndex CBicycleItemModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);

    return QModelIndex();
}


/**
 * @brief CBrakeItemModel::rowCount Returns the number of rows under the given parent.
 *                                  When the parent is valid it means that rowCount is returning
 *                                  the number of children of parent.
 *                                  But about the NOTE of Qt official documentation, this function
 *                                  should return 0 when the model is based on a table.
 *                                  So, the model of this class is based on a table, this
 *                                  returns 0.
 * @param parent    Parent object.
 * @return  Always returns 0.
 */
int CBicycleItemModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 0;
}

/**
 * @brief CBrakeItemModel::columnCount  Returns the number of columns for the children of the
 *                                      given parent.
 *                                      But about the NOTE of Qt official documentation, this function
 *                                      should return 0 when the model is based on a table.
 *                                      So, the model of this class is based on a table, this
 *                                      returns 0.
 * @param parent    Parent object.
 * @return  Always returns 0.
 */
int CBicycleItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 0;
}

/**
 * @brief CBicycleItemModel::data   Returns data specified by arguments index and role
 * @param index Index of data to return.
 * @param role  Role of data to return.
 * @return Data in model specified by argument index and role.
 */
QVariant CBicycleItemModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role);

    if (index.isValid()) {
        return this->mData[index];
    } else {
        return QVariant();
    }
}

/**
 * @brief CBicycleItemModel::setData    Set data to model, the index of model is specified
 *                                      by argument.
 * @param index Index of model.
 * @param value Value to set into model.
 * @param role
 * @return Returns true if the value can be set into the model, otherwise return false.
 */
bool CBicycleItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(role);

    if (index.isValid()) {
        this->mData[index] = value;
        emit dataChanged(index, index);

        return true;
    } else {
        return false;
    }
}

/**
 * @brief CBicycleItemModel::setData    Set data to model and raise event to update view.
 * @param index Index of model.
 * @param value Value to set to model.
 * @param updates   True if to update view at the same time.
 * @param role
 * @return  Returns true if the valur can be set into the model, otherwise returns false.
 */
bool CBicycleItemModel::setData(const QModelIndex &index, const QVariant &value, bool updates, int role)
{
    if (updates) {
        return this->setData(index, value, role);
    } else {
        if (index.isValid()) {
            this->mData[index] = value;

            return true;
        } else {
            return false;
        }
    }
}

/**
 * @brief CBicycleItemModel::setData    Update model value by argument value.
 * @param pin   GPIO pin No.
 * @param value Value to be set into model.
 */
void CBicycleItemModel::setData(const int pin, const bool value)
{
    Q_UNUSED(pin);
    Q_UNUSED(value);
    /*
     * Nothing to do in this method.
     * Logic to run in this function must be implement subclass.
     */
}

/**
 * @brief CBicycleItemModel::setData    Update model value by argument value.
 *                                      This function is supposed to be used to update rotate and
 *                                      velocity.
 * @param pin   GPIO pin No.
 * @param rotate    Rotate to be set into model.
 * @param velocity  Velocity to be set into model.
 */
void CBicycleItemModel::setData(const int pin, const uint32_t rotate, const uint32_t velocity)
{
    Q_UNUSED(pin);
    Q_UNUSED(rotate);
    Q_UNUSED(velocity);
    /*
     * Nothing to do in this method.
     * Logic to run in this function must be implement subclass.
     */
}

/**
 * @brief CBicycleItemModel::setModelRowWithPin Setup model row - GPIO pin No. table.
 * @param modelRow  Model row No.
 * @param pin   GPIO pin No.
 */
void CBicycleItemModel::setModelRowWithPin(const int modelRow, const int pin)
{
    QMap<int, int>::iterator it = this->mRowIndexPinData.find(modelRow);
    if (it != this->mRowIndexPinData.end()) {
        /*
         * The QMap has contained the pair with key modelRow.
         * To avoid registering a key duplicately, remove item with key before
         * registering a new key - value pair by modelRow and pin.
         */
        this->mRowIndexPinData.erase(it);
    }
    this->mRowIndexPinData[pin] = modelRow;
}

/**
 * @brief CBicycleItemModel::Pin2RowIndex   Convert pin number into row index of model.
 * @param pin   Pin number.
 * @return Index of row
 * @exception invalid_argument  The GPIO pin No. "pin" is invalid, not registered in map.
 */
int CBicycleItemModel::Pin2RowIndex(int pin)
{
    /*
     * About the Qt official document, the operater "[]" of QMap does not throw exception
     * if the key is invalid. Instead of htrowing exception, the operator insert new value
     * into instancea and its value is "default-constructed" value.
     * So this function needs to check argument pin before returns a value.
     */
    if (this->mRowIndexPinData.find(pin) == this->mRowIndexPinData.end()) {
        throw std::invalid_argument("Invalid GPIO pin specified.");
    }
    return this->mRowIndexPinData[pin];
}
