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

#include <QBuffer>
#include <QDate>
#include <QImageReader>
#include <QIODevice>
#include <QPixmap>
#include <QString>
#include "../model/fs/DataItem.h"
#include "../model/fs/File.h"
#include "../util/compiler.h"
#include "../util/StringHelper.h"

#include "RTFReportThumbnail.h"


RTFReportThumbnail::RTFReportThumbnail(const DataItem& item) : m_dataItem(item)
{
}

bool RTFReportThumbnail::inlineThumbnail(QString& outInlinedData)
{
    const File *file = dynamic_cast<const File *>(&m_dataItem);
    if (file)
    {
        if (QImageReader::supportedImageFormats().indexOf(StringHelper::toQString(file->extension()).toLower().toLatin1()) != -1)
        {
            try
            {
                QImageReader imageReader(StringHelper::toQString(const_cast<File *>(file)->path()));
                QImage unscaledImage(imageReader.read());

                const uint32_t pixelsPerInch = 96;
                const uint32_t wantedHeight = unscaledImage.height() > 640 ? 640 : unscaledImage.height();

                QImage image(unscaledImage.scaled(wantedHeight, wantedHeight, Qt::KeepAspectRatio));
                QByteArray byteArray;
                QBuffer buffer(&byteArray);
                buffer.open(QIODevice::WriteOnly);
                image.save(&buffer, "PNG");

                const uint32_t scaledWidth = image.width();
                const uint64_t scaledWidthInTwips = scaledWidth * 1440 / pixelsPerInch;
                const uint32_t scaledHeight = image.height();
                const uint64_t scaledHeightInTwips = scaledHeight * 1440 / pixelsPerInch;

                outInlinedData = StringHelper::toQString("{\\pict\\pngblip\\picw" + std::to_string(image.width())
                    + "\\pich" + std::to_string(image.height()) +
                    "\\picwgoal" + std::to_string(scaledWidthInTwips) + "\\pichgoal" + std::to_string(scaledHeightInTwips) +
                    "\\bin " + byteArray.toHex().toStdString()
                    + "}");

                return true;
            }
            catch (...)
            {
            }
        }
    }

    return false;
}

bool RTFReportThumbnail::write(const QString& pathName)
{
    UNUSED_VARIABLE(pathName);
    return false;
}
