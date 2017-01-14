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

#include "../model/fs/Directory.h"
#include "../util/ModelHelper.h"
#include "../util/StringHelper.h"

#include "DirectoryTreeModel.h"

DirectoryTreeModel::DirectoryTreeModel(Directory *root, QObject *parent)
    : QAbstractItemModel(parent), rootItem(root), icon(QPixmap(":/images/folder-20110506-300px.png"))
{
    icon = icon.scaledToWidth(16);
}

DirectoryTreeModel::~DirectoryTreeModel()
{

}

int DirectoryTreeModel::columnCount(const QModelIndex &parent) const
{
     #pragma unused(parent)
    return 2;
}

QVariant DirectoryTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DecorationRole)
        switch (index.column())
        {
        case 0:
            return icon;
        default:
            break;
        }

    if (role != Qt::DisplayRole)
        return QVariant();

    Directory *item = static_cast<Directory*>(index.internalPointer());
    switch(index.column())
    {
    case 0:
        return StringHelper::toQString(item->name());
    case 1:
        return ModelHelper::filesCount(*item);
    default:
        throw std::runtime_error("not implemented");
    }
}

Qt::ItemFlags DirectoryTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant DirectoryTreeModel::headerData(int section, Qt::Orientation orientation,
    int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
        case 0:
            return QString("Name");
        case 1:
            return QString("# of files");
        default:
            return QVariant();
        }        
    }
    return QVariant();
}

QModelIndex DirectoryTreeModel::index(int row, int column, const QModelIndex &parent)
const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    Directory *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<Directory*>(parent.internalPointer());

    Directory *childItem = parentItem->directories().size() ? parentItem->directories()[row] : nullptr;

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex DirectoryTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    Directory *childItem = static_cast<Directory*>(index.internalPointer());
    if (!childItem)
        return QModelIndex();
    Directory *parentNode = dynamic_cast<Directory *>(childItem->parent());
    if (!parentNode)
        return QModelIndex();
    Directory *grandparentNode = dynamic_cast<Directory *>(parentNode->parent());
    if (!grandparentNode)
        return QModelIndex();
    auto container = grandparentNode->directories();
    auto it = std::find(container.begin(), container.end(), parentNode);
    int row = it == container.end() ? 0 : std::distance(container.begin(), it);

    return createIndex(row, index.column(), parentNode);
}

int DirectoryTreeModel::rowCount(const QModelIndex &parent) const
{
    Directory *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
       parentItem = static_cast<Directory*>(parent.internalPointer());

    return parentItem->directories().size();
}
