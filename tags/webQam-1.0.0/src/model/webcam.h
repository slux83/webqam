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

#ifndef WEBCAM_H
#define WEBCAM_H

#include <QtCore>

class Webcam
{
    private:
        int m_folderId;
        int m_id;
        QUrl m_url;
        QString m_name;

    public:
        Webcam() {}
        Webcam(int id, int folderId, QUrl url, QString name);
        int id() { return m_id; }
        int folderId() { return m_folderId; }
        QUrl url() { return m_url; }
        QString name() { return m_name; }

        void setFolderId(int folderId) { m_folderId = folderId; }
        void setName(QString name) { m_name = name; }
        void setUrl(QUrl url) { m_url = url; }
};

#endif // WEBCAM_H
