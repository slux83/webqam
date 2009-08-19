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

#ifndef COMMON_H
#define COMMON_H

#include <QtCore>

//Global
#define WEBQAM_SETTINGS_FILE_NAME "webQam.conf"
#define SETTING_GUI_STATUS_BAR_TIMEOUT 5000 //in ms
#define WEBQAM_VERSION "1.0.0"
#define WEBQAM_DATETIME_FORMAT "dd/MM/yyyy hh:mm:ss"
#define WEBQAM_HOME QDir::homePath().append("/.webQam/")

//Database
#define SQLITE_DRIVER_NAME "QSQLITE"
#define SQLITE_DB_CONNECTION_NAME "webqam"
#define SQLITE_DB_FILE_NAME "webQam.sqlite"
#define SQLITE_DB_TABLE_FOLDER "folder"
#define SQLITE_DB_TABLE_WEBCAM "webcam"

//Settings
#define SETTING_OPTIONS_GROUP "options"
#define SETTING_GLOBALS_GROUP "globals"
#define SETTINGS_TIMEOUT "timeout"
#define SETTINGS_THUMB_SIZE "thumbSize"
#define SETTINGS_VERSION "version"

#endif // COMMON_H
