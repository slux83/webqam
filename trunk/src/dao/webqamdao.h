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

#ifndef WEBQAMDAO_H
#define WEBQAMDAO_H

#include <QtCore>
#include <QtSql>
#include "../model/folder.h"
#include "../model/webcam.h"

class WebQamDao : public QObject
{

    private:
        static WebQamDao *m_instance;
        WebQamDao(QObject *parent = 0);

    public:
        static WebQamDao * instance();
        QMap<int, Folder> getFolders();
        QMap<int, Webcam> getWebcams();
        bool updateFolder(int id, QString name);
        bool updateWebcam(int id, QString name, QString url);
        bool insertWebcam(Webcam webcam, QSqlError &sqlError);
        bool deleteWebcams(QList<int> ids);
};

#endif // WEBQAMDAO_H
