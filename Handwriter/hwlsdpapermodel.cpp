#include "hwlsdpapermodel.h"

#include <QSqlQuery>

HWLSDPaperModel::HWLSDPaperModel(QObject* parent)
    : QAbstractTableModel{ parent } {}

int
HWLSDPaperModel::rowCount(const QModelIndex& /*parent*/) const {
    return m_data.length();
}

int
HWLSDPaperModel::columnCount(const QModelIndex& /*parent*/) const {
    return 1;
}

QModelIndex
HWLSDPaperModel::parent(const QModelIndex&) const {
    return QModelIndex();
}

QModelIndex
HWLSDPaperModel::index(int row, int column,
                       const QModelIndex& /*parent*/) const {
    return createIndex(row, column);
}

/*QHash<int, QByteArray>
HWLSDPaperModel::roleNames() const {
    return { { Qt::UserRole + 1, "pid" } };
}*/

QVariant
HWLSDPaperModel::data(const QModelIndex& index, int role) const {
    int row = index.row();

    if (row < m_data.length() && role == Qt::DisplayRole)
        return m_data[row];
    else
        return QVariant();
}

bool
HWLSDPaperModel::insertRows(int row, int count, const QModelIndex& parent) {
    if (row <= m_data.length()) {
        beginInsertRows(parent, row, row + count - 1);
        m_data.insert(row, count, -1);
        endInsertRows();

        return true;
    } else
        return false;
}

bool
HWLSDPaperModel::removeRows(int row, int count, const QModelIndex& parent) {
    if (row >= 0 && row + count <= m_data.length()) {
        beginRemoveRows(parent, row, row + count - 1);
        m_data.remove(row, count);
        endRemoveRows();

        return true;
    } else
        return false;
}

bool
HWLSDPaperModel::select() {
    bool ok = false;
    beginResetModel();
    m_data.clear();
    // QSqlQuery query("SELECT * FROM `papers`");
    QSqlQuery query;
    if (query.exec(R"SQL(SELECT id, "test" AS `field` FROM `papers`)SQL")) {
        while (query.next()) {
            m_data.append(query.value(0));
        }
        ok = true;
    }
    qDebug() << "data length" << m_data.length();
    endResetModel();

    return ok;
}

void
HWLSDPaperModel::appendPaperRecord(int pid) {
    beginInsertRows(QModelIndex(), m_data.length(), m_data.length());
    m_data.append(pid);
    endInsertRows();
}


bool
HWLSDPaperModel::removePaperRecord(int pid) {
    int row = m_data.indexOf(pid);

    if (row >= 0) {
        beginRemoveRows(QModelIndex(), row, row);
        m_data.remove(row);
        endRemoveRows();

        return true;
    } else
        return false;
}
