# reader lib for creator to cocos2d-x plugin
set(LIB_NAME creator_reader)

include_directories(
    collider
    animation
    dragonbones/cocos2dx
    dragonbones/armature
    dragonbones/animation
    dragonbones/events
    dragonbones/factories
    dragonbones/core
    dragonbones/geom
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${COCOS2DX_ROOT_PATH}/cocos/editor-support
    ${COCOS2DX_ROOT_PATH}/cocos
    ${COCOS2DX_ROOT_PATH}/external
)

set(READER_HEADER
    CreatorReader_generated.h
    CreatorReader.h
    animation/Easing.h
    animation/AnimateClip.h
    animation/Bezier.h
    animation/AnimationClipProperties.h
    animation/AnimationManager.h
    animation/AnimationClip.h
    collider/Collider.h
    collider/Intersection.h
    collider/ColliderManager.h
    collider/Contract.h
    Macros.h
    ui/RichtextStringVisitor.h
    ui/PageView.h
)

set(READER_SOURCE
    animation/AnimateClip.cpp
    animation/AnimationClip.cpp
    animation/AnimationManager.cpp
    animation/Easing.cpp
    animation/Bezier.cpp
    collider/Collider.cpp
    collider/ColliderManager.cpp
    collider/Contract.cpp
    collider/Intersection.cpp
    CreatorReader.cpp
    ui/PageView.cpp
    ui/RichtextStringVisitor.cpp
)

if(BUILD_LUA_LIBS)
    list(APPEND READER_HEADER
        lua-bindings/reader/lua_creator_reader_auto.hpp
        lua-bindings/reader/lua_creator_reader_manual.hpp
        lua-bindings/dragonbones/lua_dragonbones_auto.hpp
        lua-bindings/dragonbones/lua_dragonbones_manual.hpp
        lua-bindings/creator_reader_bindings.hpp
    )
    list(APPEND READER_SOURCE
        lua-bindings/creator_reader_bindings.cpp
        lua-bindings/reader/lua_creator_reader_auto.cpp
        lua-bindings/reader/lua_creator_reader_manual.cpp
        lua-bindings/dragonbones/lua_dragonbones_manual.cpp
        lua-bindings/dragonbones/lua_dragonbones_auto.cpp
    )
endif()

set(COCOS_PATH ${COCOS2DX_ROOT_PATH}/cocos/)
include_directories(${COCOS2DX_ROOT_PATH}/cocos/editor-support/spine)

STRING(REGEX REPLACE "\\\\" "/" COCOS_PATH ${COCOS2DX_ROOT_PATH}/cocos/)
include(${COCOS2DX_ROOT_PATH}/cocos/editor-support/spine/CMakeLists.txt)
list(TRANSFORM COCOS_SPINE_SRC PREPEND ${COCOS_PATH})
list(TRANSFORM COCOS_SPINE_HEADER PREPEND ${COCOS_PATH})

add_library(spine ${COCOS_SPINE_SRC} ${COCOS_SPINE_HEADER})

add_library(${LIB_NAME} ${READER_SOURCE} ${READER_HEADER})

set_target_properties(${LIB_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    FOLDER "Internal"
)

add_subdirectory(dragonbones)

target_link_libraries(${LIB_NAME} PRIVATE dragonbones)
target_link_libraries(${LIB_NAME} PRIVATE cocos2d)
target_link_libraries(${LIB_NAME} PRIVATE spine)


if(XCODE OR VS)
    cocos_mark_code_files(${LIB_NAME})
endif()
