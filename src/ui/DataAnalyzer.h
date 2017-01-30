#pragma once
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

#include <memory>
#include <QCryptographicHash>
#include <QTranslator>
#include "ui_DataAnalyzer.h"
#include "../controller/DataItemSortFilterProxyModel.h"
#include "../controller/DataItemTreeModel.h"
#include "../controller/DirectoryTreeModel.h"
#include "../controller/SearchDataItemTreeModel.h"
#include "../model/GlobalInformation.h"
#include "../model/fs/DataInfo.h"
#include "../model/fs/Directory.h"

// Forward declarations
class DataItem;
class Directory;
class File;
class QMenu;


class DataAnalyzer : public QMainWindow
{
    Q_OBJECT

public:
    DataAnalyzer(QWidget *parent = Q_NULLPTR);
    ~DataAnalyzer();

public Q_SLOTS:
    void onScanTriggered();
    void onDataItemSelected(QModelIndex index);
    void onDataItemCurrentChanged(const QModelIndex &current, const QModelIndex &previous);
    void onSearchDataItemCurrentChanged(const QModelIndex &current, const QModelIndex &previous);
    void onDetailedDataItemSelected(QModelIndex index);
    void onDetailedDataItemCurrentChanged(const QModelIndex &current, const QModelIndex &previous);
    void onAnalysisTextChange();
    void onDirOnly(bool showOnlyDirs);
    void onCounterClockwiseRotate();
    void onClockwiseRotate();
    void onTagChanged(int row);
    void onAbout();
    void onAboutQtTriggered();
    void onMD5Triggered();
    void onSHA1Triggered();
    void onSHA3_512Triggered();
    void onFileMagicTriggered();
    void onCSVReportTriggered();
    void onRTFReportTriggered();
    void onHTMLReportTriggered();
    void onNewTriggered();
    void onSearchTriggered();
    void onDatasetSettingsTriggered();
    void onOpenFileExternallyTriggered();
    void onCustomContextMenuInDetailedDataItemTreeView(const QPoint &point);
    void onCustomContextMenuInDataItemTreeView(const QPoint &point);
    void onLanguageChanged(QAction* action);


protected:
    void dataItemSelected(QModelIndex index);
    void assignDataItemToAnalysis(DataItem *item);
    void hashOperation(QCryptographicHash::Algorithm algorithm, DataInfo::DataInfoE info, QString &dialogTitle);
    void loadLanguage(const QString& rLanguage);
    void switchTranslator(QTranslator& translator, const QString& filename);
    void changeEvent(QEvent *event) override;
    void updateDetailsTreeWidget();

private:
    Ui::DataAnalyzerClass ui;
    std::shared_ptr<Directory> m_topLevelDirectory;
    
    std::unique_ptr<DirectoryTreeModel> m_directoryModel;
    std::unique_ptr<DataItemTreeModel> m_fileDirectoryModel;
    std::unique_ptr<SearchDataItemTreeModel> m_searchFileDirectoryModel;

    std::unique_ptr<DataItemTreeModel> m_detailedDataItemModel;
    std::unique_ptr<DataItemSortFilterProxyModel> m_detailedDataItemSortFilterProxyModel;
    DataItem *m_analysisDataItem;
    DataItem *m_selectedDataItem;
    Directory m_searchResult;

    GlobalInformation m_globalInformation;
    QMenu *m_contextMenu;

    QTranslator m_translator; // contains the translations for this application
    QString m_currentLanguage; // contains the currently loaded language
};
