cmake_minimum_required(VERSION 3.16)

enable_language(CXX)

set(GCC_MODULE_COMN_DEFS -D__SYMBIAN32__ -D__GCC32__ -D__EPOC32__ -D__MARM__ -D__MARM_ARMI__)
set(GCC_MODULE_MODE_DEFS -DNDEBUG -D_UNICODE)
set(GCC_MODULE_DEFS ${GCC_MODULE_COMN_DEFS} ${GCC_MODULE_MODE_DEFS})

file(GLOB SDL3_sources
  "${SDL3_SOURCE_DIR}/src/*.c"
  "${SDL3_SOURCE_DIR}/src/atomic/*.c"
  "${SDL3_SOURCE_DIR}/src/core/*.c"
  "${SDL3_SOURCE_DIR}/src/core/ngage/*.c"
  "${SDL3_SOURCE_DIR}/src/core/ngage/*.cpp"
  "${SDL3_SOURCE_DIR}/src/cpuinfo/*.c"
  "${SDL3_SOURCE_DIR}/src/dynapi/*.c"
  "${SDL3_SOURCE_DIR}/src/events/*.c"
  "${SDL3_SOURCE_DIR}/src/file/*.c"
  "${SDL3_SOURCE_DIR}/src/file/generic/*.c"
  "${SDL3_SOURCE_DIR}/src/filesystem/*.c"
  "${SDL3_SOURCE_DIR}/src/filesystem/ngage/*.c"
  "${SDL3_SOURCE_DIR}/src/filesystem/posix/*.c"
  "${SDL3_SOURCE_DIR}/src/gpu/*.c"
  "${SDL3_SOURCE_DIR}/src/hidapi/*.c"
  "${SDL3_SOURCE_DIR}/src/libm/s_isinff.c"
  "${SDL3_SOURCE_DIR}/src/libm/s_isnanf.c"
  "${SDL3_SOURCE_DIR}/src/locale/*.c"
  "${SDL3_SOURCE_DIR}/src/locale/ngage/*.cpp"
  "${SDL3_SOURCE_DIR}/src/main/*.c"
  "${SDL3_SOURCE_DIR}/src/main/ngage/*.cpp"
  "${SDL3_SOURCE_DIR}/src/misc/*.c"
  "${SDL3_SOURCE_DIR}/src/misc/dummy/*.c"
  "${SDL3_SOURCE_DIR}/src/power/*.c"
  "${SDL3_SOURCE_DIR}/src/render/*.c"
  "${SDL3_SOURCE_DIR}/src/render/software/*.c"
  "${SDL3_SOURCE_DIR}/src/stdlib/*.c"
  "${SDL3_SOURCE_DIR}/src/storage/*.c"
  "${SDL3_SOURCE_DIR}/src/storage/generic/*.c"
  "${SDL3_SOURCE_DIR}/src/thread/*.c"
  "${SDL3_SOURCE_DIR}/src/thread/generic/*.c"
  #"${SDL3_SOURCE_DIR}/src/thread/generic/SDL_syscond.c"
  #"${SDL3_SOURCE_DIR}/src/thread/generic/SDL_sysrwlock.c"
  #"${SDL3_SOURCE_DIR}/src/thread/generic/SDL_systls.c"
  #"${SDL3_SOURCE_DIR}/src/thread/ngage/*.cpp"
  "${SDL3_SOURCE_DIR}/src/time/*.c"
  "${SDL3_SOURCE_DIR}/src/time/ngage/*.cpp"
  "${SDL3_SOURCE_DIR}/src/time/unix/*.c"
  "${SDL3_SOURCE_DIR}/src/timer/*.c"
  "${SDL3_SOURCE_DIR}/src/timer/ngage/*.cpp"
  "${SDL3_SOURCE_DIR}/src/tray/*.c"
  "${SDL3_SOURCE_DIR}/src/tray/dummy/*.c"
  "${SDL3_SOURCE_DIR}/src/video/*.c"
  "${SDL3_SOURCE_DIR}/src/video/ngage/*.cpp"
  "${SDL3_SOURCE_DIR}/src/video/yuv2rgb/*.c")

add_library(${PROJECT_NAME} STATIC ${SDL3_sources})

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC
  "${SDL3_SOURCE_DIR}/include"
  PRIVATE
  "${SDL3_SOURCE_DIR}/src"
  "${SDL3_SOURCE_DIR}/include/build_config")

target_compile_definitions(
  ${PROJECT_NAME}
  PUBLIC
  SDL_STATIC_LIB
  ${GCC_MODULE_DEFS})

set(test_static_libs
  ${CMAKE_CURRENT_BINARY_DIR}/libSDL3.a)

set(test_libs
  ${CMAKE_CURRENT_BINARY_DIR}/libSDL3.a
  ${EPOC_LIB}/NRenderer.lib
  ${EPOC_LIB}/cone.lib
  ${EPOC_PLATFORM}/gcc/lib/gcc-lib/arm-epoc-pe/2.9-psion-98r2/libgcc.a
  ${EPOC_LIB}/bitgdi.lib
  ${EPOC_LIB}/egcc.lib
  ${EPOC_LIB}/euser.lib
  ${EPOC_LIB}/estlib.lib
  ${EPOC_LIB}/ws32.lib
  ${EPOC_LIB}/hal.lib
  ${EPOC_LIB}/fbscli.lib
  ${EPOC_LIB}/efsrv.lib
  ${EPOC_LIB}/scdv.lib
  ${EPOC_LIB}/gdi.lib)

set(UID1 0x1000007a) # KExecutableImageUidValue, e32uid.h
set(UID2 0x100039ce) # KAppUidValue16, apadef.h
set(UID3 0x10005799) # ngage_test.exe UID

add_library(ngage_test STATIC "${CMAKE_CURRENT_SOURCE_DIR}/test/ngagetest.c")

build_exe_static(ngage_test exe ${UID1} ${UID2} ${UID3} "${test_static_libs}" "${test_libs}")

add_dependencies(
  ngage_test.exe
  ngage_test)

add_dependencies(
  ngage_test
  ${PROJECT_NAME})

target_compile_definitions(
  ngage_test
  PUBLIC
  __EXE__
  FUNCTION_NAME=__FUNCTION__
  ${GCC_DEFS}
  UID1=${UID1}
  UID2=${UID2}
  UID3=${UID3})

target_compile_options(
  ngage_test
  PUBLIC
  -O2
)

target_include_directories(
  ngage_test
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include/SDL3)
