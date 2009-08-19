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

#include "treemodelfactory.h"
#include "../../dao/webqamdao.h"
#include "treeitem.h"

TreeModel* TreeModelFactory::buildTreeModel()
{
    WebQamDao *dao = WebQamDao::instance();

    QMap<int, Folder> folders = dao->getFolders();
    QMap<int, Webcam> webcams = dao->getWebcams();

    QStringList header;
    header << QObject::tr("Webcam name") << QObject::tr("Webcam source");

    TreeModel *model = new TreeModel(header);

    TreeItem *root = model->getRootItem();

    QMap<int, TreeItem*> folderItems;

    QMapIterator<int, Folder> i(folders);
    while (i.hasNext())
    {
        i.next();
        Folder f = i.value();
        QVector<QVariant> data;
        data << f.name() << "";
        root->insertChildren(root->childCount(), 1, root->columnCount(), data);
        TreeItem *item = root->child(root->childCount() - 1);
        folderItems.insert(i.key(), item);
        item->setIsFolder(true);
        item->setId(f.id());
    }

    foreach(Webcam wc, webcams.values())
    {
        QVector<QVariant> data;
        data << wc.name() << wc.url().toString();

        if(!folderItems.contains(wc.folderId()))
        {
            root->insertChildren(root->childCount(), 1, root->columnCount(), data);
            TreeItem *item = root->child(root->childCount() - 1);
            item->setIsFolder(false);
            item->setId(wc.id());
        }else
        {
            TreeItem *folderItem = folderItems.value(wc.folderId());
            folderItem->insertChildren(folderItem->childCount(), 1, folderItem->columnCount(), data);
            TreeItem *item = folderItem->child(folderItem->childCount() - 1);
            item->setIsFolder(false);
            item->setId(wc.id());
        }
    }

    return model;
}
