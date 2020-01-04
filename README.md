<h1>wlsrun -- a command-line utility for running Windows applications from the WSL Bash prompt</h1>

<b>Please note that this utility was developed for an early release of Windows 10. I no longer use Windows enough to fix it, if it doesn't work with later
releases. So I'm afraid that, as of 2018, it has to be regarded as
unmaintained. Sorry.</b>

<h2>What is this?</h2>

Here is an example of the kind of problem that <code>wslrun</code> is intended to solve. I
have the Windows Subsystem for Linux (WSL) installed on Windows 10, and spend a lot of time 
at the Bash command prompt. I would like to be able to run Windows utilities, as well as Linux utilities,
from the command line, but
the file pathnames are completely inappropriate. For example, I might like to run SMPlayer on a directory
full of music files like this:

<pre class="codeblock">
$ /mnt/c/Program\ Files/SMPlayer/smplayer media/audio/Bach/*.flac
</pre>

This won't work, because SMPlayer won't be able to make any sense of the Linux pathnames it is presented with.
It won't even know where <code>/mnt</code> is.
<p/>
However, this will (should) work:

<pre class="codeblock">
$ wslrun /mnt/c/Program\ Files/SMPlayer/smplayer media/audio/Bach/*.flac
</pre>

Better still, I can create a script in, say, <code>/usr/bin</code> called <code>smplayer</code>, with the
following content:

<pre class="codeblock">
#!/bin/bash
$ wslrun /mnt/c/Program\ Files/SMPlayer/smplayer "$@" 
</pre>

Then I can just run SMPlayer like this:

<pre class="codeblock">
$ smplayer media/audio/Bach/*.flac
</pre>

This should work whether I specify a directory, a set of files, or a single file. It will also work if I
specify an HTTP URI, because <code>wslrun</code> will not attempt to translate a server URI (and, 
of course, it can't).
<p/>
<code>wslrun</code> is implemented in C, and has a binary size of only a few kB. The memory size at runtime
is unknown, because that depends on technical details of WSL that are not available. It uses <code>exec</code>
to replace itself in memory with the program being executed, and Windows <i>seems</i> to respect that. 
In any event, <code>wslrun</code>'s memory overhead is likely to be much smaller than that of a solution using
Perl or Python.

<h2>How it works</h2>

<code>wslrun</code> examines each argument on the command line and, if it can be converted to a well-formed, 
absolute pathname, it is converted. The converted pathname is then compared with the pattern <code>/mnt/X/</code> 
where X is a drive letter, and the <code>/mnt/X/</code> part converted to <code>X:/</code>. Other 
directory separators are converted from "/" to "\". Arguments that cannot be converted are passed as-is to the
program being launched. 
<p/>
This whole process is pretty crude, but seems to work in a surprising number of cases. Windows applications
don't usually take command-line switches but, where they do, there usually isn't a problem even if they
start with a "/", as is conventional in Windows. Potentially these arguments could match directory names, 
but they are unlikely to do so in practice. 

<h2>Building</h2>

To build <code>wlsrun</code> you will need the GCC C compiler (e.g., <code>sudo apt-get-install gcc</code>,)
and associated utilities, particularly <code>make</code>. 

<pre class="codeblock">
$ make
$ sudo make install
</pre>

There are some sample scripts that use <code>wslrun</code> in the <code>examples/</code> directory. 
These scripts should not normally be installed without checking their suitability.

<h2>Usage</h2>

<code>wslrun</code> should be invoked using the path to the executable to run, and any command-line arguments,
in Unix format. In practice, the command will normally be used in a script, with the pattern <code>"$@"</code>
to indicate passing all command-line arguments straight through.
<p/>
Setting the environment variable <code>WSLRUN_DEBUG</code> (to anything) will cause the program to display
the arguments it is processing, and what it makes of them. This can be useful for working out why things
don't work as they should.

<h2>Limitations</h2>

Fundamentally, <code>wslrun</code> is a kludge. It works on the assumption that anything on the command line
which can be interpreted as a valid filename is, in fact, a filename. This is very often the case, but it
can't be guaranteed. 
<p/>
<code>wlsrun</code> is designed to be used in an environment where files are shared between Linux and Windows.
Where that isn't the case, there is perhaps no need for a utility of this kind. Needful or not, Microsoft 
has repeatedly warned us that Windows tools should not be used to edit files in the Linux filesystem itself, 
that this will lead to data corruption. In practice, directories under <code>$HOME</code> should normally be symlinks
to directories under <code>/mnt</code>, if these files are to be shared. WSL does not expect these files
to contain Linux-specific meta-data, so WSL will not break if Windows utilities damage any such meta-data.
On the other hand, files in the Linux filesystem area <i>do</i> have Linux-specific meta-data, which needs to
be kept intact.
<p/>
Consequently, <code>wlsrun</code> will not attempt to re-write file locations whose real pathname -- after resolving
symlinks -- does not refer to a location under <code>/mnt</code>.

<h2>Legal, etc</h2>

<code>wslrun</code> is copyright (c)2017 Kevin Boone, distributed
according to the GNU Public Licence, version 3
The licensing terms mean in essence that 
you may do whatever you like with the software, at your own risk, provided
that the original authorship remains clear. There is no warranty of
any kind. I'm publishing this, as ever, in the hope that it will be of some
small use to other people; but I make no promises that it won't be useless,
or even dangerous.  


