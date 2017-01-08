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
#include <memory>
#include <vector>
#include <QDateTime>
#include <QDir>
#include "../../fs/File.h"
#include "../../fs/Directory.h"
#include "../../util/StringHelper.h"

#include "../DiskReader.h"

using namespace std;

struct _FILETIME
{};

bool DiskReader::readDirectoryStructure(Directory *const directory)
{
    QDir qdir(StringHelper::toQString(directory->path()));
    if (qdir.exists())
    {
        foreach(QFileInfo info, qdir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::AllDirs ))
        {
            if (info.isFile())
            {
                unique_ptr<File> file(new File(StringHelper::toStdWString(info.fileName()), directory));
                file->setCreationTimestamp(info.created().toTime_t());
                file->setModificationTimestamp(info.lastModified().toTime_t());
                file->setExtension(StringHelper::toStdWString(info.suffix())); // +1 skip dot

                file.get()->setSize(info.size());
                directory->addFile(std::move(file));
            }
            if (info.isDir())
            {
                unique_ptr<Directory> dir(new Directory(StringHelper::toStdWString(info.fileName()), directory));
                dir->setCreationTimestamp(info.created().toTime_t());
                dir->setModificationTimestamp(info.lastModified().toTime_t());
                directory->addDirectory(std::move(dir));
            }
        }
    }

    return true;
}

int64_t DiskReader::convertWindowsTimeToUnixTime(_FILETIME& input)
{
    throw std::runtime_error("Not implemented!");
}
