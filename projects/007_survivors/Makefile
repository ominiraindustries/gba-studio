#---------------------------------------------------------------------------------------------------------------------
# Butano game Makefile (copied from engine/butano/template/Makefile; see the Butano docs for every option).
# Build from Git Bash:  source ../../tools/env.sh && make -j8      (or double-click compilar.bat)
# TARGET is the folder name: projects/000_hello -> 000_hello.gba + 000_hello.elf (symbols for gbarun.py).
#---------------------------------------------------------------------------------------------------------------------
TARGET      	:=  $(notdir $(CURDIR))
BUILD       	:=  build
LIBBUTANO   	:=  ../../engine/butano/butano
BUTANOCOMMON	:=  $(LIBBUTANO)/../common
PYTHON      	:=  python
SOURCES     	:=  src $(BUTANOCOMMON)/src
INCLUDES    	:=  include $(BUTANOCOMMON)/include
DATA        	:=
GRAPHICS    	:=  graphics $(BUTANOCOMMON)/graphics
AUDIO       	:=  audio
AUDIOBACKEND	:=  maxmod
AUDIOTOOL		:=
DMGAUDIO    	:=  dmg_audio
DMGAUDIOBACKEND	:=  default
ROMTITLE    	:=  HEXBOUND
ROMCODE     	:=  2HXE
USERFLAGS   	:=  -DBN_CFG_SPRITES_MAX_ITEMS=192 -DBN_CFG_SPRITE_TILES_MAX_ITEMS=256
USERCXXFLAGS	:=
USERASFLAGS 	:=
USERLDFLAGS 	:=
USERLIBDIRS 	:=
USERLIBS    	:=
DEFAULTLIBS 	:=
STACKTRACE		:=
USERBUILD   	:=
EXTTOOL     	:=

ifndef LIBBUTANOABS
	export LIBBUTANOABS	:=	$(realpath $(LIBBUTANO))
endif

ifeq ($(LIBBUTANOABS),)
$(error Butano not found at $(LIBBUTANO): run python -I tools/scripts/install_toolchain.py)
endif

include $(LIBBUTANOABS)/butano.mak
