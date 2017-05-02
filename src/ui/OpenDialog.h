#pragma once
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

#include <QDialog>
#include "ui_OpenDialog.h"

class OpenDialog : public QDialog
{
    Q_OBJECT

public:
    OpenDialog(QWidget *parent = Q_NULLPTR);
    QString getInputFile() const;

public Q_SLOTS:
    virtual void onAccept();
    virtual void onFileSelect();

protected:
    virtual bool checkChosenFile(const QString &filename) const;

protected:
    Ui::OpenDialog m_uiOpenDialog;
};
