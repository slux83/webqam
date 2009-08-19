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

#include "imagehttploader.h"
#include <QtCore>

ImageHttpLoader::ImageHttpLoader() : ImageLoader()
{
    manager = new QNetworkAccessManager(this);
    m_isRunning = false;

    connect(manager, SIGNAL(finished(QNetworkReply *)),
            this, SLOT(slotReplyFinished(QNetworkReply *)));
}

void ImageHttpLoader::downloadImage(const QUrl &imageResourceLink)
{
    m_isRunning = true;

    qDebug() << "Downloading resource: " << imageResourceLink.toString();

    QNetworkRequest request(imageResourceLink);

    if(imageResourceLink.scheme() == "https")
        request.setSslConfiguration(QSslConfiguration::defaultConfiguration());

    QNetworkReply * reply = manager->get(request);

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));

}

void ImageHttpLoader::slotReplyFinished(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();
    qDebug()    << "Download finished: " << reply->request().url().toString();

    emit signalImageReady(reply->request().url(), data);

    m_isRunning = false;
}

void ImageHttpLoader::slotError(QNetworkReply::NetworkError error)
{
    qDebug() << "Download failed with error code: " << error;
    m_isRunning = false;
}
