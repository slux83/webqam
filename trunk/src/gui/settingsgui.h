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

#ifndef SETTINGSGUI_H
#define SETTINGSGUI_H

#include <QtGui>
#include "../model/folder.h"
#include "../model/tree/treemodel.h"
#include "imagemenu.h"

namespace Ui
{
    class SettingsGui;
}

class SettingsGui : public QMainWindow
{
    Q_OBJECT

    public:
        SettingsGui(QWidget *parent = 0);
        ~SettingsGui();
        void setTreeModel(TreeModel *treeModel);

    public slots:
        void showOnCenter();
        void slotSaveOptions();

    private slots:
        void treeDataSaved();
        void writeOnStatusbar(QString msg);
        void slotTabChanged(int index);
        void slotDeleteSelectedWebcams();
        void slotNeedSaveAction();

    private:
        Ui::SettingsGui *ui;
        bool m_needSaveAction;
        TreeModel *m_treeModel;
        QMenu *m_addStuffMenu;
        QSortFilterProxyModel *m_sortFilterProxyModel;
        void centerWindow();

    protected:
        void closeEvent(QCloseEvent *event);

    signals:
        void signalOptionsSaved(QMap<QString, int> options);
};

#endif // SETTINGSGUI_H
