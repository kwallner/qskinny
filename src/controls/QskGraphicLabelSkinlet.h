/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#ifndef QSK_GRAPHIC_LABEL_SKINLET_H
#define QSK_GRAPHIC_LABEL_SKINLET_H

#include "QskSkinlet.h"

class QskGraphicLabel;

class QSK_EXPORT QskGraphicLabelSkinlet : public QskSkinlet
{
    Q_GADGET

    using Inherited = QskSkinlet;

  public:
    enum NodeRole : quint8
    {
        PanelRole,
        GraphicRole,

        RoleCount
    };

    Q_INVOKABLE QskGraphicLabelSkinlet( QskSkin* = nullptr );
    ~QskGraphicLabelSkinlet() override;

    QRectF subControlRect( const QskSkinnable*,
        const QRectF&, QskAspect::Subcontrol ) const override;

    QSizeF sizeHint( const QskSkinnable*,
        Qt::SizeHint, const QSizeF& ) const override;

  protected:
    QSGNode* updateSubNode( const QskSkinnable*,
        quint8 nodeRole, QSGNode* ) const override;

  private:
    QRect graphicRect( const QskGraphicLabel*, const QRectF& ) const;
    QSGNode* updateGraphicNode( const QskGraphicLabel*, QSGNode* ) const;
};

#endif
