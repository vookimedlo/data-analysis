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
#include "../model/fs/Directory.h"
#include "../model/fs/File.h"
#include "../storage/SQLiteStorage.h"
#include "../util/compiler.h"
#include "../util/ModelHelper.h"
#include "../util/StringHelper.h"

#include "OpenOperation.h"

OpenOperation::OpenOperation(SQLiteStorage &storage) : m_cancelWorkerActivity(false), m_Storage(storage)
{    
}

OpenOperation::~OpenOperation()
{
}

void OpenOperation::start(QString dir)
{
    UNUSED_VARIABLE(dir);
    throw std::runtime_error("Not supported");
}

void OpenOperation::start()
{
    m_asyncScanWorker = std::async(std::launch::async, [&]() { this->startOperation(); });
}

void OpenOperation::cancel()
{
    m_cancelWorkerActivity = true;
}

bool OpenOperation::isFinished() const
{
    return m_asyncScanWorker.valid() ? std::future_status::ready == m_asyncScanWorker.wait_for(std::chrono::seconds(0)) : true;
}

QString OpenOperation::path() const
{
    return "...";
}

uint32_t OpenOperation::totalFilesCount() const
{
    return 0;
}

void OpenOperation::startOperation()
{   
    Directory *dir = m_Storage.load();

    m_observersProgress.call(100);
    m_asyncWorkerResult.reset(dir);
    m_observersResult.call(m_asyncWorkerResult);
}
