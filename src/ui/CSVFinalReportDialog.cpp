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

#include "../reports/ReportSettings.h"

#include "CSVFinalReportDialog.h"

CSVFinalReportDialog::CSVFinalReportDialog(ReportSettings& settings, QWidget* parent) : FinalReportDialog(settings, parent)
{
    m_uiReportDialog.checkBoxPreview->setCheckState(Qt::CheckState::Unchecked);
    m_uiReportDialog.checkBoxAnalysis->setCheckState(Qt::CheckState::Unchecked);
    m_uiReportDialog.checkBoxPreview->setEnabled(false);
    m_uiReportDialog.checkBoxAnalysis->setEnabled(false);

    m_uiReportDialog.titleLineEdit->setEnabled(false);
    m_uiReportDialog.referenceLineEdit->setEnabled(false);
    m_uiReportDialog.perexTextEdit->setEnabled(false);
    m_uiReportDialog.idLineEdit->setEnabled(false);

    defaultSettings();
}

void CSVFinalReportDialog::onDefault()
{
    onClearAll();
    defaultSettings();
}

void CSVFinalReportDialog::onSelectAll()
{
    FinalReportDialog::onSelectAll();
    m_uiReportDialog.checkBoxPreview->setCheckState(Qt::CheckState::Unchecked);
    m_uiReportDialog.checkBoxAnalysis->setCheckState(Qt::CheckState::Unchecked);
}

void CSVFinalReportDialog::defaultSettings() const
{
    m_uiReportDialog.checkBoxFiles->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxDirectories->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxTagNoTag->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxTagNotInteresting->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxTagInteresting->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxTagProof->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxBasicDataType->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxProbableDataType->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxSize->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxExtension->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxCreationDate->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxModificationDate->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxTag->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxName->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxPath->setCheckState(Qt::CheckState::Checked);
}
