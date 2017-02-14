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
    m_OutputStream << "{\\colortbl ;\\red240\\green240\\blue240;}" << std::endl;
    m_OutputStream << "{\\*\\generator Riched20 10.0.14393}\\viewkind4\\uc1\\pard\\sa200\\sl276\\slmult1\\b\\f0\\fs40" << std::endl;
    m_OutputStream << "{" << std::endl;
    m_OutputStream << rtfEscapeString(m_ReportSettings.getTitle()) << std::endl;
    m_OutputStream << "}\\par\\fs22" << std::endl;
    m_OutputStream << "{" << std::endl;
    m_OutputStream << rtfEscapeString(m_ReportSettings.getReference()) << std::endl;
    m_OutputStream << "  -  " << std::endl;
    m_OutputStream << rtfEscapeString(m_ReportSettings.getId()) << std::endl;
    m_OutputStream << "}\\par " << std::endl;

    m_OutputStream << "\\pard\\li142\\ri70\\highlight1\\b1{" << rtfEscapeString(tr("Final report: ")) << "}\\b0\\par" << std::endl;
    m_OutputStream << "{" << std::endl;
    m_OutputStream << "\\trowd\\trgaph144" << std::endl;
    m_OutputStream << "\\clbrdrt\\brdrdot" << std::endl;
    m_OutputStream << "\\clbrdrl\\brdrdot" << std::endl;
    m_OutputStream << "\\clbrdrb\\brdrdot" << std::endl;
    m_OutputStream << "\\clbrdrr\\brdrdot" << std::endl;
    m_OutputStream << "\\clshdng200\\cellx9000" << std::endl;
    m_OutputStream << rtfEscapeString(m_ReportSettings.getPerex()) << std::endl;
    m_OutputStream << "\\intbl\\cell" << std::endl;
    m_OutputStream << "\\row\\pard\\par" << std::endl;
    m_OutputStream << "}\\par" << std::endl;

    m_OutputStream << "\\pard\\tqr\\tlul\\tx9300\\tab" << std::endl;
    
    return m_OutputStream.is_open();
}

