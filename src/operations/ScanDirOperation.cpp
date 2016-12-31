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

#include <future>
#include <iostream>
#include <queue>
#include "../abstraction/DiskReader.h"
#include "../fs/Directory.h"
#include "../fs/File.h"

#include "ScanDirOperation.h"


ScanDirOperation::ScanDirOperation() : m_cancelWorkerActivity(false)
{
    
}

void ScanDirOperation::start(std::wstring dir)
{
    m_asyncScanWorker =  std::async(std::launch::async, [&](std::wstring dir) { this->doScan(dir); }, dir);
    m_cancelWorkerActivity = false;
}

void ScanDirOperation::start()
{
    throw std::runtime_error("Not implemented!");
}

void ScanDirOperation::cancel()
{
    m_cancelWorkerActivity = true;
}

bool ScanDirOperation::isFinished() const
{
    return m_asyncScanWorker.valid() ? std::future_status::ready == m_asyncScanWorker.wait_for(std::chrono::seconds(0)) : true;
}

uint32_t ScanDirOperation::totalFilesCount() const
{
    return 0;
}

std::wstring ScanDirOperation::path() const
{
    return std::wstring();
}

void ScanDirOperation::doScan(std::wstring startDir)
{
    unsigned numFiles = 0, numDirs = 0;
    DiskReader dr;
    
    m_observersScanDir.call(startDir);

    std::unique_ptr<Directory> dir(new Directory(startDir, nullptr));

    std::queue<Directory *> q;
    q.push(dir.get());

    while (!m_cancelWorkerActivity && !q.empty()) {
        auto d = q.front();
        q.pop();

        m_observersScanDir.call(d->path());

        ++numDirs;

#ifdef DEBUG_VERBOSE_SCAN
        std::wcerr << "Scanning: " << d->path() << std::endl;
#endif
        bool rslt = dr.readDirectoryStructure(d);
        d->optimize();

        for (Directory *directory : d->directories())
        {
#ifdef DEBUG_VERBOSE_SCAN_FOUND_DIRS
            std::wcerr << "Dir: " << directory.path() << std::endl;
#endif
            q.push(directory);
        }

#ifdef DEBUG_VERBOSE_SCAN_FOUND_FILES
        for (File &file : d->files())
        {
            std::wcerr << "File: " << file.path() << std::endl;
        }
#endif

        numFiles += d->files().size();
        m_observersFilesRead.call(numFiles);
    }

    std::cerr << "Total dirs in memory: " << numDirs << std::endl;
    std::cerr << "Total files in memory: " << numFiles << std::endl;
    
   m_asyncScanWorkerResult.reset(dir.release());
   m_observersProgress.call(100);

   m_observersResult.call(m_asyncScanWorkerResult);
}
