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

#include <fstream>
#include <string>
#include <QCoreApplication>

#include "ReportWriter.h"

class RTFReportWriter : public ReportWriter
{
    Q_DECLARE_TR_FUNCTIONS(RTFReportWriter);

public:
    explicit RTFReportWriter(std::string outputFilePath, const QString &rootPath);
    ~RTFReportWriter() override;
    bool open() override;
    bool write(DataItem &dataiItem) override;
    bool close() override;

protected:
    static std::string rtfEscapeString(const QString      &str);
    static std::string rtfEscapeString(const std::string  &str);
    static std::string rtfEscapeString(const std::wstring &str);

private:
    std::ofstream m_OutputStream;
    std::string m_OutputFilePath;
};
