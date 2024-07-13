#include "hwdatastorage.h"

#include <QSqlError>
#include <QSqlQuery>

QVariant
PaperData::palette() const {
    return QVariant::fromValue(m_palette);
}

QVariant
PaperData::strokeList() {
    return QVariant::fromValue(m_strokeList);
}

QJSValue
PaperData::guideline() const {
    return m_guidelineLayout;
}

HWDataStorage::HWDataStorage(QObject* parent) : QObject{ parent } {
    QDir dataDir(m_dataPath);
    if (!dataDir.exists()) dataDir.mkpath(m_dataPath);

    QString dbFilePath = m_dataPath + "/handwriter.db";
    if (!QFile::exists(dbFilePath))
        QFile::copy(":/qt/qml/BGStudio/Handwriter/handwriter.db", dbFilePath);

    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName(dbFilePath);
    qDebug() << "database open" << dbFilePath << m_database.open();

    // m_localStorageDataModel = new HWLocalStorageDataModel();
    /*m_localStorageDataModel->setTable("papers");
    qDebug() << "----->>>" << m_localStorageDataModel->select();
    if (m_localStorageDataModel->lastError().isValid())
        qDebug() << m_localStorageDataModel->lastError()
                 << m_localStorageDataModel->database().databaseName();*/
    /*m_localStorageDataModel.setQuery("SELECT `id` AS pid from `papers`");
    if (m_localStorageDataModel.lastError().isValid())
        qDebug() << m_localStorageDataModel.lastError();*/

    qDebug() << "select:" << m_papersModel.select();

    QObject::connect(this, &HWDataStorage::paperAdded, &m_papersModel,
                     &HWLSDPaperModel::appendPaperRecord);
    QObject::connect(this, &HWDataStorage::paperDeleted, &m_papersModel,
                     &HWLSDPaperModel::removePaperRecord);
}

HWLSDPaperModel*
HWDataStorage::papersModel() {
    return &m_papersModel;
}

PaperData
HWDataStorage::readFile(const QUrl& url) const {
    QFile file(url.path());

    if (file.open(QIODevice::ReadOnly)) {
        PaperData&& thePaperData = read(&file);
        file.close();
        return thePaperData;
    }

    return PaperData{};
}

PaperData
HWDataStorage::readData(const QByteArray& data) const {
    QByteArray theData(std::move(data));
    QBuffer buffer(&theData);

    if (buffer.open(QIODevice::ReadOnly)) {
        PaperData&& thePaperData = read(&buffer);
        buffer.close();
        return thePaperData;
    } else
        return PaperData{};
}

PaperData
HWDataStorage::readLocalStorageData(int id) const {
    if (m_database.isOpen()) {
        QSqlQuery query;
        query.prepare("SELECT * FROM `papers` WHERE `id`=:I");
        query.bindValue(":I", id);
        return query.exec() && query.first()
                   ? readData(query.value("data").toByteArray())
                   : PaperData{};
    } else
        return PaperData{};
}

bool
HWDataStorage::isExistLocalStorageData(int id) const {
    if (m_database.isOpen()) {
        QSqlQuery query;
        query.prepare(R"SQL(
SELECT COUNT()
FROM `papers`
WHERE `id`=:I
)SQL");
        query.bindValue(":I", id);

        return query.exec() && query.first() ? query.value(0).toInt() > 0
                                             : false;
    } else
        return false;
}

bool
HWDataStorage::saveFile(const QUrl& url, const PaperData& data) const {
    QFile file(url.path());

    if (file.open(QIODevice::WriteOnly)) {
        /*QJsonObject jsoPaper;

        QJsonArray jsaPalette;
        foreach (const QColor& color, data.m_palette) {
            jsaPalette.append(color.name());
        }
        jsoPaper["palette"] = jsaPalette;

        jsoPaper["size"] =
            QJsonObject{ { "width", data.m_paperSize.width() },
                         { "height", data.m_paperSize.height() } };
        jsoPaper["background"] = data.m_background.toString();
        jsoPaper["guideline"] = qmlEngine(this)->fromScriptValue<QJsonValue>(
            data.m_guidelineLayout);

        file.write(QJsonDocument(jsoPaper).toJson(QJsonDocument::Compact) +
                   "\n");

        file.write(::sl2b(data.m_strokeList));*/
        write(&file, data);

        file.close();

        return true;
    } else
        return false;
}

