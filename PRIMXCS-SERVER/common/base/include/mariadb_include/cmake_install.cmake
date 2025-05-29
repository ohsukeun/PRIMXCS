# Install script for directory: /root/.devel/library/mariadb-connector-c-3.3.1-src/include

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "0")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Development")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mariadb" TYPE FILE FILES
    "/root/.devel/library/mariadb-connector-c-3.3.1-src/include/mariadb_com.h"
    "/root/.devel/library/mariadb-connector-c-3.3.1-src/include/mysql.h"
    "/root/.devel/library/mariadb-connector-c-3.3.1-src/include/mariadb_stmt.h"
    "/root/.devel/library/mariadb-connector-c-3.3.1-src/include/ma_pvio.h"
    "/root/.devel/library/mariadb-connector-c-3.3.1-src/include/ma_tls.h"
    "/root/.devel/library/mariadb-connector-c-3.3.1-src/include/mariadb_version.h"
    "/root/.devel/library/mariadb-connector-c-3.3.1-src/include/ma_list.h"
    "/root/.devel/library/mariadb-connector-c-3.3.1-src/include/errmsg.h"
    "/root/.devel/library/mariadb-connector-c-3.3.1-src/include/mariadb_dyncol.h"
    "/root/.devel/library/mariadb-connector-c-3.3.1-src/include/mariadb_ctype.h"
    "/root/.devel/library/mariadb-connector-c-3.3.1-src/include/mariadb_rpl.h"
    "/root/.devel/library/mariadb-connector-c-3.3.1-src/include/mysqld_error.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Development")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Development")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mariadb/mysql" TYPE FILE FILES
    "/root/.devel/library/mariadb-connector-c-3.3.1-src/include/mysql/client_plugin.h"
    "/root/.devel/library/mariadb-connector-c-3.3.1-src/include/mysql/plugin_auth_common.h"
    "/root/.devel/library/mariadb-connector-c-3.3.1-src/include/mysql/plugin_auth.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Development")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Development")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mariadb/mariadb" TYPE FILE FILES "/root/.devel/library/mariadb-connector-c-3.3.1-src/include/mariadb/ma_io.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Development")

