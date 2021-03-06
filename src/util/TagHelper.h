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
#include <string>
#include <QCoreApplication>

class TagHelper
{
    Q_DECLARE_TR_FUNCTIONS(TagHelper);

public:
    enum TagHelperE
    {
        TagHelperE_NoTag = 0,
        TagHelperE_NotInteresting = 1,
        TagHelperE_Interesting = 2,
        TagHelperE_Proof = 3,
    };

    static std::string tagToString(uint8_t tagId);
};

