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
#include "../3rdParty/md5/md5.h"
#include "../fs/Directory.h"
#include "../fs/File.h"
#include "../util/ModelHelper.h"
#include "../util/StringHelper.h"

#include "MD5Operation.h"

MD5Operation::MD5Operation(DataItem &rootItem) : m_cancelWorkerActivity(false), m_RootItem(rootItem)
{
}

void MD5Operation::start(std::wstring dir)
{
    throw std::runtime_error("Not implemented!");
}

void MD5Operation::start()
{
    m_asyncScanWorker = std::async(std::launch::async, [&]() { this->startOperation(); });
}

void MD5Operation::cancel()
{
    m_cancelWorkerActivity = true;
}

bool MD5Operation::isFinished() const
{
    return m_asyncScanWorker.valid() ? std::future_status::ready == m_asyncScanWorker.wait_for(std::chrono::seconds(0)) : true;
}

std::wstring MD5Operation::path() const
{
    return m_RootItem.path();
}

uint32_t MD5Operation::totalFilesCount() const
{
    return ModelHelper::filesCount(m_RootItem);
}

void MD5Operation::startOperation()
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
                MD5 md5;
                if (computeMD5(md5, file->path()))
                    file->addInfo(DataInfo::DataInfoE_MD5, md5.hexdigest());

                ++numFiles;
                m_observersFilesRead.call(numFiles);
                m_observersProgress.call(numFiles);
            }
        }
    }
    else
    {
        MD5 md5;
        if (computeMD5(md5, m_RootItem.path()))
            m_RootItem.addInfo(DataInfo::DataInfoE_MD5, md5.hexdigest());

        ++numFiles;
        m_observersFilesRead.call(numFiles);
        m_observersProgress.call(numFiles);
    }

    m_observersResultVoid.call();
}

bool MD5Operation::computeMD5(MD5& md5, std::wstring path)
{
    char memblock[4096];
    std::ifstream f(StringHelper::WString2String(path), std::ios::in | std::ios::binary);

    if (f.is_open())
    {
        while (f)
        {
            f.read(memblock, sizeof(memblock));
            md5.update(memblock, f.gcount());
        }
        if (f.eof())
        {
            md5.finalize();
            f.close();
            return true; /***********/
        }
        f.close();
    }
    return false; /***********/
}
