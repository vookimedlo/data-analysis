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

#include <algorithm>
#include <memory>
#include "File.h"

#include "Directory.h"

using namespace std;

Directory::Directory(const string& name, DataItem* parent) : DataItem(name, parent)
{
}


Directory::~Directory()
{
    for_each(m_files.begin(), m_files.end(), default_delete<File>());
    for_each(m_directories.begin(), m_directories.end(), default_delete<Directory>());
    m_directories.clear();
    m_files.clear();
}

bool Directory::addDirectory(Directory *directory)
{
    if (directory) {
        m_directories.push_back(directory);
        return true;
    }
    return false;
}

bool Directory::addFile(File *file)
{
    if (file) {
        m_files.push_back(file);
        return true;
    }
    return false;
}

std::vector<Directory *>& Directory::directories()
{
#if 0
    vector<reference_wrapper<Directory>> directories;
    directories.reserve(m_directories.size());
    for (auto directory : m_directories) {
        directories.push_back(*directory);
    }
    directories.shrink_to_fit();
    return directories;
#endif
    return m_directories;
}


//vector<reference_wrapper<File>> Directory::files()
std::vector<File *>&  Directory::files()
{
#if 0
    vector<reference_wrapper<File>> files;
    files.reserve(m_files.size());
    for (auto file : m_files) {
        files.push_back(*file);
    }
    files.shrink_to_fit();
    return files;
#endif
    return m_files;
}

void Directory::optimize()
{
    m_directories.shrink_to_fit();
    m_files.shrink_to_fit();
    DataItem::optimize();
}
