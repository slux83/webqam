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

#ifndef FOLDER_H
#define FOLDER_H

#include <QtCore>

class Folder
{
    private:
        int m_id;
        int m_parentId;
        QString m_name;

    public:
        Folder() {}
        Folder(int id, int parentId, QString name);
        Folder(int id, QString name);

        int id() { return m_id; }
        int parentId() { return m_parentId; }
        QString name() { return m_name; }

        void setId(int id) { m_id = id; }
        void setName(QString name) { m_name = name; }
};

#endif // FOLDER_H
