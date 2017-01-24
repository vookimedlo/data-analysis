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
#include <memory>
#include <vector>
#include <QDateTime>
#include <QDir>
#include "../../model/fs/File.h"
#include "../../model/fs/Directory.h"
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
        foreach(QFileInfo info, qdir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::AllDirs | QDir::NoSymLinks ))
        {
            if (info.isFile())
            {
                unique_ptr<File> file(new File(StringHelper::toStdString(info.fileName()), directory));
                file->setCreationTimestamp(info.created().toTime_t());
                file->setModificationTimestamp(info.lastModified().toTime_t());
                file->setExtension(StringHelper::toStdString(info.suffix())); // +1 skip dot

                file.get()->setSize(info.size());
                directory->addFile(file);
            }
            if (info.isDir())
            {
                unique_ptr<Directory> dir(new Directory(StringHelper::toStdString(info.fileName()), directory));
                dir->setCreationTimestamp(info.created().toTime_t());
                dir->setModificationTimestamp(info.lastModified().toTime_t());
                directory->addDirectory(dir);
            }
        }
    }

    return true;
}

int64_t DiskReader::convertWindowsTimeToUnixTime(_FILETIME& input)
{
    #pragma unused(input)
    throw std::runtime_error("Not implemented!");
}
