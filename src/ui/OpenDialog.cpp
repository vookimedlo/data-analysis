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

#include "OpenDialog.h"


OpenDialog::OpenDialog(QWidget* parent) : QDialog(parent)
{
    m_uiOpenDialog.setupUi(this);
}

QString OpenDialog::getInputFile() const
{
    return result() == QDialog::Accepted ? m_uiOpenDialog.loadFromFileLineEdit->text() : "";
}

void OpenDialog::onAccept()
{
    if (checkChosenFile(m_uiOpenDialog.loadFromFileLineEdit->text()))
        accept();
}

void OpenDialog::onFileSelect()
{
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setDefaultSuffix("sqlite3");
    dialog.setOption(QFileDialog::ReadOnly, true);
    dialog.setDirectory(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    if (dialog.exec() == QDialog::Accepted)
    {
        if (!dialog.selectedFiles().isEmpty())
            m_uiOpenDialog.loadFromFileLineEdit->setText(dialog.selectedFiles()[0]);
        else
            m_uiOpenDialog.loadFromFileLineEdit->setText("");
    }
}

bool OpenDialog::checkChosenFile(const QString& filename) const
{
    QFile f(filename);

    if (filename.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("No file has been selected for opening."));
        msgBox.setInformativeText(tr("Select a file for opening."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return false;
    }

    if (!f.exists())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText(tr("Chosen file doesn't exist."));
        msgBox.setInformativeText(tr("Select an existing file for opening."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return false;
    }

    if (!f.open(QIODevice::ReadOnly))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("Chosen file cannot be opened for reading."));
        msgBox.setInformativeText(tr("Choose another file or check the file permissions."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return false;
    }

    return true;
}
