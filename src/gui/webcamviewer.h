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

#ifndef WEBCAMVIEWER_H
#define WEBCAMVIEWER_H

#include "ui_webcamviewer.h"
#include <QtGui>

class WebcamViewer : public QMainWindow, private Ui::WebcamViewer {
    Q_OBJECT

    private:
        static WebcamViewer* m_instance;
        void centerWindow();
        QPixmap m_originalPixmap;

    private slots:
        void slotSaveImageAs();
        void slotZoomImage(int value);
        void slotZoomOriginal();

    public:
        WebcamViewer(QWidget *parent = 0);
        static WebcamViewer* instance();

    public slots:
        void showOnCenter();
        void setContent(QString title, QPixmap image);

    protected:
        void changeEvent(QEvent *e);
        void closeEvent(QCloseEvent *event);
};

#endif // WEBCAMVIEWER_H
