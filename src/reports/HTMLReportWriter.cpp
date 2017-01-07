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
#include <QFile>
#include "../fs/DataItem.h"
#include "../fs/Directory.h"
#include "../fs/File.h"
#include "../util/StringHelper.h"
#include "../util/TagHelper.h"

#include "HTMLReportWriter.h"


HTMLReportWriter::HTMLReportWriter(std::string outputFilePath) : m_OutputStream(), m_OutputFilePath(outputFilePath)
{
}

HTMLReportWriter::~HTMLReportWriter()
{
    HTMLReportWriter::close();
}

bool HTMLReportWriter::open()
{
    m_OutputStream.open(m_OutputFilePath);

    QString title(tr("HTML Report"));

    // UTF BOM
    const unsigned char bom[] = { 0xEF, 0xBB, 0xBF };
    m_OutputStream.write(reinterpret_cast<const char*>(bom), sizeof(bom));

    write("<!DOCTYPE html>");
    write("<html>");
    write("<head>");

    QFile css(":/css/dataanalysis.css");
    css.open(QIODevice::ReadOnly | QIODevice::Text);

    write("<style>");
    write(QString::fromStdString(css.readAll().toStdString()));
    write("</style>");

    write(addPreparedStringInTag(prepareString(title), "title"));
    write("</head>");

    write("<body>");

    return m_OutputStream.is_open();
}

bool HTMLReportWriter::write(DataItem& dataItem)
{
    if (!m_OutputStream.is_open())
        return false;

    uint8_t tag = dataItem.isInfoValid(DataInfo::DataInfoE_Tag) ? StringHelper::toQString(dataItem.info(DataInfo::DataInfoE_Tag)).toUInt() : 0;

    write(addPreparedStringInTag(prepareString(StringHelper::toQString(dataItem.name())), "span", "name"));
    write(" - ");
    write(addPreparedStringInTag(prepareString(StringHelper::toQString(TagHelper::tagToString(tag))), "span", "tag"));
    write("<br />");
    write(addPreparedStringInTag(prepareString(StringHelper::toQString(dataItem.path())), "span", "path"));

    write("<br />");
    write(addPreparedStringInTag(prepareString(tr("Basic data type")), "span", "key"));
    write(addPreparedStringInTag(prepareString(QString(dynamic_cast<const File *>(&dataItem) ? tr("File") : tr("Directory"))), "span", "value"));
    write("<br />");
    write(addPreparedStringInTag(prepareString(tr("Size in bytes")), "span", "key"));
    write(addPreparedStringInTag(prepareString(QString::number(dataItem.size())), "span", "value"));
    write("<br />");
    write(addPreparedStringInTag(prepareString(tr("Creation date")), "span", "key"));
    write(addPreparedStringInTag(prepareString(QDateTime::fromTime_t(dataItem.creationTimestamp()).toString(Qt::SystemLocaleShortDate)), "span", "value"));
    write("<br />");
    write(addPreparedStringInTag(prepareString(tr("Modification date")), "span", "key"));
    write(addPreparedStringInTag(prepareString(QDateTime::fromTime_t(dataItem.modificationTimestamp()).toString(Qt::SystemLocaleShortDate)), "span", "value"));
    write("<br />");
    
    if (dataItem.isInfoValid(DataInfo::DataInfoE_SHA1))
    {
        write(addPreparedStringInTag(prepareString(tr("SHA-1 fingerprint")), "span", "key"));
        write(addPreparedStringInTag(prepareString(StringHelper::toQString(dataItem.info(DataInfo::DataInfoE_SHA1))), "span", "value"));
        write("<br />");
    }

    if (dataItem.isInfoValid(DataInfo::DataInfoE_Magic))
    {
        write(addPreparedStringInTag(prepareString(tr("Probable data type")), "span", "key"));
        write(addPreparedStringInTag(prepareString(StringHelper::toQString(dataItem.info(DataInfo::DataInfoE_Magic))), "span", "value"));
        write("<br />");
    }

    if (dataItem.isInfoValid(DataInfo::DataInfoE_Analysis) && !dataItem.info(DataInfo::DataInfoE_Analysis).empty())
    {
        write(addPreparedStringInTag(prepareString(tr("Analysis")), "span", "analysis-title"));
        write(addPreparedStringInTag(prepareString(StringHelper::toQString(dataItem.info(DataInfo::DataInfoE_Analysis))), "div", "analysis"));
        write("<br />");
    }

    write("<hr /><br />");

    m_OutputStream << std::endl;
    return m_OutputStream.good();
}

bool HTMLReportWriter::close()
{
    write("</body>");
    write("</html>");

    m_OutputStream.close();
    return !m_OutputStream.is_open();
}

QString HTMLReportWriter::prepareString(const QString& str)
{
    return str.toHtmlEscaped();
}

QString HTMLReportWriter::addPreparedStringInTag(const QString& str, const QString& tag, const QString& cssClass)
{
    QString startTag( "<" + tag + (!cssClass.isEmpty() ? " class=\"" + cssClass + "\">" : ">"));
    return startTag + str + "</" + tag + ">";
}


bool HTMLReportWriter::write(const QString& str)
{
    m_OutputStream << StringHelper::toStdString(str) << std::endl;
    return m_OutputStream.good();
}
