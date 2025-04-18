cmake_minimum_required(VERSION 3.12)

project(BallanceMMOClient)
set(CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)

# sets RPATH for *install*ed files
set(CMAKE_INSTALL_RPATH "\$ORIGIN/bin") # Note CMake escaping around '$' sign.
# *built* files will also use RPATH which has been set before
set(CMAKE_BUILD_WITH_INSTALL_RPATH on)

include_directories("${CMAKE_CURRENT_LIST_DIR}/../BallanceMMOCommon/include"
                    "${CMAKE_CURRENT_LIST_DIR}/../BallanceMMOCommon/include/config")

set(CONFIG_DIR ${CMAKE_CURRENT_LIST_DIR}/../BallanceMMOCommon/include/config)
configure_file(${CONFIG_DIR}/version_config.h.in ${CONFIG_DIR}/version_config.h @ONLY)

# if (CMAKE_SYSTEM_NAME MATCHES "Windows")
#     # setting vcpkg toolchain file
# #    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
# #            CACHE STRING "Vcpkg toolchain file")

#     if (MINGW)  # if mingw
#         # setting vcpkg
#         if (DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
#             set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
#         endif ()
#     endif ()
#     find_package(Protobuf REQUIRED)
#     find_package(GameNetworkingSockets REQUIRED)
# endif ()

if (NOT TARGET GameNetworkingSockets)
    find_package(Protobuf REQUIRED)
    find_package(GameNetworkingSockets REQUIRED)
endif ()

if (NOT TARGET replxx)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../submodule/replxx ${CMAKE_CURRENT_BINARY_DIR}/replxx)
endif ()

set(VIRTOOLS_SDK_PATH "${VIRTOOLS_SDK_PATH}" CACHE PATH "Path to the Virtools SDK")
set(VIRTOOLS_SDK_FETCH_FROM_GIT "${VIRTOOLS_SDK_FETCH_FROM_GIT}" CACHE BOOL "Set to ON to fetch copy of SDK from git if not otherwise locatable")
set(VIRTOOLS_SDK_FETCH_FROM_GIT_PATH "${VIRTOOLS_SDK_FETCH_FROM_GIT_PATH}" CACHE FILEPATH "location to download SDK")

if (NOT VIRTOOLS_SDK_PATH)
    if (NOT VIRTOOLS_SDK_FETCH_FROM_GIT)
    else ()
        include(FetchContent)
        set(FETCHCONTENT_BASE_DIR_SAVE ${FETCHCONTENT_BASE_DIR})
        if (VIRTOOLS_SDK_FETCH_FROM_GIT_PATH)
            get_filename_component(FETCHCONTENT_BASE_DIR "${VIRTOOLS_SDK_FETCH_FROM_GIT_PATH}" REALPATH BASE_DIR "${CMAKE_SOURCE_DIR}")
        endif ()
        FetchContent_Declare(
                Virtools_SDK
                GIT_REPOSITORY https://github.com/doyaGu/Virtools-SDK-2.1.git
                GIT_TAG main
        )
        if (NOT Virtools_SDK)
            message("Downloading Virtools SDK")
            FetchContent_Populate(Virtools_SDK)
            set(VIRTOOLS_SDK_PATH "${virtools_sdk_SOURCE_DIR}" CACHE PATH "Path to the Virtools SDK" FORCE)
        endif ()
        set(FETCHCONTENT_BASE_DIR ${FETCHCONTENT_BASE_DIR_SAVE})
    endif ()
endif ()

find_package(VirtoolsSDK REQUIRED HINTS ${VIRTOOLS_SDK_PATH})

set(ASIO_INCLUDE_DIR ../submodule/asio/asio/include)
set(GNS_EXT_INCLUDE_DIR ../submodule/GameNetworkingSockets/src/external)
set(REPLXX_INCLUDE_DIR ../submodule/replxx/include)
set(BML_LIB_DIR lib/BML)
set(BMLPLUS_LIB_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/BMLPlus)

