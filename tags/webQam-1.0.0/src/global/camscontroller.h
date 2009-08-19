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

#ifndef CAMSCONTROLLER_H
#define CAMSCONTROLLER_H

#include <QtCore>
#include "../gui/imagemenu.h"
#include "../net/imageloader.h"
#include "../model/folder.h"
#include "../model/webcam.h"

class CamsController : public QObject
{
    Q_OBJECT

    private slots:
        void slotCreatePixmapForWebcam(QUrl url, QByteArray &imageData);

    private:
        CamsController();
        static CamsController *m_instance;
        QMap<QUrl, ImageMenu*> m_webCams;
        QMap<int, Folder> m_folders;
        QList<ImageLoader*> m_loaderPool;
        ImageLoader *m_standAloneImageLoader;
        quint16 m_poolSize, m_lastThumbnailSize;
        QTimer *m_timer;

    public:
        static CamsController* instance();
        QMap<QUrl, ImageMenu*> webCams() { return m_webCams; }
        QMap<int, Folder> folders() { return m_folders; }
        QString insertWebcam(Webcam webcam);
        bool deleteWebcams(QList<int> ids);

    public slots:
        void updateCams();
        void refreshOnChange();
        void slotOptionsChanged(QMap<QString, int> options);
        void loadImagePreview(QUrl url);
};

#endif // CAMSCONTROLLER_H
