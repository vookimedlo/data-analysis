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

#include "DataItem.h"

using namespace std;


DataItem::DataItem(const std::string& name, DataItem* parent) : m_parent(parent), m_dataInfo(), m_size(0), m_creationTimestamp(0), m_modificationTimestamp(0), m_name(name)
{
}


DataItem::~DataItem()
{

}


void DataItem::setSize(uint64_t size)
{
    m_size = size;
}


void DataItem::setCreationTimestamp(int64_t timestamp)
{
    m_creationTimestamp = timestamp;
}

void DataItem::setModificationTimestamp(int64_t timestamp)
{
    m_modificationTimestamp = timestamp;
}

void DataItem::setExtension(const std::string& extension)
{
    m_extension = extension;
}

void DataItem::addInfo(const DataInfo::DataInfoE dataInfo, std::string value)
{
    m_dataInfo.addInfo(dataInfo, value);
}

std::string DataItem::name() const
{
    return m_name;
}


uint64_t DataItem::size() const
{
    return m_size;
}


int64_t DataItem::creationTimestamp() const
{
    return m_creationTimestamp;
}

int64_t DataItem::modificationTimestamp() const
{
    return m_modificationTimestamp;
}

std::string DataItem::extension() const
{
    return m_extension;
}


DataItem *DataItem::parent() const
{
    return m_parent;
}

void DataItem::optimize()
{
    m_name.shrink_to_fit();
    m_extension.shrink_to_fit();
}

std::string DataItem::path()
{
    string p(name());
    DataItem *dir = this;
    while ((dir = dir->parent())) {
        if(!dir->name().empty())
            p.insert(0, dir->name() + "/");
    }

    return p;
}

std::string DataItem::info(const DataInfo::DataInfoE dataInfo)
{
    return m_dataInfo.info(dataInfo);
}

std::vector<DataInfo::DataInfoE> DataItem::infos() const
{
    return m_dataInfo.dataInfos();
}

bool DataItem::isInfoValid(const DataInfo::DataInfoE dataInfo) const
{
    return m_dataInfo.isValid(dataInfo);
}
