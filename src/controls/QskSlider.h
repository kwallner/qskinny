/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#ifndef QSK_SLIDER_H
#define QSK_SLIDER_H

#include "QskBoundedValueInput.h"
#include "QskNamespace.h"

class QSK_EXPORT QskSlider : public QskBoundedValueInput
{
    Q_OBJECT

    Q_PROPERTY( bool isPressed READ isPressed NOTIFY pressedChanged )

    Q_PROPERTY( Qsk::Policy graduationPolicy READ graduationPolicy
        WRITE setGraduationPolicy RESET resetGraduationPolicy
        NOTIFY graduationPolicyChanged )

    Q_PROPERTY( Qt::Orientation orientation READ orientation
        WRITE setOrientation NOTIFY orientationChanged )

    Q_PROPERTY( bool tracking READ isTracking
        WRITE setTracking NOTIFY trackingChanged )

    Q_PROPERTY( qreal handlePosition READ handlePosition )

    using Inherited = QskBoundedValueInput;

  public:
    QSK_SUBCONTROLS( Panel, Groove, Fill, Scale, Tick, Handle )
    QSK_STATES( Pressed )

    explicit QskSlider( QQuickItem* parent = nullptr );
    explicit QskSlider( Qt::Orientation, QQuickItem* parent = nullptr );

    ~QskSlider() override;

    bool isPressed() const;

    void setOrientation( Qt::Orientation );
    Qt::Orientation orientation() const;

    void setGraduationPolicy( Qsk::Policy );
    void resetGraduationPolicy();
    Qsk::Policy graduationPolicy() const;

    void setTracking( bool );
    bool isTracking() const;

    qreal handlePosition() const; // [0,0, 1.0]

    QskAspect::Variation effectiveVariation() const override;

  Q_SIGNALS:
    void pressedChanged( bool );
    void orientationChanged( Qt::Orientation );
    void trackingChanged( bool );
    void graduationPolicyChanged( Qsk::Policy );

  protected:
    void mousePressEvent( QMouseEvent* ) override;
    void mouseMoveEvent( QMouseEvent* ) override;
    void mouseReleaseEvent( QMouseEvent* ) override;

    void aboutToShow() override;

  private:
    void moveHandle();
    void moveHandleTo( qreal value, const QskAnimationHint& );

    class PrivateData;
    std::unique_ptr< PrivateData > m_data;
};

#endif
