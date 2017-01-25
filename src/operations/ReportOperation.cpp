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

#include <future>
#include <queue>
#include "../model/fs/Directory.h"
#include "../model/fs/File.h"
#include "../util/compiler.h"
#include "../util/ModelHelper.h"
#include "../util/StringHelper.h"

#include "ReportOperation.h"

ReportOperation::ReportOperation(ReportWriter& writer, DataItem &rootItem) : m_cancelWorkerActivity(false), m_ReportWriter(writer), m_RootItem(rootItem)
{
}

void ReportOperation::start(QString dir)
{
    UNUSED_VARIABLE(dir);
    throw std::runtime_error("Not implemented!");
}

void ReportOperation::start()
{
    m_asyncScanWorker = std::async(std::launch::async, [&]() { this->startOperation(); });
}

void ReportOperation::cancel()
{
    m_cancelWorkerActivity = true;
}

bool ReportOperation::isFinished() const
{
    return m_asyncScanWorker.valid() ? std::future_status::ready == m_asyncScanWorker.wait_for(std::chrono::seconds(0)) : true;
}

QString ReportOperation::path() const
{
    return StringHelper::toQString(m_RootItem.path());
}

uint32_t ReportOperation::totalFilesCount() const
{
    return ModelHelper::filesCount(m_RootItem);
}

void ReportOperation::startOperation()
{
    unsigned numFiles = 0, numDirs = 0;
    std::queue<Directory *> q;
    
    m_observersScanDir.call(StringHelper::toQString(m_RootItem.path()));

    if (m_ReportWriter.open())
    {
        Directory *directory = dynamic_cast<Directory *>(&m_RootItem);
        if (directory)
        {
            q.push(directory);

            // Uncomment this if you would like to write the top level directory
            // m_ReportWriter.write(*directory);

            while (!m_cancelWorkerActivity && !q.empty()) {
                auto d = q.front();
                q.pop();

                m_observersScanDir.call(StringHelper::toQString(d->path()));
                ++numDirs;

                for (Directory *directory : d->directories())
                {
                    m_ReportWriter.write(*directory);
                    q.push(directory);
                }

                for (File *file : d->files())
                {
                    m_ReportWriter.write(*file);

                    ++numFiles;
                    m_observersFilesRead.call(numFiles);
                    m_observersProgress.call(numFiles);
                }
            }
        }
        else
        {
            m_ReportWriter.write(m_RootItem);

            ++numFiles;
            m_observersFilesRead.call(numFiles);
            m_observersProgress.call(numFiles);
        }

        m_ReportWriter.close();
    }

    m_observersResultVoid.call();
}
