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
#include <QRegularExpression>
#include <QString>

class Directory;

class SearchSettings
{
public:
    explicit SearchSettings(Directory &searchResult);
    void enableName(const QString value);
    void enableName(const QRegularExpression &value);

    void enableExtension(const QString value);
    void enableExtension(const QRegularExpression &value);

    void enableSize(uint64_t lowerBound, uint64_t upperBound);
    void enableContains(const QString value);
    void enableContains(const QRegularExpression &value);
    
    void enableTagNotInteresing();
    void enableTagInteresing();
    void enableTagProof();

    void enableDirectory();
    void enableFile();

    Directory &getSearchResult() const;
    QString getName() const;
    QRegularExpression getNameRegExp() const;
    QString getExtension() const;
    QRegularExpression getExtensionRegExp() const;
    std::pair<uint64_t, uint64_t> getSize() const;
    QString getContainedText() const;
    QRegularExpression getContainedTextRegExp() const;

    bool isNameEnabled() const;
    bool isNameRegExpEnabled() const;
    bool isExtensionEnabled() const;
    bool isExtensionRegExpEnabled() const;
    bool isSizeEnabled() const;
    bool isContainedTextEnabled() const;
    bool isContainedTextRegExpEnabled() const;
    bool isDirectoryEnabled() const;
    bool isFileEnabled() const;
    bool isNoTagEnabled() const;
    bool isNotInteresingTagEnabled() const;
    bool isInterestingTagEnabled() const;
    bool isProofTagEnabled() const;

private:
    Directory &m_searchResult;

    bool m_nameEnabled;
    QString m_name;
    bool m_nameRegExpEnabled;
    QRegularExpression m_nameRegExp;

    bool m_extensionEnabled;
    QString m_extension;
    bool m_extensionRegExpEnabled;
    QRegularExpression m_extensionRegExp;

    bool m_sizeEnabled;
    uint64_t m_sizeLowerBound;
    uint64_t m_sizeUpperBound;

    bool m_containsTextEnabled;
    QString m_containsText;
    bool m_containsTextRegExpEnabled;
    QRegularExpression m_containsTextRegExp;

    bool m_directoryEnabled;
    bool m_fileEnabled;

    bool m_tagNotInteresting;
    bool m_tagInteresting;
    bool m_tagProof;

};
