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

#include "Operations.h"


void Operations::observeProgress(std::function<void(int32_t)> observer)
{
    m_observersProgress.connect(observer);
}

void Operations::observeResult(std::function<void(std::shared_ptr<Directory>)> observer)
{
    m_observersResult.connect(observer);
}

void Operations::observeResult(std::function<void()> observer)
{
    m_observersResultVoid.connect(observer);
}

void Operations::observeScanDir(std::function<void(std::wstring)> observer)
{
    m_observersScanDir.connect(observer);
}

void Operations::observeFilesRead(std::function<void(unsigned)> observer)
{
    m_observersFilesRead.connect(observer);
}

