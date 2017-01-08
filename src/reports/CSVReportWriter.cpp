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

#include <QDateTime>
#include "ReportSettings.h"
#include "../fs/DataItem.h"
#include "../fs/Directory.h"
#include "../fs/File.h"
#include "../util/ModelHelper.h"
#include "../util/StringHelper.h"
#include "../util/TagHelper.h"

#include "CSVReportWriter.h"


CSVReportWriter::CSVReportWriter(const ReportSettings &reportSettings) : m_OutputStream(), m_Separator(";"), m_ReportSettings(reportSettings)
{
}

CSVReportWriter::~CSVReportWriter()
{
    CSVReportWriter::close();
}

bool CSVReportWriter::open()
{
    m_OutputStream.open(StringHelper::toStdString(m_ReportSettings.getFilePath()));
    
    // UTF BOM
    const unsigned char bom[] = { 0xEF, 0xBB, 0xBF };
    m_OutputStream.write(reinterpret_cast<const char*>(bom), sizeof(bom));

    // Header
    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_Name))
        writeColumn(tr("Name"));
    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_Tag))
        writeColumn(tr("Tag"));
    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_BasicDataType))
        writeColumn(tr("Type"));
//    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_Extension))
//    writeColumn(tr("Extension"));
    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_Path))
        writeColumn(tr("Path"));
    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_MD5))
        writeColumn(tr("MD5 Fingerprint"));
    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_SHA1))
        writeColumn(tr("SHA-1 Fingerprint"));

    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_Size))
        writeColumn(tr("Size in bytes"));
    
    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_CreationDate))
        writeColumn(tr("Creation date"));

    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_ModificationDate))
        writeColumn(tr("Modification date"));

    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_SHA3_512))
        writeColumn(tr("SHA3-512 fingerprint"));

    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_ProbableDataType))
        writeColumn(tr("Probable data type"));

    m_OutputStream << std::endl;
    return m_OutputStream.is_open();
}

bool CSVReportWriter::write(DataItem& dataItem)
{
    if (!m_OutputStream.is_open())
        return false;

    if (!m_ReportSettings.isRestrictionSet(ReportSettings::RestrictionE_Files) && dynamic_cast<const File *>(&dataItem))
        return true;

    if (!m_ReportSettings.isRestrictionSet(ReportSettings::RestrictionE_Directories) && dynamic_cast<const Directory *>(&dataItem))
        return true;


    uint8_t tag = dataItem.isInfoValid(DataInfo::DataInfoE_Tag) ? StringHelper::toQString(dataItem.info(DataInfo::DataInfoE_Tag)).toUInt() : 0;

    if (!m_ReportSettings.isRestrictionSet(ReportSettings::RestrictionE_NotImporatant) && tag == 1)
        return true;

    if (!m_ReportSettings.isRestrictionSet(ReportSettings::RestrictionE_Imporatant) && tag == 2)
        return true;

    if (!m_ReportSettings.isRestrictionSet(ReportSettings::RestrictionE_Proof) && tag == 3)
        return true;

    if (!m_ReportSettings.isRestrictionSet(ReportSettings::RestrictionE_NoTag) && tag == 0)
        return true;

    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_Name))
        writeColumn(dataItem.name());
    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_Tag))
        writeColumn(TagHelper::tagToString(tag));
    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_BasicDataType))
        writeColumn(QString(dynamic_cast<const File *>(&dataItem) ? tr("File") : tr("Directory")));
    //    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_Extension))
    //    writeColumn(dataItem.extension());
    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_Path))
        writeColumn(dataItem.path());
    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_MD5))
        writeColumn(dataItem.info(DataInfo::DataInfoE_MD5));
    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_SHA1))
        writeColumn(dataItem.info(DataInfo::DataInfoE_SHA1));
    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_Size))
        writeColumn(QString::number(dataItem.size()));
    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_CreationDate))
        writeColumn(QDateTime::fromTime_t(dataItem.creationTimestamp()).toString(Qt::SystemLocaleShortDate));
    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_ModificationDate))
        writeColumn(QDateTime::fromTime_t(dataItem.modificationTimestamp()).toString(Qt::SystemLocaleShortDate));
    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_SHA3_512))
        writeColumn(dataItem.info(DataInfo::DataInfoE_SHA3_512));
    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_ProbableDataType))
        writeColumn(dataItem.info(DataInfo::DataInfoE_Magic));

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
    QString escapedString = "\"" + StringHelper::toQString(str).replace(QChar('"'), QString("\"\"")) + "\"";
    m_OutputStream << StringHelper::toStdString(escapedString) << m_Separator;
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
