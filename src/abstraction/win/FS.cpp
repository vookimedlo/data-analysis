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

#include <Windows.h>
#include <QDir>
#include <QStandardPaths>

#include "../FS.h"
#include "../../util/StringHelper.h"

const QString FS::systemRootPath()
{
    return "c:";
}

const QString FS::homeDir()
{
    return QDir::homePath();
}

const QString FS::tmpDir()
{
    return QStandardPaths::writableLocation(QStandardPaths::TempLocation);
}

std::string FS::LongPathNameToShort(std::string path)
{
    wchar_t shortPath[4098];
    DWORD bytesCount = GetShortPathName(StringHelper::toStdWString(path).c_str(), shortPath, sizeof(shortPath));
    if (!bytesCount)
        *shortPath = '\0';

    return StringHelper::toStdString(std::wstring(shortPath));
}
