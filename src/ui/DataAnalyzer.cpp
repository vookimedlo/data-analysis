/****************************************************************************
Data Analysis - tool for making a basic data analysis.
Copyright(C) 2017  Michal Duda <github@vookimedlo.cz>

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
#include <QPixmap>

#include "OperationDialog.h"
#include "ui_About.h"
#include "../controller/DetailsPublisher.h"
#include "../fs/DataInfo.h"
#include "../fs/Directory.h"
#include "../fs/File.h"
#include "../operations/MagicOperation.h"
#include "../operations/MD5Operation.h"
#include "../operations/ReportOperation.h"
#include "../operations/ScanDirOperation.h"
#include "../reports/CSVReportWriter.h"
#include "../reports/RTFReportWriter.h"
#include "../util/StringHelper.h"

#include "DataAnalyzer.h"

DataAnalyzer::DataAnalyzer(QWidget *parent)
    : QMainWindow(parent), m_directoryModel(nullptr), m_fileDirectoryModel(nullptr), m_detailedDataItemModel(nullptr), m_detailedDataItemSortFilterProxyModel(new (std::nothrow) DataItemSortFilterProxyModel()), m_selectedDataItem(nullptr)
{
    ui.setupUi(this);
    ui.detailedDataItemTreeView->setModel(m_detailedDataItemSortFilterProxyModel.get());
}

void DataAnalyzer::onScanTriggered()
{
    ScanDirOperation operation;
    OperationDialog dialog(operation, OperationDialog::ModeE_DirSelect, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        m_topLevelDirectory = dialog.getResult();

        m_directoryModel.reset(new (std::nothrow) DirectoryTreeModel(m_topLevelDirectory.get()));
        m_fileDirectoryModel.reset(new (std::nothrow) DataItemTreeModel(m_topLevelDirectory.get(), DataItemTreeModel::DataItemTreeModelE_NoDetails));
        onDirOnly(ui.dirOnlyCheckBox->checkState() == Qt::CheckState::Checked);

        // Remove model binding so we can delete the old one
        ui.detailedDataItemTreeView->reset();
        ui.detailedDataItemTreeView->setModel(nullptr);

        m_detailedDataItemSortFilterProxyModel->setSourceModel(nullptr);

        m_detailedDataItemModel.reset(new (std::nothrow) DataItemTreeModel(m_topLevelDirectory.get(), DataItemTreeModel::DataItemTreeModelE_Details));

        m_detailedDataItemSortFilterProxyModel->setSourceModel(m_detailedDataItemModel.get());

        ui.detailedDataItemTreeView->setModel(m_detailedDataItemSortFilterProxyModel.get());
    }
}

void DataAnalyzer::onDataItemSelected(QModelIndex index)
{
    DataItem *item = static_cast<DataItem *>(index.internalPointer());
    Directory *directory = dynamic_cast<Directory *>(item);

    ui.dataItemTreeView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    
    if (index.isValid() && directory)
    {
        // Remove model binding so we can delete the old one
        m_detailedDataItemSortFilterProxyModel->setSourceModel(nullptr);

        ui.detailedDataItemTreeView->reset();
        ui.detailedDataItemTreeView->setModel(nullptr);

        m_detailedDataItemModel.reset(new (std::nothrow) DataItemTreeModel(directory, DataItemTreeModel::DataItemTreeModelE_Details));
             
        m_detailedDataItemSortFilterProxyModel->setSourceModel(m_detailedDataItemModel.get());
        ui.detailedDataItemTreeView->setModel(m_detailedDataItemSortFilterProxyModel.get());
                
        ui.previewWidget->clear();
    }

    dataItemSelected(index);
}

void DataAnalyzer::onDetailedDataItemSelected(QModelIndex index)
{
    ui.detailedDataItemTreeView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    dataItemSelected(m_detailedDataItemSortFilterProxyModel->mapToSource(index));
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
    if (showOnlyDirs)
        ui.dataItemTreeView->setModel(m_directoryModel.get());
    else
        ui.dataItemTreeView->setModel(m_fileDirectoryModel.get());
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
    if (m_selectedDataItem)
        DetailsPublisher::toModel(*ui.detailsTreeWidget, *ui.tagListWidget, *m_selectedDataItem);    
}

void DataAnalyzer::onAbout()
{
    Ui::aboutDialog uiAbout;
    QDialog *dialog = new QDialog();
    uiAbout.setupUi(dialog);
    dialog->exec();
    delete dialog;
}

void DataAnalyzer::onMD5Triggered()
{    
    QItemSelectionModel *selectionModel = ui.dataItemTreeView->selectionModel();
    if(selectionModel) // If the model exists
    {
        if (m_selectedDataItem) // If the model has an selection
        {
            QModelIndex currentIndex = selectionModel->currentIndex();
            if (currentIndex.isValid())
            {
                DataItem *item = static_cast<DataItem *>(currentIndex.internalPointer());
                MD5Operation operation(*item);
                
                OperationDialog dialog(operation, OperationDialog::ModeE_NoDirSelect, this);
                dialog.setTitle(tr("MD5 computation"));
                dialog.exec();
            }
        }
        else // In case of no selection, just uise the root
        {
            MD5Operation operation(*m_topLevelDirectory);

            OperationDialog dialog(operation, OperationDialog::ModeE_NoDirSelect, this);
            dialog.setTitle(tr("MD5 computation"));
            dialog.exec();
        }
    }
}

void DataAnalyzer::onFileMagicTriggered()
{
    QItemSelectionModel *selectionModel = ui.dataItemTreeView->selectionModel();
    if (selectionModel) // If the model exists
    {
        if (m_selectedDataItem) // If the model has an selection
        {
            QModelIndex currentIndex = selectionModel->currentIndex();
            if (currentIndex.isValid())
            {
                DataItem *item = static_cast<DataItem *>(currentIndex.internalPointer());
                MagicOperation operation(*item);

                OperationDialog dialog(operation, OperationDialog::ModeE_NoDirSelect, this);
                dialog.setTitle(tr("Type detection"));
                dialog.exec();
            }
        }
        else // In case of no selection, just use the root
        {
            MagicOperation operation(*m_topLevelDirectory);

            OperationDialog dialog(operation, OperationDialog::ModeE_NoDirSelect, this);
            dialog.setTitle(tr("Type detection"));
            dialog.exec();
        }
    }
}

void DataAnalyzer::onCSVReportTriggered()
{
    if (m_topLevelDirectory)
    {
        CSVReportWriter writer("C:/tmp/report.csv");
        ReportOperation operation(writer, *m_topLevelDirectory);

        OperationDialog dialog(operation, OperationDialog::ModeE_NoDirSelect, this);
        dialog.setTitle(tr("CSV report"));
        dialog.exec();
    }
}

void DataAnalyzer::onRTFReportTriggered()
{
    if (m_topLevelDirectory)
    {
        RTFReportWriter writer("C:/tmp/report.rtf");
        ReportOperation operation(writer, *m_topLevelDirectory);

        OperationDialog dialog(operation, OperationDialog::ModeE_NoDirSelect, this);
        dialog.setTitle(tr("RTF report"));
        dialog.exec();
    }
}

void DataAnalyzer::dataItemSelected(QModelIndex index)
{
    DataItem *item = static_cast<DataItem *>(index.internalPointer());
    m_selectedDataItem = item;

    ui.detailsTreeWidget->clear();
    ui.previewWidget->clear();

    File *file = dynamic_cast<File *>(item);
    if (file)
    {
        if (QImageReader::supportedImageFormats().indexOf(StringHelper::WString2QString(item->extension()).toLower().toLatin1()) != -1)
        {
            QPixmap pixmap(StringHelper::WString2QString(file->path()));
            QPixmap scaledPixmap;

            if (pixmap.width() > pixmap.height())
                scaledPixmap = pixmap.scaledToWidth(ui.previewWidget->size().width());
            else
                scaledPixmap = pixmap.scaledToHeight(ui.previewWidget->size().height());

            ui.previewWidget->setPixmap(scaledPixmap);
        }
        else if (QImageReader::supportedImageFormats().indexOf(StringHelper::WString2QString(item->extension()).toLower().toLatin1()) != -1)
        {
            QImageReader imageReader(StringHelper::WString2QString(file->path()));
            QPixmap pixmap = QPixmap::fromImageReader(&imageReader);
            QPixmap scaledPixmap;

            if (pixmap.width() > pixmap.height())
                scaledPixmap = pixmap.scaledToWidth(ui.previewWidget->size().width());
            else
                scaledPixmap = pixmap.scaledToHeight(ui.previewWidget->size().height());

            ui.previewWidget->setPixmap(scaledPixmap);

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
