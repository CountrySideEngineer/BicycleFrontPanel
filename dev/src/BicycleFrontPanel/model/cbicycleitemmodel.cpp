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

void CBicycleItemModel::setData(const int pin, const uint32_t value)
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
 * @param pin   GPIO pin No. to set into map.
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
 * @brief CBicycleItemModel::setModelColWithPin Setup model col - GPIO pin No. table.
 * @param modelCol  Model col No.
 * @param pin       GPIO pin No. to set into map.
 */
void CBicycleItemModel::setModelColWithPin(const int modelCol, const int pin)
{
    QMap<int, int>::iterator it = this->mColIndexPinData.find(modelCol);
    if (it != this->mColIndexPinData.end()) {
        /*
         * The QMap has contained the pair with key modelCol.
         * To avoid registering a key duplicately, remove item with key before
         * registering a new key - value pair by modelCol and pin.
         */
        this->mColIndexPinData.erase(it);
    }
    this->mColIndexPinData[pin] = modelCol;
}

/**
 * @brief CBicycleItemModel::Pin2RowIndex   Convert pin number into row index of model.
 * @param pin   Pin number.
 * @return Index of row
 * @exception invalid_argument  The GPIO pin No. "pin" is invalid, not registered in map.
 */
int CBicycleItemModel::Pin2RowIndex(int pin)
{
    return this->Pin2Index(pin, this->mRowIndexPinData);
}

/**
 * @brief CBicycleItemModel::Pin2ColIndex   Convert pin number into col index of model.
 * @param pin           Pin number to detect from map.
 * @return              Index of col.
 */
int CBicycleItemModel::Pin2ColIndex(int pin)
{
    return this->Pin2Index(pin, this->mColIndexPinData);
}

/**
 * @brief CBicycleItemModel::Pin2Index  Convert pin number into map index.
 * @param pin           Pin number to detect from map.
 * @param modelMap      Map to be scanned.
 * @return  Index of col.
 */
int CBicycleItemModel::Pin2Index(int pin, QMap<int, int> &modelMap)
{
    /*
     * About the Qt official document, the operator "[]" of QMap does not throw exception
     * if the key is not found in QMap. Instead of throwing exception, the operator insert new
     * value into instance and its valueis "default-construted" value.
     * (Maybe calling default constructor.)
     * So, in this function, it is needed to check whether the argument "pin" has value
     * in map "modelMap" before returns a value.
     */
    if (modelMap.find(pin) == modelMap.end()) {
        throw  std::invalid_argument("Invalid GPIO pin specified");
    }
    return modelMap[pin];
}

void CBicycleItemModel::setModelMapWithPin(const int index, const int pin, QMap<int, int> &modelMap)
{
    auto it = modelMap.find(index);
    if (it != modelMap.end()) {
        /*
         * The QMap has contained the pair with key index.
         * To avoid registering a key duplicately, remove item with key before
         * registering a new key - value pair by index and pin.
         */
        modelMap.erase(it);
    }
    modelMap[pin] = index;
}
