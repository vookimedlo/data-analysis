#-------------------------------------------------
#
# Project created by QtCreator 2017-01-02T20:42:10
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DataAnalysis
TEMPLATE = app

# std::make_unique is part of c++14
#
CONFIG += c++14

SOURCES += \
    ../../src/abstraction/qt/DiskReader.cpp \
    ../../src/controller/DataItemSortFilterProxyModel.cpp \
    ../../src/controller/DataItemTreeModel.cpp \
    ../../src/controller/DetailsPublisher.cpp \
    ../../src/controller/DirectoryTreeModel.cpp \
    ../../src/controller/SearchDataItemTreeModel.cpp \
    ../../src/controller/SearchSettings.cpp \
    ../../src/model/fs/DataInfo.cpp \
    ../../src/model/fs/DataItem.cpp \
    ../../src/model/fs/Directory.cpp \
    ../../src/model/fs/File.cpp \
    ../../src/operations/MagicOperation.cpp \
    ../../src/operations/Operations.cpp \
    ../../src/operations/ReportOperation.cpp \
    ../../src/operations/ScanDirOperation.cpp \
    ../../src/operations/SearchOperation.cpp \
    ../../src/reports/CSVReportWriter.cpp \
    ../../src/reports/RTFReportWriter.cpp \
    ../../src/ui/DataAnalyzer.cpp \
    ../../src/ui/OperationDialog.cpp \
    ../../src/util/ModelHelper.cpp \
    ../../src/util/StringHelper.cpp \
    ../../src/util/TagHelper.cpp \
    ../../src/main.cpp \
    ../../src/abstraction/unix/FS.cpp \
    ../../src/operations/HashOperation.cpp \
    ../../src/reports/HTMLReportWriter.cpp \
    ../../src/reports/ReportSettings.cpp \
    ../../src/ui/CSVFinalReportDialog.cpp \
    ../../src/ui/FinalReportDialog.cpp \
    ../../src/ui/HTMLFinalReportDialog.cpp \
    ../../src/ui/RTFFinalReportDialog.cpp \
    ../../src/ui/GlobalInformationDialog.cpp \
    ../../src/ui/SearchDialog.cpp \
    ../../src/reports/CopyReportThumbnail.cpp \
    ../../src/reports/HTMLReportThumbnailGenerator.cpp \
    ../../src/reports/RTFReportThumbnail.cpp \
    ../../src/reports/RTFReportThumbnailGenerator.cpp \
    ../../src/model/GlobalInformation.cpp \
    ../../src/storage/SQLiteStorage.cpp \
    ../../src/operations/StoreOperation.cpp \
    ../../src/ui/SaveAsDialog.cpp \
    ../../src/operations/OpenOperation.cpp \
    ../../src/ui/OpenDialog.cpp


HEADERS  += \
    ../../src/abstraction/DiskReader.h \
    ../../src/controller/DataItemSortFilterProxyModel.h \
    ../../src/controller/DataItemTreeModel.h \
    ../../src/controller/DetailsPublisher.h \
    ../../src/controller/DirectoryTreeModel.h \
    ../../src/controller/SearchDataItemTreeModel.h \
    ../../src/controller/SearchSettings.h \
    ../../src/model/fs/DataInfo.h \
    ../../src/model/fs/DataItem.h \
    ../../src/model/fs/Directory.h \
    ../../src/model/fs/File.h \
    ../../src/operations/MagicOperation.h \
    ../../src/operations/Operations.h \
    ../../src/operations/ReportOperation.h \
    ../../src/operations/ScanDirOperation.h \
    ../../src/operations/SearchOperation.h \
    ../../src/reports/CSVReportWriter.h \
    ../../src/reports/ReportWriter.h \
    ../../src/reports/RTFReportWriter.h \
    ../../src/ui/DataAnalyzer.h \
    ../../src/ui/OperationDialog.h \
    ../../src/util/ConnectionManagement.h \
    ../../src/util/ModelHelper.h \
    ../../src/util/StringHelper.h \
    ../../src/util/TagHelper.h \
    ../../src/abstraction/FS.h \
    ../../src/operations/HashOperation.h \
    ../../src/reports/HTMLReportWriter.h \
    ../../src/reports/ReportSettings.h \
    ../../src/ui/CSVFinalReportDialog.h \
    ../../src/ui/FinalReportDialog.h \
    ../../src/ui/HTMLFinalReportDialog.h \
    ../../src/ui/RTFFinalReportDialog.h \
    ../../src/ui/GlobalInformationDialog.h \
    ../../src/ui/SearchDialog.h \
    ../../src/reports/CopyReportThumbnail.h \
    ../../src/reports/HTMLReportThumbnailGenerator.h \
    ../../src/reports/ReportThumbnail.h \
    ../../src/reports/ReportThumbnailGenerator.h \
    ../../src/reports/RTFReportThumbnailGenerator.h \
    ../../src/reports/RTFReportThumbnail.h \
    ../../src/model/GlobalInformation.h \
    ../../src/util/compiler.h \
    ../../src/model/cache/Cache.h \
    ../../src/storage/SQLiteStorage.h \
    ../../src/operations/StoreOperation.h \
    ../../src/ui/SaveAsDialog.h \
    ../../src/util/UniqueId.h \
    ../../src/operations/OpenOperation.h \
    ../../src/ui/OpenDialog.h

TRANSLATIONS += \
    ../../src/resource/translations/dataanalyzer_cs_CZ.ts

RESOURCES += \
    ../../src/resource/DataAnalyzer.qrc

FORMS += \
    ../../src/ui/forms/About.ui \
    ../../src/ui/forms/DataAnalyzer.ui \
    ../../src/ui/forms/OperationDialog.ui \
    ../../src/ui/forms/FinalReportDialog.ui \
    ../../src/ui/forms/GlobalInformationDialog.ui \
    ../../src/ui/forms/SearchDialog.ui \
    ../../src/ui/forms/SaveAsDialog.ui \
    ../../src/ui/forms/OpenDialog.ui

macx: ICON = ../../src/resource/AppIcon/macos/icon.icns

unix:!macx: LIBS += -lmagic

macx: LIBS += -L$$PWD/../../src/3rdParty/libmagic/macos/lib/ -lmagic.1
macx: INCLUDEPATH += $$PWD/../../src/3rdParty/libmagic/macos/include
macx: DEPENDPATH += $$PWD/../../src/3rdParty/libmagic/macos/include
