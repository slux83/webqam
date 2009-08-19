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

#include "sqldrivermanager.h"

bool SqlDriverManager::validateSystemForSQLITE()
{
    qDebug() << "Available SQL drivers:" << drivers();

    return QSqlDatabase::isDriverAvailable(SQLITE_DRIVER_NAME);
}

bool SqlDriverManager::initDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase(SQLITE_DRIVER_NAME, SQLITE_DB_CONNECTION_NAME);
    db.setDatabaseName(SQLITE_DB_FILE_NAME);

    bool allreadyInit = QFile::exists(QDir::currentPath().append(QDir::toNativeSeparators("/").append(SQLITE_DB_FILE_NAME)));

    if(allreadyInit) return true;

    db.open();

    QFile sqlInitDbFile(":/sql/init_database");

    if (!sqlInitDbFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qCritical()    << "Unable to read the sql init file: "
                        << sqlInitDbFile.errorString();
        return false;
    }

    QString sqlStatement;
    while (!sqlInitDbFile.atEnd())
        sqlStatement.append(sqlInitDbFile.readLine());

    sqlInitDbFile.close();

    //Create main database structure
    qDebug() << "Start creating database structure";

    if(!db.transaction())
    {
        qCritical() << "Cannot begin the transaction:"
                    << db.lastError();
    }

    QSqlQuery queryCreateBatch = QSqlQuery(db);
    foreach(const QString query, sqlStatement.split(";", QString::SkipEmptyParts))
    {
        if(query.trimmed() == "") continue;

        if(!queryCreateBatch.exec(query))
        {
            qCritical() << "Init query error:" << query;
            qCritical() << "Unable to execute the query to popolate the database:"
                        << queryCreateBatch.lastError();
            db.rollback();
            return false;
        }
    }

    if(!db.commit())
    {
        qCritical() << "Unable to commit queries:" << db.lastError();
        return false;
    }

    qDebug() << "Database main structure created!";

    return true;
}
