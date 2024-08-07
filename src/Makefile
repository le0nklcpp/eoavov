CXXFLAGS= -O3 -fomit-frame-pointer -ffast-math -Wno-reorder
override CXXFLAGS+= -Wall -fsigned-char -fno-exceptions -fno-rtti

PLATFORM= $(shell uname -s)
PLATFORM_ARCH= $(shell uname -m)
PLATFORM_PREFIX= native

INCLUDES= -Ishared -Iengine -Ienet/include

STRIP=
ifeq (,$(findstring -g,$(CXXFLAGS)))
ifeq (,$(findstring -pg,$(CXXFLAGS)))
  STRIP=strip
endif
endif

MV=mv

ifneq (,$(findstring MINGW,$(PLATFORM)))
WINDRES= windres
ifneq (,$(findstring 64,$(PLATFORM)))
ifneq (,$(findstring CROSS,$(PLATFORM)))
  CXX=x86_64-w64-mingw32-g++
  WINDRES=x86_64-w64-mingw32-windres
ifneq (,$(STRIP))
  STRIP=x86_64-w64-mingw32-strip
endif
endif
WINLIB=lib64
WINBIN=../bin64
override CXX+= -m64
override WINDRES+= -F pe-x86-64
else
ifneq (,$(findstring CROSS,$(PLATFORM)))
  CXX=i686-w64-mingw32-g++
  WINDRES=i686-w64-mingw32-windres
ifneq (,$(STRIP))
  STRIP=i686-w64-mingw32-strip
endif
endif
WINLIB=lib
WINBIN=../bin
override CXX+= -m32
override WINDRES+= -F pe-i386
endif
CLIENT_INCLUDES= $(INCLUDES) -Iinclude
STD_LIBS= -static-libgcc -static-libstdc++
CLIENT_LIBS= -mwindows $(STD_LIBS) -L$(WINBIN) -L$(WINLIB) -lSDL2 -lSDL2_image -lSDL2_mixer -lz -lopengl32 -lenet -lws2_32 -lwinmm
else	
CLIENT_INCLUDES= $(INCLUDES) -I/usr/X11R6/include `sdl2-config --cflags`
CLIENT_LIBS= -Lenet -lenet -L/usr/X11R6/lib -lX11 `sdl2-config --libs` -lSDL2_image -lSDL2_mixer -lz -lGL 
endif
ifeq ($(PLATFORM),Linux)
CLIENT_LIBS+= -lrt
else
ifneq (,$(findstring GNU,$(PLATFORM))) 
CLIENT_LIBS+= -lrt 
endif 
endif
MOD_OBJS= \
	game/acoustic.o \
	game/client.o \
	game/creatures.o \
	game/entities.o \
	game/eoavov.o \
	game/fpsentity.o \
	game/hudmessage.o \
	game/inventory.o \
	game/prop.o \
	game/render.o \
	game/envzones.o \
	game/task.o \
	game/player.o \
	game/dict.o \
	game/hudtexture.o \
	game/rail.o \
	game/projectile.o \
	game/debris.o \
	game/arrayassoc.o \
        game/waypoint.o
CLIENT_OBJS= \
	shared/crypto.o \
	shared/geom.o \
	shared/glemu.o \
	shared/stream.o \
	shared/tools.o \
	shared/zip.o \
	engine/aa.o \
	engine/bih.o \
	engine/blend.o \
	engine/client.o	\
	engine/command.o \
	engine/console.o \
	engine/dynlight.o \
	engine/grass.o \
	engine/light.o \
	engine/main.o \
	engine/material.o \
	engine/menus.o \
	engine/movie.o \
	engine/normal.o	\
	engine/octa.o \
	engine/octaedit.o \
	engine/octarender.o \
	engine/physics.o \
	engine/pvs.o \
	engine/rendergl.o \
	engine/renderlights.o \
	engine/rendermodel.o \
	engine/renderparticles.o \
	engine/rendersky.o \
	engine/rendertext.o \
	engine/renderva.o \
	engine/server.o	\
	engine/serverbrowser.o \
	engine/shader.o \
	engine/sound.o \
	engine/stain.o \
	engine/texture.o \
	engine/ui.o \
	engine/water.o \
	engine/world.o \
	engine/worldio.o \
	$(MOD_OBJS)
