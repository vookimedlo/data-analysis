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

#include "AboutComponentsDialog.h"


AboutComponentsDialog::AboutComponentsDialog(QWidget* parent) : QDialog(parent)
{
    m_uiAboutComponentsDialog.setupUi(this);
    m_uiAboutComponentsDialog.listWidget->setCurrentRow(0);
}

void AboutComponentsDialog::onSelectedComponentChanged(int row)
{
    switch (row)
    {
    case 0:
        m_uiAboutComponentsDialog.textBrowser->setSource(tr("qrc:/text/aboutMagic.html"));
        break;
    case 1:
        m_uiAboutComponentsDialog.textBrowser->setSource(tr("qrc:/text/aboutRegEx.html"));
        break;
    case 2:
        m_uiAboutComponentsDialog.textBrowser->setSource(tr("qrc:/text/aboutZlib.html"));
        break;
    case 3:
        m_uiAboutComponentsDialog.textBrowser->setSource(tr("qrc:/text/aboutQt.html"));
        break;
    case 4:
        m_uiAboutComponentsDialog.textBrowser->setSource(tr("qrc:/text/aboutOpenClipart.html"));
        break;

    default:
        m_uiAboutComponentsDialog.textBrowser->clear();
    }

}
