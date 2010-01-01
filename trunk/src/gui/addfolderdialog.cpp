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

#include "addfolderdialog.h"

AddFolderDialog* AddFolderDialog::m_instance = 0;

AddFolderDialog::AddFolderDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setVisible(false);
}

AddFolderDialog* AddFolderDialog::instance()
{
    if(m_instance == 0)
        m_instance = new AddFolderDialog();

    return m_instance;
}

void AddFolderDialog::changeEvent(QEvent *e)
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

void AddFolderDialog::slotShowDialog()
{
    folderNameLineEdit->clear();
    folderNameLineEdit->setFocus(Qt::OtherFocusReason);
    show();
}
