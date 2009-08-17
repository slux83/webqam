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
#include "gui/systemtray.h"
#include "net/imagehttploader.h"
#include "dao/sqldrivermanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(!SystemTray::isSystemTrayAvailable())
    {
    	qDebug() << "system tray not available";
        QMessageBox::critical(0, QObject::tr("webQam - critical"),
                              QObject::tr("Your system doesn't supports system tray. Aborting..."));
        return 1;
    }

    if(!SqlDriverManager::validateSystemForSQLITE())
    {
        qCritical() << "CRITICAL: Sqlite driver not found";

        QMessageBox::critical(0, QObject::tr("webQam - critical"),
                QObject::tr("No database drivers found! This application requires SQLITE Qt database driver. "
                "Please check the documentation how to build the "
                "QSQLITE plugin."));

        return 1;
    }

    if(!SqlDriverManager::initDatabase())
    {
        QMessageBox::critical(0, QObject::tr("webQam - Cannot execute query"),
                QObject::tr("Unable to create database main structure"));

        //Delete empty sqlite file
        QFile sqliteFile(QDir::currentPath().append(QDir::toNativeSeparators("/").append(SQLITE_DB_FILE_NAME)));
        if(sqliteFile.exists() && sqliteFile.size() == 0)
            qDebug() << "Deleting empty sql file:" << sqliteFile.remove();

        return 1;
    }


    SystemTray::instance()->show();

    return a.exec();
}
