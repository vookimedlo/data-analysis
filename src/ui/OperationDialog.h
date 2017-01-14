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
#include "ui_OperationDialog.h"
#include "../operations/Operations.h"

class OperationDialog : public QDialog, Ui::OperationDialog
{
    Q_OBJECT

public:
    enum ModeE : uint8_t {
        ModeE_NoDirSelect,
        ModeE_DirSelect,
    };
public:
    OperationDialog(Operations &operations, ModeE mode, QWidget *parent);
    void setTitle(const QString &title);

    void onScannedDirChangeNotification(std::wstring name);
    void onResultReadyNotification(std::shared_ptr<Directory> directory);
    void onResultReadyNotification();
    std::shared_ptr<Directory> getResult() const;

public Q_SLOTS:
    void onUpdateProgress(int value);
    void onScanningFinished();
    void onStart();
    void onCancel();
    void onDirSelect();
    void onFilesRead(unsigned value);
    void onResultReady();
    void onScanDirChange(QString value);
    void reject() override; 

signals:
    void filesRead(unsigned value);
    void scannedDirChanged(QString value);
    void progressChanged(int value);
    void resultReady();

private:
    bool m_IsScanningFinished;
    ModeE m_Mode;
    Operations &m_Operations;
    std::shared_ptr<Directory> m_result;
};
