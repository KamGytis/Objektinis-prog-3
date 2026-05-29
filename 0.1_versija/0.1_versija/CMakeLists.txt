cmake_minimum_required(VERSION 3.14)

project(StudentuValdymoSistema VERSION 1.0 LANGUAGES CXX)

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#  Main 
set(MAIN_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/studentas.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/zmogus.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/generator.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/vector_ops.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/list_ops.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/deque_ops.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/testavimas.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/testavimas_klases.cpp"
)

add_executable(studentu_programa ${MAIN_SOURCES})
target_include_directories(studentu_programa PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

#  Google Test Framework 
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Unit Tests Target (studentas + vector klases)
set(TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/unit_tests.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/vector_unit_tests.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/studentas.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/zmogus.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/generator.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/vector_ops.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/list_ops.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/deque_ops.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/testavimas.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/testavimas_klases.cpp"
)

add_executable(tests ${TEST_SOURCES})
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tests PRIVATE gtest gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)

# Doxygen dokumentacija

find_package(Doxygen OPTIONAL_COMPONENTS dot)
if(DOXYGEN_FOUND)
    set(DOXYGEN_PROJECT_NAME "StudentuValdymoSistema v3.0")
    set(DOXYGEN_PROJECT_BRIEF "Studentu valdymo sistema su nuosava Vector klase")
    set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/docs")
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_LATEX NO)
    doxygen_add_docs(docs
        "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMENT "Generuojama Doxygen dokumentacija...")
    message(STATUS "Doxygen rastas - galima naudoti 'cmake --build . --target docs'")
else()
    message(STATUS "Doxygen nerastas - dokumentacija nebus generuojama")
endif()

# CPack (Setup.exe / MSI diegimo paketas)

set(CPACK_PACKAGE_NAME              "StudentuValdymoSistema")
set(CPACK_PACKAGE_VENDOR            "VU")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Studentu valdymo sistema v3.0")
set(CPACK_PACKAGE_VERSION_MAJOR     "3")
set(CPACK_PACKAGE_VERSION_MINOR     "0")
set(CPACK_PACKAGE_VERSION_PATCH     "0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "VU/StudentuValdymoSistema")
 
# Windows NSIS (Setup.exe)
set(CPACK_NSIS_DISPLAY_NAME         "Studentu Valdymo Sistema v3.0")
set(CPACK_NSIS_INSTALL_ROOT         "C:/Program Files")
set(CPACK_NSIS_PACKAGE_NAME         "Studentu Valdymo Sistema v3.0")
set(CPACK_NSIS_MUI_ICON             "${CMAKE_CURRENT_SOURCE_DIR}/icon.ico")
set(CPACK_NSIS_MUI_UNIICON          "${CMAKE_CURRENT_SOURCE_DIR}/icon.ico")
 
# Sukurti darbastalio ir Start meniu nuorodas
set(CPACK_NSIS_CREATE_ICONS_EXTRA
    "CreateShortCut '$DESKTOP\\\\StudentuValdymoSistema.lnk' '$INSTDIR\\\\studentu_programa.exe'
     CreateShortCut '$SMPROGRAMS\\\\VU\\\\StudentuValdymoSistema\\\\StudentuValdymoSistema.lnk' '$INSTDIR\\\\studentu_programa.exe'
     CreateShortCut '$SMPROGRAMS\\\\VU\\\\StudentuValdymoSistema\\\\Uninstall.lnk' '$INSTDIR\\\\Uninstall.exe'"
)
set(CPACK_NSIS_DELETE_ICONS_EXTRA
    "Delete '$DESKTOP\\\\StudentuValdymoSistema.lnk'
     Delete '$SMPROGRAMS\\\\VU\\\\StudentuValdymoSistema\\\\StudentuValdymoSistema.lnk'
     Delete '$SMPROGRAMS\\\\VU\\\\StudentuValdymoSistema\\\\Uninstall.lnk'
     RMDir  '$SMPROGRAMS\\\\VU\\\\StudentuValdymoSistema'
     RMDir  '$SMPROGRAMS\\\\VU'"
)
# Administratoriaus teis?s
set(CPACK_NSIS_EXECUTIONLEVEL "admin")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
 
# ?diegimo failai - testavimo duomenys
install(TARGETS studentu_programa
        RUNTIME DESTINATION ".")
install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/studentai1000.txt"
    "${CMAKE_CURRENT_SOURCE_DIR}/studentai100000.txt"
    DESTINATION "."
    OPTIONAL)
 
include(CPack)