cmake_minimum_required(VERSION 3.12)
set(CMAKE_BUILD_PARALLEL_LEVEL "2" CACHE STRING "Number of threads to use for compilation")

set(SF_INCLUDE_BASE 
    include/Buffer.h
    include/Cache.h
    include/Config.h
    include/Debug.h
    include/Game.h
    include/include.h
    include/uotype.h
    include/xml.h
)
source_group(include FILES ${SF_INCLUDE_BASE})
set(SF_INCLUDE_LOADERS 
    include/loaders/ArtLoader.h
    include/loaders/GroundTextures.h
    include/loaders/HueLoader.h
    include/loaders/LocalMapPatches.h
    include/loaders/Map.h
    include/loaders/MapPatches.h
    include/loaders/TileDataLoader.h
    include/loaders/UOMap.h
    include/loaders/VerdataLoader.h
)
source_group(include\\loaders FILES ${SF_INCLUDE_LOADERS})
set(SF_INCLUDE_RENDERER 
    include/renderer/MapBlock.h
    include/renderer/MapBuffer.h
    include/renderer/Node.h
    include/renderer/Renderer.h
    include/renderer/SDLScreen.h
    include/renderer/StaticObjects.h
    include/renderer/Texture.h
    include/renderer/TextureBuffer.h
    include/renderer/World.h
)
source_group(include\\renderer FILES ${SF_INCLUDE_RENDERER})

set(SF_BASE 
    src/Buffer.cpp
    src/Config.cpp
    src/Debug.cpp
    src/Game.cpp
    src/MapRenderer.cpp
    src/xml.cpp
)
source_group(src FILES ${SF_BASE})
set(SF_LOADERS
    src/loaders/ArtLoader.cpp
    src/loaders/GroundTextures.cpp
    src/loaders/HueLoader.cpp
    src/loaders/LocalMapPatches.cpp
    src/loaders/TileDataLoader.cpp
    src/loaders/UOMap.cpp
    src/loaders/VerdataLoader.cpp
)
source_group(src\\loaders FILES ${SF_LOADERS})
set(SF_RENDERER
    src/renderer/MapBlock.cpp
    src/renderer/MapBuffer.cpp
    src/renderer/Node.cpp
    src/renderer/Renderer.cpp
    src/renderer/SDLScreen.cpp
    src/renderer/StaticObjects.cpp
    src/renderer/Texture.cpp
    src/renderer/TextureBuffer.cpp
)
source_group(src\\renderer FILES ${SF_RENDERER})

project(ReMapRenderer)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    # 64 bits
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin64/")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    # 32 bits
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
endif()

# These linker hints need to be set before the call to add_executable
if (WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        link_directories("${CMAKE_SOURCE_DIR}/_winlibs/x64/")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        link_directories("${CMAKE_SOURCE_DIR}/_winlibs/x86/")
    endif()
else (WIN32)
    include_directories(/usr/X11R6/include/)
    #include_directories(/usr/include/SDL/)
    link_directories(/usr/lib /usr/X11R6/lib)
endif (WIN32)

include_directories("${CMAKE_SOURCE_DIR}/include/" "${CMAKE_SOURCE_DIR}/include_sdl/")

set(CMAKE_CXX_STANDARD 11) # c++11
add_executable(ReMapRenderer 
    ${SF_INCLUDE_BASE} ${SF_INCLUDE_LOADERS} ${SF_INCLUDE_RENDERER} 
    ${SF_BASE} ${SF_LOADERS} ${SF_RENDERER}
)

if (WIN32)
    target_link_libraries(ReMapRenderer SDLmain SDL OpenGL32)
    target_compile_definitions(ReMapRenderer PUBLIC _CRT_SECURE_NO_WARNINGS)
else (WIN32)
    target_link_libraries(ReMapRenderer GL GLU SDL_ttf SDL_net SDL_mixer SDL_image SDL pthread)
    target_compile_definitions(ReMapRenderer PUBLIC HAVE_OPENGL)
endif (WIN32)

if (MSVC)
    set (CMAKE_CXX_FLAGS "/O2 /EHsc /Oy /Oi /MP")
else (MSVC)
    set(CMAKE_CXX_FLAGS "-s -O3")
endif(MSVC)