set(BMMO_CLIENT_HDR
        BallanceMMOClient.h
        bml_includes.h
        client.h
        CommandMMO.h
        config_manager.h
        console_window.h
        dumpfile.h
        exported_client.h
        game_objects.h
        game_repository.h
        game_state.h
        label_sprite.h
        local_state_handler_base.h
        local_state_handler_impl.h
        log_manager.h
        server_list.h
        text_sprite.h
        utils.h
)
set(BMMO_CLIENT_SRC BallanceMMOClient.cpp game_repository.cpp
        config_manager.cpp console_window.cpp dumpfile.cpp server_list.cpp utils.cpp)
set(BMMO_COMMON_SRC_DIRECTORY ../BallanceMMOCommon/src)
file(GLOB BMMO_COMMON_SRC ${BMMO_COMMON_SRC_DIRECTORY}/entity/*.cpp ${BMMO_COMMON_SRC_DIRECTORY}/utility/*.cpp)

add_library(BallanceMMOClient SHARED ${BMMO_CLIENT_HDR} ${BMMO_CLIENT_SRC} ${BMMO_COMMON_SRC})
set_target_properties(BallanceMMOClient PROPERTIES
                      OUTPUT_NAME "BallanceMMOClient"
                      SUFFIX ".bmod"
                      ARCHIVE_OUTPUT_NAME "BallanceMMOClient"
)
target_include_directories(BallanceMMOClient PRIVATE
                            "${CMAKE_CURRENT_LIST_DIR}/include/"
                            ${REPLXX_INCLUDE_DIR}
                            ${ASIO_INCLUDE_DIR}
                            ${GNS_EXT_INCLUDE_DIR})
target_link_directories(BallanceMMOClient PRIVATE
                            ${BML_LIB_DIR})
target_link_libraries(BallanceMMOClient PRIVATE
                        replxx
                        GameNetworkingSockets::shared
                        BML.lib)

add_library(BallanceMMOClient-BMLPlus SHARED
            ${BMMO_CLIENT_HDR}
            ${BMMO_CLIENT_SRC}
            ${BMMO_COMMON_SRC})
set_target_properties(BallanceMMOClient-BMLPlus PROPERTIES
                      OUTPUT_NAME "BallanceMMOClient"
                      SUFFIX ".bmodp"
                      ARCHIVE_OUTPUT_NAME "BallanceMMOClient+"
)
target_include_directories(BallanceMMOClient-BMLPlus PRIVATE
                            "${CMAKE_CURRENT_LIST_DIR}/include/"
                            ${REPLXX_INCLUDE_DIR}
                            ${ASIO_INCLUDE_DIR}
                            ${GNS_EXT_INCLUDE_DIR})
target_link_directories(BallanceMMOClient-BMLPlus PRIVATE
                            ${BMLPLUS_LIB_DIR})
target_link_libraries(BallanceMMOClient-BMLPlus PRIVATE
                        replxx
                        GameNetworkingSockets::shared
                        CK2
                        VxMath
                        ${BMLPLUS_LIB_DIR}/BMLPlus.lib)
if (MSVC)
    # set(compile_options /Wall)
else()
    set(compile_options -Wall -Wextra -Wno-unknown-pragmas)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10)
        list(APPEND compile_options -fconcepts)
    endif()
endif()

target_compile_options(BallanceMMOClient PRIVATE ${compile_options})
target_compile_options(BallanceMMOClient-BMLPlus PRIVATE ${compile_options})
if (WIN32)
# Prevents Windows.h from adding unnecessary includes, and defining min/max as macros 
    target_compile_definitions(BallanceMMOClient PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
    target_compile_definitions(BallanceMMOClient-BMLPlus PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX BMMO_USE_BML_PLUS)
endif() 
target_compile_definitions(BallanceMMOClient PRIVATE BMMO_INCLUDE_INTERNAL REPLXX_STATIC)
target_compile_definitions(BallanceMMOClient-BMLPlus PRIVATE BMMO_INCLUDE_INTERNAL REPLXX_STATIC)

foreach (LIB IN ITEMS BallanceMMOClient BallanceMMOClient-BMLPlus)
    # Disable msvc unsafe warnings
    target_compile_definitions(${LIB} PRIVATE
            $<$<C_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
            $<$<C_COMPILER_ID:MSVC>:_CRT_NONSTDC_NO_WARNINGS>
            )
endforeach ()
