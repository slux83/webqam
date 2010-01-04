/******************************************************************************
 ** webQam - An useful systray application that show and organize your webcams
 **
 ** Copyright (C) 2009 Alessio Di Fazio
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

#include <QtGui>

#include "treeitem.h"
#include "treemodel.h"
#include "../../global/common.h"

TreeModel::TreeModel(const QStringList &headers, QObject *parent)
    : QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;

    foreach (QString header, headers)
        rootData << header;

    m_rootItem = new TreeItem(rootData);
}

TreeModel::~TreeModel()
{
    delete m_rootItem;
}

 Qt::DropActions TreeModel::supportedDropActions() const
 {
     return Qt::MoveAction;
 }

int TreeModel::columnCount(const QModelIndex &/*parent*/) const
{
    return m_rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TreeItem *item = getItem(index);

    if(role == Qt::DecorationRole && index.column() == 0 && item->isFolder())
        return QPixmap(":/icons/folder");

    if(role == Qt::DecorationRole && index.column() == 0 && !item->isFolder())
        return QPixmap(":/icons/webcam");

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();


    return item->data(index.column());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    TreeItem *item = getItem(index);
    if(item->isFolder() && index.column() == 1)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if(!item->isFolder() && index.column() == 1)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;

    if(item->isFolder())
        return Qt::ItemIsEditable | Qt::ItemIsEnabled |
            Qt::ItemIsSelectable | Qt::ItemIsDropEnabled | Qt::ItemIsDropEnabled;
    else
        return Qt::ItemIsEditable | Qt::ItemIsEnabled |
            Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDragEnabled;
}

TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item) return item;
    }
    return m_rootItem;
}

QStringList TreeModel::mimeTypes() const
{
    QStringList types;
    types << WEBQAM_MIME_TYPE;
    return types;
}

bool TreeModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
                          int row, int column, const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction)
        return true;

    if (!data->hasFormat(WEBQAM_MIME_TYPE))
        return false;

    //Unpack data
    QVector<QVariant> itemData;
    int id, totalDroppedWebcams;
    bool isFolder;
    QByteArray encodedData = data->data(WEBQAM_MIME_TYPE);
    QDataStream stream(&encodedData, QIODevice::ReadOnly);

    stream >> totalDroppedWebcams;
    qDebug() << "Dropped webcams:" << totalDroppedWebcams;

    for(int i=0; i<totalDroppedWebcams; i++)
    {
        stream >> itemData >> id >> isFolder;

        if(isFolder) continue; //folder D&D is not supported now

        //Drop inside the root item
        if(row == -1 && column == -1) {}

        TreeItem *folderItem = getItem(parent);
        folderItem->insertChildren(folderItem->childCount(), 1, folderItem->columnCount(), itemData);
        TreeItem *item = folderItem->child(folderItem->childCount() - 1);
        item->setIsFolder(isFolder);
        item->setId(id);
    }

    return true;
}

QMimeData* TreeModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    stream << indexes.size();

    foreach (QModelIndex index, indexes)
    {
        if (index.isValid())
        {
            TreeItem *treeItem = getItem(index);
            QVector<QVariant> itemData = treeItem->getItemData();

            //Stream: NUM_DROPS,DATA(QVector<QVariant>),ID(int),IS_FOLDER(bool),etc...
            stream << itemData;
            stream << treeItem->id();
            stream << treeItem->isFolder();
        }
    }

    mimeData->setData(WEBQAM_MIME_TYPE, encodedData);
    return mimeData;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_rootItem->data(section);

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    TreeItem *parentItem = getItem(parent);

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

bool TreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = m_rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, m_rootItem->columnCount());
    endInsertRows();

    return success;
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem->parent();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool TreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = m_rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (m_rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem = getItem(parent);

    return parentItem->childCount();
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value,
                        int role)
{
    if (role != Qt::EditRole)
        return false;

    TreeItem *item = getItem(index);

    bool result = item->setData(index.column(), value);
    if(result)
        emit propertySaved();

    return result;
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    return m_rootItem->setData(section, value);
}

