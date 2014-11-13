/*
    Copyright 2008 by Robert Knight <robertknight@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301  USA.
*/

#ifndef PTYTEST_H
#define PTYTEST_H

#include "../Pty.h"

namespace Konsole
{

class PtyTest : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void testFlowControl();
    void testEraseChar();
    void testUseUtmp();
    void testWindowSize();

    void testRunProgram();
};

}

#endif // PTYTEST_H

