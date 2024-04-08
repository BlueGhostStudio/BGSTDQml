#include "hwguidelinebase.h"

#include <hwsettings.h>

#include <QMetaObject>
#include <QQmlEngine>

HWGuideLineBase::HWGuideLineBase(QQuickItem* parent) {
    m_guideLineStyle = m_guideLinePresets[0];
    m_settings = qmlEngine(parent)->singletonInstance<HWSettings*>(
        "BGStudio.Handwriter", "HWSettings");
}

void
HWGuideLineBase::setGuideLine(const QVariantMap& define) {
    bool hasChanged = false;

    /*hasChanged = define.contains("preset");

    m_guideLineStyle =
        m_guidelinePresets[hasChanged ? define["preset"].toInt() : 0];*/
    bool ok;
    int presetIndex = define["preset"].toInt(&ok);

    if (ok && m_preset != presetIndex) {
        m_guideLineStyle = m_guideLinePresets[presetIndex];
        m_preset = presetIndex;
        hasChanged = true;
    }

    int type = define["type"].toInt(&ok);
    if (ok && m_guideLineStyle.type != type) {
        m_guideLineStyle.type = type;
        hasChanged = true;
    }

    qreal spacingMM = define["spacingMm"].toReal(&ok);
    if (ok && m_guideLineStyle.spacingMm != spacingMM) {
        m_guideLineStyle.spacingMm = spacingMM;
        hasChanged = true;
    }

    qreal horizontalPadding = define["horizontalPadding"].toReal(&ok);
    if (ok && m_guideLineStyle.horizontalPadding != horizontalPadding) {
        m_guideLineStyle.horizontalPadding = horizontalPadding;
        hasChanged = true;
    }

    qreal topPadding = define["topPadding"].toReal(&ok);
    if (ok && m_guideLineStyle.topPadding != topPadding) {
        m_guideLineStyle.topPadding = topPadding;
        hasChanged = true;
    }

    qreal bottomPadding = define["bottomPadding"].toReal(&ok);
    if (ok && m_guideLineStyle.bottomPadding != bottomPadding) {
        m_guideLineStyle.bottomPadding = bottomPadding;
        hasChanged = true;
    }

    qreal lineHeightMm = define["lineHeightMm"].toReal(&ok);
    if (ok && m_guideLineStyle.lineHeightMm != lineHeightMm) {
        m_guideLineStyle.lineHeightMm = lineHeightMm;
        hasChanged = true;
    }

    QColor color(define["color"].toString());
    if (color.isValid() && m_guideLineStyle.color != color) {
        m_guideLineStyle.color = color;
        hasChanged = true;
    }

    if (hasChanged) {
        m_preset = -1;
        updateGuideLine();
        QMetaObject::invokeMethod(toQObject(), "guideLineChanged");
        QMetaObject::invokeMethod(toQObject(), "typeChanged");
        QMetaObject::invokeMethod(toQObject(), "colorChanged");
    }
}

QVariantMap
HWGuideLineBase::guideLine() const {
    return QVariantMap(
        { { "type", m_guideLineStyle.type },
          { "spacingMm", m_guideLineStyle.spacingMm },
          { "horizontalPadding", m_guideLineStyle.horizontalPadding },
          { "topPadding", m_guideLineStyle.topPadding },
          { "bottomPadding", m_guideLineStyle.bottomPadding },
          { "lineHeightMm", m_guideLineStyle.lineHeightMm },
          { "color", m_guideLineStyle.color } });
}

HWGLType::Type HWGuideLineBase::type() const {
    return (HWGLType::Type)m_guideLineStyle.type;
}

qreal
HWGuideLineBase::spacingPx() const {
    return m_spacingPx;
}

qreal
HWGuideLineBase::lineHeightPx() const {
    return m_lineHeightPx;
}

qreal
HWGuideLineBase::horizontalPaddingPx() const {
    return m_horizontalPaddingPx;
}

qreal
HWGuideLineBase::topPaddingPx() const {
    return m_topPaddingPx;
}

