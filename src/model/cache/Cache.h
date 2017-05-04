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

class Cache
{
  public:
    Cache() :
        m_isDirty(true),
        m_isStored(false),
        m_uniqueId(0)
    {}

    virtual ~Cache() {}

    void setDirty(bool isDirty) {m_isDirty = isDirty;}
    bool isDirty() const {return m_isDirty;}

    void setStored(bool isStored) {m_isStored = isStored;}
    bool isStored() const {return m_isStored;}

    void setUniqueId(uint64_t id) {m_uniqueId = id;}
    uint64_t getUniqueId() const {return m_uniqueId;}

private:
    bool m_isDirty;
    bool m_isStored;
    uint64_t m_uniqueId;
};
