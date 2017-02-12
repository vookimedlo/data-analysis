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
#include <QStandardPaths>
#include "../reports/ReportSettings.h"

#include "RTFFinalReportDialog.h"

RTFFinalReportDialog::RTFFinalReportDialog(ReportSettings& settings, QWidget* parent) : FinalReportDialog(settings, parent)
{
    m_uiReportDialog.checkBoxPreview->setCheckState(Qt::CheckState::Unchecked);
    m_uiReportDialog.checkBoxPreview->setEnabled(false);

    defaultSettings();
    m_uiReportDialog.titleLineEdit->setText(settings.getTitle());
    m_uiReportDialog.referenceLineEdit->setText(settings.getReference());
    m_uiReportDialog.idLineEdit->setText(settings.getId());
    m_uiReportDialog.perexTextEdit->setPlainText(settings.getPerex());
}

void RTFFinalReportDialog::onAccept()
{
    if (checkChosenFile(m_uiReportDialog.exportToFileLineEdit->text()))
        FinalReportDialog::onAccept();
}

void RTFFinalReportDialog::onDefault()
{
    onClearAll();
    defaultSettings();
}

void RTFFinalReportDialog::onFileSelect()
{
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("rtf");
    dialog.setOption(QFileDialog::DontConfirmOverwrite, true);
    dialog.setDirectory(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    if (dialog.exec() == QDialog::Accepted)
    {
        if (!dialog.selectedFiles().isEmpty())
            m_uiReportDialog.exportToFileLineEdit->setText(dialog.selectedFiles()[0]);
        else
            m_uiReportDialog.exportToFileLineEdit->setText("");
    }
}

void RTFFinalReportDialog::onSelectAll()
{
    FinalReportDialog::onSelectAll();
    m_uiReportDialog.checkBoxPreview->setCheckState(Qt::CheckState::Unchecked);
    m_uiReportDialog.checkBoxAnalysis->setCheckState(Qt::CheckState::Unchecked);
}

void RTFFinalReportDialog::defaultSettings() const
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
    m_uiReportDialog.checkBoxAnalysis->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxTag->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxName->setCheckState(Qt::CheckState::Checked);
    m_uiReportDialog.checkBoxPath->setCheckState(Qt::CheckState::Checked);
}
