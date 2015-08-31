/*
 *  Copyright (c) 2003 Lubos Lunak <l.lunak@kde.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "konqueror.h"

#include <kconfiggroup.h>
#include <QDBusMessage>
#include <QDBusConnection>
#include <QCheckBox>
#include <klocale.h>

namespace KCMPerformance
{

Konqueror::Konqueror( QWidget* parent_P )
    : Konqueror_ui( parent_P )
    {
    rb_never_reuse->setWhatsThis(
        i18n( "Disables the minimization of memory usage and allows you "
              "to make each browsing activity independent from the others" ));
    rb_file_browsing_reuse->setWhatsThis(
        i18n( "<p>With this option activated, only one instance of Konqueror "
              "used for file browsing will exist in the memory of your computer "
              "at any moment, "
              "no matter how many file browsing windows you open, "
              "thus reducing resource requirements.</p>"
              "<p>Be aware that this also means that, if something goes wrong, "
              "all your file browsing windows will be closed simultaneously</p>" ));
    rb_always_reuse->setWhatsThis(
        i18n( "<p>With this option activated, only one instance of Konqueror "
              "will exist in the memory of your computer at any moment, "
              "no matter how many browsing windows you open, "
              "thus reducing resource requirements.</p>"
              "<p>Be aware that this also means that, if something goes wrong, "
              "all your browsing windows will be closed simultaneously.</p>" ));
    connect( rb_never_reuse, SIGNAL(toggled(bool)), SIGNAL(changed()));
    connect( rb_file_browsing_reuse, SIGNAL(toggled(bool)), SIGNAL(changed()));
    connect( rb_always_reuse, SIGNAL(toggled(bool)), SIGNAL(changed()));
    rb_file_browsing_reuse->setChecked( true );

    defaults();
    }

void Konqueror::load()
    {
    KConfig _cfg( "konquerorrc" );
    KConfigGroup cfg(&_cfg, "Reusing" );
    allowed_parts = cfg.readEntry( "SafeParts", "SAFE" );
    if( allowed_parts == "ALL" )
        rb_always_reuse->setChecked( true );
    else if( allowed_parts.isEmpty())
        rb_never_reuse->setChecked( true );
    else
        rb_file_browsing_reuse->setChecked( true );
    }

void Konqueror::save()
    {
    KConfig _cfg( "konquerorrc" );
    KConfigGroup cfg(&_cfg, "Reusing" );
    if( rb_always_reuse->isChecked())
        allowed_parts = "ALL";
    else if( rb_never_reuse->isChecked())
        allowed_parts = "";
    else
        {
        if( allowed_parts.isEmpty() || allowed_parts == "ALL" )
            allowed_parts = "SAFE";
        // else - keep allowed_parts as read from the file, as the user may have modified the list there
        }
    cfg.writeEntry( "SafeParts", allowed_parts );
    cfg.sync();
    QDBusMessage message =
        QDBusMessage::createSignal("/KonqMain", "org.kde.Konqueror.Main", "reparseConfiguration");
    QDBusConnection::sessionBus().send(message);
    }

void Konqueror::defaults()
    {
    rb_file_browsing_reuse->setChecked( true );
    allowed_parts = "SAFE";
    }

} // namespace

#include "moc_konqueror.cpp"
