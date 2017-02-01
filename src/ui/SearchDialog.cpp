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

#include "../controller/SearchSettings.h"

#include "SearchDialog.h"

SearchDialog::SearchDialog(SearchSettings &settings, QWidget* parent) : QDialog(parent), m_settings(settings)
{
    m_uiSearchDialog.setupUi(this);
}

void SearchDialog::onAccept()
{
    uint64_t lowerSize = m_uiSearchDialog.sizeLeftLineEdit->text().toULongLong();
    uint64_t upperSize = m_uiSearchDialog.sizeRightLineEdit->text().toULongLong();

    if (m_uiSearchDialog.directoryCheckBox->isChecked())
        m_settings.enableDirectory();

    if (m_uiSearchDialog.fileCheckBox->isChecked())
        m_settings.enableFile();

    if (m_uiSearchDialog.nameCheckBox->isChecked())
        m_settings.enableName(m_uiSearchDialog.nameLineEdit->text());

    if (m_uiSearchDialog.extensionCheckBox->isChecked())
        m_settings.enableExtension(m_uiSearchDialog.extensionLineEdit->text());

    if (m_uiSearchDialog.sizeCheckBox->isChecked())
        m_settings.enableSize(lowerSize, upperSize);

    if (m_uiSearchDialog.containsCheckBox->isChecked())
        m_settings.enableContains(m_uiSearchDialog.containsLineEdit->text());

    accept();
}

