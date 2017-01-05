#pragma once
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

#include <memory>
#include <QCryptographicHash>
#include "ui_DataAnalyzer.h"
#include "../controller/DataItemSortFilterProxyModel.h"
#include "../controller/DataItemTreeModel.h"
#include "../controller/DirectoryTreeModel.h"
#include "../fs/DataInfo.h"

class DataItem;
class Directory;
class File;


class DataAnalyzer : public QMainWindow
{
    Q_OBJECT

public:
    DataAnalyzer(QWidget *parent = Q_NULLPTR);

public Q_SLOTS:
    void onScanTriggered();
    void onDataItemSelected(QModelIndex index);
    void onDetailedDataItemSelected(QModelIndex index);
    void onAnalysisTextChange();
    void onDirOnly(bool showOnlyDirs);
    void onCounterClockwiseRotate();
    void onClockwiseRotate();
    void onTagChanged(int row);
    void onAbout();
    void onMD5Triggered();
    void onFileMagicTriggered();
    void onCSVReportTriggered();
    void onRTFReportTriggered();
    void onSHA1Triggered();
    void onSHA3_512Triggered();

protected:
    void dataItemSelected(QModelIndex index);
    void assignDataItemToAnalysis(DataItem *item);
    void hashOperation(QCryptographicHash::Algorithm algorithm, DataInfo::DataInfoE info, QString &dialogTitle);

private:
    Ui::DataAnalyzerClass ui;
    std::shared_ptr<Directory> m_topLevelDirectory;
    
    std::unique_ptr<DirectoryTreeModel> m_directoryModel;
    std::unique_ptr<DataItemTreeModel> m_fileDirectoryModel;

    std::unique_ptr<DataItemTreeModel> m_detailedDataItemModel;
    std::unique_ptr<DataItemSortFilterProxyModel> m_detailedDataItemSortFilterProxyModel;
    DataItem *m_analysisDataItem;

    DataItem *m_selectedDataItem;
};
