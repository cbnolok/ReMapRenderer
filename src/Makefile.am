VPATH = loaders renderer gui csl net granny
AM_CXXFLAGS=@CXXFLAGS@ -I../include -Wall -pedantic

bin_PROGRAMS=ui2d
ui2d_LDADD=@GL_LIBS@ ./csl/libZCslG.so.4
ui2d_SOURCES= ui2d.cpp \
SDLEvent.cpp \
SDLScreen.cpp \
Config.cpp \
Debug.cpp \
xml.cpp \
Game.cpp \
Renderer.cpp \
Texture.cpp \
TextureBuffer.cpp \
ArtLoader.cpp \
VerdataLoader.cpp \
GroundTextures.cpp \
Buffer.cpp \
HueLoader.cpp \
SkillLoader.cpp \
UOMap.cpp \
TileDataLoader.cpp \
LocalMapPatches.cpp \
MapBuffer.cpp \
MapBlock.cpp \
StaticObjects.cpp \
GumpLoader.cpp \
Border.cpp \
Button.cpp \
Control.cpp \
Container.cpp \
GUIHandler.cpp \
Gump.cpp \
Image.cpp \
Stack.cpp \
Client.cpp \
Packet.cpp \
huffman.cpp \
DynamicObjects.cpp \
Characters.cpp \
FontLoader.cpp \
Label.cpp \
CSLHandler.cpp \
ItemContainer.cpp \
ImageArt.cpp \
TextManager.cpp \
Textbox.cpp \
InputField.cpp
