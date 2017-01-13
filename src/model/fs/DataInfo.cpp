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

#include <algorithm>

#include "DataInfo.h"

DataInfo::DataInfo() : m_infoKeys(), m_infoValues()
{
   m_infoKeys.shrink_to_fit();
   m_infoValues.shrink_to_fit();
}

DataInfo::~DataInfo()
{
}

void DataInfo::addInfo(const DataInfoE dataInfo, std::string value)
{
    // Check if data with given dataInfo are already stored in the cache
   if (isValid(dataInfo))
   {
       // If yes ...
       uint8_t index = 0;

       for (const DataInfoE di : m_infoKeys)
       {
           if (di == dataInfo) break;
           ++index;
       }

       m_infoValues[index] = value;
   }
   else
   {
       // If no ...
       m_infoKeys.push_back(dataInfo);
       m_infoValues.push_back(value);
   }

   // Just save the memory - speed is not a big deal
   m_infoKeys.shrink_to_fit();
   m_infoValues.shrink_to_fit();
}

std::string DataInfo::info(const DataInfoE dataInfo)
{
    uint8_t index = 0;

    for (const DataInfoE di : m_infoKeys)
    {
        if (di == dataInfo) break;
        ++index;
    }

    if (index < m_infoKeys.size())
        return m_infoValues[index];

    return std::string();
}

std::vector<DataInfo::DataInfoE> DataInfo::dataInfos() const
{
    return m_infoKeys;
}

bool DataInfo::isValid(const DataInfoE dataInfo) const
{
    return m_infoKeys.end() != std::find(m_infoKeys.begin(), m_infoKeys.end(), dataInfo);
}
