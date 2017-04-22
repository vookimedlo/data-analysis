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

#include "SaveAsDialog.h"


SaveAsDialog::SaveAsDialog(QWidget* parent) : QDialog(parent)
{
    m_uiSaveAsDialog.setupUi(this);
}

QString SaveAsDialog::getOutputFile() const
{
    return result() == QDialog::Accepted ? m_uiSaveAsDialog.exportToFileLineEdit->text() : "";
}

void SaveAsDialog::onAccept()
{
    if (checkChosenFile(m_uiSaveAsDialog.exportToFileLineEdit->text()))
        accept();
}

void SaveAsDialog::onFileSelect()
{
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("sqllite3");
    dialog.setOption(QFileDialog::DontConfirmOverwrite, true);
    dialog.setDirectory(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    if (dialog.exec() == QDialog::Accepted)
    {
        if (!dialog.selectedFiles().isEmpty())
            m_uiSaveAsDialog.exportToFileLineEdit->setText(dialog.selectedFiles()[0]);
        else
            m_uiSaveAsDialog.exportToFileLineEdit->setText("");
    }
}

bool SaveAsDialog::checkChosenFile(const QString& filename) const
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
