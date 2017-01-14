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

#include <QtWidgets>

#include "DataItemSortFilterProxyModel.h"
#include "DataItemTreeModel.h"
#include "../model/fs/DataItem.h"
#include "../model/fs/Directory.h"

DataItemSortFilterProxyModel::DataItemSortFilterProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{
}

void DataItemSortFilterProxyModel::setFilterMinimumDate(const QDate &date)
{
    minDate = date;
    invalidateFilter();
}

void DataItemSortFilterProxyModel::setFilterMaximumDate(const QDate &date)
{
    maxDate = date;
    invalidateFilter();
}

bool DataItemSortFilterProxyModel::filterAcceptsRow(int sourceRow,
    const QModelIndex &sourceParent) const
{
    QModelIndex nameIndex = sourceModel()->index(sourceRow, 0, sourceParent);
    QModelIndex modificationDateIndex = sourceModel()->index(sourceRow, 4, sourceParent);

    return sourceModel()->data(nameIndex).toString().contains(filterRegExp())
        && dateInRange(sourceModel()->data(modificationDateIndex).toDate());
}

bool DataItemSortFilterProxyModel::lessThan(const QModelIndex &left,
    const QModelIndex &right) const
{
    QVariant leftData = sourceModel()->data(left);
    QVariant rightData = sourceModel()->data(right);

    const DataItem* const leftItem = static_cast<const DataItem *>(left.internalPointer());
    const DataItem* const rightItem = static_cast<const DataItem *>(right.internalPointer());
    const Directory* const leftItemDir = dynamic_cast<const Directory *>(leftItem);
    const Directory* const rightItemDir = dynamic_cast<const Directory *>(rightItem);

    /* Keep folders on top */
    if ((leftItemDir && rightItemDir) || (!leftItemDir && !rightItemDir))
    {
        switch (left.column())
        {
        case DataItemTreeModel::ColumnTypeE_FilesCount:
        case DataItemTreeModel::ColumnTypeE_Size:
            return leftData.toULongLong() < rightData.toULongLong();
        case DataItemTreeModel::ColumnTypeE_ModificationTimestamp:
            return static_cast<DataItem *>(left.internalPointer())->modificationTimestamp() < static_cast<DataItem *>(right.internalPointer())->modificationTimestamp();
        default:
            return QString::localeAwareCompare(leftData.toString(), rightData.toString()) < 0;
        }
    }
    
    if (leftItem)
        return false;

    return true;    
}

bool DataItemSortFilterProxyModel::dateInRange(const QDate &date) const
{
    return (!minDate.isValid() || date > minDate) && (!maxDate.isValid() || date < maxDate);
}
