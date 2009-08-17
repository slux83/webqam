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

#ifndef ADDWEBCAMDIALOG_H
#define ADDWEBCAMDIALOG_H

#include "ui_addwebcamdialog.h"
#include <QtCore>

class AddWebcamDialog : public QDialog, private Ui::AddWebcamDialog
{
    Q_OBJECT

private:
    static AddWebcamDialog* m_instance;

public:
    AddWebcamDialog(QWidget *parent = 0);
    static AddWebcamDialog* instance();

private slots:
    void slotUpdateWebcamPreview(const QString& url);
    void slotHandlePreviewBoxToggled(bool on);
    void slotInsertWebcam();
    void slotLockUnlockSaveAction(const QString& url);

protected:
    void changeEvent(QEvent *e);

public slots:
    void slotShowDialog();
    void slotWebcamPreviewReady(QUrl url, QByteArray& imageData);

};

#endif // ADDWEBCAMDIALOG_H
