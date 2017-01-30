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

#include <QDate>

#include "../model/fs/Directory.h"
#include "../model/fs/File.h"
#include "../util/compiler.h"
#include "../util/ModelHelper.h"
#include "../util/StringHelper.h"

#include "SearchDataItemTreeModel.h"


SearchDataItemTreeModel::SearchDataItemTreeModel(Directory *root, DataItemTreeModelE mode, QObject *parent)
    : DataItemTreeModel(root, mode, parent)
{
    directoryIcon = directoryIcon.scaledToWidth(16);
    fileIcon = fileIcon.scaledToWidth(16);
}

SearchDataItemTreeModel::~SearchDataItemTreeModel()
{

}

QModelIndex SearchDataItemTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    DataItem *childItem = static_cast<DataItem*>(index.internalPointer());
    if (!childItem || rootItem == childItem)
        return QModelIndex();
    //DataItem *parentNode = dynamic_cast<DataItem *>(childItem->parent());
    auto itpd = std::find(rootItem->directories().begin(), rootItem->directories().end(), childItem);
    auto itpf = std::find(rootItem->files().begin(), rootItem->files().end(), childItem);
    DataItem *parentNode = (itpd == rootItem->directories().end() && itpf == rootItem->files().end()) ? dynamic_cast<DataItem *>(childItem->parent()) : rootItem;
    if (!parentNode)
        return QModelIndex();
//    Directory *grandparentNode = dynamic_cast<Directory *>(parentNode->parent());
    Directory *grandparentNode = parentNode == rootItem ? nullptr : 
                                            (std::find(rootItem->directories().begin(), rootItem->directories().end(), parentNode->parent()) == rootItem->directories().end()
                                            && std::find(rootItem->files().begin(), rootItem->files().end(), parentNode->parent()) == rootItem->files().end())
                                            ? dynamic_cast<Directory *>(parentNode->parent())
                                            : rootItem;
    if (!grandparentNode)
        return QModelIndex();

    if (parentNode == rootItem)
    return QModelIndex();

    auto container = grandparentNode->directories();

    auto it = std::find(container.begin(), container.end(), parentNode);
    int row = it != container.end() ? std::distance(container.begin(), it) : 0;

    // I was doing return createIndex(row, index.colum()).
    // In this case, the parent is not the same when selecting all cells in a row (default selection mode).
    // The parent should always be column 0, and the correct implementations should return createIndex(row , 0).
    return createIndex(row, 0, parentNode);
}
