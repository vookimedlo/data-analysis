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
#include <Shlwapi.h> // library Shlwapi.lib

#include <memory>
#include <vector>
#include "../../model/fs/File.h"
#include "../../model/fs/Directory.h"

#include "../DiskReader.h"

using namespace std;


#define TICKS_PER_SECOND 10000000
#define EPOCH_DIFFERENCE 11644473600LL

bool DiskReader::readDirectoryStructure(Directory *const directory)
{
    wstring search_path = directory->path() + wstring(L"/*.*");
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {        
            wstring filename(fd.cFileName);
            // Directories
            if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                // Skip current dir and up dir
                if (!filename.compare(L".") || !filename.compare(L".."))
                    continue;

                unique_ptr<Directory> dir(new Directory(wstring(fd.cFileName), directory));               
                dir->setCreationTimestamp(convertWindowsTimeToUnixTime(fd.ftCreationTime));
                dir->setModificationTimestamp(convertWindowsTimeToUnixTime(fd.ftLastWriteTime));
                directory->addDirectory(dir);

            }
            // Files
            else {
                ULARGE_INTEGER ul;
                ul.HighPart = fd.nFileSizeHigh;
                ul.LowPart = fd.nFileSizeLow;

                unique_ptr<File> file(new File(wstring(fd.cFileName), directory));
                file->setCreationTimestamp(convertWindowsTimeToUnixTime(fd.ftCreationTime));
                file->setModificationTimestamp(convertWindowsTimeToUnixTime(fd.ftLastWriteTime));
                
                PTSTR extension = PathFindExtension(fd.cFileName);
                if (*extension != '\0') {
                    file->setExtension(extension + 1); // +1 skip dot
                }

                file.get()->setSize(ul.QuadPart);
                directory->addFile(file);
            }
        } while (::FindNextFile(hFind, &fd));
        ::FindClose(hFind);
    }

    return true;
}

int64_t DiskReader::convertWindowsTimeToUnixTime(_FILETIME& input)
{
    int64_t temp;
    LARGE_INTEGER li;
    li.HighPart = input.dwHighDateTime;
    li.LowPart = input.dwLowDateTime;

    temp = li.QuadPart / TICKS_PER_SECOND; // convert from 100ns intervals to seconds;
    temp = temp - EPOCH_DIFFERENCE;  // subtract number of seconds between epochs
    return temp;
}
