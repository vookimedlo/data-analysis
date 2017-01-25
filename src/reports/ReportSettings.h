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
#include <QString>

class ReportSettings
{
public:
    enum RestrictionE
    {
        RestrictionE_Files = 1,
        RestrictionE_Directories = 1 << 2,
        RestrictionE_NoTag = 1 << 3,
        RestrictionE_NotImporatant = 1 << 4,
        RestrictionE_Imporatant = 1 << 5,
        RestrictionE_Proof = 1 << 6,
    };

    enum PropertiesE : uint64_t
    {
        PropertiesE_BasicDataType = 1,
        PropertiesE_ProbableDataType = 1 << 2,
        PropertiesE_Size = 1 << 3,
        PropertiesE_CreationDate = 1 << 4,
        PropertiesE_ModificationDate = 1 << 5,
        PropertiesE_MD5 = 1 << 6,
        PropertiesE_SHA1 = 1 << 7,
        PropertiesE_SHA3_512 = 1 << 8,
        PropertiesE_Preview = 1 << 9,
        PropertiesE_Analysis = 1 << 10,
        PropertiesE_Name = 1 << 11,
        PropertiesE_Path = 1 << 12,
        PropertiesE_Tag = 1 << 13,
        PropertiesE_Extension = 1 << 14,
    };

    ReportSettings();
    virtual ~ReportSettings() {}
    
    void setFilePath(const QString &filePath);
    QString getFilePath() const;

    void setTitle(const QString &title);
    QString getTitle() const;

    void setId(const QString &id);
    QString getId() const;
    
    void setReference(const QString &reference);
    QString getReference() const;
    
    void setPerex(const QString &perex);
    QString getPerex() const;

    void addRestriction(RestrictionE restriction);
    bool isRestrictionSet(RestrictionE restriction) const;
    void clearRestrictions();

    void addProperty(PropertiesE property);
    bool isPropertySet(PropertiesE property) const;
    void clearProperties();


private:
    QString m_filePath;
    QString m_title;
    QString m_id;
    QString m_reference;
    QString m_perex;
    uint32_t m_restrictions;
    uint64_t m_properties;
};
