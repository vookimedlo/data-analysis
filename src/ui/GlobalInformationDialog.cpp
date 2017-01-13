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

#include "../model/GlobalInformation.h"

#include "GlobalInformationDialog.h"
#include "../util/StringHelper.h"


GlobalInformationDialog::GlobalInformationDialog(GlobalInformation& info, bool populateInfoToUi, QWidget* parent) : QDialog(parent), m_info(info)
{
    m_uiGlobalInformationDialog.setupUi(this);
    if (populateInfoToUi)
    {
        m_uiGlobalInformationDialog.referenceNumberLineEdit->setText(StringHelper::toQString(info.getReferenceNumber()));
        m_uiGlobalInformationDialog.referenceLineEdit->setText(StringHelper::toQString(info.getReference()));
        m_uiGlobalInformationDialog.idLineEdit->setText(StringHelper::toQString(info.getId()));
    }
}

void GlobalInformationDialog::onAccept()
{
    m_info.setReferenceNumber(StringHelper::toStdString(m_uiGlobalInformationDialog.referenceNumberLineEdit->text()));
    m_info.setReference(StringHelper::toStdString(m_uiGlobalInformationDialog.referenceLineEdit->text()));
    m_info.setId(StringHelper::toStdString(m_uiGlobalInformationDialog.idLineEdit->text()));
    accept();
}
