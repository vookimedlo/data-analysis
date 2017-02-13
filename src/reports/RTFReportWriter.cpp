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

#include <QDate>
#include <QString>


#include "../model/fs/DataItem.h"
#include "../model/fs/Directory.h"
#include "../model/fs/File.h"
#include "../util/ModelHelper.h"
#include "../util/StringHelper.h"
#include "../util/TagHelper.h"
#include "ReportSettings.h"
#include "ReportThumbnailGenerator.h"

#include "RTFReportWriter.h"

RTFReportWriter::RTFReportWriter(const ReportSettings &reportSettings, ReportThumbnailGenerator &reportThumbnailGenerator, const QString &rootPath) : ReportWriter(rootPath), m_OutputStream(), m_ReportSettings(reportSettings), m_ReportThumbnailGenerator(reportThumbnailGenerator)
{
}

RTFReportWriter::~RTFReportWriter()
{
    RTFReportWriter::close();
}

bool RTFReportWriter::open()
{
    m_OutputStream.open(StringHelper::toStdString(m_ReportSettings.getFilePath()));

    m_OutputStream << "{\\rtf1\\ansi\\ansicpg1250\\deff0\\nouicompat\\deflang1029{\\fonttbl{\\f0\\fnil\\fcharset238 Calibri;}{\\f1\\fnil\\fcharset0 Calibri;}}" << std::endl;
    m_OutputStream << "{\\*\\generator Riched20 10.0.14393}\\viewkind4\\uc1\\pard\\sa200\\sl276\\slmult1\\f0\\fs22 ";
    m_OutputStream << rtfEscapeString(m_ReportSettings.getTitle()) << std::endl;
    m_OutputStream << "\\par " << std::endl;
    m_OutputStream << rtfEscapeString(m_ReportSettings.getReference()) << std::endl;
    m_OutputStream << "\\par " << std::endl;
    m_OutputStream << rtfEscapeString(m_ReportSettings.getId()) << std::endl;
    m_OutputStream << "\\par " << std::endl;

    m_OutputStream << "\\pard\\sb500\\sa200\\sl276\\slmult1\\qj\\fs24";
    m_OutputStream << rtfEscapeString(m_ReportSettings.getPerex()) << std::endl;
    m_OutputStream << "\\par" << std::endl;

    m_OutputStream << "\\pard\\sb500\\sa200\\sl276\\slmult1 ---------------------------------------------------------------------------------------------------------------\\par" << std::endl;

    return m_OutputStream.is_open();
}

bool RTFReportWriter::write(DataItem& dataItem)
{
//    if (m_ReportSettings.isRestrictionSet(ReportSettings::RestrictionE_Directories))
//        return true;

    uint8_t tag = dataItem.isInfoValid(DataInfo::DataInfoE_Tag) ? StringHelper::toQString(dataItem.info(DataInfo::DataInfoE_Tag)).toUInt() : 0;

    if (!m_ReportSettings.isRestrictionSet(ReportSettings::RestrictionE_NotImporatant) && tag == 1)
        return true;

    if (!m_ReportSettings.isRestrictionSet(ReportSettings::RestrictionE_Imporatant) && tag == 2)
        return true;

    if (!m_ReportSettings.isRestrictionSet(ReportSettings::RestrictionE_Proof) && tag == 3)
        return true;

    if (!m_ReportSettings.isRestrictionSet(ReportSettings::RestrictionE_NoTag) && tag == 0)
        return true;

    m_OutputStream << "\\pard\\sa200\\sl276\\slmult1\\b ";
    m_OutputStream << rtfEscapeString(dataItem.name());
    m_OutputStream << " \\b0 - ";

    m_OutputStream << rtfEscapeString(TagHelper::tagToString(tag));
    m_OutputStream << "\\par " << std::endl;

    m_OutputStream << "\\i ";
    m_OutputStream << rtfEscapeString(dataItem.path());
    m_OutputStream << "\\i0 \\par " << std::endl;

    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_Size))
    {
        m_OutputStream << "Size: ";
        m_OutputStream << rtfEscapeString(QString::number(dataItem.size()));
        m_OutputStream << " (in bytes)";
        m_OutputStream << "\\line ";
    }

    m_OutputStream << "Creation Date: ";
    m_OutputStream << rtfEscapeString(QDateTime::fromTime_t(dataItem.creationTimestamp()).toString(Qt::SystemLocaleShortDate));
    m_OutputStream << "\\line ";
    m_OutputStream << "Modification Date: ";
    m_OutputStream << rtfEscapeString(QDateTime::fromTime_t(dataItem.modificationTimestamp()).toString(Qt::SystemLocaleShortDate));
    m_OutputStream << "\\line ";

    m_OutputStream << "\\pard\\sa200\\sl276\\slmult1\\qj ";
    m_OutputStream << rtfEscapeString(dataItem.info(DataInfo::DataInfoE_Analysis));
    m_OutputStream << "\\par ";

    m_OutputStream << "\\pard\\sa200\\sl276\\slmult1 ";

    std::unique_ptr<ReportThumbnail> thumbnail = m_ReportThumbnailGenerator.generate(dataItem);
    QString inlinedPicture;
    if (thumbnail->inlineThumbnail(inlinedPicture))
        m_OutputStream << StringHelper::toStdString(inlinedPicture);

    m_OutputStream << "\\par ";

    m_OutputStream << "Type: ";
    m_OutputStream << (dynamic_cast<const File *>(&dataItem) ? "File" : "Directory");
    m_OutputStream << "\\par ";
    
    if (dataItem.isInfoValid(DataInfo::DataInfoE_MD5))
    {
        m_OutputStream << "Type: ";
        m_OutputStream << dataItem.info(DataInfo::DataInfoE_MD5);
        m_OutputStream << "\\line ";
        m_OutputStream << "\\par ";
    }

    m_OutputStream << "\\pard\\sb500\\sa200\\sl276\\slmult1 ---------------------------------------------------------------------------------------------------------------\\par" << std::endl;

    return m_OutputStream.good();
}

bool RTFReportWriter::close()
{
    m_OutputStream << "}";
    m_OutputStream.close();
    return !m_OutputStream.is_open();
}

std::string RTFReportWriter::rtfEscapeString(const QString& str)
{
    return rtfEscapeString(StringHelper::toStdWString(str));
}

std::string RTFReportWriter::rtfEscapeString(const std::string& str)
{
    return rtfEscapeString(StringHelper::toStdWString(str));
}

std::string RTFReportWriter::rtfEscapeString(const std::wstring &str)
{
    QString strNew;

    foreach(QChar cr, str)
    {
        ushort code = cr.unicode();
        
        // Do not escape ascii 127bit chars
        if (code < 0x80)
        {
            strNew += cr.toLatin1();
        }
        else
        {
            strNew += "\\u";
            strNew += QString::number(code, 10);
            strNew += "?";
        }
    }

    return strNew.toStdString();
}
