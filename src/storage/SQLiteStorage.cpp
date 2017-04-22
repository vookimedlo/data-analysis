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

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include "../model/fs/DataItem.h"
#include "../model/fs/Directory.h"
#include "../model/fs/File.h"
#include "../util/StringHelper.h"

#include "SQLiteStorage.h"

SQLiteStorage::SQLiteStorage(const QString& path) : m_db(QSqlDatabase::addDatabase("QSQLITE"))
{
    m_db.setDatabaseName(path);
}

SQLiteStorage::~SQLiteStorage()
{
    m_db.commit();
    m_db.close();
}

bool SQLiteStorage::open()
{
    if (!m_db.open())
    {
       qDebug() << "DB: connection with database failed";
    }
    else
    {
       qDebug() << "DB: connection OK";
    }

    return m_db.isOpen();
}

void SQLiteStorage::close()
{
    m_db.commit();
    m_db.close();
}

bool SQLiteStorage::store(const DataItem &item, QSqlQuery &query)
{
    bool success = false;

    query.bindValue(":id", item.getUniqueId());
    query.bindValue(":parent", item.parent() ? item.parent()->getUniqueId() : item.getUniqueId());
    query.bindValue(":name", StringHelper::toQString(item.name()));
    query.bindValue(":extension", StringHelper::toQString(item.extension()));
    query.bindValue(":size", item.size());
    query.bindValue(":modificationTimestamp", item.modificationTimestamp());
    query.bindValue(":creationTimestamp", item.creationTimestamp());
    query.bindValue(":tag", item.isInfoValid(DataInfo::DataInfoE_Tag) ? QString::fromStdString(item.info(DataInfo::DataInfoE_Tag)).toInt() : 0);

    success = query.exec();

    if (!success)
        qDebug() << "DB error: " << query.lastError();

    if (item.isInfoValid(DataInfo::DataInfoE_Analysis))
    {
        QSqlQuery query;
        query.prepare("INSERT INTO Analysis (dataItemId, analysis) VALUES (:id, :analysis)");
        query.bindValue(":id", item.getUniqueId());
        query.bindValue(":analysis", QString::fromStdString(item.info(DataInfo::DataInfoE_Analysis)));

        success &= query.exec();

        if (!success)
            qDebug() << "DB error: " << query.lastError();
    }

    for (DataInfo::DataInfoE info : item.infos())
    {
        switch (info)
        {
        case DataInfo::DataInfoE_Tag:
        case DataInfo::DataInfoE_Analysis:
            continue;
        default:
        {
            QSqlQuery query;
            query.prepare("INSERT INTO Information (dataItemId, typeId, value) VALUES (:id, :typeId, :value)");
            query.bindValue(":id", item.getUniqueId());
            query.bindValue(":typeId", info);
            query.bindValue(":value", QString::fromStdString(item.info(info)));

            success &= query.exec();

            if (!success)
                qDebug() << "DB error: " << query.lastError();
        }
        }
    }

    return success;
}

bool SQLiteStorage::store(const Directory &item)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Directory (id, parent, name, extension, size, modificationTimestamp, creationTimestamp, tag) VALUES (:id, :parent, :name, :extension, :size, :modificationTimestamp, :creationTimestamp, :tag)");

    return store(item, query);
}

bool SQLiteStorage::store(const File &item)
{
    QSqlQuery query;
    query.prepare("INSERT INTO File (id, parent, name, extension, size, modificationTimestamp, creationTimestamp) VALUES (:id, :parent, :name, :extension, :size, :modificationTimestamp, :creationTimestamp)");

    return store(item, query);
}

DataItem *SQLiteStorage::load()
{
    return nullptr;
}