bool RTFReportWriter::write(DataItem& dataItem)
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

    m_OutputStream << "\\par\\pard\\tqr\\tleq\\tx8200\\b\\fs24" << std::endl << "{";
    m_OutputStream << rtfEscapeString(dataItem.name());
    m_OutputStream << "}" << std::endl;
    m_OutputStream << "\\b0\\fs22 ";
    if (!tag == 0)
    {
        m_OutputStream << " - ";
        m_OutputStream << rtfEscapeString(TagHelper::tagToString(tag));
    }
    m_OutputStream << "\\par " << std::endl;

    m_OutputStream << "\\pard\\sa200\\sl240\\slmult1\\tqr\\tleq\\tx8200\\i ";
    m_OutputStream << rtfEscapeString(ModelHelper::removePartOfPath(dataItem, m_rootPath));
    m_OutputStream << "\\i0\\par " << std::endl;
    

    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_BasicDataType))
    {
        m_OutputStream << "\\pard\\b{Basic data type:}\\b0\\tx2880\\tab{";
        m_OutputStream << rtfEscapeString(QString(dynamic_cast<const File *>(&dataItem) ? "File" : "Directory"));
        m_OutputStream << "}\\par ";
    }

    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_Extension) && dynamic_cast<const File *>(&dataItem))
    {
        m_OutputStream << "\\pard\\b{Extension:}\\b0\\tx2880\\tab{";
        m_OutputStream << rtfEscapeString(dataItem.extension());
        m_OutputStream << "}\\par ";
    }

    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_Size) && dynamic_cast<const File *>(&dataItem))
    {
        m_OutputStream << "\\pard\\b{Size in bytes:}\\b0\\tx2880\\tab{";
        m_OutputStream << rtfEscapeString(QString::number(dataItem.size()));
        m_OutputStream << "}\\par ";
    }

    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_CreationDate))
    {
        m_OutputStream << "\\pard\\b{Creation Date:}\\b0\\tx2880\\tab{";
        m_OutputStream << rtfEscapeString(QDateTime::fromTime_t(dataItem.creationTimestamp()).toString(Qt::SystemLocaleShortDate));
        m_OutputStream << "}\\par ";
    }

    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_ModificationDate))
    {
        m_OutputStream << "\\pard\\b{Modification Date:}\\b0\\tx2880\\tab{";
        m_OutputStream << rtfEscapeString(QDateTime::fromTime_t(dataItem.modificationTimestamp()).toString(Qt::SystemLocaleShortDate));
        m_OutputStream << "}\\par ";
    }

    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_MD5) 
        && dynamic_cast<const File *>(&dataItem)
        && dataItem.isInfoValid(DataInfo::DataInfoE_MD5))
    {
        m_OutputStream << "\\pard\\b{MD5 fingerprint:}\\b0\\tx2880\\tab{";
        m_OutputStream << rtfEscapeString(dataItem.info(DataInfo::DataInfoE_MD5));
        m_OutputStream << "}\\par ";
    }

    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_SHA1) 
        && dynamic_cast<const File *>(&dataItem)
        && dataItem.isInfoValid(DataInfo::DataInfoE_SHA1))
    {
        m_OutputStream << "\\pard\\b{SHA-1 fingerprint:}\\b0\\tx2880\\tab{";
        m_OutputStream << rtfEscapeString(dataItem.info(DataInfo::DataInfoE_SHA1));
        m_OutputStream << "}\\par ";
    }

    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_SHA3_512) 
        && dynamic_cast<const File *>(&dataItem)
        && dataItem.isInfoValid(DataInfo::DataInfoE_SHA3_512))
    {
        m_OutputStream << "\\pard\\b{SHA3_512 fingerprint:}\\b0\\par{";
        m_OutputStream << rtfEscapeString(dataItem.info(DataInfo::DataInfoE_SHA3_512));
        m_OutputStream << "}\\par ";
    }

    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_ProbableDataType) 
        && dynamic_cast<const File *>(&dataItem)
        && dataItem.isInfoValid(DataInfo::DataInfoE_Magic))
    {
        m_OutputStream << "\\pard\\b{Probable data type:}\\b0\\par{";
        m_OutputStream << rtfEscapeString(dataItem.info(DataInfo::DataInfoE_Magic));
        m_OutputStream << "}\\par ";
    }

    if (m_ReportSettings.isPropertySet(ReportSettings::PropertiesE_Analysis) 
        && dataItem.isInfoValid(DataInfo::DataInfoE_Analysis)
        && !dataItem.info(DataInfo::DataInfoE_Analysis).empty())
    {
        m_OutputStream << "\\pard\\sa200\\sl276\\slmult1\\b{Analysis:}\\b0\\par{";
        m_OutputStream << "\\trowd\\trgaph144" << std::endl;
        m_OutputStream << "\\clbrdrt\\brdrdot" << std::endl;
        m_OutputStream << "\\clbrdrl\\brdrdot" << std::endl;
        m_OutputStream << "\\clbrdrb\\brdrdot" << std::endl;
        m_OutputStream << "\\clbrdrr\\brdrdot" << std::endl;
        m_OutputStream << "\\clshdng200\\cellx9000" << std::endl;
        m_OutputStream << rtfEscapeString(dataItem.info(DataInfo::DataInfoE_Analysis));
        m_OutputStream << "\\intbl\\cell" << std::endl;
        m_OutputStream << "\\row\\pard\\par" << std::endl;
        m_OutputStream << "}\\par ";
    }

    std::unique_ptr<ReportThumbnail> thumbnail = m_ReportThumbnailGenerator.generate(dataItem);
    QString inlinedPicture;
    if (thumbnail->inlineThumbnail(inlinedPicture))
        m_OutputStream << StringHelper::toStdString(inlinedPicture);
    m_OutputStream << "\\par ";

    m_OutputStream << "\\pard\\tqr\\tlul\\tx9300\\tab\\par" << std::endl;

    return m_OutputStream.good();
}

bool RTFReportWriter::close()
{
    m_OutputStream << "\\pard\\tqr\\tlul\\tx9300\\tab\\par" << std::endl;
    m_OutputStream << "\\pard\\qc\\tqr\\tleq\\i\\fs20 " << rtfEscapeString(tr("Generated on "));
                
    m_OutputStream << rtfEscapeString(QDate::currentDate().toString("yyyy-MM-dd"));
    m_OutputStream << rtfEscapeString(tr(" by the DataAnalyzer")) << " \\par";
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
