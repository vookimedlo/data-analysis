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

#include <QFileDialog>
#include "../util/StringHelper.h"

#include "OperationDialog.h"

OperationDialog::OperationDialog(Operations &operations, ModeE mode, QWidget* parent) : QDialog(parent), m_IsScanningFinished(false), m_Mode(mode), m_Operations(operations)
{
    setupUi(this);
    m_Operations.observeProgress(std::bind(&OperationDialog::progressChanged, this, std::placeholders::_1));
    m_Operations.observeResult([&](std::shared_ptr<Directory> dir) {onResultReadyNotification(dir); });
    m_Operations.observeResult([&]() {onResultReadyNotification(); });
    m_Operations.observeScanDir(std::bind(&OperationDialog::onScannedDirChangeNotification, this, std::placeholders::_1));
    m_Operations.observeFilesRead(std::bind(&OperationDialog::filesRead, this, std::placeholders::_1));

    if (m_Mode == ModeE_NoDirSelect)
    {
        labelScanFSValue->setText(operations.path());
        dirSelectButton->setVisible(false);
    }
}

void OperationDialog::setTitle(const QString& title)
{
    setWindowTitle(title);
}

void OperationDialog::setPath(const QString &value)
{
    labelScanFSValue->setText(value);
}

void OperationDialog::onScannedDirChangeNotification(QString name)
{
    scannedDirChanged(name);
}

void OperationDialog::onResultReadyNotification(std::shared_ptr<Directory> directory)
{
    resultReady();
    m_result = directory;
}

void OperationDialog::onResultReadyNotification()
{
    resultReady();
}

std::shared_ptr<Directory> OperationDialog::getResult() const
{
    return m_result;
}

void OperationDialog::onUpdateProgress(int value)
{
    progressBar->setValue(value);
}

void OperationDialog::onScanningFinished()
{
    m_IsScanningFinished = true;
}

void OperationDialog::onStart()
{
    dirSelectButton->setEnabled(false);
    startButton->setEnabled(false);
    
    if (m_Mode == ModeE_DirSelect)
    {
        /* Infinite progressbar behavior */
        progressBar->setMaximum(0);
        progressBar->setMinimum(0);
        m_Operations.start(labelScanFSValue->text());
    }
    else
    {
        progressBar->setMaximum(m_Operations.totalFilesCount());
        progressBar->setMinimum(0);
        m_Operations.start();
    }
}

void OperationDialog::onCancel()
{
    reject();
}

void OperationDialog::onDirSelect()
{
    QFileDialog dialog(this);
    dialog.setDirectory(labelScanFSValue->text());
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    if (dialog.exec() == QDialog::Accepted)
    {
        labelScanFSValue->setText(dialog.directory().absolutePath());
    }
}

void OperationDialog::onFilesRead(unsigned value)
{
    labelScanFileNumberValue->setText(QString::fromStdString(std::to_string(value)));
}

void OperationDialog::onResultReady()
{
    if (m_Mode == ModeE_NoDirSelect)
    {
        cancelButton->setVisible(false);
    }
    else
    {
        /* Defined value is known so do not use maximum behavior anymore */
        progressBar->setMaximum(100);        
    }

    labelScanDirValue->clear();
    stackedWidget->setCurrentIndex(0);
    okButton->setFocus();
}

void OperationDialog::onScanDirChange(QString value)
{
    labelScanDirValue->setText(value);
}

void OperationDialog::reject()
{
    m_Operations.cancel();
    while (!m_Operations.isFinished()); //TODO: use Event instead of this ugly hack
    QDialog::reject();
}
