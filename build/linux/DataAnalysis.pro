#-------------------------------------------------
#
# Project created by QtCreator 2017-01-02T20:42:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DataAnalysis
TEMPLATE = app


SOURCES += \
    ../../src/3rdParty/md5/md5.cpp \
    ../../src/abstraction/qt/DiskReader.cpp \
    ../../src/controller/DataItemSortFilterProxyModel.cpp \
    ../../src/controller/DataItemTreeModel.cpp \
    ../../src/controller/DetailsPublisher.cpp \
    ../../src/controller/DirectoryTreeModel.cpp \
    ../../src/fs/DataInfo.cpp \
    ../../src/fs/DataItem.cpp \
    ../../src/fs/Directory.cpp \
    ../../src/fs/File.cpp \
    ../../src/operations/MagicOperation.cpp \
    ../../src/operations/MD5Operation.cpp \
    ../../src/operations/Operations.cpp \
    ../../src/operations/ReportOperation.cpp \
    ../../src/operations/ScanDirOperation.cpp \
    ../../src/reports/CSVReportWriter.cpp \
    ../../src/reports/RTFReportWriter.cpp \
    ../../src/ui/DataAnalyzer.cpp \
    ../../src/ui/OperationDialog.cpp \
    ../../src/util/ModelHelper.cpp \
    ../../src/util/StringHelper.cpp \
    ../../src/util/TagHelper.cpp \
    ../../src/main.cpp \
    ../../src/abstraction/unix/FS.cpp

HEADERS  += \
    ../../src/3rdParty/md5/md5.h \
    ../../src/abstraction/DiskReader.h \
    ../../src/controller/DataItemSortFilterProxyModel.h \
    ../../src/controller/DataItemTreeModel.h \
    ../../src/controller/DetailsPublisher.h \
    ../../src/controller/DirectoryTreeModel.h \
    ../../src/fs/DataInfo.h \
    ../../src/fs/DataItem.h \
    ../../src/fs/Directory.h \
    ../../src/fs/File.h \
    ../../src/operations/MagicOperation.h \
    ../../src/operations/MD5Operation.h \
    ../../src/operations/Operations.h \
    ../../src/operations/ReportOperation.h \
    ../../src/operations/ScanDirOperation.h \
    ../../src/reports/CSVReportWriter.h \
    ../../src/reports/ReportWriter.h \
    ../../src/reports/RTFReportWriter.h \
    ../../src/ui/DataAnalyzer.h \
    ../../src/ui/OperationDialog.h \
    ../../src/util/ConnectionManagement.h \
    ../../src/util/ModelHelper.h \
    ../../src/util/StringHelper.h \
    ../../src/util/TagHelper.h \
    ../../src/abstraction/FS.h

RESOURCES += \
    ../../src/resource/DataAnalyzer.qrc

FORMS += \
    ../../src/ui/forms/About.ui \
    ../../src/ui/forms/DataAnalyzer.ui \
    ../../src/ui/forms/OperationDialog.ui

unix:!macx: LIBS += -lmagic
