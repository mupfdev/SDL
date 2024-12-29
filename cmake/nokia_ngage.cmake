cmake_minimum_required(VERSION 3.16)

enable_language(CXX)

file(GLOB SDL3_sources
  "${SDL3_SOURCE_DIR}/src/*.c"
  "${SDL3_SOURCE_DIR}/src/atomic/*.c"
  "${SDL3_SOURCE_DIR}/src/audio/*.c"
  "${SDL3_SOURCE_DIR}/src/audio/dummy/*.c"
  "${SDL3_SOURCE_DIR}/src/camera/*.c"
  "${SDL3_SOURCE_DIR}/src/camera/dummy/*.c"
  "${SDL3_SOURCE_DIR}/src/core/*.c"
  "${SDL3_SOURCE_DIR}/src/core/ngage/*.c"
  "${SDL3_SOURCE_DIR}/src/cpuinfo/*.c"
  "${SDL3_SOURCE_DIR}/src/dynapi/*.c"
  "${SDL3_SOURCE_DIR}/src/events/*.c"
  "${SDL3_SOURCE_DIR}/src/file/*.c"
  "${SDL3_SOURCE_DIR}/src/file/generic/*.c"
  "${SDL3_SOURCE_DIR}/src/filesystem/*.c"
  "${SDL3_SOURCE_DIR}/src/filesystem/posix/*.c"
  "${SDL3_SOURCE_DIR}/src/gpu/*.c"
  "${SDL3_SOURCE_DIR}/src/haptic/*.c"
  "${SDL3_SOURCE_DIR}/src/haptic/dummy/*.c"
  "${SDL3_SOURCE_DIR}/src/joystick/*.c"
  "${SDL3_SOURCE_DIR}/src/haptic/*.c"
  "${SDL3_SOURCE_DIR}/src/hidapi/*.c"
  "${SDL3_SOURCE_DIR}/src/libm/s_isinff.c"
  "${SDL3_SOURCE_DIR}/src/libm/s_isnanf.c"
  "${SDL3_SOURCE_DIR}/src/locale/*.c"
  "${SDL3_SOURCE_DIR}/src/main/*.c"
  "${SDL3_SOURCE_DIR}/src/misc/*.c"
  "${SDL3_SOURCE_DIR}/src/power/*.c"
  "${SDL3_SOURCE_DIR}/src/render/*.c"
  "${SDL3_SOURCE_DIR}/src/render/*/*.c"
  "${SDL3_SOURCE_DIR}/src/sensor/*.c"
  "${SDL3_SOURCE_DIR}/src/stdlib/*.c"
  "${SDL3_SOURCE_DIR}/src/storage/*.c"
  "${SDL3_SOURCE_DIR}/src/thread/*.c"
  "${SDL3_SOURCE_DIR}/src/thread/generic/SDL_syscond.c"
  "${SDL3_SOURCE_DIR}/src/thread/generic/SDL_sysrwlock.c"
  "${SDL3_SOURCE_DIR}/src/thread/generic/SDL_systls.c"
  "${SDL3_SOURCE_DIR}/src/thread/ngage/*.cpp"
  "${SDL3_SOURCE_DIR}/src/time/*.c"
  "${SDL3_SOURCE_DIR}/src/timer/*.c"
  "${SDL3_SOURCE_DIR}/src/video/*.c"
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
  PRIVATE
  __GCC32__)

set(SDL3_libs
  ${EPOC_PLATFORM}/gcc/lib/gcc-lib/arm-epoc-pe/2.9-psion-98r2/libgcc.a
  ${EPOC_LIB}/egcc.lib
  ${EPOC_LIB}/estlib.lib
  ${EPOC_LIB}/euser.lib)

#DELETE ME LATER
set(UID1 0x1000007a) # KDynamicLibraryUidValue, e32uid.h
set(UID2 0x1000008d)
set(UID3 0x10005D73) # SDL.dll UID

build_dll(${PROJECT_NAME} dll ${UID1} ${UID2} ${UID3} "${SDL3_libs}")

add_dependencies(${PROJECT_NAME}.dll ${PROJECT_NAME})
#DELETE ME LATER
