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

#include <future>
#include "../fs/DataInfo.h"
#include "../operations/Operations.h"


/// Forward declarations
class DataItem;
class QCryptographicHash;

class HashOperation : public Operations
{
public:
    HashOperation(QCryptographicHash &hash, DataInfo::DataInfoE info, DataItem &rootItem);
    void start(std::wstring dir) override;
    void start() override;
    void cancel() override;
    bool isFinished() const override;
    std::wstring path() const override;
    uint32_t totalFilesCount() const override;

protected:
    void startOperation();
    static bool computeHash(QCryptographicHash& hash, std::wstring path);

private:
    std::future<void> m_asyncScanWorker;
    bool m_cancelWorkerActivity;
    DataItem &m_RootItem;
    QCryptographicHash &m_hash;
    DataInfo::DataInfoE m_info;
};