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


#include "ReportSettings.h"


ReportSettings::ReportSettings() : m_restrictions(0), m_properties(0)
{
}

void ReportSettings::setFilePath(const QString& filePath)
{
    m_filePath = filePath;
}

QString ReportSettings::getFilePath() const
{
    return m_filePath;
}

void ReportSettings::setTitle(const QString& title)
{
    m_title = title;
}

QString ReportSettings::getTitle() const
{
    return m_title;
}

void ReportSettings::setReference(const QString& reference)
{
    m_reference = reference;
}

QString ReportSettings::getReference() const
{
    return m_reference;
}

void ReportSettings::setPerex(const QString& perex)
{
    m_perex = perex;
}

QString ReportSettings::getPerex() const
{
    return m_perex;
}

void ReportSettings::addRestriction(RestrictionE restriction)
{
    m_restrictions |= restriction;
}

bool ReportSettings::isRestrictionSet(RestrictionE restriction) const
{
    return m_restrictions & restriction;
}

void ReportSettings::clearRestrictions()
{
    m_restrictions = 0;
}

void ReportSettings::addProperty(PropertiesE property)
{
    m_properties |= property;
}

bool ReportSettings::isPropertySet(PropertiesE property) const
{
    return m_properties & property;
}

void ReportSettings::clearProperties()
{
    m_properties = 0;
}
