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

#include "../reports/ReportSettings.h"

#include "HTMLFinalReportDialog.h"

HTMLFinalReportDialog::HTMLFinalReportDialog(ReportSettings& settings, QWidget* parent) : FinalReportDialog(settings, parent)
{    
    defaultSettings();
    m_uiReportDialog.titleLineEdit->setText(settings.getTitle());
    m_uiReportDialog.referenceLineEdit->setText(settings.getReference());
    m_uiReportDialog.idLineEdit->setText(settings.getId());
}

void HTMLFinalReportDialog::onDefault()
{
    onClearAll();
    defaultSettings();
}

void HTMLFinalReportDialog::defaultSettings() const
{
    m_uiReportDialog.checkBoxFiles->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxDirectories->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxTagProof->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxBasicDataType->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxProbableDataType->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxSize->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxCreationDate->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxModificationDate->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxMD5->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxSHA1->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxPreview->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxAnalysis->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxTag->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxName->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxPath->setCheckState(Qt::CheckState::Checked);
}
