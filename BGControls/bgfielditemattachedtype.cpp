#include "bgfielditemattachedtype.h"

#include <QDebug>

#include "bgfielditem.h"

BGFieldItemAttachedType::BGFieldItemAttachedType(QObject* parent)
    : QObject{ parent } {}

BGFieldItem*
BGFieldItemAttachedType::field() const {
    return m_field;
}

void
BGFieldItemAttachedType::setField(BGFieldItem* field) {
    m_field = field;
    if (m_orientationChanged) m_field->setOrientation(m_orientation);
    if (m_fillWidthChanged) m_field->setFillWidth(m_fillWidth);
}

Qt::Orientation
BGFieldItemAttachedType::orientation() const {
    return m_field ? m_field->orientation() : m_orientation;
}

void
BGFieldItemAttachedType::setOrientation(Qt::Orientation orientation) {
    m_orientation = orientation;
    m_orientationChanged = true;
    if (m_field && m_field->orientation() != orientation) {
        m_field->setOrientation(orientation);
        emit orientationChanged();
    }
}

bool
BGFieldItemAttachedType::fillWidth() const {
    return m_field ? m_field->fillWidth() : m_fillWidth;
}

void
BGFieldItemAttachedType::setFillWidth(bool fillWidth) {
    m_fillWidth = fillWidth;
    m_fillWidthChanged = true;
    if (m_field && m_field->fillWidth() != fillWidth) {
        m_field->setFillWidth(fillWidth);
        emit fillWidthChanged();
    }
}
