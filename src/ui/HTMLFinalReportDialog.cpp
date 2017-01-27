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
#include <QMessageBox>
#include <QStandardPaths>
#include "../reports/ReportSettings.h"

#include "HTMLFinalReportDialog.h"

HTMLFinalReportDialog::HTMLFinalReportDialog(ReportSettings& settings, QWidget* parent) : FinalReportDialog(settings, parent)
{    
    defaultSettings();
    m_uiReportDialog.titleLineEdit->setText(settings.getTitle());
    m_uiReportDialog.referenceLineEdit->setText(settings.getReference());
    m_uiReportDialog.idLineEdit->setText(settings.getId());
    m_uiReportDialog.perexTextEdit->setPlainText(settings.getPerex());
}

void HTMLFinalReportDialog::onAccept()
{
    if (checkChosenFile(m_uiReportDialog.exportToFileLineEdit->text()))
        FinalReportDialog::onAccept();
}

void HTMLFinalReportDialog::onDefault()
{
    onClearAll();
    defaultSettings();
}

void HTMLFinalReportDialog::onFileSelect()
{
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("html");
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

bool HTMLFinalReportDialog::checkChosenFile(const QString &filename) const
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

    QFileInfo reportDir(filename);
    QString reportDirPath(reportDir.canonicalPath() + "/" + reportDir.baseName() + "/");
    QDir dir(reportDirPath);
    if (dir.exists())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("Directory '%1' exists, but should be used for the report related files.").arg(reportDirPath));
        msgBox.setInformativeText(tr("Either remove the directory, or select a different file for report saving."));
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
