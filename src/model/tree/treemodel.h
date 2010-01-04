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

#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QtCore>

class TreeItem;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

    signals:
        void propertySaved();

    public:
        TreeModel(const QStringList &headers, QObject *parent = 0);
        ~TreeModel();
        QVariant data(const QModelIndex &index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const;

        QModelIndex index(int row, int column,
                          const QModelIndex &parent = QModelIndex()) const;
        QModelIndex parent(const QModelIndex &index) const;

        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        int columnCount(const QModelIndex &parent = QModelIndex()) const;

        Qt::DropActions supportedDropActions() const;
        QStringList mimeTypes() const;

        QMimeData* mimeData(const QModelIndexList &indexes) const;

        bool dropMimeData(const QMimeData *data,
                          Qt::DropAction action,
                          int row, int column, const QModelIndex &parent);

        Qt::ItemFlags flags(const QModelIndex &index) const;
        bool setData(const QModelIndex &index, const QVariant &value,
                     int role = Qt::EditRole);
        bool setHeaderData(int section, Qt::Orientation orientation,
                           const QVariant &value, int role = Qt::EditRole);

        bool insertColumns(int position, int columns,
                           const QModelIndex &parent = QModelIndex());
        bool removeColumns(int position, int columns,
                           const QModelIndex &parent = QModelIndex());
        bool insertRows(int position, int rows,
                        const QModelIndex &parent = QModelIndex());
        bool removeRows(int position, int rows,
                        const QModelIndex &parent = QModelIndex());

        TreeItem *getRootItem() { return m_rootItem; }

    private:
        TreeItem *getItem(const QModelIndex &index) const;

        TreeItem *m_rootItem;
};

#endif
