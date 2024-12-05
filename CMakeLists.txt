# CMakeList.txt : CMake project for gameProject, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("gameProject")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Знайдіть бібліотеку SFML
find_package(SFML 2.6.2 COMPONENTS system window graphics REQUIRED)

# Add source to this project's executable.
add_executable (gameProject "gameProject.cpp" "gameProject.h"   "Hero.h" "TextureManager.h" "AudioManager.h" "LoadResources.h" "gameLoop.cpp" "Utils.h" "Npc.h" "Apple.h" "Panda.h"  )

# Підключіть бібліотеки SFML до проєкту
target_link_libraries(gameProject sfml-system sfml-window sfml-graphics sfml-audio)

#if (CMAKE_VERSION VERSION_GREATER 3.12)
#  set_property(TARGET gameProject PROPERTY CXX_STANDARD 20)
#endif()

# TODO: Add tests and install targets if needed.

