/*
 * Copyright (C) 2023 Jan Grulich <jgrulich@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef QADWAITA_DECORATION_H
#define QADWAITA_DECORATION_H

#include <QtCore/QDateTime>
#include <QtWaylandClient/private/qwaylandabstractdecoration_p.h>

using namespace QtWaylandClient;

class QDBusVariant;
class QPainter;

class QAdwaitaDecoration : public QWaylandAbstractDecoration
{
public:
    enum Placement { Left = 0, Right = 1 };
    enum Button { None = 0x0, Close = 0x1, Minimize = 0x02, Maximize = 0x04 };
    Q_DECLARE_FLAGS(Buttons, Button);

    QAdwaitaDecoration();
    virtual ~QAdwaitaDecoration() = default;

protected:
    QMargins margins(MarginsType marginsType = Full) const override;
    void paint(QPaintDevice *device) override;
    void paintButton(Button button, QPainter *painter);
    bool handleMouse(QWaylandInputDevice *inputDevice, const QPointF &local, const QPointF &global,
                     Qt::MouseButtons b, Qt::KeyboardModifiers mods) override;
#if QT_VERSION >= 0x060000
    bool handleTouch(QWaylandInputDevice *inputDevice, const QPointF &local, const QPointF &global,
                     QEventPoint::State state, Qt::KeyboardModifiers mods) override;
#else
    bool handleTouch(QWaylandInputDevice *inputDevice, const QPointF &local, const QPointF &global,
                     Qt::TouchPointState state, Qt::KeyboardModifiers mods) override;
#endif

private Q_SLOTS:
    void settingChanged(const QString &group, const QString &key, const QDBusVariant &value);

private:
    void initTitlebarLayout();
    void updateTitlebarLayout(const QString &layout);
    QRect windowContentGeometry() const;

    void forceRepaint();
    void loadConfiguration();

    void processMouseTop(QWaylandInputDevice *inputDevice, const QPointF &local, Qt::MouseButtons b,
                         Qt::KeyboardModifiers mods);
    void processMouseBottom(QWaylandInputDevice *inputDevice, const QPointF &local,
                            Qt::MouseButtons b, Qt::KeyboardModifiers mods);
    void processMouseLeft(QWaylandInputDevice *inputDevice, const QPointF &local,
                          Qt::MouseButtons b, Qt::KeyboardModifiers mods);
    void processMouseRight(QWaylandInputDevice *inputDevice, const QPointF &local,
                           Qt::MouseButtons b, Qt::KeyboardModifiers mods);

    bool clickButton(Qt::MouseButtons b, Button btn);
    bool doubleClickButton(Qt::MouseButtons b, const QPointF &local, const QDateTime &currentTime);
    bool updateButtonHoverState(Button hoveredButton);

    QRectF buttonRect(Button button) const;

    // Default GNOME configuraiton
    Placement m_placement = Right;
    Buttons m_buttons = Close;

    QStaticText m_windowTitle;
    Button m_clicking = None;

    Buttons m_hoveredButtons = None;
    QDateTime m_lastButtonClick;
    QPointF m_lastButtonClickPosition;

    // Colors
    QColor m_backgroundColor;
    QColor m_foregroundColor;
    QColor m_backgroundInactiveColor;
    QColor m_foregroundInactiveColor;
    QColor m_borderColor;
    QColor m_borderInactiveColor;
    QColor m_buttonBackgroundColor;
    QColor m_buttonHoverColor;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QAdwaitaDecoration::Buttons)

#endif // QADWAITA_DECORATION_H