CLIENT_PCH= shared/cube.h.gch engine/engine.h.gch

default: all

all: client

clean:
	-$(RM) $(CLIENT_PCH) $(CLIENT_OBJS) eoavov
	$(MAKE) -C enet clean
clearmod:
	-$(RM) $(MOD_OBJS) eoavov
fixspace:
	sed -i 's/[ \t]*$$//; :rep; s/^\([ ]*\)\t/\1    /g; trep' shared/*.c shared/*.cpp shared/*.h engine/*.cpp engine/*.h eoavov/game/*.cpp eoavov/game/*.h

$(filter-out shared/%,$(CLIENT_PCH)): $(filter shared/%,$(CLIENT_PCH))
$(CLIENT_PCH): %.h.gch: %.h
	$(CXX) $(CXXFLAGS) -x c++-header -o $@.tmp $<
	$(MV) $@.tmp $@

$(CLIENT_OBJS): CXXFLAGS += $(CLIENT_INCLUDES)
$(filter shared/%,$(CLIENT_OBJS)): $(filter shared/%,$(CLIENT_PCH))
$(filter engine/%,$(CLIENT_OBJS)): $(filter engine/%,$(CLIENT_PCH))

# $(filter-out standalone/shared/%,$(SERVER_PCH)): $(filter standalone/shared/%,$(SERVER_PCH))
# $(SERVER_PCH): standalone/%.h.gch: %.h
#	$(CXX) $(CXXFLAGS) -x c++-header -o $@.tmp $<
#	$(MV) $@.tmp $@

# $(SERVER_MASTER_OBJS): CXXFLAGS += $(SERVER_INCLUDES)
# $(filter standalone/shared/%,$(SERVER_MASTER_OBJS)): $(filter standalone/shared/%,$(SERVER_PCH))
# $(filter standalone/engine/%,$(SERVER_MASTER_OBJS)): $(filter standalone/engine/%,$(SERVER_PCH))
# $(filter standalone/game/%,$(SERVER_MASTER_OBJS)): $(filter standalone/game/%,$(SERVER_PCH))

# $(SERVER_MASTER_OBJS): standalone/%.o: %.cpp
#	$(CXX) $(CXXFLAGS) -c -o $@ $<

ifneq (,$(findstring MINGW,$(PLATFORM)))
client: $(CLIENT_OBJS)
	$(WINDRES) -I vcpp -i vcpp/mingw.rc -J rc -o vcpp/mingw.res -O coff 
	$(CXX) $(CXXFLAGS) -o $(WINBIN)/eoavov.exe vcpp/mingw.res -no-pie $(CLIENT_OBJS) $(CLIENT_LIBS)


install: all
else
client:	libenet $(CLIENT_OBJS)
	$(CXX) $(CXXFLAGS) -o eoavov -no-pie $(CLIENT_OBJS) $(CLIENT_LIBS)

shared/tessfont.o: shared/tessfont.c
	$(CXX) $(CXXFLAGS) -c -o $@ $< `freetype-config --cflags`

tessfont: shared/tessfont.o
	$(CXX) $(CXXFLAGS) -o tessfont shared/tessfont.o `freetype-config --libs` -lz

install: all
	cp -f eoavov	../bin_unix/$(PLATFORM_PREFIX)
ifneq (,$(STRIP))
	$(STRIP) ../bin_unix/$(PLATFORM_PREFIX)_client
endif
endif

enet/libenet.a:
	$(MAKE) -C enet
libenet: enet/libenet.a

depend:
	makedepend -Y -Ishared -Iengine  $(CLIENT_OBJS:.o=.cpp)
	makedepend -a -o.h.gch -Y -Ishared -Iengine  $(CLIENT_PCH:.h.gch=.h)


# DO NOT DELETE

shared/crypto.o: shared/cube.h shared/tools.h shared/geom.h shared/ents.h
shared/crypto.o: shared/command.h shared/glexts.h shared/glemu.h
shared/crypto.o: shared/iengine.h shared/igame.h
shared/geom.o: shared/cube.h shared/tools.h shared/geom.h shared/ents.h
shared/geom.o: shared/command.h shared/glexts.h shared/glemu.h
shared/geom.o: shared/iengine.h shared/igame.h
shared/glemu.o: shared/cube.h shared/tools.h shared/geom.h shared/ents.h
shared/glemu.o: shared/command.h shared/glexts.h shared/glemu.h
shared/glemu.o: shared/iengine.h shared/igame.h
shared/stream.o: shared/cube.h shared/tools.h shared/geom.h shared/ents.h
shared/stream.o: shared/command.h shared/glexts.h shared/glemu.h
shared/stream.o: shared/iengine.h shared/igame.h
shared/tools.o: shared/cube.h shared/tools.h shared/geom.h shared/ents.h
shared/tools.o: shared/command.h shared/glexts.h shared/glemu.h
shared/tools.o: shared/iengine.h shared/igame.h
shared/zip.o: shared/cube.h shared/tools.h shared/geom.h shared/ents.h
shared/zip.o: shared/command.h shared/glexts.h shared/glemu.h
shared/zip.o: shared/iengine.h shared/igame.h
engine/aa.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/aa.o: shared/ents.h shared/command.h shared/glexts.h shared/glemu.h
engine/aa.o: shared/iengine.h shared/igame.h engine/world.h engine/octa.h
engine/aa.o: engine/light.h engine/texture.h engine/bih.h engine/model.h
engine/bih.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/bih.o: shared/ents.h shared/command.h shared/glexts.h shared/glemu.h
engine/bih.o: shared/iengine.h shared/igame.h engine/world.h engine/octa.h
engine/bih.o: engine/light.h engine/texture.h engine/bih.h engine/model.h
engine/blend.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/blend.o: shared/ents.h shared/command.h shared/glexts.h shared/glemu.h
engine/blend.o: shared/iengine.h shared/igame.h engine/world.h engine/octa.h
engine/blend.o: engine/light.h engine/texture.h engine/bih.h engine/model.h
engine/client.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/client.o: shared/ents.h shared/command.h shared/glexts.h
engine/client.o: shared/glemu.h shared/iengine.h shared/igame.h
engine/client.o: engine/world.h engine/octa.h engine/light.h engine/texture.h
engine/client.o: engine/bih.h engine/model.h
engine/command.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/command.o: shared/ents.h shared/command.h shared/glexts.h
engine/command.o: shared/glemu.h shared/iengine.h shared/igame.h
engine/command.o: engine/world.h engine/octa.h engine/light.h
engine/command.o: engine/texture.h engine/bih.h engine/model.h
engine/console.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/console.o: shared/ents.h shared/command.h shared/glexts.h
engine/console.o: shared/glemu.h shared/iengine.h shared/igame.h
engine/console.o: engine/world.h engine/octa.h engine/light.h
engine/console.o: engine/texture.h engine/bih.h engine/model.h
engine/dynlight.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/dynlight.o: shared/ents.h shared/command.h shared/glexts.h
engine/dynlight.o: shared/glemu.h shared/iengine.h shared/igame.h
engine/dynlight.o: engine/world.h engine/octa.h engine/light.h
engine/dynlight.o: engine/texture.h engine/bih.h engine/model.h
engine/grass.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/grass.o: shared/ents.h shared/command.h shared/glexts.h shared/glemu.h
engine/grass.o: shared/iengine.h shared/igame.h engine/world.h engine/octa.h
engine/grass.o: engine/light.h engine/texture.h engine/bih.h engine/model.h
engine/light.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/light.o: shared/ents.h shared/command.h shared/glexts.h shared/glemu.h
engine/light.o: shared/iengine.h shared/igame.h engine/world.h engine/octa.h
engine/light.o: engine/light.h engine/texture.h engine/bih.h engine/model.h
engine/main.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/main.o: shared/ents.h shared/command.h shared/glexts.h shared/glemu.h
engine/main.o: shared/iengine.h shared/igame.h engine/world.h engine/octa.h
engine/main.o: engine/light.h engine/texture.h engine/bih.h engine/model.h
engine/material.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/material.o: shared/ents.h shared/command.h shared/glexts.h
engine/material.o: shared/glemu.h shared/iengine.h shared/igame.h
engine/material.o: engine/world.h engine/octa.h engine/light.h
engine/material.o: engine/texture.h engine/bih.h engine/model.h
engine/menus.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/menus.o: shared/ents.h shared/command.h shared/glexts.h shared/glemu.h
engine/menus.o: shared/iengine.h shared/igame.h engine/world.h engine/octa.h
engine/menus.o: engine/light.h engine/texture.h engine/bih.h engine/model.h
engine/movie.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/movie.o: shared/ents.h shared/command.h shared/glexts.h shared/glemu.h
engine/movie.o: shared/iengine.h shared/igame.h engine/world.h engine/octa.h
engine/movie.o: engine/light.h engine/texture.h engine/bih.h engine/model.h
engine/normal.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/normal.o: shared/ents.h shared/command.h shared/glexts.h
engine/normal.o: shared/glemu.h shared/iengine.h shared/igame.h
engine/normal.o: engine/world.h engine/octa.h engine/light.h engine/texture.h
engine/normal.o: engine/bih.h engine/model.h
engine/octa.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/octa.o: shared/ents.h shared/command.h shared/glexts.h shared/glemu.h
engine/octa.o: shared/iengine.h shared/igame.h engine/world.h engine/octa.h
engine/octa.o: engine/light.h engine/texture.h engine/bih.h engine/model.h
engine/octaedit.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/octaedit.o: shared/ents.h shared/command.h shared/glexts.h
engine/octaedit.o: shared/glemu.h shared/iengine.h shared/igame.h
engine/octaedit.o: engine/world.h engine/octa.h engine/light.h
engine/octaedit.o: engine/texture.h engine/bih.h engine/model.h
engine/octarender.o: engine/engine.h shared/cube.h shared/tools.h
engine/octarender.o: shared/geom.h shared/ents.h shared/command.h
engine/octarender.o: shared/glexts.h shared/glemu.h shared/iengine.h
engine/octarender.o: shared/igame.h engine/world.h engine/octa.h
engine/octarender.o: engine/light.h engine/texture.h engine/bih.h
engine/octarender.o: engine/model.h
engine/physics.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/physics.o: shared/ents.h shared/command.h shared/glexts.h
engine/physics.o: shared/glemu.h shared/iengine.h shared/igame.h
engine/physics.o: engine/world.h engine/octa.h engine/light.h
engine/physics.o: engine/texture.h engine/bih.h engine/model.h engine/mpr.h
engine/pvs.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/pvs.o: shared/ents.h shared/command.h shared/glexts.h shared/glemu.h
engine/pvs.o: shared/iengine.h shared/igame.h engine/world.h engine/octa.h
engine/pvs.o: engine/light.h engine/texture.h engine/bih.h engine/model.h
engine/rendergl.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/rendergl.o: shared/ents.h shared/command.h shared/glexts.h
engine/rendergl.o: shared/glemu.h shared/iengine.h shared/igame.h
engine/rendergl.o: engine/world.h engine/octa.h engine/light.h
engine/rendergl.o: engine/texture.h engine/bih.h engine/model.h
engine/renderlights.o: engine/engine.h shared/cube.h shared/tools.h
engine/renderlights.o: shared/geom.h shared/ents.h shared/command.h
engine/renderlights.o: shared/glexts.h shared/glemu.h shared/iengine.h
engine/renderlights.o: shared/igame.h engine/world.h engine/octa.h
engine/renderlights.o: engine/light.h engine/texture.h engine/bih.h
engine/renderlights.o: engine/model.h
engine/rendermodel.o: engine/engine.h shared/cube.h shared/tools.h
engine/rendermodel.o: shared/geom.h shared/ents.h shared/command.h
engine/rendermodel.o: shared/glexts.h shared/glemu.h shared/iengine.h
engine/rendermodel.o: shared/igame.h engine/world.h engine/octa.h
engine/rendermodel.o: engine/light.h engine/texture.h engine/bih.h
engine/rendermodel.o: engine/model.h engine/ragdoll.h engine/animmodel.h
engine/rendermodel.o: engine/vertmodel.h engine/skelmodel.h engine/hitzone.h
engine/rendermodel.o: engine/md2.h engine/md3.h engine/md5.h engine/obj.h
engine/rendermodel.o: engine/smd.h engine/iqm.h
engine/renderparticles.o: engine/engine.h shared/cube.h shared/tools.h
engine/renderparticles.o: shared/geom.h shared/ents.h shared/command.h
engine/renderparticles.o: shared/glexts.h shared/glemu.h shared/iengine.h
engine/renderparticles.o: shared/igame.h engine/world.h engine/octa.h
engine/renderparticles.o: engine/light.h engine/texture.h engine/bih.h
engine/renderparticles.o: engine/model.h engine/explosion.h
engine/renderparticles.o: engine/lensflare.h engine/lightning.h
engine/rendersky.o: engine/engine.h shared/cube.h shared/tools.h
engine/rendersky.o: shared/geom.h shared/ents.h shared/command.h
engine/rendersky.o: shared/glexts.h shared/glemu.h shared/iengine.h
engine/rendersky.o: shared/igame.h engine/world.h engine/octa.h
engine/rendersky.o: engine/light.h engine/texture.h engine/bih.h
engine/rendersky.o: engine/model.h
engine/rendertext.o: engine/engine.h shared/cube.h shared/tools.h
engine/rendertext.o: shared/geom.h shared/ents.h shared/command.h
engine/rendertext.o: shared/glexts.h shared/glemu.h shared/iengine.h
engine/rendertext.o: shared/igame.h engine/world.h engine/octa.h
engine/rendertext.o: engine/light.h engine/texture.h engine/bih.h
engine/rendertext.o: engine/model.h
engine/renderva.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/renderva.o: shared/ents.h shared/command.h shared/glexts.h
engine/renderva.o: shared/glemu.h shared/iengine.h shared/igame.h
engine/renderva.o: engine/world.h engine/octa.h engine/light.h
engine/renderva.o: engine/texture.h engine/bih.h engine/model.h
engine/server.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/server.o: shared/ents.h shared/command.h shared/glexts.h
engine/server.o: shared/glemu.h shared/iengine.h shared/igame.h
engine/server.o: engine/world.h engine/octa.h engine/light.h engine/texture.h
engine/server.o: engine/bih.h engine/model.h
engine/serverbrowser.o: engine/engine.h shared/cube.h shared/tools.h
engine/serverbrowser.o: shared/geom.h shared/ents.h shared/command.h
engine/serverbrowser.o: shared/glexts.h shared/glemu.h shared/iengine.h
engine/serverbrowser.o: shared/igame.h engine/world.h engine/octa.h
engine/serverbrowser.o: engine/light.h engine/texture.h engine/bih.h
engine/serverbrowser.o: engine/model.h
engine/shader.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/shader.o: shared/ents.h shared/command.h shared/glexts.h
engine/shader.o: shared/glemu.h shared/iengine.h shared/igame.h
engine/shader.o: engine/world.h engine/octa.h engine/light.h engine/texture.h
engine/shader.o: engine/bih.h engine/model.h
engine/sound.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/sound.o: shared/ents.h shared/command.h shared/glexts.h shared/glemu.h
engine/sound.o: shared/iengine.h shared/igame.h engine/world.h engine/octa.h
engine/sound.o: engine/light.h engine/texture.h engine/bih.h engine/model.h
engine/stain.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/stain.o: shared/ents.h shared/command.h shared/glexts.h shared/glemu.h
engine/stain.o: shared/iengine.h shared/igame.h engine/world.h engine/octa.h
engine/stain.o: engine/light.h engine/texture.h engine/bih.h engine/model.h
engine/texture.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/texture.o: shared/ents.h shared/command.h shared/glexts.h
engine/texture.o: shared/glemu.h shared/iengine.h shared/igame.h
engine/texture.o: engine/world.h engine/octa.h engine/light.h
engine/texture.o: engine/texture.h engine/bih.h engine/model.h
engine/ui.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/ui.o: shared/ents.h shared/command.h shared/glexts.h shared/glemu.h
engine/ui.o: shared/iengine.h shared/igame.h engine/world.h engine/octa.h
engine/ui.o: engine/light.h engine/texture.h engine/bih.h engine/model.h
engine/ui.o: engine/textedit.h
engine/water.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/water.o: shared/ents.h shared/command.h shared/glexts.h shared/glemu.h
engine/water.o: shared/iengine.h shared/igame.h engine/world.h engine/octa.h
engine/water.o: engine/light.h engine/texture.h engine/bih.h engine/model.h
engine/world.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/world.o: shared/ents.h shared/command.h shared/glexts.h shared/glemu.h
engine/world.o: shared/iengine.h shared/igame.h engine/world.h engine/octa.h
engine/world.o: engine/light.h engine/texture.h engine/bih.h engine/model.h
engine/worldio.o: engine/engine.h shared/cube.h shared/tools.h shared/geom.h
engine/worldio.o: shared/ents.h shared/command.h shared/glexts.h
engine/worldio.o: shared/glemu.h shared/iengine.h shared/igame.h
engine/worldio.o: engine/world.h engine/octa.h engine/light.h
engine/worldio.o: engine/texture.h engine/bih.h engine/model.h
shared/cube.h.gch: shared/tools.h shared/geom.h shared/ents.h
shared/cube.h.gch: shared/command.h shared/glexts.h shared/glemu.h
shared/cube.h.gch: shared/iengine.h shared/igame.h
engine/engine.h.gch: shared/cube.h shared/tools.h shared/geom.h shared/ents.h
engine/engine.h.gch: shared/command.h shared/glexts.h shared/glemu.h
engine/engine.h.gch: shared/iengine.h shared/igame.h engine/world.h
engine/engine.h.gch: engine/octa.h engine/light.h engine/texture.h
engine/engine.h.gch: engine/bih.h engine/model.h
game/game.h.gch: shared/cube.h shared/tools.h shared/geom.h shared/ents.h
game/game.h.gch: shared/command.h shared/glexts.h shared/glemu.h
game/game.h.gch: shared/iengine.h shared/igame.h game/ai.h

standalone/shared/crypto.o: shared/cube.h shared/tools.h shared/geom.h
standalone/shared/crypto.o: shared/ents.h shared/command.h shared/iengine.h
standalone/shared/crypto.o: shared/igame.h
standalone/shared/stream.o: shared/cube.h shared/tools.h shared/geom.h
standalone/shared/stream.o: shared/ents.h shared/command.h shared/iengine.h
standalone/shared/stream.o: shared/igame.h
standalone/shared/tools.o: shared/cube.h shared/tools.h shared/geom.h
standalone/shared/tools.o: shared/ents.h shared/command.h shared/iengine.h
standalone/shared/tools.o: shared/igame.h
standalone/engine/command.o: engine/engine.h shared/cube.h shared/tools.h
standalone/engine/command.o: shared/geom.h shared/ents.h shared/command.h
standalone/engine/command.o: shared/iengine.h shared/igame.h engine/world.h
standalone/engine/server.o: engine/engine.h shared/cube.h shared/tools.h
standalone/engine/server.o: shared/geom.h shared/ents.h shared/command.h
standalone/engine/server.o: shared/iengine.h shared/igame.h engine/world.h
standalone/engine/worldio.o: engine/engine.h shared/cube.h shared/tools.h
standalone/engine/worldio.o: shared/geom.h shared/ents.h shared/command.h
standalone/engine/worldio.o: shared/iengine.h shared/igame.h engine/world.h
standalone/game/entities.o: game/game.h shared/cube.h shared/tools.h
standalone/game/entities.o: shared/geom.h shared/ents.h shared/command.h
standalone/game/entities.o: shared/iengine.h shared/igame.h game/ai.h
standalone/game/server.o: game/game.h shared/cube.h shared/tools.h
standalone/game/server.o: shared/geom.h shared/ents.h shared/command.h
standalone/game/server.o: shared/iengine.h shared/igame.h game/ai.h
standalone/game/server.o: game/ctf.h game/extinfo.h game/aiman.h
standalone/engine/master.o: shared/cube.h shared/tools.h shared/geom.h
standalone/engine/master.o: shared/ents.h shared/command.h shared/iengine.h
standalone/engine/master.o: shared/igame.h

standalone/shared/cube.h.gch: shared/tools.h shared/geom.h shared/ents.h
standalone/shared/cube.h.gch: shared/command.h shared/iengine.h
standalone/shared/cube.h.gch: shared/igame.h
standalone/engine/engine.h.gch: shared/cube.h shared/tools.h shared/geom.h
standalone/engine/engine.h.gch: shared/ents.h shared/command.h
standalone/engine/engine.h.gch: shared/iengine.h shared/igame.h
standalone/engine/engine.h.gch: engine/world.h
standalone/game/game.h.gch: shared/cube.h shared/tools.h shared/geom.h
standalone/game/game.h.gch: shared/ents.h shared/command.h shared/iengine.h
standalone/game/game.h.gch: shared/igame.h game/ai.h
