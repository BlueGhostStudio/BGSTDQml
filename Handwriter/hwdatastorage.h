#ifndef HWDATASTORAGE_H
#define HWDATASTORAGE_H

#include <dataconversion.h>

#include <QObject>
#include <QQmlEngine>
#include <QSqlDatabase>

#include "hwlsdpapermodel.h"

struct PaperData {
    Q_GADGET
    QML_ANONYMOUS

    Q_PROPERTY(QSizeF paperSize MEMBER m_paperSize)
    Q_PROPERTY(QUrl background MEMBER m_background)
    Q_PROPERTY(QVariant palette READ palette)

    Q_PROPERTY(QJSValue guideline READ guideline)

    Q_PROPERTY(QVariant strokeList READ strokeList)

    Q_PROPERTY(bool isValid MEMBER m_isValid)

public:
    QVariant palette() const;
    QVariant strokeList();
    QJSValue guideline() const;

    QSizeF m_paperSize;
    QUrl m_background;
    QList<QColor> m_palette;
    QJSValue m_guidelineLayout;
    QList<Stroke> m_strokeList;

    bool m_isValid = false;
};

class HWDataStorage : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(HWLSDPaperModel* papersModel READ papersModel CONSTANT)

public:
    explicit HWDataStorage(QObject* parent = nullptr);

    HWLSDPaperModel* papersModel();

    Q_INVOKABLE PaperData readFile(const QUrl& url) const;
    Q_INVOKABLE PaperData readData(const QByteArray& data) const;
    Q_INVOKABLE PaperData readLocalStorageData(int id) const;

    Q_INVOKABLE bool isExistLocalStorageData(int id) const;

    Q_INVOKABLE bool saveFile(const QUrl& url, const PaperData& data) const;
    Q_INVOKABLE QByteArray saveData(const PaperData& data) const;
    Q_INVOKABLE int insertPaper(const PaperData& data);
    Q_INVOKABLE bool updatePaper(int id, const PaperData& data);
    Q_INVOKABLE bool deletePaper(int id);

    // Q_INVOKABLE QVariantList looseLeafPapers() const;

    Q_INVOKABLE QList<Stroke> byteArray2StrokeList(
        const QByteArray& data) const;
    Q_INVOKABLE QByteArray
    strokeList2ByteArray(const QList<Stroke>& strokes) const;

    Q_INVOKABLE QByteArray b2a(const QByteArray& data) const;
    Q_INVOKABLE QByteArray a2b(const QByteArray& data) const;

signals:
    void paperUpdated(int id);
    void paperAdded(int id);
    void paperDeleted(int id);

private:
    PaperData read(QIODevice* io) const;
    void write(QIODevice* io, const PaperData& data) const;

private:
    QString m_dataPath{ QStandardPaths::writableLocation(
                            QStandardPaths::DocumentsLocation) +
                        "/HandWriter" };
    QSqlDatabase m_database;
    HWLSDPaperModel m_papersModel;
};

#endif  // HWDATASTORAGE_H
