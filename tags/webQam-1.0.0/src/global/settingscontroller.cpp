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

#include "settingscontroller.h"
#include "common.h"

#include <QDebug>

SettingsController* SettingsController::m_instance = 0;

SettingsController::SettingsController(const QString &fileName, Format format, QObject *parent)
    : QSettings(fileName, format, parent)
{
    //Store global options
    beginGroup(SETTING_GLOBALS_GROUP);
        setValue(SETTINGS_VERSION, WEBQAM_VERSION);
    endGroup();

    sync();
}

SettingsController* SettingsController::instance()
{

    if(!m_instance)
    {
        QString settingsPath = WEBQAM_HOME.append(WEBQAM_SETTINGS_FILE_NAME);
        qDebug() << "WebQam home is " << WEBQAM_HOME;
        m_instance = new SettingsController(settingsPath, QSettings::IniFormat);
    }

    return m_instance;
}

QMap<QString, int> SettingsController::readOptions()
{
    QMap<QString, int> data;

    beginGroup(SETTING_OPTIONS_GROUP);

    foreach(QString key, allKeys())
        data.insert(key, value(key).toInt());

    endGroup();

    return data;
}

void SettingsController::writeOptions(QMap<QString, int> &options)
{
    beginGroup(SETTING_OPTIONS_GROUP);

    foreach(QString key, options.keys())
        setValue(key, options.value(key));

    endGroup();

    sync();
}
