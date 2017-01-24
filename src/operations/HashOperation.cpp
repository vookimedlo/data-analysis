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
#include <QCryptographicHash>
#include <QFile>
#include "../model/fs/Directory.h"
#include "../model/fs/File.h"
#include "../util/ModelHelper.h"
#include "../util/StringHelper.h"

#include "HashOperation.h"

HashOperation::HashOperation(QCryptographicHash &hash, DataInfo::DataInfoE info, DataItem &rootItem) : m_cancelWorkerActivity(false), m_RootItem(rootItem), m_hash(hash), m_info(info)
{
}

void HashOperation::start(QString dir)
{
    #pragma unused(dir)
    throw std::runtime_error("Not implemented!");
}

void HashOperation::start()
{
    m_asyncScanWorker = std::async(std::launch::async, [&]() { this->startOperation(); });
}

void HashOperation::cancel()
{
    m_cancelWorkerActivity = true;
}

bool HashOperation::isFinished() const
{
    return m_asyncScanWorker.valid() ? std::future_status::ready == m_asyncScanWorker.wait_for(std::chrono::seconds(0)) : true;
}

QString HashOperation::path() const
{
    return StringHelper::toQString(m_RootItem.path());
}

uint32_t HashOperation::totalFilesCount() const
{
    return ModelHelper::filesCount(m_RootItem);
}

void HashOperation::startOperation()
{
    unsigned numFiles = 0, numDirs = 0;
    std::queue<Directory *> q;
    
    m_observersScanDir.call(StringHelper::toQString(m_RootItem.path()));

    Directory *directory = dynamic_cast<Directory *>(&m_RootItem);
    if (directory)
    {
        q.push(directory);

        while (!m_cancelWorkerActivity && !q.empty()) {
            auto d = q.front();
            q.pop();

            m_observersScanDir.call(StringHelper::toQString(d->path()));
            ++numDirs;

            for (Directory *directory : d->directories())
            {
                q.push(directory);
            }

            for (File *file : d->files())
            {
                if (computeHash(m_hash, StringHelper::toQString(file->path())))
                    file->addInfo(m_info, QString(m_hash.result().toHex()).toStdString());

                ++numFiles;
                m_observersFilesRead.call(numFiles);
                m_observersProgress.call(numFiles);
            }
        }
    }
    else
    {
        if (computeHash(m_hash, StringHelper::toQString(m_RootItem.path())))
            m_RootItem.addInfo(m_info, QString(m_hash.result().toHex()).toStdString());

        ++numFiles;
        m_observersFilesRead.call(numFiles);
        m_observersProgress.call(numFiles);
    }

    m_observersResultVoid.call();
}

bool HashOperation::computeHash(QCryptographicHash& hash, QString path)
{
    QFile file(path);
    hash.reset();
    
    if (file.open(QIODevice::ReadOnly))
        return hash.addData(&file);

    return false;
}
