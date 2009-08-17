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

#include "settingsgui.h"
#include "addwebcamdialog.h"
#include "ui_settingsgui.h"
#include "../model/tree/treemodel.h"
#include "../model/tree/treeitem.h"
#include "../global/settingscontroller.h"
#include "../global/camscontroller.h"

SettingsGui::SettingsGui(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SettingsGui)
{
    ui->setupUi(this);
    m_treeModel = 0;

    //Sort filter proxy model init
    m_sortFilterProxyModel = new QSortFilterProxyModel(this);
    m_sortFilterProxyModel->setDynamicSortFilter(true);
    m_sortFilterProxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    m_sortFilterProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    //connections
    connect(ui->lineEditFilter, SIGNAL(textChanged(const QString &)),
        m_sortFilterProxyModel, SLOT(setFilterFixedString(const QString &)));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(slotSaveOptions()));
    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(slotSaveOptions()));
    connect(this, SIGNAL(signalOptionsSaved(QMap<QString,int>)),
        CamsController::instance(), SLOT(slotOptionsChanged(QMap<QString,int>)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(slotTabChanged(int)));
    connect(ui->pushButtonNewWebcam, SIGNAL(clicked()), AddWebcamDialog::instance(), SLOT(slotShowDialog()));
    connect(ui->pushButtonDeleteWebcams, SIGNAL(clicked()), this, SLOT(slotDeleteSelectedWebcams()));

    //init options reading settings
    QMap<QString, int> options = SettingsController::instance()->readOptions();

    if(options.contains(SETTINGS_TIMEOUT))
        ui->spinBoxTimeout->setValue(options.value(SETTINGS_TIMEOUT));
    if(options.contains(SETTINGS_THUMB_SIZE))
        ui->spinBoxThumbnailSize->setValue(options.value(SETTINGS_THUMB_SIZE));
}

SettingsGui::~SettingsGui()
{
    delete ui;
}

void SettingsGui::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}

void SettingsGui::centerWindow()
{
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    QPoint center(screenGeometry.width() / 2 - width() / 2,
                   screenGeometry.height() / 2 - height() / 2);

    move(center);
}

void SettingsGui::showOnCenter()
{
    centerWindow();
    show();
}

void SettingsGui::setTreeModel(TreeModel *treeModel)
{
    if(m_treeModel)
        delete m_treeModel;

    m_treeModel = treeModel;

    connect(m_treeModel, SIGNAL(propertySaved()), this, SLOT(treeDataSaved()));

    m_sortFilterProxyModel->setSourceModel(m_treeModel);
    ui->treeView->setModel(m_sortFilterProxyModel);
    ui->treeView->expandAll();
    ui->treeView->resizeColumnToContents(0);
}

void SettingsGui::treeDataSaved()
{
    writeOnStatusbar(tr("Property saved"));
}

void SettingsGui::writeOnStatusbar(QString msg)
{
    statusBar()->showMessage(msg);

    QTimer::singleShot(SETTING_GUI_STATUS_BAR_TIMEOUT, statusBar(), SLOT(clearMessage()));
}

void SettingsGui::slotSaveOptions()
{
    qDebug() << "saving options...";

    QMap<QString, int> options;

    options.insert(SETTINGS_THUMB_SIZE, ui->spinBoxThumbnailSize->value());
    options.insert(SETTINGS_TIMEOUT, ui->spinBoxTimeout->value());

    SettingsController::instance()->writeOptions(options);

    writeOnStatusbar(tr("Options saved"));

    emit signalOptionsSaved(options);
}

void SettingsGui::slotTabChanged(int index)
{
    ui->pushButton->setDisabled(index == 1);
}

void SettingsGui::slotDeleteSelectedWebcams()
{
    //QItemSelectionModel *treeSelectionModel = ui->treeView->selectionModel();

    //QModelIndexList selectedIndexes = treeSelectionModel->selectedRows(0);
    QItemSelection itemSelection = ui->treeView->selectionModel()->selection();
    QModelIndexList selectedIndexes =
            m_sortFilterProxyModel->mapSelectionToSource(itemSelection).indexes();

    if(selectedIndexes.isEmpty())
    {
        QMessageBox::information(this, tr("webQam - Info"), tr("Please select webcams"));
        return;
    }

    //Confirm operation
    if(QMessageBox::question(this, tr("webQam - question"), tr("Delete selected webcams?"),
                             QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
        return;

    QList<int> webcamIds;
    foreach(QModelIndex index, selectedIndexes)
    {
        TreeItem * treeItem = static_cast<TreeItem*>(index.internalPointer());
        if(!treeItem)
        {
            qCritical() << "void SettingsGui::slotDeleteSelectedWebcams() - NULL treeItem";
            continue;
        }

        if(treeItem->isFolder()) continue;  //Skeep folders

        if(!webcamIds.contains(treeItem->id()))
            webcamIds.append(treeItem->id());
    }

    //Delete selected webcams
    qDebug() << "Deleting webcams: " << webcamIds;
    CamsController::instance()->deleteWebcams(webcamIds);
}
