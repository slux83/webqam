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

#include "addwebcamdialog.h"
#include "../global/camscontroller.h"
#include "../model/webcam.h"

AddWebcamDialog* AddWebcamDialog::m_instance = 0;

AddWebcamDialog::AddWebcamDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    //connects
    connect(lineEditUrl, SIGNAL(textChanged(const QString &)),
            this, SLOT(slotUpdateWebcamPreview(const QString &)));
    connect(previewBox, SIGNAL(toggled(bool)), this, SLOT(slotHandlePreviewBoxToggled(bool)));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(slotInsertWebcam()));
    connect(lineEditName, SIGNAL(textChanged(QString)), this, SLOT(slotLockUnlockSaveAction(QString)));
    connect(lineEditUrl, SIGNAL(textChanged(QString)), this, SLOT(slotLockUnlockSaveAction(QString)));

    setVisible(false);
}

AddWebcamDialog* AddWebcamDialog::instance()
{
    if(m_instance == 0)
        m_instance = new AddWebcamDialog();

    return m_instance;
}

void AddWebcamDialog::slotUpdateWebcamPreview(const QString &url)
{
    if(!previewBox->isChecked())
        return;

    CamsController::instance()->loadImagePreview(url);
}

void AddWebcamDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type())
    {
        case QEvent::LanguageChange:
            retranslateUi(this);
            break;
        default:
            break;
    }
}

void AddWebcamDialog::slotShowDialog()
{
    //Clear form before show dialog
    preview->clear();
    lineEditName->clear();
    lineEditUrl->clear();

    show();
}

void AddWebcamDialog::slotHandlePreviewBoxToggled(bool on)
{
    if(on) //update preview whene preview box is enabled
        slotUpdateWebcamPreview(lineEditUrl->text());
}

void AddWebcamDialog::slotInsertWebcam()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    Webcam webcam;
    webcam.setFolderId(0); //No folder support at present
    webcam.setName(lineEditName->text());
    webcam.setUrl(lineEditUrl->text());

    QString result = CamsController::instance()->insertWebcam(webcam);

    if(!result.isEmpty())
        QMessageBox::warning(this, tr("webQam - Warning"), result);
    else
        hide();

    QApplication::restoreOverrideCursor();
}

void AddWebcamDialog::slotLockUnlockSaveAction(const QString& url)
{
    Q_UNUSED(url);

    buttonBox->setDisabled(lineEditName->text().trimmed().isEmpty() ||
            lineEditUrl->text().trimmed().isEmpty());
}

void AddWebcamDialog::slotWebcamPreviewReady(QUrl /* url */, QByteArray& imageData)
{
    QPixmap pixmap;

    if(!pixmap.loadFromData(imageData))
        preview->setText(tr("Image not valid or not loaded"));
    else
        preview->setPixmap(pixmap);
}
