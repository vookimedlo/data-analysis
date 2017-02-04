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

SearchSettings::SearchSettings(Directory& searchResult) : m_searchResult(searchResult),
                                                          m_nameEnabled(false),
                                                          m_nameRegExpEnabled(false),
                                                          m_extensionEnabled(false),
                                                          m_extensionRegExpEnabled(false),
                                                          m_sizeEnabled(false),
                                                          m_sizeLowerBound(0),
                                                          m_sizeUpperBound(0),
                                                          m_containsTextEnabled(false),
                                                          m_containsTextRegExpEnabled(false),
                                                          m_directoryEnabled(false),
                                                          m_fileEnabled(false),
                                                          m_tagNotInteresting(false),
                                                          m_tagInteresting(false),
                                                          m_tagProof(false)
{

}

void SearchSettings::enableName(const QString value)
{
    m_name = value;
    m_nameEnabled = true;
    m_nameRegExpEnabled = false;
}

void SearchSettings::enableName(const QRegularExpression& value)
{
    m_nameRegExp = value;
    m_nameEnabled = false;
    m_nameRegExpEnabled = true;
}

void SearchSettings::enableExtension(const QString value)
{
    m_extension = value;
    m_extensionEnabled = true;
    m_extensionRegExpEnabled = false;
}

void SearchSettings::enableExtension(const QRegularExpression& value)
{
    m_extensionRegExp = value;
    m_extensionEnabled = false;
    m_extensionRegExpEnabled = false;
}

void SearchSettings::enableSize(uint64_t lowerBound, uint64_t upperBound)
{
    m_sizeLowerBound = lowerBound;
    m_sizeUpperBound = upperBound;
    m_sizeEnabled = true;
}

void SearchSettings::enableContains(const QString value)
{
    m_containsText = value;
    m_containsTextEnabled = true;
    m_containsTextRegExpEnabled = false;
}

void SearchSettings::enableContains(const QRegularExpression& value)
{
    m_containsTextRegExp = value;
    m_containsTextEnabled = false;
    m_containsTextRegExpEnabled = true;
}

void SearchSettings::enableTagNotInteresing()
{
    m_tagNotInteresting = true;
}

void SearchSettings::enableTagInteresing()
{
    m_tagInteresting = true;
}

void SearchSettings::enableTagProof()
{
    m_tagProof = true;
}

void SearchSettings::enableDirectory()
{
    m_directoryEnabled = true;
}

void SearchSettings::enableFile()
{
    m_fileEnabled = true;
}

Directory& SearchSettings::getSearchResult() const
{
    return m_searchResult;
}

QString SearchSettings::getName() const
{
    return m_name;
}

QRegularExpression SearchSettings::getNameRegExp() const
{
    return m_nameRegExp;
}

QString SearchSettings::getExtension() const
{
    return m_extension;
}

QRegularExpression SearchSettings::getExtensionRegExp() const
{
    return m_extensionRegExp;
}

std::pair<uint64_t, uint64_t> SearchSettings::getSize() const
{
    return std::pair<uint64_t, uint64_t>(m_sizeLowerBound, m_sizeUpperBound);
}

QString SearchSettings::getContainedText() const
{
    return m_containsText;
}

QRegularExpression SearchSettings::getContainedTextRegExp() const
{
    return m_containsTextRegExp;
}

bool SearchSettings::isNameEnabled() const
{
    return m_nameEnabled;
}

bool SearchSettings::isNameRegExpEnabled() const
{
    return m_nameRegExpEnabled;
}

bool SearchSettings::isExtensionEnabled() const
{
    return m_extensionEnabled;
}

bool SearchSettings::isExtensionRegExpEnabled() const
{
    return m_extensionRegExpEnabled;
}

bool SearchSettings::isSizeEnabled() const
{
    return m_sizeEnabled;
}

bool SearchSettings::isContainedTextEnabled() const
{
    return m_containsTextEnabled;
}

bool SearchSettings::isContainedTextRegExpEnabled() const
{
    return m_containsTextRegExpEnabled;
}

bool SearchSettings::isDirectoryEnabled() const
{
    return m_directoryEnabled;
}

bool SearchSettings::isFileEnabled() const
{
    return m_fileEnabled;
}

bool SearchSettings::isNoTagEnabled() const
{
    return !m_tagProof && !m_tagInteresting && !m_tagNotInteresting;
}

bool SearchSettings::isNotInteresingTagEnabled() const
{
    return m_tagNotInteresting;
}

bool SearchSettings::isInterestingTagEnabled() const
{
    return m_tagInteresting;
}

bool SearchSettings::isProofTagEnabled() const
{
    return m_tagProof;
}
