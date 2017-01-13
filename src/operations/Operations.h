#pragma once
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

#include <cstdint>
#include <functional>
#include <memory>
#include "../model/fs/Directory.h"
#include "../util/ConnectionManagement.h"

class Operations
{
public:
    virtual ~Operations() {};
    virtual void start(std::wstring dir) = 0;
    virtual void start() = 0;
    virtual void cancel() = 0;
    virtual bool isFinished() const = 0;

    //TODO: should be moved to the FileOperations.h
    virtual std::wstring path() const = 0;
    virtual uint32_t totalFilesCount() const = 0;

    virtual void observeProgress(std::function<void(int32_t)> observer);
    virtual void observeResult(std::function<void(std::shared_ptr<Directory>)> observer);
    virtual void observeResult(std::function<void(void)> observer);
    virtual void observeScanDir(std::function<void(std::wstring)> observer);
    virtual void observeFilesRead(std::function<void(unsigned)> observer);

protected:
    CSignal<std::function<void(int32_t)>> m_observersProgress;
    CSignal<std::function<void(std::shared_ptr<Directory>)>> m_observersResult;
    CSignal<std::function<void(void)>> m_observersResultVoid;
    CSignal<std::function<void(std::wstring)>> m_observersScanDir;
    CSignal<std::function<void(unsigned)>> m_observersFilesRead;
};
