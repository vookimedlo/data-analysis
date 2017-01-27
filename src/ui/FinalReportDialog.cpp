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

#include <QMessageBox>
#include "../reports/ReportSettings.h"

#include "FinalReportDialog.h"


FinalReportDialog::FinalReportDialog(ReportSettings &settings, QWidget* parent) : QDialog(parent), m_reportSettings(settings)
{
    m_uiReportDialog.setupUi(this);
}

void FinalReportDialog::onAccept()
{
    if (m_uiReportDialog.checkBoxFiles->isChecked())
        m_reportSettings.addRestriction(ReportSettings::RestrictionE_Files);
    if (m_uiReportDialog.checkBoxDirectories->isChecked())
        m_reportSettings.addRestriction(ReportSettings::RestrictionE_Directories);
    if (m_uiReportDialog.checkBoxTagNoTag->isChecked())
        m_reportSettings.addRestriction(ReportSettings::RestrictionE_NoTag);
    if (m_uiReportDialog.checkBoxTagNotInteresting->isChecked())
        m_reportSettings.addRestriction(ReportSettings::RestrictionE_NotImporatant);
    if (m_uiReportDialog.checkBoxTagInteresting->isChecked())
        m_reportSettings.addRestriction(ReportSettings::RestrictionE_Imporatant);
    if (m_uiReportDialog.checkBoxTagProof->isChecked())
        m_reportSettings.addRestriction(ReportSettings::RestrictionE_Proof);

    if (m_uiReportDialog.checkBoxBasicDataType->isChecked())
        m_reportSettings.addProperty(ReportSettings::PropertiesE_BasicDataType);
    if (m_uiReportDialog.checkBoxProbableDataType->isChecked())
        m_reportSettings.addProperty(ReportSettings::PropertiesE_ProbableDataType);
    if (m_uiReportDialog.checkBoxSize->isChecked())
        m_reportSettings.addProperty(ReportSettings::PropertiesE_Size);
    if (m_uiReportDialog.checkBoxExtension->isChecked())
        m_reportSettings.addProperty(ReportSettings::PropertiesE_Extension);
    if (m_uiReportDialog.checkBoxCreationDate->isChecked())
        m_reportSettings.addProperty(ReportSettings::PropertiesE_CreationDate);
    if (m_uiReportDialog.checkBoxModificationDate->isChecked())
        m_reportSettings.addProperty(ReportSettings::PropertiesE_ModificationDate);
    if (m_uiReportDialog.checkBoxMD5->isChecked())
        m_reportSettings.addProperty(ReportSettings::PropertiesE_MD5);
    if (m_uiReportDialog.checkBoxSHA1->isChecked())
        m_reportSettings.addProperty(ReportSettings::PropertiesE_SHA1);
    if (m_uiReportDialog.checkBoxSHA3->isChecked())
        m_reportSettings.addProperty(ReportSettings::PropertiesE_SHA3_512);
    if (m_uiReportDialog.checkBoxPreview->isChecked())
        m_reportSettings.addProperty(ReportSettings::PropertiesE_Preview);
    if (m_uiReportDialog.checkBoxAnalysis->isChecked())
        m_reportSettings.addProperty(ReportSettings::PropertiesE_Analysis);
    if (m_uiReportDialog.checkBoxTag->isChecked())
        m_reportSettings.addProperty(ReportSettings::PropertiesE_Tag);
    if (m_uiReportDialog.checkBoxName->isChecked())
        m_reportSettings.addProperty(ReportSettings::PropertiesE_Name);
    if (m_uiReportDialog.checkBoxPath->isChecked())
        m_reportSettings.addProperty(ReportSettings::PropertiesE_Path);

    m_reportSettings.setTitle(m_uiReportDialog.titleLineEdit->text());
    m_reportSettings.setId(m_uiReportDialog.idLineEdit->text());
    m_reportSettings.setReference(m_uiReportDialog.referenceLineEdit->text());
    m_reportSettings.setPerex(m_uiReportDialog.perexTextEdit->toPlainText());
    m_reportSettings.setFilePath(m_uiReportDialog.exportToFileLineEdit->text());

    accept();
}

void FinalReportDialog::onClearAll()
{
    changeCheckBoxState(Qt::CheckState::Unchecked);
}

void FinalReportDialog::onDefault()
{

}

void FinalReportDialog::onFileSelect()
{

}

void FinalReportDialog::onSelectAll()
{
    changeCheckBoxState(Qt::CheckState::Checked);
}

bool FinalReportDialog::checkChosenFile(const QString& filename) const
{
    QFile f(filename);

    if (filename.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("No file has been selected for saving."));
        msgBox.setInformativeText(tr("Select the file for saving."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return false;
    }

    if (f.exists())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText(tr("Chosen file already exists."));
        msgBox.setInformativeText(tr("Do you want to overwrite the chosen file?"));
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        if (msgBox.exec() != QMessageBox::Ok)
            return false;
    }

    if (!f.open(QIODevice::Truncate | QIODevice::WriteOnly))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("Chosen file cannot be opened for writing."));
        msgBox.setInformativeText(tr("Choose another file."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return false;
    }

    return true;
}

void FinalReportDialog::changeCheckBoxState(Qt::CheckState state)
{
    m_uiReportDialog.checkBoxFiles->setCheckState(state);
    m_uiReportDialog.checkBoxDirectories->setCheckState(state);
    m_uiReportDialog.checkBoxTagNoTag->setCheckState(state);
    m_uiReportDialog.checkBoxTagNotInteresting->setCheckState(state);
    m_uiReportDialog.checkBoxTagInteresting->setCheckState(state);
    m_uiReportDialog.checkBoxTagProof->setCheckState(state);
    m_uiReportDialog.checkBoxBasicDataType->setCheckState(state);
    m_uiReportDialog.checkBoxProbableDataType->setCheckState(state);
    m_uiReportDialog.checkBoxSize->setCheckState(state);
    m_uiReportDialog.checkBoxExtension->setCheckState(state);
    m_uiReportDialog.checkBoxCreationDate->setCheckState(state);
    m_uiReportDialog.checkBoxModificationDate->setCheckState(state);
    m_uiReportDialog.checkBoxMD5->setCheckState(state);
    m_uiReportDialog.checkBoxSHA1->setCheckState(state);
    m_uiReportDialog.checkBoxSHA3->setCheckState(state);
    m_uiReportDialog.checkBoxPreview->setCheckState(state);
    m_uiReportDialog.checkBoxAnalysis->setCheckState(state);
    m_uiReportDialog.checkBoxTag->setCheckState(state);
    m_uiReportDialog.checkBoxName->setCheckState(state);
    m_uiReportDialog.checkBoxPath->setCheckState(state);
}
