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

#include <queue>
#include "../model/fs/Directory.h"

#include "ModelHelper.h"

uint32_t ModelHelper::filesCount(DataItem& rootItem)
{
    uint32_t numFiles = 0;
    std::queue<DataItem *> q;

    if (!dynamic_cast<Directory*>(&rootItem))
        return 1; /* This is a file so return 1 */

    q.push(&rootItem);

    while (!q.empty()) 
    {
        auto d = q.front();
        q.pop();

        for (Directory *directory : dynamic_cast<Directory *>(d)->directories())
        {
            q.push(directory);
        }

        numFiles += dynamic_cast<Directory *>(d)->files().size();
    }

    return numFiles;
}
