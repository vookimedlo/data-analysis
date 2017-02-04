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
    QRegularExpression rx("0|[1-9][0-9]{0,17}");
    QValidator *validator = new QRegularExpressionValidator(rx, this);
    m_uiSearchDialog.sizeLeftLineEdit->setValidator(validator);
    m_uiSearchDialog.sizeRightLineEdit->setValidator(validator);
}

void SearchDialog::onAccept()
{
    static QRegularExpression::PatternOptions reOpts = QRegularExpression::PatternOption::CaseInsensitiveOption | QRegularExpression::PatternOption::UseUnicodePropertiesOption | QRegularExpression::PatternOption::MultilineOption;

    uint64_t lowerSize = m_uiSearchDialog.sizeLeftLineEdit->text().toULongLong();
    uint64_t upperSize = m_uiSearchDialog.sizeRightLineEdit->text().toULongLong();

    if (m_uiSearchDialog.directoryCheckBox->isChecked())
        m_settings.enableDirectory();

    if (m_uiSearchDialog.fileCheckBox->isChecked())
        m_settings.enableFile();

    if (m_uiSearchDialog.nameCheckBox->isChecked())
    {
        if (m_uiSearchDialog.nameRegExpCheckBox->isChecked())
            m_settings.enableName(QRegularExpression(m_uiSearchDialog.nameLineEdit->text(), reOpts));
        else
            m_settings.enableName(m_uiSearchDialog.nameLineEdit->text());
    }

    if (m_uiSearchDialog.extensionCheckBox->isChecked())
    {
        if (m_uiSearchDialog.extensionRegExpCheckBox->isChecked())
            m_settings.enableExtension(QRegularExpression(m_uiSearchDialog.extensionLineEdit->text(), reOpts));
        else
            m_settings.enableExtension(m_uiSearchDialog.extensionLineEdit->text());
    }
        
    if (m_uiSearchDialog.sizeCheckBox->isChecked())
        m_settings.enableSize(lowerSize, upperSize);

    if (m_uiSearchDialog.containsCheckBox->isChecked())
    {
        if (m_uiSearchDialog.containsRegExpCheckBox->isChecked())
            m_settings.enableContains(QRegularExpression(m_uiSearchDialog.containsLineEdit->text(), reOpts));
        else
            m_settings.enableContains(m_uiSearchDialog.containsLineEdit->text());
    }

    if (m_uiSearchDialog.tagNotInterestingcheckBox->isChecked())
        m_settings.enableTagNotInteresing();

    if (m_uiSearchDialog.tagInterestingCheckBox->isChecked())
        m_settings.enableTagInteresing();

    if (m_uiSearchDialog.tagProofCheckBox->isChecked())
        m_settings.enableTagProof();

    accept();
}
