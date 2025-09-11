# mmm
A geany plugin with small tools like inline math, graph creating, unit converstion 

mmm plugin
=====================

About
-----
mmm should become a plugin that helps people with for example math, graphs unit convertion in geany.


Requirements
------------
for compiling the dislin library needs to be available on the system


Installation
------------

adjust the following lines in the meson.build to install in the correct path
maybe you have an extra plugin folder activated in geany, so you want it to install in that one

#install_dir : join_paths(get_option('prefix'), get_option('libdir'), 'geany')
install_dir : join_paths('~/tools/geanyplugins')

depending where to install change the prefix accordingly

meson setup builddir (-Dprefix=/usr) << Dprefix only if install_dir with prefix>>
cd builddir
ninja
sudo ninja install


Usage
-----



Known issues
------------



License
-------
mmm is distributed under the terms of the GNU General Public
License as published by the Free Software Foundation; either version 2 of
the License, or (at your option) any later version. A copy of this license
can be found in the file COPYING included with the source code of this
program.

----
zlib License

Copyright (C) 2015, 2016 Lewis Van Winkle

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgement in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
----


Ideas, questions, patches and bug reports
-----------------------------------------
