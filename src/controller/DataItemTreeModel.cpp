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

#include <QDate>

#include "../fs/Directory.h"
#include "../fs/File.h"
#include "../util/ModelHelper.h"
#include "../util/StringHelper.h"

#include "DataItemTreeModel.h"


DataItemTreeModel::DataItemTreeModel(Directory *root, DataItemTreeModelE mode, QObject *parent)
    : QAbstractItemModel(parent), rootItem(root), directoryIcon(QPixmap(":/images/folder-20110506-300px.png")), fileIcon(QPixmap(":/images/matt-icons_file-x-generic-300px.png")), m_mode(mode)
{
    directoryIcon = directoryIcon.scaledToWidth(16);
    fileIcon = fileIcon.scaledToWidth(16);
}

DataItemTreeModel::~DataItemTreeModel()
{

}

int DataItemTreeModel::columnCount(const QModelIndex &parent) const
{    
    #pragma unused(parent)
    return m_mode == DataItemTreeModelE_NoDetails ? 2 : 5;
}

QVariant DataItemTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DecorationRole)
        switch (index.column())
        {
        case ColumnTypeE_Name:
        {
            DataItem *item = static_cast<DataItem*>(index.internalPointer());
            return dynamic_cast<Directory*>(item) ? directoryIcon: fileIcon;
        }
        default:
            break;
        }

    if (role != Qt::DisplayRole)
        return QVariant();

    DataItem *item = static_cast<DataItem*>(index.internalPointer());
    switch(index.column())
    {
    case ColumnTypeE_Name:
        return StringHelper::toQString(item->name());
    case ColumnTypeE_FilesCount:
        return ModelHelper::filesCount(*item);
    case ColumnTypeE_Size:
        return QString::number(item->size());
    case ColumnTypeE_Extension:
        return StringHelper::toQString(item->extension());
    case ColumnTypeE_ModificationTimestamp:
        return QDateTime::fromTime_t(item->modificationTimestamp()).toString(Qt::SystemLocaleShortDate);
    default:
        throw std::runtime_error("not implemented");
    }
}

Qt::ItemFlags DataItemTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant DataItemTreeModel::headerData(int section, Qt::Orientation orientation,
    int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
        case ColumnTypeE_Name:
            return QString("Name");
        case ColumnTypeE_FilesCount:
            return QString("# of files");
        case ColumnTypeE_Size:
            return QString("Velikost");
        case ColumnTypeE_Extension:
            return QString("Extension");
        case ColumnTypeE_ModificationTimestamp:
            return QString("Modification timestamp");
        default:
            return QVariant();
        }        
    }
    return QVariant();
}

QModelIndex DataItemTreeModel::index(int row, int column, const QModelIndex &parent)
const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    DataItem *parentDataItem = nullptr;

    if (!parent.isValid())
        parentDataItem = rootItem;
    else
        parentDataItem = static_cast<DataItem*>(parent.internalPointer());

    Directory *parentItem = dynamic_cast<Directory *>(parentDataItem);

    if(!parentItem)
    {
        return QModelIndex();
    }

    
    DataItem *childItem = nullptr;
    if (row < parentItem->directories().size())
    {
        childItem = parentItem->directories()[row];
    } 
    else if (parentItem->directories().size() <= row && parentItem->files().size())
    {
        childItem = parentItem->files()[row - parentItem->directories().size()];
    }

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex DataItemTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    DataItem *childItem = static_cast<DataItem*>(index.internalPointer());
    if (!childItem || rootItem == childItem)
        return QModelIndex();
    DataItem *parentNode = dynamic_cast<DataItem *>(childItem->parent());
    if (!parentNode)
        return QModelIndex();
    Directory *grandparentNode = dynamic_cast<Directory *>(parentNode->parent());
    if (!grandparentNode)
        return QModelIndex();

        if (parentNode == rootItem)
        return QModelIndex();

    auto container = grandparentNode->directories();

    auto it = std::find(container.begin(), container.end(), parentNode);
    int row = it != container.end() ? std::distance(container.begin(), it) : 0;

    return createIndex(row, index.column(), parentNode);
}

int DataItemTreeModel::rowCount(const QModelIndex &parent) const
{
    Directory *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
    {
        DataItem *parentDataItem = static_cast<DataItem*>(parent.internalPointer());;
        parentItem = dynamic_cast<Directory *>(parentDataItem);
        if (!parentItem)
            return 0;
    }
       

    return parentItem->directories().size() + parentItem->files().size();
}
