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

#include <QFile>
#include <QString>
#include "../model/fs/DataItem.h"
#include "../model/fs/File.h"
#include "../util/compiler.h"
#include "../util/StringHelper.h"

#include "CopyReportThumbnail.h"


CopyReportThumbnail::CopyReportThumbnail(const DataItem& item) : m_item(item)
{
}

bool CopyReportThumbnail::inlineThumbnail(QString& outInlinedData)
{
    UNUSED_VARIABLE(outInlinedData);
    return false;
}

bool CopyReportThumbnail::write(const QString& pathName)
{
    const File *file = dynamic_cast<const File *>(&m_item);
    if (!file)
        return false;

    return QFile::copy(StringHelper::toQString(const_cast<DataItem&>(m_item).path()), pathName);
}
