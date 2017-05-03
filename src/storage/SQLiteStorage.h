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

#include <QSqlDatabase>

class DataItem;
class Directory;
class File;
class GlobalInformation;
class QSqlQuery;

class SQLiteStorage
{
public:
      SQLiteStorage(const QString& path);
      ~SQLiteStorage();

      bool open();
      void close();

      bool store(const Directory &item);
      bool store(const File &item);
      bool store(const QString &referenceNumber, const QString &reference, const QString &id, const QString &finalReport);
      Directory *load();
      void load(GlobalInformation &globalInformation, QString &finalReport);

protected:
      void fillDataItem(DataItem &item, QSqlQuery &query);
      bool store(const DataItem &item, QSqlQuery &query);

private:
      QSqlDatabase m_db;
};
