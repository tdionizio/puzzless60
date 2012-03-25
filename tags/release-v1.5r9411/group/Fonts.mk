
ifeq (WINS,$(findstring WINS, $(PLATFORM)))
ZDIR=$(EPOCROOT)epoc32\release\$(PLATFORM)\$(CFG)\Z
else
ZDIR=$(EPOCROOT)epoc32\data\z
endif

DSTDIR=$(ZDIR)\resource\apps\0xa000ef77
SRCDIR=..\fonts

DSTFONTS=$(DSTDIR)\Vera.ttf $(DSTDIR)\VeraBd.ttf $(DSTDIR)\VeraMono.ttf $(DSTDIR)\VeraMoBd.ttf
SRCFONTS=$(SRCDIR)\Vera.ttf $(SRCDIR)\VeraBd.ttf $(SRCDIR)\VeraMono.ttf $(SRCDIR)\VeraMoBd.ttf

do_nothing :
	@rem do_nothing

MAKMAKE : do_nothing

BLD : do_nothing

CLEAN :
	@echo ...Deleting Fonts $(DSTFONTS)
	del /q /f $(DSTFONTS)

LIB : do_nothing

CLEANLIB : do_nothing

# ----------------------------------------------------------------------------
# NOTE: if you have JUSTINTIME enabled for your S60 3rd FP1 or newer SDK
# and this command crashes, consider adding "/X" to the command line.
# See <http://forum.nokia.com/document/Forum_Nokia_Technical_Library_v1_35/contents/FNTL/Build_process_fails_at_mif_file_creation_in_S60_3rd_Ed_FP1_SDK.htm>
# ----------------------------------------------------------------------------

RESOURCE : $(DSTFONTS)

$(DSTFONTS) : $(SRCFONTS)
	-mkdir $(DSTDIR)
	copy $(SRCDIR)\Vera.ttf $(DSTDIR)\Vera.ttf
	copy $(SRCDIR)\VeraBd.ttf $(DSTDIR)\VeraBd.ttf
	copy $(SRCDIR)\VeraMono.ttf $(DSTDIR)\VeraMono.ttf
	copy $(SRCDIR)\VeraMoBd.ttf $(DSTDIR)\VeraMoBd.ttf

FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo 

FINAL : do_nothing

