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
#include "../controller/SearchSettings.h"
#include "../model/fs/Directory.h"
#include "../model/fs/File.h"
#include "../util/compiler.h"
#include "../util/ModelHelper.h"
#include "../util/StringHelper.h"

#include "SearchOperation.h"

SearchOperation::SearchOperation(SearchSettings& settings, DataItem &rootItem) : m_cancelWorkerActivity(false), m_settings(settings), m_RootItem(rootItem)
{
}

void SearchOperation::start(QString dir)
{
    UNUSED_VARIABLE(dir);
    throw std::runtime_error("Not implemented!");
}

void SearchOperation::start()
{
    m_asyncScanWorker = std::async(std::launch::async, [&]() { this->startOperation(); });
}

void SearchOperation::cancel()
{
    m_cancelWorkerActivity = true;
}

bool SearchOperation::isFinished() const
{
    return m_asyncScanWorker.valid() ? std::future_status::ready == m_asyncScanWorker.wait_for(std::chrono::seconds(0)) : true;
}

QString SearchOperation::path() const
{
    return StringHelper::toQString(m_RootItem.path());
}

uint32_t SearchOperation::totalFilesCount() const
{
    return ModelHelper::filesCount(m_RootItem);
}

void SearchOperation::startOperation()
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

            if (m_settings.isDirectoryEnabled())
            {
                bool isSearched = false;
                bool hasFailed = false;
                if (m_settings.isNameEnabled() && !hasFailed)
                {
                    isSearched = StringHelper::toQString(directory->name()).contains(m_settings.getName());
                    hasFailed = !isSearched;
                }

                if (m_settings.isExtensionEnabled() && !hasFailed)
                {
                    isSearched = StringHelper::toQString(directory->extension()).contains(m_settings.getExtension());
                    hasFailed = !isSearched;
                }
#if 0
                if (m_settings.isSizeEnabled() && !hasFailed)
                {
                    isSearched = m_settings.getSize().first <= directory->size() && directory->size() <= m_settings.getSize().second;
                    hasFailed = !isSearched;
                }
#endif

                if (isSearched && !hasFailed)
                    m_settings.getSearchResult().addDirectory(directory);
            }


            for (Directory *directory : d->directories())
            {
                q.push(directory);
            }

            if (m_settings.isFileEnabled())
            {
                for (File *file : d->files())
                {
                    bool isSearched = false;
                    bool hasFailed = false;
                    if (m_settings.isNameEnabled() && !hasFailed)
                    {
                        isSearched = StringHelper::toQString(file->name()).contains(m_settings.getName());
                        hasFailed = !isSearched;
                    }
                        
                    if (m_settings.isExtensionEnabled() && !hasFailed)
                    {
                        isSearched = StringHelper::toQString(file->extension()).contains(m_settings.getExtension());
                        hasFailed = !isSearched;
                    }

                    if (m_settings.isSizeEnabled() && !hasFailed)
                    {
                        isSearched = m_settings.getSize().first <= file->size() && file->size() <= m_settings.getSize().second;
                        hasFailed = !isSearched;
                    }

                    if (isSearched && !hasFailed)
                        m_settings.getSearchResult().addFile(file);

                    ++numFiles;
                    m_observersFilesRead.call(numFiles);
                    m_observersProgress.call(numFiles);
                }
            }
            else
            {
                // We are not seaching for any files, so just mark them as checked.
                numFiles += d->files().size();
                m_observersFilesRead.call(numFiles);
                m_observersProgress.call(numFiles);
            }
        }
    }
    else
    {
        bool isSearched = false;
        bool hasFailed = false;
        if (m_settings.isNameEnabled() && !hasFailed)
        {
            isSearched = StringHelper::toQString(m_RootItem.name()).contains(m_settings.getName());
            hasFailed = !isSearched;
        }

        if (m_settings.isExtensionEnabled() && !hasFailed)
        {
            isSearched = StringHelper::toQString(m_RootItem.extension()).contains(m_settings.getExtension());
            hasFailed = !isSearched;
        }

        if (m_settings.isSizeEnabled() && !hasFailed)
        {
            isSearched = m_settings.getSize().first <= m_RootItem.size() && m_RootItem.size() <= m_settings.getSize().second;
            hasFailed = !isSearched;
        }

        if (isSearched && !hasFailed)
            m_settings.getSearchResult().addFile(dynamic_cast<File *>(&m_RootItem));

        ++numFiles;
        m_observersFilesRead.call(numFiles);
        m_observersProgress.call(numFiles);
    }

    m_observersResultVoid.call();
}