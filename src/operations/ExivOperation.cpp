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
#include <exiv2/exiv2.hpp>
#include <QString>
#include "../abstraction/FS.h"
#include "../model/fs/Directory.h"
#include "../model/fs/File.h"
#include "../util/ModelHelper.h"
#include "../util/StringHelper.h"

#include "ExivOperation.h"

ExivOperation::ExivOperation(DataItem &rootItem) : m_cancelWorkerActivity(false), m_RootItem(rootItem)
{    
}

ExivOperation::~ExivOperation()
{
}

void ExivOperation::start(std::wstring dir)
{
    #pragma unused(dir)
    throw std::runtime_error("Not supported");
}

void ExivOperation::start()
{
    m_asyncScanWorker = std::async(std::launch::async, [&]() { this->startOperation(); });
}

void ExivOperation::cancel()
{
    m_cancelWorkerActivity = true;
}

bool ExivOperation::isFinished() const
{
    return m_asyncScanWorker.valid() ? std::future_status::ready == m_asyncScanWorker.wait_for(std::chrono::seconds(0)) : true;
}

std::wstring ExivOperation::path() const
{
    return m_RootItem.path();
}

uint32_t ExivOperation::totalFilesCount() const
{
    return ModelHelper::filesCount(m_RootItem);
}

void ExivOperation::startOperation()
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
                std::string str = StringHelper::toStdString(path);

                try {
//                    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(StringHelper::toStdString(path).c_str());
                    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open("a.jpg", false);

                    
                
                    if (image.get() != 0)
                    {
 //                   assert(image.get() != 0);
                        image->readMetadata();
                        Exiv2::ExifData &exifData = image->exifData();
                        file->addInfo(DataInfo::DataInfoE_Magic, exifData.begin()->toString());
                        std::cout << "&&&&&&&&&&&&&&&" << exifData.begin()->toString() << std::endl;
                    }
                    else
                        {
                        file->addInfo(DataInfo::DataInfoE_Magic, "else");

                        }
                }
                catch (Exiv2::AnyError& e) {
                    const char* a = e.what();
                    file->addInfo(DataInfo::DataInfoE_Magic, "exception");
                    std::cout << "*** error exiv2 exception '" << a << "' ***" << std::endl;
                }
                catch (...) {
                    // Do nothing - just skipping file
                }

                ++numFiles;
                m_observersFilesRead.call(numFiles);
                m_observersProgress.call(numFiles);
            }
        }
    }
    else
    {
//        const char *result = magic_file(m_MagicCookie, StringHelper::toQString(FS::LongPathNameToShort(m_RootItem.path())).toStdString().c_str());
//        if (result)
//            m_RootItem.addInfo(DataInfo::DataInfoE_Magic, result);

        ++numFiles;
        m_observersFilesRead.call(numFiles);
        m_observersProgress.call(numFiles);
    }

    m_observersResultVoid.call();
}
