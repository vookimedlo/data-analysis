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

#include <fstream>
#include <future>
#include <queue>
#include <QString>
#include "../abstraction/FS.h"
#include "../model/fs/Directory.h"
#include "../model/fs/File.h"
#include "../util/ModelHelper.h"
#include "../util/StringHelper.h"

#include "MagicOperation.h"

MagicOperation::MagicOperation(DataItem &rootItem) : m_cancelWorkerActivity(false), m_RootItem(rootItem)
{    
    m_MagicCookie = magic_open(MAGIC_NONE);
    if (m_MagicCookie == NULL) {
        //printf("unable to initialize magic library\n");
        return;
    }

    if (magic_load(m_MagicCookie, NULL) != 0) {
        //printf("cannot load magic database - %s\n", magic_error(magic_cookie));
        magic_close(m_MagicCookie);
        return;
    }
    //printf("%s\n", magic_full);
}

MagicOperation::~MagicOperation()
{
    magic_close(m_MagicCookie);
}

void MagicOperation::start(std::wstring dir)
{
    #pragma unused(dir)
    throw std::runtime_error("Not supported");
}

void MagicOperation::start()
{
    m_asyncScanWorker = std::async(std::launch::async, [&]() { this->startOperation(); });
}

void MagicOperation::cancel()
{
    m_cancelWorkerActivity = true;
}

bool MagicOperation::isFinished() const
{
    return m_asyncScanWorker.valid() ? std::future_status::ready == m_asyncScanWorker.wait_for(std::chrono::seconds(0)) : true;
}

std::wstring MagicOperation::path() const
{
    return m_RootItem.path();
}

uint32_t MagicOperation::totalFilesCount() const
{
    return ModelHelper::filesCount(m_RootItem);
}

void MagicOperation::startOperation()
{
    unsigned numFiles = 0, numDirs = 0;
    std::queue<Directory *> q;
    
    m_observersScanDir.call(m_RootItem.path());

    Directory *directory = dynamic_cast<Directory *>(&m_RootItem);
    if (directory)
    {
        q.push(directory);

        while (!m_cancelWorkerActivity && !q.empty()) {
            auto d = q.front();
            q.pop();

            m_observersScanDir.call(d->path());
            ++numDirs;

            for (Directory *directory : d->directories())
            {
                q.push(directory);
            }

            for (File *file : d->files())
            {
                QString path(StringHelper::toQString(FS::LongPathNameToShort(file->path())));
                
                // check if file is readable, otherwise skip it => leads to crash
                std::ifstream f(path.toStdString(), std::ios::in | std::ios::binary);
                if (f.is_open())
                {
                    const char *result = magic_file(m_MagicCookie, path.toStdString().c_str());
                    if (result)
                        file->addInfo(DataInfo::DataInfoE_Magic, result);
                }

                ++numFiles;
                m_observersFilesRead.call(numFiles);
                m_observersProgress.call(numFiles);
            }
        }
    }
    else
    {
        const char *result = magic_file(m_MagicCookie, StringHelper::toQString(FS::LongPathNameToShort(m_RootItem.path())).toStdString().c_str());
        if (result)
            m_RootItem.addInfo(DataInfo::DataInfoE_Magic, result);

        ++numFiles;
        m_observersFilesRead.call(numFiles);
        m_observersProgress.call(numFiles);
    }

    m_observersResultVoid.call();
}
