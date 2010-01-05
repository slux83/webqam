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

#include "imagemenu.h"
#include "webcamviewer.h"
#include "../global/common.h"
#include <QtCore>

ImageMenu::ImageMenu(const QString &title, int folderId, quint16 thumbnailSize,
                     QPixmap originalImage, QWidget *parent) : QMenu(title, parent)
{
    m_folderId = folderId;
    setToolTip(tr("Click to enlarge"));
    m_thumbnailSize = thumbnailSize;

    if(!originalImage.isNull())
        setImage(originalImage);
}

QSize ImageMenu::sizeHint() const
{
    if(image.isNull())
        return QSize(200, 50);
    else
        return image.size() + QSize(10, 25); //padding
}

void ImageMenu::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->pos().x() > sizeHint().width() ||
       event->pos().y() > sizeHint().height())
        return;

    WebcamViewer *wv = WebcamViewer::instance();

    wv->setContent(title(), m_originalImage);
    wv->showOnCenter();

    event->accept();
}

void ImageMenu::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);

    if(!image.isNull())
    {
        painter.drawPixmap(5, 5, image.width(), image.height(), image);
        painter.drawText(QRect(0, image.height() + 5, image.width(), 20),
                          Qt::AlignRight, tr("Last update: %1").arg(QDateTime::currentDateTime().toString(WEBQAM_DATETIME_FORMAT)));
    }
    else
        painter.drawText(QRect(0, 0, sizeHint().width(), sizeHint().height()),
                          Qt::AlignCenter, tr("Image not valid or not loaded"));

    painter.end();
}


void ImageMenu::setImage(QPixmap newImage)
{
    m_originalImage = newImage;

    if(!m_originalImage.isNull() && m_originalImage.width() > m_thumbnailSize)
        image = m_originalImage.scaledToWidth((int)m_thumbnailSize, Qt::SmoothTransformation);
    else
        image = m_originalImage;    //keep original size

    //refresh when data is ready and the menu is visible
    if(isVisible())
    {
        repaint();
        m_caller->trigger();
    }
}

