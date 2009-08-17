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

#include "systemtray.h"
#include "aboutdialog.h"
#include "imagemenu.h"
#include "../global/camscontroller.h"
#include "../model/tree/treemodelfactory.h"

SystemTray* SystemTray::m_instance = 0;

SystemTray::SystemTray(QObject *parent) : QSystemTrayIcon(parent)
{
    icon = QIcon(":/icons/systray");
    setIcon(icon);

    m_settingsGui = new SettingsGui();
    m_masterMenu = 0;

    initContextMenu();
}

SystemTray* SystemTray::instance()
{
    if(!m_instance)
        m_instance = new SystemTray();

    return m_instance;
}

void SystemTray::initContextMenu(bool createModel)
{
    if(m_masterMenu) //clear all old data
    {
        qDebug() << "Refreshing all webcams...";

        //webcamMenus are destroyed by Controller
        CamsController::instance()->refreshOnChange();

        //folders
        qDeleteAll(m_folders);
        m_folders.clear();

        m_masterMenu->clear();

        delete m_masterMenu;
    }

    m_masterMenu = new QMenu();

    QMenu *webCamsMenu = m_masterMenu->addMenu(QIcon(":/icons/webcam1"), tr("Webcams"));

    QList<Folder> folders = CamsController::instance()->folders().values();
    foreach(Folder folder, folders)
    {
        m_folders.insert(folder.id(), webCamsMenu->addMenu(folder.name()));
    }

    QList<ImageMenu*> webcamMenus = CamsController::instance()->webCams().values();
    foreach(ImageMenu *webcamMenu, webcamMenus)
    {
        if(webcamMenu->folderId() == 0)
        {
            //put this webcam menu on top (without a folder parent menu)
            webcamMenu->setCaller(webCamsMenu->addMenu(webcamMenu));
        }
        else
        {
            webcamMenu->setCaller(
                    m_folders.value(webcamMenu->folderId())->addMenu(webcamMenu));
        }
    }

    m_masterMenu->addSeparator();
    connect(m_masterMenu->addAction(QIcon(":/icons/configure"), tr("Options...")), SIGNAL(triggered()), m_settingsGui, SLOT(showOnCenter()));
    connect(m_masterMenu->addAction(QIcon(":/icons/about"), tr("About webQam")), SIGNAL(triggered()),
            AboutDialog::instance(), SLOT(show()));
    m_masterMenu->addSeparator();
    connect(m_masterMenu->addAction(QIcon(":/icons/exit"), tr("Exit")), SIGNAL(triggered()), qApp, SLOT(quit()));
    setContextMenu(m_masterMenu);

    if(createModel)
        m_settingsGui->setTreeModel(TreeModelFactory::buildTreeModel());
}

void SystemTray::refresh(bool createModel)
{
    initContextMenu(createModel);
}
