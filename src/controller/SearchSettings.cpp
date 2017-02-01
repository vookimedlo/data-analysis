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

#include "SearchSettings.h"

SearchSettings::SearchSettings(Directory& searchResult) : m_searchResult(searchResult), m_nameRequested(false), m_extensionRequested(false), m_sizeRequested(false), m_sizeLowerBound(0), m_sizeUpperBound(0), m_containsRequested(false), m_contains(), m_directoryRequested(false), m_fileRequested(false)
{

}

void SearchSettings::enableName(const QString value)
{
    m_name = value;
    m_nameRequested = true;
}

void SearchSettings::enableExtension(const QString value)
{
    m_extension = value;
    m_extensionRequested = true;
}

void SearchSettings::enableSize(uint64_t lowerBound, uint64_t upperBound)
{
    m_sizeLowerBound = lowerBound;
    m_sizeUpperBound = upperBound;
    m_sizeRequested = true;
}

void SearchSettings::enableContains(const QString value)
{
    m_contains = value;
    m_containsRequested = true;
}

void SearchSettings::enableDirectory()
{
    m_directoryRequested = true;
}

void SearchSettings::enableFile()
{
    m_fileRequested = true;
}

Directory& SearchSettings::getSearchResult() const
{
    return m_searchResult;
}

QString SearchSettings::getName() const
{
    return m_name;
}

QString SearchSettings::getExtension() const
{
    return m_extension;
}

std::pair<uint64_t, uint64_t> SearchSettings::getSize() const
{
    return std::pair<uint64_t, uint64_t>(m_sizeLowerBound, m_sizeUpperBound);
}

QString SearchSettings::getContains() const
{
    return m_contains;
}

bool SearchSettings::isNameEnabled() const
{
    return m_nameRequested;
}

bool SearchSettings::isExtensionEnabled() const
{
    return m_extensionRequested;
}

bool SearchSettings::isSizeEnabled() const
{
    return m_sizeRequested;
}

bool SearchSettings::isContainedEnabled() const
{
    return m_containsRequested;
}

bool SearchSettings::isDirectoryEnabled() const
{
    return m_directoryRequested;
}

bool SearchSettings::isFileEnabled() const
{
    return m_fileRequested;
}
