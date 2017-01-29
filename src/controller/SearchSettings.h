#pragma once
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

#include <cstdint>
#include <utility>
#include <QString>

class Directory;

class SearchSettings
{
public:
    explicit SearchSettings(Directory &searchResult);
    void enableName(const QString value);
    void enableExtension(const QString value);
    void enableSize(uint64_t lowerBound, uint64_t upperBound);
    void enableDirectory();
    void enableFile();

    Directory &getSearchResult() const;
    QString getName() const;
    QString getExtension() const;
    std::pair<uint64_t, uint64_t> getSize() const;


    bool isNameEnabled() const;
    bool isExtensionEnabled() const;
    bool isSizeEnabled() const;
    bool isDirectoryEnabled() const;
    bool isFileEnabled() const;

private:
    Directory &m_searchResult;

    bool m_nameRequested;
    QString m_name;

    bool m_extensionRequested;
    QString m_extension;
    
    bool m_sizeRequested;
    uint64_t m_sizeLowerBound;
    uint64_t m_sizeUpperBound;

    bool m_directoryRequested;
    bool m_fileRequested;
};