qreal
HWGuideLineBase::bottomPaddingPx() const {
    return m_bottomPaddingPx;
}

int
HWGuideLineBase::rowCount() const {
    return m_rowCount;
}

int
HWGuideLineBase::columnCount() const {
    return m_columnCount;
}

QColor
HWGuideLineBase::color() const {
    return m_guideLineStyle.color;
}

void
HWGuideLineBase::setPreset(int preset) {
    if (preset != m_preset && preset < m_guideLinePresets.length()) {
        m_preset = preset;
        m_guideLineStyle = m_guideLinePresets[preset];
        updateGuideLine();
        QMetaObject::invokeMethod(toQObject(), "presetChanged");
        QMetaObject::invokeMethod(toQObject(), "guideLineChanged");
        QMetaObject::invokeMethod(toQObject(), "typeChanged");
        QMetaObject::invokeMethod(toQObject(), "colorChanged");
    }
}

int
HWGuideLineBase::preset() const {
    return m_preset;
}

void
HWGuideLineBase::updateGuideLine() {
    QQuickItem* parentItem =
        qobject_cast<QQuickItem*>(toQObject())->parentItem();
    qreal paperWidth = parentItem->width();
    qreal paperHeight = parentItem->height();

    m_spacingPx = m_guideLineStyle.spacingMm * 0.0393701 * m_settings->dpi();
    m_lineHeightPx =
        m_guideLineStyle.lineHeightMm * 0.0393701 * m_settings->dpi();

    if (m_guideLineStyle.type == 0) {
        qreal topPaddingPx = m_guideLineStyle.topPadding * m_spacingPx;
        qreal bottomPaddingPx = m_guideLineStyle.bottomPadding * m_spacingPx;
        qreal horizontalPaddingPx =
            m_guideLineStyle.horizontalPadding * m_spacingPx;

        m_columnCount = (paperWidth - horizontalPaddingPx * 2) / m_spacingPx;
        m_horizontalPaddingPx = (paperWidth - m_columnCount * m_spacingPx) / 2;

        m_rowCount =
            (paperHeight - topPaddingPx - bottomPaddingPx) / m_spacingPx;
        qreal offset = (paperHeight - topPaddingPx - bottomPaddingPx -
                        m_rowCount * m_spacingPx) /
                       2;
        m_topPaddingPx = topPaddingPx + offset;
        m_bottomPaddingPx = bottomPaddingPx + offset;
    } else {
        m_topPaddingPx =
            m_guideLineStyle.topPadding * 0.0393701 * m_settings->dpi();
        m_bottomPaddingPx =
            m_guideLineStyle.bottomPadding * 0.0393701 * m_settings->dpi();

        m_columnCount = 1;
        /*m_rowCount = (paperHeight - m_topPaddingPx - m_bottomPaddingPx) /
                     (m_lineHeightPx + m_spacingPx);*/
        qreal contentHeight = paperHeight - m_topPaddingPx - m_bottomPaddingPx;
        m_rowCount =
            (contentHeight + m_spacingPx) / (m_lineHeightPx + m_spacingPx);
        qreal offset =
            (contentHeight - m_rowCount * (m_lineHeightPx + m_spacingPx) +
             m_spacingPx) /
            2;
        m_topPaddingPx += offset;
        m_bottomPaddingPx += offset;
        m_horizontalPaddingPx =
            m_guideLineStyle.horizontalPadding * 0.0393701 * m_settings->dpi();
    }

    QMetaObject::invokeMethod(toQObject(), "spacingPxChanged");
    QMetaObject::invokeMethod(toQObject(), "lineHeightPxChanged");
    QMetaObject::invokeMethod(toQObject(), "horizontalPaddingPxChanged");
    QMetaObject::invokeMethod(toQObject(), "topPaddingPxChanged");
    QMetaObject::invokeMethod(toQObject(), "bottomPaddingPxChanged");
    QMetaObject::invokeMethod(toQObject(), "rowCountChanged");
    QMetaObject::invokeMethod(toQObject(), "columnCountChanged");
}
