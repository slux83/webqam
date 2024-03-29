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

#ifndef TREEITEM_H
#define TREEITEM_H

#include <QtCore>

class TreeItem
{
    public:
        TreeItem(const QVector<QVariant> &data, TreeItem *parent = 0);
        ~TreeItem();

        TreeItem *child(int number);
        int childCount() const;
        int columnCount() const;
        QVariant data(int column) const;
        bool insertChildren(int position, int count, int columns, QVector<QVariant> data);
        bool insertChildren(int position, int count, int columns);
        bool insertColumns(int position, int columns);
        TreeItem *parent();
        bool removeChildren(int position, int count);
        bool removeColumns(int position, int columns);
        int childNumber() const;
        bool setData(int column, const QVariant &value);
        void setIsFolder(bool isFolder) { m_isFolder = isFolder; }
        bool isFolder() { return m_isFolder; }
        void setId(int id) { m_id = id; }
        int id() { return m_id; }
        QVector<QVariant> getItemData() { return itemData; }

    private:
        QList<TreeItem*> childItems;
        QVector<QVariant> itemData;
        TreeItem *parentItem;
        bool m_isFolder;
        int m_id;
};

#endif
