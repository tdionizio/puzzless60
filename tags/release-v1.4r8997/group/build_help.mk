
ifeq (WINS,$(findstring WINS, $(PLATFORM)))
ZDIR=$(EPOCROOT)epoc32\release\$(PLATFORM)\$(CFG)\Z
else
ZDIR=$(EPOCROOT)epoc32\data\z
endif

DSTDIR=$(ZDIR)\resource\apps\0xa000ef77
SRCDIR=..\help

do_nothing :
	@rem do_nothing

# build the help from the MAKMAKE step so the header file generated
# will be found by cpp.exe when calculating the dependency information
# in the mmp makefiles.

MAKMAKE : do_nothing

BLD : do_nothing

CLEAN : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE : $(DSTDIR)\index.html

$(DSTDIR)\index.html : $(SRCDIR)\puzzles.but $(SRCDIR)\html\index.html
	echo "Copying elp files (emulator)"
ifeq (WINSCW,$(findstring WINSCW, $(PLATFORM)))
	echo "Copy to $(ZDIR)"
	-mkdir $(DSTDIR)
	-del $(DSTDIR)\*.html
	copy $(SRCDIR)\html\*.html $(DSTDIR)
endif

		
FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES : do_nothing

FINAL : do_nothing
