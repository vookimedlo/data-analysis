/****************************************************************************
Data Analysis - tool for making a basic data analysis.
Copyright(C) 2017  Michal Duda <github@vookimedlo.cz>

https://github.com/vookimedlo/data-analysis

This program is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/

#include <fstream>
#include <QImageReader>
#include <QCryptographicHash>
#include <QDesktopServices>
#include <QMenu>
#include <QPixmap>
#include <QUrl>

#include "GlobalInformationDialog.h"
#include "OperationDialog.h"
#include "ui_About.h"
#include "ui_FinalReportDialog.h"
#include "../controller/DetailsPublisher.h"
#include "../controller/SearchSettings.h"
#include "../model/GlobalInformation.h"
#include "../model/fs/File.h"
#include "../operations/MagicOperation.h"
#include "../operations/HashOperation.h"
#include "../operations/ReportOperation.h"
#include "../operations/ScanDirOperation.h"
#include "../reports/CSVReportWriter.h"
#include "../reports/HTMLReportThumbnailGenerator.h"
#include "../reports/HTMLReportWriter.h"
#include "../reports/ReportSettings.h"
#include "../reports/RTFReportWriter.h"
#include "../ui/SearchDialog.h"
#include "../ui/CSVFinalReportDialog.h"
#include "../ui/HTMLFinalReportDialog.h"
#include "../util/compiler.h"
#include "../util/StringHelper.h"

#include "DataAnalyzer.h"
#include "../operations/SearchOperation.h"

DataAnalyzer::DataAnalyzer(QWidget *parent)
    : QMainWindow(parent),
      m_directoryModel(nullptr),
      m_fileDirectoryModel(nullptr),
      m_searchFileDirectoryModel(nullptr),
      m_detailedDataItemModel(nullptr),
      m_detailedDataItemSortFilterProxyModel(new (std::nothrow) DataItemSortFilterProxyModel()),
      m_selectedDataItem(nullptr),
      m_searchResult(std::string(StringHelper::toStdString(tr("Search Root"))), nullptr),
      m_globalInformation()
{
    ui.setupUi(this);
    ui.detailedDataItemTreeView->header()->setSortIndicator(0, Qt::AscendingOrder);
    ui.detailedDataItemTreeView->setModel(m_detailedDataItemSortFilterProxyModel.get());

    ui.detailsTreeWidget->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

    // Context menu
    m_contextMenu = new QMenu(this);
    m_contextMenu->addAction(tr("Open file externally"), this, SLOT(onOpenFileExternallyTriggered()));
    m_contextMenu->addSeparator();
    m_contextMenu->addMenu(ui.menuOperation);
    m_contextMenu->addMenu(ui.menuEdit);

    // Context menu connection
    connect(ui.detailedDataItemTreeView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenuInDetailedDataItemTreeView(const QPoint &)));
    connect(ui.dataItemTreeView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenuInDataItemTreeView(const QPoint &)));

    // Create an exclusive translations menu
    QActionGroup* langGroup = new QActionGroup(ui.menuTranslations);
    langGroup->setExclusive(true);
    langGroup->addAction(ui.actionTranEnglish);
    langGroup->addAction(ui.actionTranCzech);

    connect(langGroup, SIGNAL(triggered(QAction *)), this, SLOT(onLanguageChanged(QAction *)));

    QString locale = QLocale::system().name();
    locale.truncate(locale.lastIndexOf('_'));
    loadLanguage(locale);

    // Select/check correct language menu item
    if (locale == "cs")
        ui.actionTranCzech->setChecked(true);
    else
        ui.actionTranEnglish->setChecked(true);

    m_searchResult.addDirectory(new Directory(std::string(StringHelper::toStdString(tr("Search Results"))), &m_searchResult));
}

DataAnalyzer::~DataAnalyzer()
{
    m_searchResult.directories()[0]->directories().clear();
    m_searchResult.directories()[0]->files().clear();
}

void DataAnalyzer::onScanTriggered()
{
    ScanDirOperation operation;
    OperationDialog dialog(operation, OperationDialog::ModeE_DirSelect, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        // Do the disconnection before the model is cleared
        disconnect(SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onDataItemCurrentChanged(const QModelIndex &, const QModelIndex &)));
        disconnect(SIGNAL(clicked(const QModelIndex&)), this, SLOT(onDataItemCurrentClicked(const QModelIndex &)));
        disconnect(SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onDetailedDataItemCurrentChanged(const QModelIndex &, const QModelIndex &)));
        disconnect(SIGNAL(clicked(const QModelIndex&)), this, SLOT(onDetailedDataItemClicked(const QModelIndex &)));
        disconnect(SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onSearchDataItemCurrentChanged(const QModelIndex &, const QModelIndex &)));
        disconnect(SIGNAL(clicked(const QModelIndex&)), this, SLOT(onSearchDDataItemClicked(const QModelIndex &)));

        ui.detailsTreeWidget->clear();
        ui.previewWidget->clear();

        m_topLevelDirectory = dialog.getResult();

        m_searchFileDirectoryModel.reset(nullptr);
        m_directoryModel.reset(new (std::nothrow) DirectoryTreeModel(m_topLevelDirectory.get()));
        m_fileDirectoryModel.reset(new (std::nothrow) DataItemTreeModel(m_topLevelDirectory.get(), DataItemTreeModel::DataItemTreeModelE_NoDetails));

        // Remove model binding so we can delete the old one
        ui.detailedDataItemTreeView->reset();
        ui.detailedDataItemTreeView->setModel(nullptr);

        m_detailedDataItemSortFilterProxyModel->setSourceModel(nullptr);

        m_detailedDataItemModel.reset(new (std::nothrow) DataItemTreeModel(m_topLevelDirectory.get(), DataItemTreeModel::DataItemTreeModelE_Details));

        m_detailedDataItemSortFilterProxyModel->setSourceModel(m_detailedDataItemModel.get());

        ui.detailedDataItemTreeView->setModel(m_detailedDataItemSortFilterProxyModel.get());

        onDirOnly(ui.dirOnlyCheckBox->checkState() == Qt::CheckState::Checked);

        // Columns resize policy
        if (ui.dataItemTreeView->header()->count() > 0)
            ui.dataItemTreeView->header()->setSectionResizeMode(0, QHeaderView::Interactive);

        for (int i = 1; i < ui.dataItemTreeView->header()->count(); ++i)
        {
            ui.dataItemTreeView->header()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
        }
            
        // Columns resize policy
        if (ui.detailedDataItemTreeView->header()->count() > 0)
            ui.detailedDataItemTreeView->header()->setSectionResizeMode(0, QHeaderView::Stretch);

        for (int i = 1; i < ui.detailedDataItemTreeView->header()->count(); ++i)
        {
            ui.detailedDataItemTreeView->header()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
        }

        connect(ui.detailedDataItemTreeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onDetailedDataItemCurrentChanged(const QModelIndex &, const QModelIndex &)));
        connect(ui.detailedDataItemTreeView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onDetailedDataItemClicked(const QModelIndex &)));
        connect(ui.dataItemTreeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onDataItemCurrentChanged(const QModelIndex &, const QModelIndex &)));
        connect(ui.dataItemTreeView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onDataItemCurrentClicked(const QModelIndex &)));

        ui.fileOperationsTabWidget->setCurrentWidget(ui.scannedDataTab);          
    }
}

void DataAnalyzer::onDataItemSelected(QModelIndex index)
{
    DataItem *item = static_cast<DataItem *>(index.internalPointer());
    Directory *directory = dynamic_cast<Directory *>(item);
    
    if (index.isValid() && directory)
    {
        // Do the disconnection before the model is cleared
        disconnect(SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onDetailedDataItemCurrentChanged(const QModelIndex &, const QModelIndex &)));
        disconnect(SIGNAL(clicked(const QModelIndex&)), this, SLOT(onDetailedDataItemClicked(const QModelIndex &)));

        // Remove model binding so we can delete the old one
        m_detailedDataItemSortFilterProxyModel->setSourceModel(nullptr);

        ui.detailedDataItemTreeView->reset();
        ui.detailedDataItemTreeView->setModel(nullptr);

        m_detailedDataItemModel.reset(new (std::nothrow) DataItemTreeModel(directory, DataItemTreeModel::DataItemTreeModelE_Details));
             
        m_detailedDataItemSortFilterProxyModel->setSourceModel(m_detailedDataItemModel.get());
        ui.detailedDataItemTreeView->setModel(m_detailedDataItemSortFilterProxyModel.get());
                
        ui.previewWidget->clear();

        // Columns resize policy
        if (ui.detailedDataItemTreeView->header()->count() > 0)
            ui.detailedDataItemTreeView->header()->setSectionResizeMode(0, QHeaderView::Stretch);

        for (int i = 1; i < ui.detailedDataItemTreeView->header()->count(); ++i)
        {
           ui.detailedDataItemTreeView->header()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
        }       

        connect(ui.detailedDataItemTreeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onDetailedDataItemCurrentChanged(const QModelIndex &, const QModelIndex &)));
        connect(ui.detailedDataItemTreeView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onDetailedDataItemClicked(const QModelIndex &)));
    }

    dataItemSelected(index);
}

void DataAnalyzer::onDataItemClicked(QModelIndex index)
{
    if (ui.dataItemTreeView->model() && ui.dataItemTreeView->selectionModel()->hasSelection())
        onDataItemSelected(index);
}

void DataAnalyzer::onDataItemCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    UNUSED_VARIABLE(previous);
    onDataItemSelected(current);
}

void DataAnalyzer::onSearchDDataItemClicked(QModelIndex index)
{
    if (ui.searchDataItemTreeView->model() && ui.searchDataItemTreeView->selectionModel()->hasSelection())
        onDataItemSelected(index);
}

void DataAnalyzer::onSearchDataItemCurrentChanged(const QModelIndex& current, const QModelIndex& previous)
{
    UNUSED_VARIABLE(previous);
    onDataItemSelected(current);
}

void DataAnalyzer::onDetailedDataItemSelected(QModelIndex index)
{
    dataItemSelected(m_detailedDataItemSortFilterProxyModel->mapToSource(index));
}

void DataAnalyzer::onDetailedDataItemClicked(QModelIndex index)
{
    if (ui.detailedDataItemTreeView->model() && ui.detailedDataItemTreeView->selectionModel()->hasSelection())
        onDetailedDataItemSelected(index);
}

void DataAnalyzer::onDetailedDataItemCurrentChanged(const QModelIndex& current, const QModelIndex& previous)
{
    UNUSED_VARIABLE(previous);
    if (ui.detailedDataItemTreeView->model() && ui.detailedDataItemTreeView->selectionModel()->hasSelection())
        onDetailedDataItemSelected(current);
}

void DataAnalyzer::onAnalysisTextChange()
{
    if (m_analysisDataItem)
        m_analysisDataItem->addInfo(DataInfo::DataInfoE_Analysis, ui.analysisTextEdit->toPlainText().toStdString());
}

void DataAnalyzer::onDirOnly(bool showOnlyDirs)
{
    ui.dataItemTreeView->reset();
    ui.dataItemTreeView->setModel(nullptr);

    // Do the disconnection before the model is cleared
    disconnect(SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onDataItemCurrentChanged(const QModelIndex &, const QModelIndex &)));
    disconnect(SIGNAL(clicked(const QModelIndex&)), this, SLOT(onDataItemClicked(const QModelIndex &)));

    if (showOnlyDirs)
        ui.dataItemTreeView->setModel(m_directoryModel.get());
    else
        ui.dataItemTreeView->setModel(m_fileDirectoryModel.get());

    // Columns resize policy
    if (ui.dataItemTreeView->header()->count() > 0)
        ui.dataItemTreeView->header()->setSectionResizeMode(0, QHeaderView::Interactive);

    for (int i = 1; i < ui.dataItemTreeView->header()->count(); ++i)
    {
        ui.dataItemTreeView->header()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
    }

    if (ui.dataItemTreeView->model())
    {
        ui.dataItemTreeView->selectionModel()->select(ui.dataItemTreeView->model()->index(0, 0, ui.dataItemTreeView->rootIndex()), QItemSelectionModel::Rows | QItemSelectionModel::ClearAndSelect);
        onDataItemSelected(ui.dataItemTreeView->model()->index(0, 0, ui.dataItemTreeView->rootIndex()));
        ui.dataItemTreeView->expand(ui.dataItemTreeView->model()->index(0, 0, ui.dataItemTreeView->rootIndex()));
    }

    connect(ui.dataItemTreeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onDataItemCurrentChanged(const QModelIndex &, const QModelIndex &)));
    connect(ui.dataItemTreeView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onDataItemClicked(const QModelIndex &)));
}

void DataAnalyzer::onCounterClockwiseRotate()
{
    QTransform transform;
    QTransform trans = transform.rotate(270);
    QPixmap(ui.previewWidget->pixmap()->transformed(trans));
    ui.previewWidget->setPixmap(QPixmap(ui.previewWidget->pixmap()->transformed(trans)));
}

void DataAnalyzer::onClockwiseRotate()
{
    QTransform transform;
    QTransform trans = transform.rotate(90);
    ui.previewWidget->setPixmap(QPixmap(ui.previewWidget->pixmap()->transformed(trans)));
}

void DataAnalyzer::onTagChanged(int row)
{
    UNUSED_VARIABLE(row);
    if (m_selectedDataItem)
        DetailsPublisher::toModel(*ui.detailsTreeWidget, *ui.tagListWidget, *m_selectedDataItem);    
}

void DataAnalyzer::onAbout()
{
    Ui::aboutDialog uiAbout;
    QDialog dialog(this);
    uiAbout.setupUi(&dialog);
    dialog.exec();
}

void DataAnalyzer::onAboutQtTriggered()
{
    QApplication::aboutQt();
}

void DataAnalyzer::onMD5Triggered()
{    
    QString dialogTitile(tr("MD5 computation"));
    hashOperation(QCryptographicHash::Md5, DataInfo::DataInfoE_MD5, dialogTitile);
}

void DataAnalyzer::onSHA1Triggered()
{
    QString dialogTitile(tr("SHA-1 computation"));
    hashOperation(QCryptographicHash::Sha1, DataInfo::DataInfoE_SHA1, dialogTitile);
}

void DataAnalyzer::onSHA3_512Triggered()
{
    QString dialogTitile(tr("SHA3-512 computation"));
    hashOperation(QCryptographicHash::Sha3_512, DataInfo::DataInfoE_SHA3_512, dialogTitile);
}

void DataAnalyzer::onFileMagicTriggered()
{
    if (m_topLevelDirectory.get())
    {
        DataItem &item = m_selectedDataItem ? *m_selectedDataItem : *m_topLevelDirectory;  // If the model has a selection
        MagicOperation operation(item);

        OperationDialog dialog(operation, OperationDialog::ModeE_NoDirSelect, this);
        dialog.setTitle(tr("Type detection"));
        dialog.exec();
        updateDetailsTreeWidget();
    }
}

void DataAnalyzer::onCSVReportTriggered()
{
    if (m_topLevelDirectory)
    {
        QString dialogTitle(tr("CSV report"));
        ReportSettings settings;
        CSVFinalReportDialog reportDialog(settings, this);
        reportDialog.setWindowTitle(dialogTitle);

        if (reportDialog.exec() == QDialog::Accepted)
        {
            QString rootPath = StringHelper::toQString(m_topLevelDirectory->directories()[0]->name());
            CSVReportWriter writer(settings, rootPath);
            ReportOperation operation(writer, *(m_topLevelDirectory->directories()[0]));

            OperationDialog dialog(operation, OperationDialog::ModeE_NoDirSelect, this);
            dialog.setTitle(dialogTitle);
            dialog.exec();
        }
    }
}

void DataAnalyzer::onRTFReportTriggered()
{
    if (m_topLevelDirectory)
    {
        QString rootPath = StringHelper::toQString(m_topLevelDirectory->directories()[0]->name());
        RTFReportWriter writer("C:/tmp/report.rtf", rootPath);
        ReportOperation operation(writer, *(m_topLevelDirectory->directories()[0]));

        OperationDialog dialog(operation, OperationDialog::ModeE_NoDirSelect, this);
        dialog.setTitle(tr("RTF report"));
        dialog.exec();
    }
}

void DataAnalyzer::onHTMLReportTriggered()
{
    if (m_topLevelDirectory)
    {
        QString dialogTitle(tr("HTML report"));
        ReportSettings settings;
        settings.setTitle(StringHelper::toQString(m_globalInformation.getReferenceNumber()));
        settings.setReference(StringHelper::toQString(m_globalInformation.getReference()));
        settings.setId(StringHelper::toQString(m_globalInformation.getId()));
        settings.setPerex(ui.reportTextEdit->toPlainText());

        HTMLFinalReportDialog reportDialog(settings, this);
        reportDialog.setWindowTitle(dialogTitle); 

        if (reportDialog.exec() == QDialog::Accepted)
        {            
            HTMLReportThumbnailGenerator generator;
            QString rootPath = StringHelper::toQString(m_topLevelDirectory->directories()[0]->name());
            HTMLReportWriter writer(settings, generator, rootPath);
            ReportOperation operation(writer, *(m_topLevelDirectory->directories()[0]));

            OperationDialog dialog(operation, OperationDialog::ModeE_NoDirSelect, this);
            dialog.setTitle(dialogTitle);

            if (dialog.exec() == QDialog::Accepted)
            {
                ui.reportTextEdit->setPlainText(settings.getPerex());
                m_globalInformation.setReferenceNumber(StringHelper::toStdString(settings.getTitle()));
                m_globalInformation.setReference(StringHelper::toStdString(settings.getReference()));
                m_globalInformation.setId(StringHelper::toStdString(settings.getId()));
            }
        }
    }
}

void DataAnalyzer::onNewTriggered()
{  
    QString dialogTitle(tr("Dataset settings"));
    GlobalInformationDialog dialog(m_globalInformation, false, this);
    dialog.setWindowTitle(dialogTitle);  

    if (dialog.exec() == QDialog::Accepted)
    {
        // Do the disconnection before the model is cleared
        disconnect(SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onDataItemCurrentChanged(const QModelIndex &, const QModelIndex &)));
        disconnect(SIGNAL(clicked(const QModelIndex&)), this, SLOT(onDataItemCurrentClicked(const QModelIndex &)));
        disconnect(SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onDetailedDataItemCurrentChanged(const QModelIndex &, const QModelIndex &)));
        disconnect(SIGNAL(clicked(const QModelIndex&)), this, SLOT(onDetailedDataItemClicked(const QModelIndex &)));
        disconnect(SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onSearchDataItemCurrentChanged(const QModelIndex &, const QModelIndex &)));
        disconnect(SIGNAL(clicked(const QModelIndex&)), this, SLOT(onSearchDDataItemClicked(const QModelIndex &)));

        ui.searchDataItemTreeView->setModel(nullptr);
        ui.searchDataItemTreeView->reset();
        m_searchFileDirectoryModel.reset();

        ui.detailedDataItemTreeView->setModel(nullptr);
        ui.detailedDataItemTreeView->reset();
        m_detailedDataItemSortFilterProxyModel->setSourceModel(nullptr);
        m_detailedDataItemModel.reset();

        ui.dataItemTreeView->setModel(nullptr);
        ui.dataItemTreeView->reset();
        m_directoryModel.reset();
        m_fileDirectoryModel.reset();


        ui.detailsTreeWidget->clear();
        ui.previewWidget->clear();
        ui.tagListWidget->setCurrentRow(0);

        m_topLevelDirectory.reset();
        m_selectedDataItem = nullptr;
        ui.detailedDataItemTreeView->header()->setSortIndicator(0, Qt::AscendingOrder);

        ui.fileOperationsTabWidget->setCurrentWidget(ui.scannedDataTab);
    }
}

void DataAnalyzer::onSearchTriggered()
{
    if (m_topLevelDirectory || m_selectedDataItem)
    {
        SearchSettings settings(*(m_searchResult.directories()[0]));
        SearchDialog dialog(settings, this);
        if (dialog.exec() == QDialog::Accepted)
        {
            disconnect(SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onSearchDataItemCurrentChanged(const QModelIndex &, const QModelIndex &)));
            disconnect(SIGNAL(clicked(const QModelIndex&)), this, SLOT(onSearchDDataItemClicked(const QModelIndex &)));

            ui.searchDataItemTreeView->setModel(nullptr);
            ui.searchDataItemTreeView->reset();
            m_searchFileDirectoryModel.reset(nullptr);
            
            // TODO: This is not good, we have to use shared pointers
            m_searchResult.directories()[0]->directories().clear();
            m_searchResult.directories()[0]->files().clear();

            QString dialogTitle(tr("Searching ..."));
            SearchOperation operation(settings, m_selectedDataItem ? *m_selectedDataItem : *(m_topLevelDirectory->directories()[0]));
            OperationDialog dialog(operation, OperationDialog::ModeE_NoDirSelect, this);
            dialog.setTitle(dialogTitle);

            if (dialog.exec() == QDialog::Accepted)
            {
                m_searchFileDirectoryModel.reset(new SearchDataItemTreeModel(&m_searchResult, SearchDataItemTreeModel::DataItemTreeModelE_NoDetails));
                ui.searchDataItemTreeView->setModel(m_searchFileDirectoryModel.get());
                connect(ui.searchDataItemTreeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onSearchDataItemCurrentChanged(const QModelIndex &, const QModelIndex &)));
                connect(ui.searchDataItemTreeView->selectionModel(), SIGNAL(currentClicked(const QModelIndex&)), this, SLOT(onSearchDataItemClicked(const QModelIndex &)));

                ui.fileOperationsTabWidget->setCurrentWidget(ui.searchTab);

                if (ui.searchDataItemTreeView->model())
                {
                    ui.searchDataItemTreeView->expand(ui.searchDataItemTreeView->model()->index(0, 0, ui.searchDataItemTreeView->rootIndex()));
                    ui.searchDataItemTreeView->selectionModel()->select(ui.searchDataItemTreeView->model()->index(0, 0, ui.searchDataItemTreeView->rootIndex()), QItemSelectionModel::Rows | QItemSelectionModel::ClearAndSelect);
                    onDataItemSelected(ui.searchDataItemTreeView->model()->index(0, 0, ui.searchDataItemTreeView->rootIndex()));
                }
            }
        }
    }
}

void DataAnalyzer::onDatasetSettingsTriggered()
{
    QString dialogTitle(tr("Dataset settings"));
    GlobalInformationDialog dialog(m_globalInformation, true, this);
    dialog.setWindowTitle(dialogTitle);

    dialog.exec();
}

void DataAnalyzer::onOpenFileExternallyTriggered()
{
    File *file = dynamic_cast<File *>(m_selectedDataItem);
    if (file)
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(StringHelper::toQString(file->path())));
    }
    else
    {
            
    }
}

void DataAnalyzer::onCustomContextMenuInDetailedDataItemTreeView(const QPoint& point)
{
    QModelIndex index = ui.detailedDataItemTreeView->indexAt(point);

    if (index.isValid())
    {
        dataItemSelected(m_detailedDataItemSortFilterProxyModel->mapToSource(index));
        m_contextMenu->exec(ui.detailedDataItemTreeView->mapToGlobal(point));
    }
}

void DataAnalyzer::onCustomContextMenuInDataItemTreeView(const QPoint& point)
{
    QModelIndex index = ui.dataItemTreeView->indexAt(point);

    if (index.isValid())
    {
        dataItemSelected(index);
        m_contextMenu->exec(ui.dataItemTreeView->mapToGlobal(point));
    }
}

void DataAnalyzer::onLanguageChanged(QAction* action)
{
    if (action) {
        QString language = action == ui.actionTranCzech ? "cs" : "en";
        
        // load the language dependant on the action
        loadLanguage(language);
    }
}

void DataAnalyzer::dataItemSelected(QModelIndex index)
{
    DataItem *item = static_cast<DataItem *>(index.internalPointer());
 
    /* Skip already proecessed item. */
    if (m_selectedDataItem == item)
        return;
    
    m_selectedDataItem = item;

    ui.detailsTreeWidget->clear();
    ui.previewWidget->clear();

    File *file = dynamic_cast<File *>(item);
    if (file)
    {
        if (QImageReader::supportedImageFormats().indexOf(StringHelper::toQString(item->extension()).toLower().toLatin1()) != -1)
        {
            try
            {
                QImageReader imageReader(StringHelper::toQString(file->path()));
                QPixmap pixmap = QPixmap::fromImageReader(&imageReader);
                QPixmap scaledPixmap;

                if (pixmap.width() > pixmap.height())
                    scaledPixmap = pixmap.scaledToWidth(ui.previewWidget->size().width());
                else
                    scaledPixmap = pixmap.scaledToHeight(ui.previewWidget->size().height());

                ui.previewWidget->setPixmap(scaledPixmap);
            }
            catch (...)
            {
                ui.previewWidget->clear();
            }
        }
        else
        {
            ui.previewWidget->clear();
        }
    }

    assignDataItemToAnalysis(item);
    DetailsPublisher::toUI(*ui.detailsTreeWidget, *ui.tagListWidget, *item);
}

