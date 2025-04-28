# Qt specific settings
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(Z_VCPKG_OPENSSL_USE_SINGLE_CONFIG ON)
find_package(OpenSSL CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(Qt6 COMPONENTS
    Core
    Qml
    Xml
    Gui
    Widgets
    Quick
    Network
    Sql
    LinguistTools
    REQUIRED)

# The internal target will not be installed and only act as requirements for
# main and tests internally
set(target_name_internal uiqt_internal)

file(GLOB_RECURSE internal_srcs "src/*.cpp")
file(GLOB_RECURSE internal_hdrs "include/*.hpp")

list(FILTER internal_srcs EXCLUDE REGEX "main.cpp")

add_library(${target_name_internal} OBJECT)
warn_target(${target_name_internal})
harden_target(${target_name_internal})
sanitize_target(${target_name_internal})
target_code_coverage(${target_name_internal} ALL)

generate_git_header(VERSION_NAMESPACE_PREFIX uiqt)

target_sources(${target_name_internal} PRIVATE ${internal_srcs}
                                               ${internal_hdrs})

target_include_interface_directories(
  ${target_name_internal} ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_BINARY_DIR}/git_version)

target_link_libraries(
  ${target_name_internal}
  PUBLIC Qt6::Core
         Qt6::Gui
         Qt6::Widgets
         Qt6::Network
         Qt6::Sql
)

# Add i18n
file(GLOB TS_FILES "i18n/*.ts")
set_source_files_properties(
  ${TS_FILES} PROPERTIES OUTPUT_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/i18n")
qt_add_translation(QM_FILES ${TS_FILES})

# Add Qt resources
set(RESOURCE_FILES resources/resources.qrc)

qt_add_resources(
  ${target_name_internal}
  RESOURCES
  ${RESOURCE_FILES}
  PREFIX
  "/i18n"
  BASE
  "${CMAKE_CURRENT_BINARY_DIR}/i18n"
  FILES
  ${QM_FILES})

# The main target only contains main logits. Here it is recommended to put the
# parts that need to be verified and tested in the target target_name_internal
set(target_name uiqt)

file(GLOB_RECURSE _srcs "src/main.cpp")

add_executable(${target_name} ${_srcs})
warn_target(${target_name})
harden_target(${target_name})
sanitize_target(${target_name})
target_code_coverage(${target_name} ALL)

target_link_libraries(${target_name} PRIVATE ${target_name_internal})

add_custom_command(
  TARGET ${target_name}
  POST_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:unofficial::sqlite3::sqlite3>
    $<TARGET_FILE_DIR:${target_name}>
  COMMAND
    ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:OpenSSL::SSL>
    $<TARGET_FILE_DIR:${target_name}>
  COMMENT "Sync SQLite3 runtime library to build path.")

set(qt_plugin
    ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}$<$<CONFIG:Debug>:/debug>/Qt6/plugins/
)

add_custom_command(
  TARGET ${target_name}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${qt_plugin}
          $<TARGET_FILE_DIR:${target_name}>;
  COMMENT "Sync qt plugin to build path.")

add_custom_command(
  TARGET ${target_name}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${CMAKE_CURRENT_BINARY_DIR}/i18n
          $<TARGET_FILE_DIR:${target_name}>/i18n;
  COMMENT "Sync qt i81n to build path.")

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/i18n
        DESTINATION $<$<CONFIG:Debug>:debug/>${CMAKE_INSTALL_BINDIR})

install(DIRECTORY ${qt_plugin}
        DESTINATION $<$<CONFIG:Debug>:debug/>${CMAKE_INSTALL_BINDIR})

install(TARGETS ${target_name}
        RUNTIME DESTINATION $<$<CONFIG:Debug>:debug/>${CMAKE_INSTALL_BINDIR})

install(
  IMPORTED_RUNTIME_ARTIFACTS
  unofficial::sqlite3::sqlite3
  OpenSSL::SSL
  OpenSSL::Crypto
  RUNTIME_DEPENDENCY_SET
  plugins_runtime
  DESTINATION
  $<$<CONFIG:Debug>:debug/>${CMAKE_INSTALL_BINDIR}
  COMPONENT
  Runtime)

install_dependency(TARGETS ${target_name})

if(BUILD_TESTING)
  add_test_subdirectory(tests)
endif()
