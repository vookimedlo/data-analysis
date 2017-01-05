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

#include <QDateTime>
#include <QLabel>
#include <QListWidget>
#include <QTreeWidget>
#include "../util/StringHelper.h"
#include "../fs/DataItem.h"

#include "DetailsPublisher.h"


void DetailsPublisher::toUI(QTreeWidget& treeWidget, QListWidget& tagListWidget, DataItem& item)
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(&treeWidget);
    treeItem->setText(0, tr("Full path"));
    treeItem->setText(1, StringHelper::WString2QString(item.path()));

    treeItem = new QTreeWidgetItem(&treeWidget);
    treeItem->setText(0, tr("Name"));
    treeItem->setText(1, StringHelper::WString2QString(item.name()));

    treeItem = new QTreeWidgetItem(&treeWidget);
    treeItem->setText(0, tr("Size"));
    treeItem->setText(1, QString::number(item.size()));

    treeItem = new QTreeWidgetItem(&treeWidget);
    treeItem->setText(0, tr("Creation timestamp"));
    treeItem->setText(1, QDateTime::fromTime_t(item.creationTimestamp()).toString(Qt::SystemLocaleLongDate));

    treeItem = new QTreeWidgetItem(&treeWidget);
    treeItem->setText(0, tr("Modification timestamp"));
    treeItem->setText(1, QDateTime::fromTime_t(item.modificationTimestamp()).toString(Qt::SystemLocaleLongDate));

    for (DataInfo::DataInfoE info : item.infos())
    {
        switch (info)
        {
        case DataInfo::DataInfoE_Tag:
        case DataInfo::DataInfoE_Analysis:
            continue;
        case DataInfo::DataInfoE_MD5:
            treeItem = new QTreeWidgetItem(&treeWidget);
            treeItem->setText(0, tr("MD5 fingerprint"));
            break;
        case DataInfo::DataInfoE_Magic:
        {
            treeItem = new QTreeWidgetItem(&treeWidget);
            treeItem->setText(0, tr("Data type detection"));

            QLabel *label = new (std::nothrow) QLabel(QString::fromStdString(item.info(info)));
            label->setWordWrap(true);
            treeWidget.setItemWidget(treeItem, 1, label);
            break;
        }
        case DataInfo::DataInfoE_SHA1:
            treeItem = new QTreeWidgetItem(&treeWidget);
            treeItem->setText(0, tr("SHA-1 fingerprint"));
            break;
        case DataInfo::DataInfoE_SHA3_512:
            treeItem = new QTreeWidgetItem(&treeWidget);
            treeItem->setText(0, tr("SHA3-512 fingerprint"));
            break;
        default:
            treeItem = new QTreeWidgetItem(&treeWidget);
            treeItem->setText(0, "----");            
        }

        if (info != DataInfo::DataInfoE_Magic)
           treeItem->setText(1, QString::fromStdString(item.info(info)));
    }

    tagListWidget.setCurrentRow(item.isInfoValid(DataInfo::DataInfoE_Tag) ? QString::fromStdString(item.info(DataInfo::DataInfoE_Tag)).toInt() : 0);
}

void DetailsPublisher::toModel(QTreeWidget& treeWidget, QListWidget& tagListWidget, DataItem& item)
{
    if (tagListWidget.currentRow() >= 0)
        item.addInfo(DataInfo::DataInfoE_Tag, QString::number(tagListWidget.currentRow()).toStdString());
}
