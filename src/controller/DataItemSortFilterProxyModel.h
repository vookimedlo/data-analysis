#pragma once
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

#include <QDate>
#include <QSortFilterProxyModel>

class DataItemSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    DataItemSortFilterProxyModel(QObject *parent = nullptr);

    QDate filterMinimumDate() const { return minDate; }
    void setFilterMinimumDate(const QDate &date);

    QDate filterMaximumDate() const { return maxDate; }
    void setFilterMaximumDate(const QDate &date);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:
    bool dateInRange(const QDate &date) const;

    QDate minDate;
    QDate maxDate;
};

