// krazy:excludeall=copyright (email of Maxim is missing)
/*
    This file is a part of the KDE project

    Copyright © 2006 Zack Rusin <zack@kde.org>
    Copyright © 2006-2007, 2008 Fredrik Höglund <fredrik@kde.org>

    The stack blur algorithm was invented by Mario Klingemann <mario@quasimondo.com>

    This implementation is based on the version in Anti-Grain Geometry Version 2.4,
    Copyright © 2002-2005 Maxim Shemanarev (http://www.antigrain.com)

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
    OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
    IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
    NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
    THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "kpixmapmodifier.h"

#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QSize>

namespace {
    /** Helper class for drawing frames for KPixmapModifier::applyFrame(). */
    class TileSet
    {
    public:
        enum { LeftMargin = 3, TopMargin = 2, RightMargin = 3, BottomMargin = 4 };

        enum Tile { TopLeftCorner = 0, TopSide, TopRightCorner, LeftSide,
                    RightSide, BottomLeftCorner, BottomSide, BottomRightCorner,
                    NumTiles };

        TileSet()
        {
            QImage image(8 * 3, 8 * 3, QImage::Format_ARGB32_Premultiplied);

            QPainter p(&image);
            p.setCompositionMode(QPainter::CompositionMode_Source);
            p.fillRect(image.rect(), Qt::transparent);
            p.fillRect(image.rect().adjusted(3, 3, -3, -3), Qt::black);
            p.end();

            QPixmap pixmap = QPixmap::fromImage(image);
            m_tiles[TopLeftCorner]     = pixmap.copy(0, 0, 8, 8);
            m_tiles[TopSide]           = pixmap.copy(8, 0, 8, 8);
            m_tiles[TopRightCorner]    = pixmap.copy(16, 0, 8, 8);
            m_tiles[LeftSide]          = pixmap.copy(0, 8, 8, 8);
            m_tiles[RightSide]         = pixmap.copy(16, 8, 8, 8);
            m_tiles[BottomLeftCorner]  = pixmap.copy(0, 16, 8, 8);
            m_tiles[BottomSide]        = pixmap.copy(8, 16, 8, 8);
            m_tiles[BottomRightCorner] = pixmap.copy(16, 16, 8, 8);
        }

        void paint(QPainter* p, const QRect& r)
        {
            p->drawPixmap(r.topLeft(), m_tiles[TopLeftCorner]);
            if (r.width() - 16 > 0) {
                p->drawTiledPixmap(r.x() + 8, r.y(), r.width() - 16, 8, m_tiles[TopSide]);
            }
            p->drawPixmap(r.right() - 8 + 1, r.y(), m_tiles[TopRightCorner]);
            if (r.height() - 16 > 0) {
                p->drawTiledPixmap(r.x(), r.y() + 8, 8, r.height() - 16,  m_tiles[LeftSide]);
                p->drawTiledPixmap(r.right() - 8 + 1, r.y() + 8, 8, r.height() - 16, m_tiles[RightSide]);
            }
            p->drawPixmap(r.x(), r.bottom() - 8 + 1, m_tiles[BottomLeftCorner]);
            if (r.width() - 16 > 0) {
                p->drawTiledPixmap(r.x() + 8, r.bottom() - 8 + 1, r.width() - 16, 8, m_tiles[BottomSide]);
            }
            p->drawPixmap(r.right() - 8 + 1, r.bottom() - 8 + 1, m_tiles[BottomRightCorner]);

            const QRect contentRect = r.adjusted(LeftMargin + 1, TopMargin + 1,
                                                 -(RightMargin + 1), -(BottomMargin + 1));
            p->fillRect(contentRect, Qt::transparent);
        }

        QPixmap m_tiles[NumTiles];
    };
}

void KPixmapModifier::scale(QPixmap& pixmap, const QSize& scaledSize)
{
    if (scaledSize.isEmpty()) {
        pixmap = QPixmap();
        return;
    }

    pixmap = pixmap.scaled(scaledSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void KPixmapModifier::applyFrame(QPixmap& icon, const QSize& scaledSize)
{
    static TileSet tileSet;

    // Resize the icon to the maximum size minus the space required for the frame
    const QSize size(scaledSize.width() - TileSet::LeftMargin - TileSet::RightMargin,
                     scaledSize.height() - TileSet::TopMargin - TileSet::BottomMargin);
    scale(icon, size);

    QPixmap framedIcon(icon.size().width() + TileSet::LeftMargin + TileSet::RightMargin,
                       icon.size().height() + TileSet::TopMargin + TileSet::BottomMargin);
    framedIcon.fill(Qt::transparent);

    QPainter painter;
    painter.begin(&framedIcon);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    tileSet.paint(&painter, framedIcon.rect());
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawPixmap(TileSet::LeftMargin, TileSet::TopMargin, icon);

    icon = framedIcon;
}

QSize KPixmapModifier::sizeInsideFrame(const QSize& frameSize)
{
    return QSize(frameSize.width() - TileSet::LeftMargin - TileSet::RightMargin,
                 frameSize.height() - TileSet::TopMargin - TileSet::BottomMargin);
}

