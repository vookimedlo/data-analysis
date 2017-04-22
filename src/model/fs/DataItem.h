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
#include "DataInfo.h"
#include "../cache/Cache.h"

class DataItem : public Cache
{
  public:
    explicit DataItem(const std::string &filename, DataItem* parent = nullptr);
    virtual ~DataItem();

    void setSize(uint64_t size);
    void setCreationTimestamp(int64_t timestamp);
    void setModificationTimestamp(int64_t timestamp);
    void setExtension(const std::string &extension);
    void addInfo(const DataInfo::DataInfoE dataInfo, std::string value);

    virtual std::string name() const;
    virtual uint64_t size() const;
    virtual int64_t creationTimestamp() const;
    virtual int64_t modificationTimestamp() const;
    virtual std::string extension() const;

    virtual DataItem* parent() const;
    virtual void optimize();

    std::string path();
    std::string info(const DataInfo::DataInfoE dataInfo) const;
    std::vector<DataInfo::DataInfoE> infos() const;
    bool isInfoValid(const DataInfo::DataInfoE dataInfo) const;    
    
private:
    DataItem* m_parent;
    DataInfo m_dataInfo;

    uint64_t m_size;
    int64_t m_creationTimestamp;
    int64_t m_modificationTimestamp;
    std::string m_name;
    std::string m_extension;
};
