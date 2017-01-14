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
#include <string>
#include <vector>

class DataInfo
{
  public:
	  enum DataInfoE : uint8_t {
          DataInfoE_Analysis,
          DataInfoE_Tag,
          DataInfoE_MD5,
          DataInfoE_Magic,
          DataInfoE_SHA1,
          DataInfoE_SHA3_512,
	  };
 
    DataInfo();
    virtual ~DataInfo();

    void addInfo(const DataInfoE dataInfo, std::string value);
    std::string info(const DataInfoE dataInfo);
    std::vector<DataInfoE> dataInfos() const;
    bool isValid(const DataInfoE dataInfo) const;

  private:
    std::vector<DataInfoE> m_infoKeys;
    std::vector<std::string> m_infoValues;

    // Hash implementation uses much more data than just the vectors
    // Hash is not needed unless there could be many possible DataInfo stored ...
    //std::unordered_map<DataInfoE, std::string> info;
};

