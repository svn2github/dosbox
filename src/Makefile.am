AM_CPPFLAGS = -I$(top_srcdir)/include

SUBDIRS = cpu debug dos fpu gui hardware libs ints misc shell platform 

bin_PROGRAMS = dosbox dosprog

if HAVE_WINDRES
ico_stuff = winres.rc
endif

.rc.o:
	$(WINDRES) -o $@ $<

dosbox_SOURCES = dosbox.cpp $(ico_stuff)
dosbox_LDADD = cpu/libcpu.a debug/libdebug.a dos/libdos.a fpu/libfpu.a  hardware/libhardware.a gui/libgui.a \
               ints/libints.a misc/libmisc.a shell/libshell.a hardware/mame/libmame.a \
               hardware/serialport/libserial.a libs/gui_tk/libgui_tk.a

dosprog_SOURCES = $(dosbox_SOURCES)
dosprog_LDADD = cpu/libcpu.a debug/libdebug.a dos/libdos.a fpu/libfpu.a  hardware/libhardware.a gui/libgui_prog.a \
               ints/libints.a misc/libmisc.a shell/libshell.a hardware/serialport/libserial.a libs/gui_tk/libgui_tk.a

EXTRA_DIST = winres.rc dosbox.ico
