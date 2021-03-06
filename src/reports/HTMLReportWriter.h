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
#include <fstream>
#include <string>
#include <QCoreApplication>
#include "ReportWriter.h"

class ReportThumbnailGenerator;
class ReportSettings;
class QString;

class HTMLReportWriter : public ReportWriter
{
    Q_DECLARE_TR_FUNCTIONS(HTMLReportWriter);

public:
    explicit HTMLReportWriter(const ReportSettings &reportSettings, ReportThumbnailGenerator &reportThumbnailGenerator, const QString &rootPath);
    ~HTMLReportWriter() override;
    bool open() override;
    bool write(DataItem &dataiItem) override;
    bool close() override;

protected:
    QString prepareString(const QString& str);
    QString addPreparedStringInTag(const QString& str, const QString& tag, const QString& cssClass = QString());
    bool write(const QString& str);

private:
    std::ofstream m_OutputStream;
    const ReportSettings &m_ReportSettings;
    ReportThumbnailGenerator &m_ReportThumbnailGenerator;
    uint64_t m_UniqueNumber;
};
