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

#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QtCore>
#include "common.h"

class SettingsController : public QSettings
{
    Q_OBJECT;

    public:
        static SettingsController* instance();

        QMap<QString, int> readOptions();
        void writeOptions(QMap<QString, int> &options);


    private:
        static SettingsController* m_instance;

        SettingsController(const QString &fileName,
                Format format = NativeFormat, QObject *parent = 0);
};

#endif // SETTINGSCONTROLLER_H
