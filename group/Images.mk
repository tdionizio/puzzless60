ifeq (WINS,$(findstring WINS, $(PLATFORM)))
ZDIR=$(EPOCROOT)epoc32\release\$(PLATFORM)\$(CFG)\Z
else
ZDIR=$(EPOCROOT)epoc32\data\z
endif

TARGETDIR=$(ZDIR)\resource\apps\0xa000ef77
ICONTARGETFILENAME=$(TARGETDIR)\games.mif

HEADERDIR=$(EPOCROOT)epoc32\include
HEADERFILENAME=$(HEADERDIR)\puzzles_0xa000ef77.mbg

ICONDIR=..\gfx

do_nothing :
	@rem do_nothing

MAKMAKE : do_nothing

BLD : do_nothing

CLEAN : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE: $(ICONTARGETFILENAME)

C24=/c24
C24=

$(ICONTARGETFILENAME): Images.mk \
		$(ICONDIR)\rect.bmp \
		$(ICONDIR)\pegs.bmp \
		$(ICONDIR)\keen.bmp \
		$(ICONDIR)\mines.bmp \
		$(ICONDIR)\inertia.bmp \
		$(ICONDIR)\fifteen.bmp \
		$(ICONDIR)\twiddle.bmp \
		$(ICONDIR)\cube.bmp \
		$(ICONDIR)\netslide.bmp \
		$(ICONDIR)\dominosa.bmp \
		$(ICONDIR)\blackbox.bmp \
		$(ICONDIR)\lightup.bmp \
		$(ICONDIR)\unequal.bmp \
		$(ICONDIR)\tents.bmp \
		$(ICONDIR)\galaxies.bmp \
		$(ICONDIR)\guess.bmp \
		$(ICONDIR)\flip.bmp \
		$(ICONDIR)\loopy.bmp \
		$(ICONDIR)\pattern.bmp \
		$(ICONDIR)\solo.bmp \
		$(ICONDIR)\sixteen.bmp \
		$(ICONDIR)\filling.bmp \
		$(ICONDIR)\net.bmp \
		$(ICONDIR)\slant.bmp \
		$(ICONDIR)\samegame.bmp \
		$(ICONDIR)\map.bmp \
		$(ICONDIR)\bridges.bmp \
		$(ICONDIR)\towers.bmp \
		$(ICONDIR)\magnets.bmp \
		$(ICONDIR)\singles.bmp \
		$(ICONDIR)\signpost.bmp \
		$(ICONDIR)\range.bmp \
		$(ICONDIR)\pearl.bmp \
		$(ICONDIR)\untangle.bmp
	mifconv $(ICONTARGETFILENAME)  \
		 /H$(HEADERFILENAME)  \
		 /c24 $(ICONDIR)\blackbox.bmp  \
		 $(C24) $(ICONDIR)\bridges.bmp  \
		 $(C24) $(ICONDIR)\cube.bmp  \
		 $(C24) $(ICONDIR)\dominosa.bmp  \
		 $(C24) $(ICONDIR)\fifteen.bmp  \
		 $(C24) $(ICONDIR)\filling.bmp  \
		 $(C24) $(ICONDIR)\flip.bmp  \
		 $(C24) $(ICONDIR)\galaxies.bmp  \
		 $(C24) $(ICONDIR)\guess.bmp  \
		 $(C24) $(ICONDIR)\inertia.bmp  \
		 $(C24) $(ICONDIR)\keen.bmp  \
		 $(C24) $(ICONDIR)\lightup.bmp  \
		 $(C24) $(ICONDIR)\loopy.bmp  \
		 $(C24) $(ICONDIR)\magnets.bmp  \
		 $(C24) $(ICONDIR)\map.bmp  \
		 $(C24) $(ICONDIR)\mines.bmp  \
		 $(C24) $(ICONDIR)\net.bmp  \
		 $(C24) $(ICONDIR)\netslide.bmp  \
		 $(C24) $(ICONDIR)\pattern.bmp  \
		 $(C24) $(ICONDIR)\pearl.bmp \
		 $(C24) $(ICONDIR)\pegs.bmp  \
		 $(C24) $(ICONDIR)\range.bmp  \
		 $(C24) $(ICONDIR)\rect.bmp  \
		 $(C24) $(ICONDIR)\samegame.bmp  \
		 $(C24) $(ICONDIR)\signpost.bmp  \
		 $(C24) $(ICONDIR)\singles.bmp  \
		 $(C24) $(ICONDIR)\sixteen.bmp  \
		 $(C24) $(ICONDIR)\slant.bmp  \
		 $(C24) $(ICONDIR)\solo.bmp  \
		 $(C24) $(ICONDIR)\tents.bmp  \
		 $(C24) $(ICONDIR)\towers.bmp  \
		 $(C24) $(ICONDIR)\twiddle.bmp  \
		 $(C24) $(ICONDIR)\unequal.bmp  \
		 $(C24) $(ICONDIR)\untangle.bmp
	
FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo $(HEADERFILENAME)&& \
	@echo $(ICONTARGETFILENAME) \
	@echo $(OTHERICONTARGETFILENAME)

FINAL : do_nothing
