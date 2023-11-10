# create a jar in cmake, see
# https://stackoverflow.com/questions/14542872/what-is-the-correct-way-to-use-add-jar-with-java-bindings-produced-by-swig

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/class)
file(GLOB java_files "main/*.java")
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/build.jar
  COMMAND ${Java_JAVAC_EXECUTABLE} -d ${CMAKE_CURRENT_BINARY_DIR}/class
          ${java_files}
  COMMAND ${Java_JAR_EXECUTABLE} -cfM ${CMAKE_CURRENT_BINARY_DIR}/build.jar -C
          ${CMAKE_CURRENT_BINARY_DIR}/class .
  DEPENDS ${java_files}
  VERBATIM)

set(target_name main_jar)

add_jar(${target_name} SOURCE ${java_files}
        INCLUDE_JARS ${CMAKE_CURRENT_BINARY_DIR}/build.jar)
add_custom_command(
  TARGET ${target_name}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          ${CMAKE_CURRENT_BINARY_DIR}/${target_name}.jar ${CMAKE_BINARY_DIR}/bin
  VERBATIM)