QByteArray
HWDataStorage::saveData(const PaperData& data) const {
    QByteArray outData;
    QBuffer buffer(&outData);

    if (buffer.open(QIODevice::WriteOnly)) {
        write(&buffer, data);

        buffer.close();
    }

    return outData;
}

int
HWDataStorage::insertPaper(const PaperData& data) {
    if (m_database.isOpen()) {
        QSqlQuery query;
        query.prepare(R"SQL(
INSERT INTO `papers`
(`data`)
VALUES
(:D)
)SQL");
        query.bindValue(":D", saveData(data));

        bool ok = query.exec();
        if (ok) {
            int id = query.lastInsertId().toInt();
            emit paperAdded(id);
            return id;
        } else
            return -1;
    } else
        return -1;
}

bool
HWDataStorage::updatePaper(int id, const PaperData& data) {
    if (isExistLocalStorageData(id)) {
        QSqlQuery query;
        query.prepare(R"SQL(
UPDATE `papers`
SET
`data`=:D
WHERE `id`=:I
)SQL");
        query.bindValue(":D", saveData(data));
        query.bindValue(":I", id);

        bool ok = query.exec();
        if (ok) emit paperUpdated(id);

        return ok;
    } else
        return false;
}

bool
HWDataStorage::deletePaper(int id) {
    if (isExistLocalStorageData(id)) {
        qDebug() << "exist paper";
        QSqlQuery query;
        query.prepare(R"SQL(
DELETE FROM `papers`
WHERE `id`=:I
)SQL");
        query.bindValue(":I", id);

        bool ok = query.exec();
        if (ok) emit paperDeleted(id);

        return ok;
    } else {
        qDebug() << "no exist paprer";
        return false;
    }
}

/*QVariantList
HWDataStorage::looseLeafPapers() const {}*/

QList<Stroke>
HWDataStorage::byteArray2StrokeList(const QByteArray& data) const {
    return ::b2sl(data);
}

QByteArray
HWDataStorage::strokeList2ByteArray(const QList<Stroke>& strokes) const {
    return ::sl2b(strokes);
}

QByteArray
HWDataStorage::b2a(const QByteArray& data) const {
    return data.toBase64();
}

QByteArray
HWDataStorage::a2b(const QByteArray& data) const {
    return QByteArray::fromBase64(data);
}

PaperData
HWDataStorage::read(QIODevice* io) const {
    PaperData thePaperData;
    thePaperData.m_isValid = true;

    QJsonObject jsoData = QJsonDocument::fromJson(io->readLine()).object();
    QJsonObject jsoSize = jsoData["size"].toObject();
    thePaperData.m_paperSize =
        QSizeF(jsoSize["width"].toDouble(), jsoSize["height"].toDouble());

    QJsonArray jsaPalette = jsoData["palette"].toArray();
    foreach (QJsonValue jsvColor, jsaPalette) {
        thePaperData.m_palette.append(QColor::fromString(jsvColor.toString()));
    }

    thePaperData.m_background = jsoData["background"].toString();

    thePaperData.m_guidelineLayout =
        qmlEngine(this)->toScriptValue(jsoData["guideline"].toObject());

    thePaperData.m_strokeList = ::b2sl(io->readAll());

    return thePaperData;
}

void
HWDataStorage::write(QIODevice* io, const PaperData& data) const {
    QJsonObject jsoPaper;

    QJsonArray jsaPalette;
    foreach (const QColor& color, data.m_palette) {
        jsaPalette.append(color.name());
    }
    jsoPaper["palette"] = jsaPalette;

    jsoPaper["size"] = QJsonObject{ { "width", data.m_paperSize.width() },
                                    { "height", data.m_paperSize.height() } };
    jsoPaper["background"] = data.m_background.toString();
    jsoPaper["guideline"] =
        qmlEngine(this)->fromScriptValue<QJsonValue>(data.m_guidelineLayout);

    io->write(QJsonDocument(jsoPaper).toJson(QJsonDocument::Compact) + "\n");

    io->write(::sl2b(data.m_strokeList));
}
