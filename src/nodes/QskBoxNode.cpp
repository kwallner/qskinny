/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "QskBoxNode.h"
#include "QskBoxShadowNode.h"
#include "QskBoxRectangleNode.h"
#include "QskSGNode.h"

#include "QskGradient.h"
#include "QskGradientDirection.h"
#include "QskShadowMetrics.h"
#include "QskBoxBorderMetrics.h"
#include "QskBoxBorderColors.h"
#include "QskRgbValue.h"

namespace
{
    enum Role
    {
        ShadowRole,
        ShadowFillRole,
        BoxRole,
        FillRole
    };
}

static void qskUpdateChildren( QSGNode* parentNode, quint8 role, QSGNode* node )
{
    static const QVector< quint8 > roles =
        { ShadowRole, ShadowFillRole, BoxRole, FillRole };

    auto oldNode = QskSGNode::findChildNode( parentNode, role );
    QskSGNode::replaceChildNode( roles, role, parentNode, oldNode, node );
}

template< typename Node >
static inline Node* qskNode( QSGNode* parentNode, quint8 role )
{
    using namespace QskSGNode;

    auto node = static_cast< Node* > ( findChildNode( parentNode, role ) );

    if ( node == nullptr )
    {
        node = new Node();
        setNodeRole( node, role );
    }

    return node;
}

QskBoxNode::QskBoxNode()
{
}

QskBoxNode::~QskBoxNode()
{
}

void QskBoxNode::updateNode( const QRectF& rect,
    const QskBoxShapeMetrics& shape, const QskBoxBorderMetrics& borderMetrics,
    const QskBoxBorderColors& borderColors, const QskGradient& gradient,
    const QskShadowMetrics& shadowMetrics, const QColor& shadowColor )
{
    using namespace QskSGNode;

    QskBoxShadowNode* shadowNode = nullptr;
    QskBoxRectangleNode* shadowFillNode = nullptr;
    QskBoxRectangleNode* rectNode = nullptr;
    QskBoxRectangleNode* fillNode = nullptr;

    if ( !rect.isEmpty() )
    {
        const auto hasFilling = gradient.isVisible();
        const auto hasBorder = !borderMetrics.isNull() && borderColors.isVisible();

        const auto hasShadow = hasFilling && !shadowMetrics.isNull()
            && QskRgb::isVisible( shadowColor );

        if ( hasShadow )
        {
            const auto shadowRect = shadowMetrics.shadowRect( rect );
            const auto blurRadius = shadowMetrics.blurRadius();

            if ( blurRadius <= 0.0 )
            {
                // QskBoxRectangleNode allows scene graph batching
                shadowFillNode = qskNode< QskBoxRectangleNode >( this, ShadowFillRole );
                shadowFillNode->updateFilling( shadowRect, shape, shadowColor );
            }
            else
            {
                shadowNode = qskNode< QskBoxShadowNode >( this, ShadowRole );
                shadowNode->setShadowData( shadowRect, shape, blurRadius, shadowColor );
            }
        }

        if ( hasBorder || hasFilling )
        {
            rectNode = qskNode< QskBoxRectangleNode >( this, BoxRole );

            if ( hasBorder && hasFilling )
            {
                const bool doCombine = rectNode->hasHint( QskFillNode::PreferColoredGeometry )
                    && QskBoxRectangleNode::isCombinedGeometrySupported( gradient );

                if ( !doCombine )
                    fillNode = qskNode< QskBoxRectangleNode >( this, FillRole );
            }

            if ( fillNode )
            {
                rectNode->updateBorder( rect, shape, borderMetrics, borderColors );
                fillNode->updateFilling( rect, shape, borderMetrics, gradient );
            }
            else
            {
                rectNode->updateBox( rect, shape, borderMetrics, borderColors, gradient );
            }
        }
    }

    qskUpdateChildren( this, ShadowRole, shadowNode );
    qskUpdateChildren( this, ShadowFillRole, shadowFillNode );
    qskUpdateChildren( this, BoxRole, rectNode );
    qskUpdateChildren( this, FillRole, fillNode );
}
