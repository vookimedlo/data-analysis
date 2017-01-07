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

#include "StringHelper.h"

std::string StringHelper::toStdString(const QString& string)
{
    return string.toStdString();
}

std::string StringHelper::toStdString(const std::wstring& string)
{
#ifdef _MSC_VER
    return QString::fromUtf16(reinterpret_cast<const ushort*>(string.c_str())).toStdString();
#else
    return QString::fromStdWString(string).toStdString();
#endif
}

std::wstring StringHelper::toStdWString(const QString& string)
{
#ifdef _MSC_VER
    return std::wstring(reinterpret_cast<const wchar_t*>(string.utf16()));
#else
    return string.toStdWString();
#endif
}

std::wstring StringHelper::toStdWString(const std::string& string)
{
#ifdef _MSC_VER
    return std::wstring(reinterpret_cast<const wchar_t*>(QString::fromStdString(string).utf16()));
#else
    return QString::fromStdString(string).toStdWString();
#endif
}

QString StringHelper::toQString(const std::string& string)
{
    return QString::fromStdString(string);
}

QString StringHelper::toQString(const std::wstring& string)
{
#ifdef _MSC_VER
    return QString::fromUtf16(reinterpret_cast<const ushort*>(string.c_str()));
#else
    return QString::fromStdWString(string);
#endif
}
