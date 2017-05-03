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
#include <unordered_map>
#include "../model/GlobalInformation.h"
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

bool SQLiteStorage::store(const QString &referenceNumber, const QString &reference, const QString &id, const QString &finalReport)
{
    QSqlQuery query;
    query.prepare("UPDATE Report SET referenceNumber = :referenceNumber, reference = :reference, id = :id, finalReport = :finalReport");
    query.bindValue(":referenceNumber", referenceNumber);
    query.bindValue(":reference", reference);
    query.bindValue(":id", id);
    query.bindValue(":finalReport", finalReport);

    bool success = query.exec();

    if (!success)
        qDebug() << "DB error: " << query.lastError();

    return success;
}

Directory *SQLiteStorage::load()
{
    std::unordered_map<uint64_t, File *> files;
    std::unordered_map<uint64_t, Directory *> directories;
    directories[0] = nullptr;

// TODO: error conditions
    {
        QSqlQuery query("SELECT id, parent, name, extension, size, modificationTimestamp, creationTimestamp, tag FROM Directory ORDER BY id");
        while (query.next()) {
            uint64_t id = query.value(0).toULongLong();
            uint64_t parentId = query.value(1).toULongLong();
            QString name = query.value(2).toString();
            Directory *dir = new Directory(StringHelper::toStdString(name), id == parentId ? nullptr : directories[parentId]);
            fillDataItem(*dir, query);
            dir->setUniqueId(id);
            directories[id] = dir;

            if (id != parentId)
                directories[parentId]->addDirectory(dir);
        }
    }

    if (directories[0])
    {
        QSqlQuery query("SELECT id, parent, name, extension, size, modificationTimestamp, creationTimestamp, tag FROM File ORDER BY id");
        while (query.next()) {
            uint64_t id = query.value(0).toULongLong();
            uint64_t parentId = query.value(1).toULongLong();
            QString name = query.value(2).toString();
            File *file = new File(StringHelper::toStdString(name), directories[parentId]);
            fillDataItem(*file, query);
            file->setUniqueId(id);
            files[id] = file;
            // TODO: check if the parent really exists
            directories[parentId]->addFile(file);
        }

        {
            QSqlQuery query("SELECT dataItemId, typeId, value FROM Information");
            while (query.next()) {
                uint64_t id = query.value(0).toULongLong();
                uint64_t typeId = query.value(1).toULongLong();
                QString value = query.value(2).toString();

                if (files.find(id) != files.end())
                    files[id]->addInfo(static_cast<DataInfo::DataInfoE>(typeId), value.toStdString());
                else if (directories.find(id) != directories.end())
                    directories[id]->addInfo(static_cast<DataInfo::DataInfoE>(typeId), value.toStdString());
                else {
                    std::for_each(files.begin(), files.end(), [](std::unordered_map<uint64_t, File *>::value_type &item){delete item.second;});
                    std::for_each(directories.begin(), directories.end(), [](std::unordered_map<uint64_t, Directory *>::value_type &item){delete item.second;});
                    return nullptr;
                }
            }
        }

        {
            QSqlQuery query("SELECT dataItemId, analysis FROM Analysis");
            while (query.next()) {
                uint64_t id = query.value(0).toULongLong();
                QString value = query.value(1).toString();

                if (files.find(id) != files.end())
                    files[id]->addInfo(DataInfo::DataInfoE_Analysis, value.toStdString());
                else if (directories.find(id) != directories.end())
                    directories[id]->addInfo(DataInfo::DataInfoE_Analysis, value.toStdString());
                else {
                    std::for_each(files.begin(), files.end(), [](std::unordered_map<uint64_t, File *>::value_type &item){delete item.second;});
                    std::for_each(directories.begin(), directories.end(), [](std::unordered_map<uint64_t, Directory *>::value_type &item){delete item.second;});
                    return nullptr;
                }
            }
        }
    }

    return directories[0];
}

void SQLiteStorage::fillDataItem(DataItem &item, QSqlQuery &query)
{
    QString extension = query.value(3).toString();
    uint64_t size = query.value(4).toULongLong();
    uint64_t modificationTimestamp = query.value(5).toULongLong();
    uint64_t creationTimestmap = query.value(6).toULongLong();
    uint32_t tag = query.value(7).toUInt();

    item.setSize(size);
    item.setExtension(StringHelper::toStdString(extension));
    item.setModificationTimestamp(modificationTimestamp);
    item.setCreationTimestamp(creationTimestmap);
    item.addInfo(DataInfo::DataInfoE_Tag, QString::number(tag).toStdString());
    item.setDirty(false);
    item.setStored(true);
}

void SQLiteStorage::load(GlobalInformation &globalInformation, QString &finalReport)
{
    QSqlQuery query("SELECT finalReport, referenceNumber, reference, id FROM Report");
    while (query.next()) {
        finalReport.clear();
        finalReport += query.value(0).toString();
        globalInformation.setReferenceNumber(StringHelper::toStdString(query.value(1).toString()));
        globalInformation.setReference(StringHelper::toStdString(query.value(2).toString()));
        globalInformation.setId(StringHelper::toStdString(query.value(3).toString()));
    }
}
