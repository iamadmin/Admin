PROJECT(IAMADMIN)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#Build Type
SET(CMAKE_BUILD_TYPE Debug)
#SET(CMAKE_BUILD_TYPE Release)

#Install Path
SET(CMAKE_INSTALL_PREFIX /usr/local/iamadmin)

INCLUDE_DIRECTORIES(/usr/include/mysql)
#LINK_LIBRARIES(mysqlclient)

ADD_SUBDIRECTORY(src bin)
ADD_SUBDIRECTORY(src/util)
ADD_SUBDIRECTORY(src/util/config)
ADD_SUBDIRECTORY(src/model)
ADD_SUBDIRECTORY(src/control)

FILE(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/etc ${PROJECT_BINARY_DIR}/logs)

#Copy config file to binary directory.
CONFIGURE_FILE(src/etc/agentd.conf etc)
CONFIGURE_FILE(src/etc/masterd.conf etc)


INSTALL(DIRECTORY src/etc dist/logs dist/tools
	 DESTINATION ./)

INSTALL(PROGRAMS dist/tools/service.sh
	 DESTINATION tools)