void DataAnalyzer::assignDataItemToAnalysis(DataItem* item)
{
    m_analysisDataItem = item;

    if (!item)
        return;

    if (item->isInfoValid(DataInfo::DataInfoE_Analysis))
    {
        ui.analysisTextEdit->setPlainText(QString::fromStdString(item->info(DataInfo::DataInfoE_Analysis)));
    }
    else
    {
        ui.analysisTextEdit->setPlainText(QString(""));
    }
}

void DataAnalyzer::hashOperation(QCryptographicHash::Algorithm algorithm, DataInfo::DataInfoE info, QString &dialogTitle)
{
    QCryptographicHash hash(algorithm);

    if (m_topLevelDirectory.get())
    {
        DataItem &item = m_selectedDataItem ? *m_selectedDataItem : *m_topLevelDirectory;  // If the model has a selection
        HashOperation operation(hash, info, item);

        OperationDialog dialog(operation, OperationDialog::ModeE_NoDirSelect, this);
        dialog.setTitle(dialogTitle);
        dialog.exec();
        updateDetailsTreeWidget();
    }
}

void DataAnalyzer::loadLanguage(const QString& language)
{
    if (m_currentLanguage != language)
    {
        m_currentLanguage = language;
        QLocale locale = QLocale(m_currentLanguage);
        QLocale::setDefault(locale);
        switchTranslator(m_translator, QString(":/translation/dataanalyzer_%1.qm").arg(language));

//        QString languageName = QLocale::languageToString(locale.language());
//        ui.statusBar->showMessage(tr("Current Language changed to %1").arg(languageName));
    }
}

void DataAnalyzer::switchTranslator(QTranslator& translator, const QString& filename)
{
    // remove the old translator
    qApp->removeTranslator(&translator);

    // load the new translator
    if (translator.load(filename))
        qApp->installTranslator(&translator);
}

void DataAnalyzer::changeEvent(QEvent *event)
{
    if (event)
    {
        switch (event->type())
        {
            // this event is sent if a translator is loaded
        case QEvent::LanguageChange:
            ui.retranslateUi(this);
            break;

            // this event is sent, if the system, language changes
        case QEvent::LocaleChange:
        {
            QString locale = QLocale::system().name();
            locale.truncate(locale.lastIndexOf('_'));
            loadLanguage(locale);
            break;
        }
        }
    }
    
    QMainWindow::changeEvent(event);
}

void DataAnalyzer::updateDetailsTreeWidget()
{
    if (m_selectedDataItem)
    {
        ui.detailsTreeWidget->clear();
        DetailsPublisher::toUI(*ui.detailsTreeWidget, *ui.tagListWidget, *m_selectedDataItem);
    }
}
