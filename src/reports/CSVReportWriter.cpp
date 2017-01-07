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

#include "../fs/DataItem.h"
#include "../fs/Directory.h"
#include "../fs/File.h"
#include "../util/ModelHelper.h"
#include "../util/StringHelper.h"
#include "../util/TagHelper.h"

#include "CSVReportWriter.h"


CSVReportWriter::CSVReportWriter(std::string outputFilePath) : m_OutputStream(), m_OutputFilePath(outputFilePath), m_Separator(";")
{
}

CSVReportWriter::~CSVReportWriter()
{
    CSVReportWriter::close();
}

bool CSVReportWriter::open()
{
    m_OutputStream.open(m_OutputFilePath);
    
    // UTF BOM
    const unsigned char bom[] = { 0xEF, 0xBB, 0xBF };
    m_OutputStream.write(reinterpret_cast<const char*>(bom), sizeof(bom));

    // Header
    writeColumn(tr("Name"));
    writeColumn(tr("Tag"));
    writeColumn(tr("Type"));
    writeColumn(tr("Extension"));
    writeColumn(tr("Path"));
    writeColumn(tr("MD5 Fingerprint"));
    writeColumn(tr("SHA-1 Fingerprint"));

    m_OutputStream << std::endl;
    return m_OutputStream.is_open();
}

bool CSVReportWriter::write(DataItem& dataItem)
{
    if (!m_OutputStream.is_open())
        return false;
    
    uint8_t tag = dataItem.isInfoValid(DataInfo::DataInfoE_Tag) ? QString::fromStdString(dataItem.info(DataInfo::DataInfoE_Tag)).toUInt() : 0;

    writeColumn(dataItem.name());
    writeColumn(TagHelper::tagToString(tag));
    writeColumn(QString(dynamic_cast<const File *>(&dataItem) ? tr("File") : tr("Directory")));
    writeColumn(dataItem.extension());
    writeColumn(dataItem.path());
    writeColumn(dataItem.info(DataInfo::DataInfoE_MD5));
    writeColumn(dataItem.info(DataInfo::DataInfoE_SHA1));

    m_OutputStream << std::endl;
    return m_OutputStream.good();
}

bool CSVReportWriter::close()
{
    m_OutputStream.close();
    return !m_OutputStream.is_open();
}

bool CSVReportWriter::writeColumn(const std::string& str)
{
    m_OutputStream << str << m_Separator;
    return m_OutputStream.good();
}

bool CSVReportWriter::writeColumn(const std::wstring& str)
{
    return writeColumn(StringHelper::toStdString(str));
}

bool CSVReportWriter::writeColumn(const QString& str)
{
    return writeColumn(StringHelper::toStdWString(str));
}
