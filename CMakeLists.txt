
cmake_minimum_required(VERSION 3.5)


project(geometry CXX)

set(SSL_CERT_FILE "C:/mingw/mingw64/ssl/certs/cacert.pem")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake")

set(GEOMETRY_INCLUDE_DIRS "include")

find_package(Httplib)

if (NOT HTTPLIB_INCLUDE_DIR)
  file(DOWNLOAD https://github.com/yhirose/cpp-httplib/archive/refs/tags/v0.8.8.tar.gz
      "${CMAKE_BINARY_DIR}/deps/cpp-httplib-v0.8.8.tar.gz"
      STATUS HTTPLIB_DOWNLOAD_STATUS_LIST LOG HTTPLIB_DOWNLOAD_LOG
      SHOW_PROGRESS)

  list(GET HTTPLIB_DOWNLOAD_STATUS_LIST 0 HTTPLIB_DOWNLOAD_STATUS)

  if (HTTPLIB_DOWNLOAD_STATUS EQUAL 0)
    execute_process(COMMAND ${CMAKE_COMMAND} -E
        tar xzf "${CMAKE_BINARY_DIR}/deps/cpp-httplib-v0.8.8.tar.gz"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/deps/")

    file (GLOB HTTPLIB_DIRECTORIES RELATIVE "${CMAKE_BINARY_DIR}/deps/"
        "${CMAKE_BINARY_DIR}/deps/cpp-httplib-[0-9]*.[0-9]*.[0-9]*")

    if (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.6.0")
      list(FILTER HTTPLIB_DIRECTORIES EXCLUDE REGEX "cpp-httplib-.*\.tar\.gz")
    endif ()

    list(LENGTH HTTPLIB_DIRECTORIES HTTPLIB_DIRECTORIES_LEN)

    if (HTTPLIB_DIRECTORIES_LEN EQUAL 1)
      list(GET HTTPLIB_DIRECTORIES 0 HTTPLIB_INCLUDE_DIR)

      set(HTTPLIB_INCLUDE_DIR "${CMAKE_BINARY_DIR}/deps/${HTTPLIB_INCLUDE_DIR}")

      set(GEOMETRY_INCLUDE_DIRS ${GEOMETRY_INCLUDE_DIRS} "${HTTPLIB_INCLUDE_DIR}")

      message(STATUS
          "Successfully downloaded httplib into ${CMAKE_BINARY_DIR}/deps/${HTTPLIB_INCLUDE_DIR}/")
    else ()
      message(FATAL_ERROR "Problem unpacking httplib!"
          "  Expected only one directory cpp-httplib-x.y.z/; found ${HTTPLIB_DIRECTORIES}."
          " Try removing the directory ${CMAKE_BINARY_DIR}/deps and reconfiguring.")
    endif ()
  else ()
    list(GET HTTPLIB_DOWNLOAD_STATUS_LIST 1 HTTPLIB_DOWNLOAD_ERROR)
    message(FATAL_ERROR
        "Could not download httplib! Error code ${HTTPLIB_DOWNLOAD_STATUS}: ${HTTPLIB_DOWNLOAD_ERROR}!"
        "  Error log: ${HTTPLIB_DOWNLOAD_LOG}")
  endif ()
else ()
  set(GEOMETRY_INCLUDE_DIRS ${GEOMETRY_INCLUDE_DIRS} "${HTTPLIB_INCLUDE_DIR}")
endif ()

find_package(NlohmannJson)

if (NOT NLOHMANN_JSON_INCLUDE_DIR)
  set(NLOHMANN_JSON_DIR "nlohmann-json-3.9.1")

  file(DOWNLOAD https://github.com/nlohmann/json/releases/download/v3.9.1/json.hpp
      "${CMAKE_BINARY_DIR}/deps/${NLOHMANN_JSON_DIR}/nlohmann/json.hpp"
      STATUS NLOHMANN_JSON_DOWNLOAD_STATUS_LIST LOG NLOHMANN_JSON_DOWNLOAD_LOG
      SHOW_PROGRESS)

  list(GET NLOHMANN_JSON_DOWNLOAD_STATUS_LIST 0 NLOHMANN_JSON_DOWNLOAD_STATUS)

  if (NLOHMANN_JSON_DOWNLOAD_STATUS EQUAL 0)

    file(SHA256 "${CMAKE_BINARY_DIR}/deps/${NLOHMANN_JSON_DIR}/nlohmann/json.hpp" NLOHMANN_JSON_LOCAL_HASH)

    set(NLOHMANN_JSON_EXPECTED_HASH "7804b38146921d03374549c9e2a5e3acda097814c43caf2b96a0278e58df26e0")

    if (NLOHMANN_JSON_LOCAL_HASH STREQUAL NLOHMANN_JSON_EXPECTED_HASH)
      set(NLOHMANN_JSON_INCLUDE_DIR "${CMAKE_BINARY_DIR}/deps/${NLOHMANN_JSON_DIR}")

      set(GEOMETRY_INCLUDE_DIRS ${GEOMETRY_INCLUDE_DIRS} "${NLOHMANN_JSON_INCLUDE_DIR}")

      message(STATUS
          "Successfully downloaded nlohmann json into ${CMAKE_BINARY_DIR}/deps/${NLOHMANN_JSON_DIR}/")
    else ()
      message(FATAL_ERROR "Error checking hash for nlohmann json!"
        " Try removing the directory ${CMAKE_BINARY_DIR}/deps and reconfiguring.")
    endif ()
  else ()
    list(GET NLOHMANN_JSON_DOWNLOAD_STATUS_LIST 1 NLOHMANN_JSON_DOWNLOAD_ERROR)
    message(FATAL_ERROR
        "Could not download httplib! Error code ${NLOHMANN_JSON_DOWNLOAD_STATUS}: ${NLOHMANN_JSON_DOWNLOAD_ERROR}!"
        "  Error log: ${NLOHMANN_JSON_DOWNLOAD_LOG}")
  endif ()
else ()
  set(GEOMETRY_INCLUDE_DIRS ${GEOMETRY_INCLUDE_DIRS} "${NLOHMANN_JSON_INCLUDE_DIR}")
endif ()

if (NOT MSVC)
  set(GEOMETRY_LIBS "pthread")
endif ()

if (WIN32)
  set(WINDOWS_LIBS "ws2_32")
endif (WIN32)

include_directories(${GEOMETRY_INCLUDE_DIRS})

####################################################################
#    Исходные файлы серверной части. Сюда нужно добавить файлы
#    - include/my_algorithm_name.hpp
#    - methods/my_algorithm_name_method.cpp
####################################################################

add_executable(
  geometry_server
  include/common.hpp
  include/edge.hpp
  include/edge_impl.hpp
  include/half_plane.hpp
  include/orientation.hpp
  include/point.hpp
  include/point_impl.hpp
  include/polygon.hpp
  include/polygon_impl.hpp
  include/voronoi_polygon.hpp
  methods/main.cpp
  methods/methods.hpp
  methods/voronoi_polygon_method.cpp
)

####################################################################
#    Конец вставки
####################################################################



target_link_libraries(geometry_server ${GEOMETRY_LIBS} ${WINDOWS_LIBS})

####################################################################
#    Исходные файлы клиентской части (набор тестов).
#    Сюда нужно добавить файлы
#    - include/my_algorithm_name.hpp
#    - tests/my_algorithm_name_test.cpp
####################################################################

add_executable(
  geometry_test
  include/common.hpp
  include/edge.hpp
  include/edge_impl.hpp
  include/orientation.hpp
  include/point.hpp
  include/point_impl.hpp
  include/polygon.hpp
  include/polygon_impl.hpp
  include/voronoi_polygon.hpp
  tests/edge_test.cpp
  tests/io.hpp
  tests/main.cpp
  tests/orientation_test.cpp
  tests/point_test.cpp
  tests/polygon_test.cpp
  tests/test.hpp
  tests/test_core.cpp
  tests/test_core.hpp
  tests/voronoi_polygon_test.cpp
)

#####################################################################
#    Конец вставки
####################################################################



target_link_libraries(geometry_test ${GEOMETRY_LIBS} ${WINDOWS_LIBS})

