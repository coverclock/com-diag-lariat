com-diag-lariat
===============

Copyright 2011-2014 by the Digital Aggregates Corporation, Colorado, USA.

LICENSE

This package is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

As a special exception, if other files instantiate templates or
use macros or inline functions from this file, or you compile
this file and link it with other works to produce a work based on
this file, this file does not by itself cause the resulting work
to be covered by the GNU Lesser General Public License. However
the source code for this file must still be made available in
accordance with the GNU Lesser General Public License.

This exception does not invalidate any other reasons why a work
based on this package might be covered by the GNU Lesser General
Public License.

This package is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, contact

    Free Software Foundation, Inc.
    59 Temple Place, Suite 330
    Boston MA 02111-1307 USA
    http://www.gnu.org/copyleft/lesser.txt

Alternative commercial licensing terms are available from the copyright
holder.

ABSTRACT

This file is part of the Digital Aggregates Corporation Lariat package.
Lariat provides some minor additional functionality on top of Google Test
and Google Mock which has proven useful in automated unit test environments.
This license does not apply to Google Test itself, only to the additions
provided by Lariat. Lariat has legitimately found its way into the unit
test environments of some of the Corporation's clients, so don't be
surprised if it crops up in your travels elsewhere.

This software is an original work of its author(s).

Information regarding Lariat can be found here:

    http://www.diag.com/navigation/downloads/Lariat.html

CONTACT

    Chip Overclock
    Digital Aggregates Corporation
    3440 Youngfield Street, #209
    Wheat Ridge CO 80033 USA
    http://www.diag.com
    mailto:coverclock@diag.com

BUILD

    cd ~/src
    git clone https://github.com/google/googletest
    ( cd googletest; mkdir out; cd out; cmake -Dgtest_build_samples=ON -Dgtest_build_tests=ON ..; make; make test )
    git clone https://github.com/coverclock/com-diag-lariat
    make -C com-diag-lariat/Lariat pristine all
