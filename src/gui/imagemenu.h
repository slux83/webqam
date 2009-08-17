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

#ifndef IMAGEMENU_H
#define IMAGEMENU_H

#include <QtGui>

class ImageMenu : public QMenu
{
    Q_OBJECT

    private:
        int m_folderId;
        quint16 m_thumbnailSize;
        QAction *m_caller;
        QPixmap image, m_originalImage;

    public:
        ImageMenu(const QString &title,
                  int folderId,
                  quint16 thumbnailSize,
                  QPixmap originalImage = QPixmap(),
                  QWidget *parent = 0);
        void setImage(QPixmap newImage);
        int folderId() { return m_folderId; }
        inline void setCaller(QAction *caller) { m_caller = caller; }

        //superclass override
        QSize sizeHint() const;

        inline void setThumbnailSize(quint16 size) { m_thumbnailSize = size; }

    protected:
        void paintEvent(QPaintEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
};

#endif // IMAGEMENU_H
