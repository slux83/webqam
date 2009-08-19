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

#ifndef IMAGEHTTPLOADER_H
#define IMAGEHTTPLOADER_H

#include <QtNetwork>

#include "imageloader.h"

class ImageHttpLoader : public ImageLoader
{
    Q_OBJECT

    private:
        QNetworkAccessManager *manager;
        bool m_isRunning;

    private slots:
        void slotReplyFinished(QNetworkReply *reply);
        void slotError(QNetworkReply::NetworkError);

    public:
        ImageHttpLoader();

        void downloadImage(const QUrl &imageResourceLink);
        bool isRunning() { return m_isRunning; }
};

#endif // IMAGEHTTPLOADER_H
