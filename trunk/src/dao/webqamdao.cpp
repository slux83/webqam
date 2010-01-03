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

#include "webqamdao.h"
#include "../global/common.h"

WebQamDao* WebQamDao::m_instance = 0;

WebQamDao::WebQamDao(QObject *parent) : QObject(parent) { }

WebQamDao* WebQamDao::instance()
{
    if(m_instance == 0)
        m_instance = new WebQamDao();

    return m_instance;
}

QMap<int, Folder> WebQamDao::getFolders()
{
    QMap<int, Folder> folders;

    QSqlDatabase sqlDb = QSqlDatabase::database(SQLITE_DB_CONNECTION_NAME);

    QSqlQuery query = sqlDb.exec(QString("SELECT * FROM ") + SQLITE_DB_TABLE_FOLDER);


    while (query.next())
    {
        QSqlRecord record = query.record();

        Folder folder(record.value("id").toInt(),
                      record.value("parent").toInt(),
                      record.value("name").toString());

        folders.insert(folder.id(), folder);
    }

    return folders;
}

QMap<int, Webcam> WebQamDao::getWebcams()
{
    QMap<int, Webcam> webcams;
    QSqlDatabase sqlDb = QSqlDatabase::database(SQLITE_DB_CONNECTION_NAME);

    QSqlQuery query = sqlDb.exec(QString("SELECT * FROM ") + SQLITE_DB_TABLE_WEBCAM);


    while (query.next())
    {
        QSqlRecord record = query.record();

        Webcam webcam(record.value("id").toInt(),
                      record.value("folder").toInt(),
                      record.value("url").toString(),
                      record.value("name").toString());

        webcams.insert(webcam.id(), webcam);
    }

    return webcams;
}

bool WebQamDao::updateFolder(int id, QString name)
{
    QSqlDatabase sqlDb = QSqlDatabase::database(SQLITE_DB_CONNECTION_NAME);

    QSqlQuery query(sqlDb);
    query.prepare(QString("UPDATE ") + SQLITE_DB_TABLE_FOLDER +
            " SET name = :name WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":name", name);

    return query.exec();
}

bool WebQamDao::updateWebcam(int id, QString name, QString url)
{
    QSqlDatabase sqlDb = QSqlDatabase::database(SQLITE_DB_CONNECTION_NAME);

    QSqlQuery query(sqlDb);
    query.prepare(QString("UPDATE ") + SQLITE_DB_TABLE_WEBCAM +
            " SET name = :name, url = :url WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":name", name);
    query.bindValue(":url", url);

    return query.exec();
}

bool WebQamDao::insertWebcam(Webcam webcam, QSqlError &sqlError)
{
    QSqlDatabase sqlDb = QSqlDatabase::database(SQLITE_DB_CONNECTION_NAME);

    QSqlQuery query(sqlDb);
    query.prepare(QString("INSERT INTO ") + SQLITE_DB_TABLE_WEBCAM +
            " (folder, name, url) VALUES(:folder, :name, :url)");

    query.bindValue(":folder", webcam.folderId());
    query.bindValue(":name", webcam.name());
    query.bindValue(":url", webcam.url().toString());

    bool result = query.exec();

    sqlError = query.lastError();
    if(!result)
    {
        qWarning() << "Insert new webcam error:" << query.lastError();
    }

    return result;
}

bool WebQamDao::insertFolder(Folder folder, QSqlError &sqlError)
{
    QSqlDatabase sqlDb = QSqlDatabase::database(SQLITE_DB_CONNECTION_NAME);

    QSqlQuery query(sqlDb);
    query.prepare(QString("INSERT INTO ") + SQLITE_DB_TABLE_FOLDER +
            " (name) VALUES(:name)");

    query.bindValue(":name", folder.name());

    bool result = query.exec();

    sqlError = query.lastError();
    if(!result)
    {
        qWarning() << "Insert new folder error:" << query.lastError();
    }

    return result;
}

bool WebQamDao::deleteWebcams(QList<int> ids)
{
    QSqlDatabase sqlDb = QSqlDatabase::database(SQLITE_DB_CONNECTION_NAME);
    bool returnVal = true;

    if(!sqlDb.transaction())
    {
        qCritical() << "Cannot begin the transaction:"
                    << sqlDb.lastError();
    }

    foreach(int id, ids)
    {
        QSqlQuery query(sqlDb);
        query.prepare(QString("DELETE FROM ") + SQLITE_DB_TABLE_WEBCAM + " WHERE id = :ids");
        query.bindValue(":id", id);

        if(!query.exec())
        {
            QSqlError sqlError = query.lastError();
            qWarning() << "Delete webcam error: " << query.lastError() << " ID: " << id;
            returnVal = false;
        }
    }

    if(returnVal)
        sqlDb.commit();
    else
        sqlDb.rollback();

    return returnVal;
}

bool WebQamDao::deleteFolders(QList<int> ids)
{
    QSqlDatabase sqlDb = QSqlDatabase::database(SQLITE_DB_CONNECTION_NAME);
    bool returnVal = true;

    if(!sqlDb.transaction())
    {
        qCritical() << "Cannot begin the transaction:"
                << sqlDb.lastError();
    }

    foreach(int id, ids)
    {
        QSqlQuery query(sqlDb);
        query.prepare(QString("DELETE FROM ") + SQLITE_DB_TABLE_FOLDER + " WHERE id = :ids");
        query.bindValue(":id", id);

        if(!query.exec())
        {
            QSqlError sqlError = query.lastError();
            qWarning() << "Delete folder error: " << query.lastError() << " ID: " << id;
            returnVal = false;
        }
    }

    if(returnVal)
        sqlDb.commit();
    else
        sqlDb.rollback();

    return returnVal;
}
