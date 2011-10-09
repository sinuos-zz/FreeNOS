#
# Copyright (C) 2010 Niek Linnenbank
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

from SCons.Script import *
from autoconf import *

#
# Prepares a given environment, by adding library dependencies.
#
def UseLibraries(env, libs = []):

    # Loop all required libraries.
    for lib in libs:

	# Add them to the include and linker path.
	env.Append(CPPPATH = [ '#lib/' + lib ])
	env.Append(LIBPATH = [ '#' + env['BUILDROOT'] + '/lib/' + lib ])
	env.Append(LIBS    = [ lib ])

# Create target, host and kernel environments.
target = Environment(tools    = ["default", "bootimage", "iso", "binary"],
		     toolpath = ["site_scons"])
host   = Environment()
kernel = None

# Provide the UseLibraries method.
target.AddMethod(UseLibraries, "UseLibraries")
host.AddMethod(UseLibraries, "UseLibraries")

# Global top-level configuration.
global_vars = Variables('build.conf')
global_vars.Add('VERSION', 'FreeNOS release version number')
global_vars.Add(EnumVariable('ARCH', 'Target machine CPU architecture', 'x86',
			      allowed_values = ('x86', 'mips')))
global_vars.Add(EnumVariable('SYSTEM', 'Target machine system type', 'pc',
			      allowed_values = ('pc', 'mipssim', 'adm5120')))
global_vars.Add(PathVariable('COMPILER', 'Target compiler chain', None))
global_vars.Add('BUILDROOT','Object output directory')
global_vars.Add(BoolVariable('VERBOSE', 'Output verbose compilation commands', False))
global_vars.Update(target)
global_vars.Update(host)

# System-specific configuration.
system_vars = Variables(target['COMPILER'])
system_vars.Add('CC', 'C Compiler')
system_vars.Add('AS', 'Assembler')
system_vars.Add('LD', 'Linker')
system_vars.Add('CFLAGS', 'C Compiler flags')
system_vars.Add('ASFLAGS', 'Assembler flags')
system_vars.Add('LINKFLAGS', 'Linker flags')
system_vars.Add('LINKKERN', 'Linker flags for the kernel linker script')
system_vars.Add('LINKUSER', 'Linker flags for user programs linker script')
system_vars.Add('CPPPATH', 'C Preprocessor include directories')
system_vars.Update(target)

# Enables verbose compilation command output.
if not target['VERBOSE']:
    target['CCCOMSTR']     = host['CCCOMSTR']     = "  CC  $TARGET"
    target['ASCOMSTR']     = host['ASCOMSTR']     = "  AS  $TARGET"
    target['ASPPCOMSTR']   = host['ASPPCOMSTR']   = "  AS  $TARGET"
    target['ARCOMSTR']     = host['ARCOMSTR']     = "  AR  $TARGET"
    target['RANLIBCOMSTR'] = host['RANLIBCOMSTR'] = "  LIB $TARGET"
    target['LINKCOMSTR']   = host['LINKCOMSTR']   = "  LD  $TARGET"

# Verify the configured CFLAGS.
if not GetOption('clean'):
    CheckCFlags(target)
    CheckCFlags(host)

# Kernel environment is the same as target, except for linker scripts.
kernel = target.Clone()

# Append linker script flags.
kernel.Append(LINKFLAGS = target['LINKKERN'])
target.Append(LINKFLAGS = target['LINKUSER'])

# Host environment uses a different output directory.
host.Replace(ARCH      = 'host')
host.Replace(SYSTEM    = 'host')
host.Replace(BUILDROOT = 'build/host')
host.Append (CPPFLAGS  = '-DHOST')
host.Append (CPPPATH   = [ '#include' ])

# Provide configuration help.
Help(global_vars.GenerateHelpText(target))
Help(system_vars.GenerateHelpText(target))

# Make a symbolic link to the system-specific headers.
try:
    os.unlink("include/FreeNOS")
except:
    pass

try:
    os.symlink(target['ARCH'], "include/FreeNOS")
except:
    pass

