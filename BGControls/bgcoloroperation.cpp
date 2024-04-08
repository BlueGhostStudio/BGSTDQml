#include "bgcoloroperation.h"

bool
isGrayscale(const QColor& color) {
    // Get the red, green, and blue components of the color
    int r = color.red();
    int g = color.green();
    int b = color.blue();

    // Check if the color is grayscale
    return (qAbs(r - g) <= 1) && (qAbs(g - b) <= 1) && (qAbs(b - r) <= 1);
}

QColor
BGColor_spin(const QColor& color, int amount) {
    int hue = (color.hslHue() + amount) % 360;
    if (hue < 0) hue += 360;

    return QColor::fromHsl(hue, color.hslSaturation(), color.lightness(),
                           color.alpha());
}

QColor
BGColor_shade(bool dark, const QColor& color, int shade, int mix) {
    if (shade == 0 || shade > mix) return color;

    qreal lightness = color.lightnessF();
    if ((dark && shade > 0) || (!dark && shade < 0))
        lightness += (1 - lightness) / mix * qAbs(shade);
    else
        lightness -= lightness / mix * qAbs(shade);

    bool isGray = isGrayscale(color);
    qreal saturation = color.hslSaturationF();
    if (shade > 0)
        saturation += (1 - saturation) / mix * qAbs(shade);
    else
        saturation -= saturation / mix * qAbs(shade);

    QColor shadeColor = QColor::fromHslF(color.hslHueF(), saturation, lightness,
                                         color.alphaF());
    if (isGray) {
        int gray = qGray(shadeColor.rgb());
        return QColor::fromRgb(gray, gray, gray, color.alpha());
    } else
        return shadeColor;
}

QColor
BGColor_brightnessInversion(const QColor& color, qreal delta) {    ;
    qreal lightness = qRound(color.lightnessF() * 1e2) / 1e2f;
    if (lightness > 0.5)
        return QColor::fromHslF(color.hslHueF(), color.hslSaturationF(),
                                qMax(0.0, lightness - delta), color.alphaF());
    else
        return QColor::fromHslF(color.hslHueF(), color.hslSaturationF(),
                                qMin(1.0, lightness + delta), color.alphaF());
}

QColor
BGColor_contrast(const QColor& color, const QColor& dark, const QColor& light,
                 qreal threshold) {
    qreal lightness = qRound(color.lightnessF() * 1e2) / 1e2f;
    if (lightness > threshold)
        return dark;
    else
        return light;
}

QColor
BGColor_contrast2Color(const QColor& bgColor, const QColor& fgColor,
                       qreal delta) {
    qreal bgl = qRound(bgColor.lightnessF() * 1e2) / 1e2f;
    qreal fgl = qRound(fgColor.lightnessF() * 1e2) / 1e2f;
    qreal diff = fgl - bgl;

    if (qAbs(diff) < delta) {
        if ((diff >= 0 && (bgl + delta <= 1 || bgl <= 0.5)) ||
            (diff < 0 && (bgl - delta < 0 && bgl < 0.5)))
            fgl = qMin<qreal>(bgl + delta, 1);
        else
            fgl = qMax<qreal>(bgl - delta, 0);

        return QColor::fromHslF(fgColor.hueF(), fgColor.saturationF(), fgl,
                                fgColor.alphaF());
    } else
        return fgColor;
}

QColor
BGColor_brightnessInversion(const QColor& color) {
    return QColor::fromHslF(color.hslHueF(), color.hslSaturationF(),
                            1 - color.lightnessF(), color.alphaF());
}

// -----------------------------------------------------------

BGColorOperation::BGColorOperation(QObject* parent) : QObject{ parent } {}

QColor
BGColorOperation::spin(const QColor& color, int amount) {
    return BGColor_spin(color, amount);
}

QColor
BGColorOperation::shade(bool dark, const QColor& color, int shade, int mix) {
    return BGColor_shade(dark, color, shade, mix);
}

QColor
BGColorOperation::brightnessInversion(const QColor& color, qreal delta) {
    return BGColor_brightnessInversion(color, delta);
}

QColor
BGColorOperation::contrast(const QColor& color, const QColor& dark,
                           const QColor& light, qreal threshold) {
    return BGColor_contrast(color, dark, light, threshold);
}

QColor
BGColorOperation::contrast2Color(const QColor& bgColor, const QColor& fgColor,
                                 qreal delta) {
    return BGColor_contrast2Color(bgColor, fgColor, delta);
}

QColor
BGColorOperation::brightnessInversion(const QColor& color) {
    return BGColor_brightnessInversion(color);
}
