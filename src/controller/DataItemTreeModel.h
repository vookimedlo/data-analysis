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

#include <QAbstractItemModel>
#include <QPixmap>
#include <QModelIndex>
#include <QVariant>

/// Forward declarations
class Directory;

class DataItemTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum ColumnTypeE : uint8_t
    {
        ColumnTypeE_Name,
        ColumnTypeE_FilesCount,
        ColumnTypeE_Size,
        ColumnTypeE_Extension,
        ColumnTypeE_ModificationTimestamp,
    };

public:
    enum DataItemTreeModelE { DataItemTreeModelE_NoDetails, DataItemTreeModelE_Details};
    explicit DataItemTreeModel(Directory *root, DataItemTreeModelE mode, QObject *parent = nullptr);
    ~DataItemTreeModel();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private:
    Directory *rootItem;
    QPixmap directoryIcon;
    QPixmap fileIcon;
    DataItemTreeModelE m_mode;
};

