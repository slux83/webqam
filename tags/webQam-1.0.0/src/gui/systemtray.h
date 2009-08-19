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

#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QtGui>

#include "settingsgui.h"

class SystemTray : public QSystemTrayIcon
{
    Q_OBJECT

    private:
        static SystemTray *m_instance;
        QIcon icon;
        QMenu *m_masterMenu;
        void initContextMenu(bool createModel = true);
        SettingsGui *m_settingsGui;
        QMap<int, QMenu*> m_folders;
        SystemTray(QObject *parent = 0);

    public:
        static SystemTray* instance();
        void refresh(bool createModel);

};

#endif // SYSTEMTRAY_H
