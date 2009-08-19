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

#include "webcamviewer.h"

WebcamViewer* WebcamViewer::m_instance = 0;

WebcamViewer::WebcamViewer(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    //connects
    connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(slotSaveImageAs()));
    connect(zoomBox, SIGNAL(valueChanged(int)), this, SLOT(slotZoomImage(int)));
    connect(pushButtonOriginalZoom, SIGNAL(clicked()), this, SLOT(slotZoomOriginal()));
}

void WebcamViewer::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}

void WebcamViewer::centerWindow()
{
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    QPoint center(screenGeometry.width() / 2 - width() / 2,
                   screenGeometry.height() / 2 - height() / 2);

    move(center);
}

void WebcamViewer::showOnCenter()
{
    centerWindow();
    show();
}

void WebcamViewer::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type())
    {
        case QEvent::LanguageChange:
            retranslateUi(this);
            break;
        default:
            break;
    }
}

WebcamViewer* WebcamViewer::instance()
{
    if(!m_instance)
        m_instance = new WebcamViewer();

    return m_instance;
}

void WebcamViewer::setContent(QString title, QPixmap image)
{
    //disable action save and zoom if image is null
    actionSaveAs->setDisabled(image.isNull());
    zoomBox->setDisabled(image.isNull());
    pushButtonOriginalZoom->setDisabled(image.isNull());

    if(!image.isNull())
    {
        imageBox->setPixmap(image);
        statusBar()->showMessage(tr("Image size: %1x%2").arg(QString::number(image.width()),
                                                            QString::number(image.height())),
                                 10000);
        m_originalPixmap = image;
        zoomBox->setMaximum(image.width());
        zoomBox->setValue(image.width());
    }
    else
    {
        imageBox->setText(tr("Image not valid or not loaded"));
        statusBar()->clearMessage();
    }

    setWindowTitle(QString("webQam - ").append(title));
}

void WebcamViewer::slotSaveImageAs()
{
    const QPixmap *image = imageBox->pixmap();

    if(image->isNull())
    {
        qWarning() << "Cannot save image: pixmap is null";
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(
                        this,
                        tr("Save Image As..."),
                        QDir::homePath(),
                        tr("Images (*.png)"));

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    qDebug() << "Saving image:" << fileName;

    if(!image->save(fileName, "PNG"))
        statusBar()->showMessage(tr("ERROR: Cannot save image"), 10000);
    else
        statusBar()->showMessage(tr("Image saved."), 10000);

    QApplication::restoreOverrideCursor();
}

void WebcamViewer::slotZoomImage(int value)
{
    QPixmap scaledPixmap = m_originalPixmap.scaledToWidth(value, Qt::SmoothTransformation);
    imageBox->setPixmap(scaledPixmap);
    statusBar()->showMessage(tr("Image size: %1x%2 (%3%)").arg(QString::number(scaledPixmap.width()),
                                                            QString::number(scaledPixmap.height()),
                                                            QString::number((scaledPixmap.width() * 100 )/m_originalPixmap.width())),
                            10000);
}

void WebcamViewer::slotZoomOriginal()
{
    zoomBox->setValue(m_originalPixmap.width());
}
