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

#include "camscontroller.h"
#include "settingscontroller.h"
#include "../gui/systemtray.h"
#include "../gui/addwebcamdialog.h"
#include "../net/imagehttploader.h"
#include "../dao/webqamdao.h"
#include "common.h"
#include <QtGui>
#include <QtSql>

CamsController* CamsController::m_instance = 0;

CamsController::CamsController() : QObject()
{
    m_timer = new QTimer(this);

    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateCams()));

    refreshOnChange();

    m_standAloneImageLoader = new ImageHttpLoader();

    connect(m_standAloneImageLoader, SIGNAL(signalImageReady(QUrl,QByteArray&)),
            AddWebcamDialog::instance(), SLOT(slotWebcamPreviewReady(QUrl, QByteArray&)));

    slotOptionsChanged(SettingsController::instance()->readOptions());
}

void CamsController::refreshOnChange()
{
    //Clear all
    qDeleteAll(m_webCams.values());
    m_webCams.clear();
    qDeleteAll(m_loaderPool);
    m_loaderPool.clear();
    m_folders.clear();

    m_folders = WebQamDao::instance()->getFolders();

    foreach(Webcam webcam, WebQamDao::instance()->getWebcams().values())
    {
        m_webCams.insert(webcam.url(),
                         new ImageMenu(webcam.name(), webcam.folderId(), m_lastThumbnailSize));
    }

    if(m_webCams.size() > 5)
        m_poolSize = m_webCams.size() / 3;
    else
        m_poolSize = 5;

    qDebug() << "using" << m_poolSize << "simultaneous connections.";

    //Init loader pool
    for(int i=0; i<m_poolSize; i++)
    {
        ImageLoader *loader = new ImageHttpLoader();
        m_loaderPool.append(loader);
        connect(loader, SIGNAL(signalImageReady(QUrl, QByteArray &)), this,
                SLOT(slotCreatePixmapForWebcam(QUrl, QByteArray &)));
    }

    //refresh images
    QTimer::singleShot(0, this, SLOT(updateCams()));
}

CamsController* CamsController::instance()
{
    if(m_instance == 0)
        m_instance = new CamsController();

    return m_instance;
}

void CamsController::updateCams()
{
    int i = 0;
    foreach(const QUrl &camUrl, m_webCams.keys())
    {
        int index = ++i % m_poolSize;
        ImageLoader *loader = m_loaderPool.at(index);

        loader->downloadImage(camUrl);
    }
}

void CamsController::slotCreatePixmapForWebcam(QUrl url, QByteArray &imageData)
{
    QPixmap pixmap;

    if(!pixmap.loadFromData(imageData))
        qDebug() << "Image corrupted: " << url.toString();

    m_webCams.value(url)->setImage(pixmap);
}

void CamsController::slotOptionsChanged(QMap<QString, int> options)
{
    int newTimeout = options.value(SETTINGS_TIMEOUT, 1) * 60 * 1000;
    qDebug() << "Restarting timer with new time:" << newTimeout;
    m_timer->start(newTimeout);

    if(!options.contains(SETTINGS_TIMEOUT))
        qWarning() << "cannot find timeout key on settings... default value is used";

    if(!options.contains(SETTINGS_THUMB_SIZE))
        qWarning() << "cannot find thumbnailsize key on settings... default value is used";

    quint16 newThumbSize = (quint16)options.value(SETTINGS_THUMB_SIZE, 320);
    m_lastThumbnailSize = newThumbSize;

    qDebug() << "Thumbnail size:" << newThumbSize;

    //set changed thumbnail size to each ImageMenu(s)
    foreach(ImageMenu *imageMenu, m_webCams.values())
        imageMenu->setThumbnailSize(newThumbSize);

    //and start now the first
    QTimer::singleShot(0, this, SLOT(updateCams()));
}

/**
    \return QString filled with the error string to show if any
*/
QString CamsController::insertFolder(Folder folder)
{
    QSqlError sqlError;
    WebQamDao::instance()->insertFolder(folder, sqlError);

    QString returnValue;

    switch(sqlError.number())
    {
        case -1:
        case  0:
            returnValue = "";
            SystemTray::instance()->refresh(true);
            break;

        case 19:    //constraint failed
            returnValue = tr("Folder name allready exists! Please choose another one.");
            break;
        default:
            returnValue = tr("Unknown error: ") + sqlError.text();
    }

    return returnValue;
}

/**
    \return QString filled with the error string to show if any
*/
QString CamsController::insertWebcam(Webcam webcam)
{
    QSqlError sqlError;
    WebQamDao::instance()->insertWebcam(webcam, sqlError);
    QString returnValue;

    switch(sqlError.number())
    {
        case -1:
        case  0:
            returnValue = "";
            SystemTray::instance()->refresh(true);
            break;

        case 19:    //constraint failed
            returnValue = tr("Url allready exists! Please choose another one.");
            break;
        default:
            returnValue = tr("Unknown error: ") + sqlError.text();
    }

    return returnValue;
}

bool CamsController::deleteWebcams(QList<int> ids)
{
    bool result = WebQamDao::instance()->deleteWebcams(ids);

    if(result)
        SystemTray::instance()->refresh(true);  //refresh all

    return result;
}

bool CamsController::deleteFolders(QList<int> ids)
{
    bool result = WebQamDao::instance()->deleteFolders(ids);

    if(result)
        SystemTray::instance()->refresh(true);  //refresh all

    return result;
}

void CamsController::loadImagePreview(QUrl url)
{
    m_standAloneImageLoader->downloadImage(url);
}
