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

#include <fstream>
#include <future>
#include <queue>
#include <QString>
#include "../abstraction/FS.h"
#include "../model/GlobalInformation.h"
#include "../model/fs/Directory.h"
#include "../model/fs/File.h"
#include "../storage/SQLiteStorage.h"
#include "../util/compiler.h"
#include "../util/ModelHelper.h"
#include "../util/StringHelper.h"

#include "StoreOperation.h"

StoreOperation::StoreOperation(SQLiteStorage &storage, DataItem &rootItem, const GlobalInformation &globalInformation, const QString finalReport) : m_cancelWorkerActivity(false), m_Storage(storage), m_RootItem(rootItem), m_globalInformation(globalInformation), m_finalReport(finalReport)
{    
}

StoreOperation::~StoreOperation()
{
}

void StoreOperation::start(QString dir)
{
    UNUSED_VARIABLE(dir);
    throw std::runtime_error("Not supported");
}

void StoreOperation::start()
{
    m_asyncScanWorker = std::async(std::launch::async, [&]() { this->startOperation(); });
}

void StoreOperation::cancel()
{
    m_cancelWorkerActivity = true;
}

bool StoreOperation::isFinished() const
{
    return m_asyncScanWorker.valid() ? std::future_status::ready == m_asyncScanWorker.wait_for(std::chrono::seconds(0)) : true;
}

QString StoreOperation::path() const
{
    return StringHelper::toQString(m_RootItem.path());
}

uint32_t StoreOperation::totalFilesCount() const
{
    return ModelHelper::filesCount(m_RootItem);
}

void StoreOperation::startOperation()
{
    unsigned numFiles = 0, numDirs = 0;
    std::queue<Directory *> q;
    
    m_observersScanDir.call(StringHelper::toQString(m_RootItem.path()));

    QSqlDatabase::database().transaction();

    Directory *directory = dynamic_cast<Directory *>(&m_RootItem);
    if (directory)
    {
        q.push(directory);

        while (!m_cancelWorkerActivity && !q.empty()) {
            auto d = q.front();
            q.pop();

            // Clear cache flags before the "Save as" operation
            d->setDirty(false);
            d->setStored(false);

            if (m_Storage.store(*d))
                d->setStored(true);

            m_observersScanDir.call(StringHelper::toQString(d->path()));
            ++numDirs;

            for (Directory *directory : d->directories())
            {
                q.push(directory);
            }

            for (File *file : d->files())
            {
                // Clear cache flags before the "Save as" operation
                file->setDirty(false);
                file->setStored(false);

                if (m_Storage.store(*file))
                    file->setStored(true);

                ++numFiles;
                m_observersFilesRead.call(numFiles);
                m_observersProgress.call(numFiles);
            }
        }
    }

    m_Storage.store(StringHelper::toQString(m_globalInformation.getReferenceNumber()),
                    StringHelper::toQString(m_globalInformation.getReference()),
                    StringHelper::toQString(m_globalInformation.getId()),
                    m_finalReport
                    );

    QSqlDatabase::database().commit();

    m_observersResultVoid.call();
}
