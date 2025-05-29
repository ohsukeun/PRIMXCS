###############################################################################
#                                                                             #
# Theme  : Makefile for 'swc(standard template wrapped classed' test          #
# Author : Park Sungsoo                                                       #
# Date   : 2021. 08. 31                                                       #
#                                                                             #
###############################################################################

###############################################################################
#macro definitions
include version.info

BUILD_DATE=$(shell date +"%Y-%m-%d")
BUILD_TIME=$(shell date +"%H:%M:%S")

VERSION_DEFINE=-DPROJECT_NAME=${PROJECT_NAME} \
               -DMAJOR_VER=${MAJOR_VER} \
               -DMINOR1_VER=${MINOR1_VER} \
               -DMINOR2_VER=${MINOR2_VER} \
               -DBUILD_NO_VER=${BUILD_NO} \
               -DADD_TAG=\"${ADD_TAG}\" \
               -DBIN_STR=\"${PROJECT_NAME}\" \
               -DBUILD_DATE_STR=\"${BUILD_DATE}\" \
               -DBUILD_TIME_STR=\"${BUILD_TIME}\"

###############################################################################
#set rule
.SUFFIXES : .cpp .o

###############################################################################
#dynamic linker definition
DYN_LINK= 

###############################################################################
#Compiler Define
CC = g++

###############################################################################
#Complie Option
CFLAGS_64 = -g -Wall -O2 -m64 -O0 -static \
            -D_REENTRANT \
            -DUNIX64 \
            ${VERSION_DEFINE}
CFLAGS_32 = -g -Wall -O2 -O0 -static \
            -D_REENTRANT \
            ${VERSION_DEFINE}

###############################################################################
#Macro Definition
OS_ARCH:=$(shell getconf LONG_BIT)

###############################################################################
#Directory
SRC_DIR              = ./src/
SRC_SWC_DIR          = ../src/
SRC_LOGGER_DIR       = ../../logger/src/

###############################################################################
#Include Directory
INC = -I ./include/ \
      -I ../include/ \
	  -I ../../logger/include/ \
      -I ../../../../../gtest/include/ \
	  -I /use/include/ 

###############################################################################
#Library Path
LIBPATH_64 = -L /usr/lib64 \
			 -L ../../../../../gtest 
LIBPATH_32 = -L /usr/lib \
			 -L ../../../../../gtest 

###############################################################################
#Sub-Path

###############################################################################
#Target Archive

###############################################################################
#Libraries
LIBS = -lpthread \
       -lssl \
       -lcrypto \
       -lgtest

###############################################################################
#Object Files
OBJECT        = $(patsubst ${SRC_DIR}%.cpp, ${SRC_DIR}%.o, $(wildcard ${SRC_DIR}*.cpp))
OBJECT_SWC    = $(patsubst ${SRC_SWC_DIR}%.cpp, ${SRC_SWC_DIR}%.o, $(wildcard ${SRC_SWC_DIR}*.cpp))
OBJECT_LOGGER = $(patsubst ${SRC_LOGGER_DIR}%.cpp, ${SRC_LOGGER_DIR}%.o, $(wildcard ${SRC_LOGGER_DIR}*.cpp))

###############################################################################
#Target
TARGET = ${PROJECT_NAME}

###############################################################################
#Make
${TARGET} : ${OBJECT} ${OBJECT_SWC} ${OBJECT_LOGGER}
	@echo
	@echo ==============================================================================
	@echo Make ${TARGET} [${OS_ARCH} bit mode] ...........
	@echo ==============================================================================
	@echo
ifeq (${OS_ARCH},64)
	${CC} -o ${TARGET} ${OBJECT} ${OBJECT_SWC} ${OBJECT_LOGGER} ${LIBPATH_64} ${LIBS} ${DYN_LINK}
	mv -f $(TARGET) ./bin
else
	${CC} -o ${TARGET} ${OBJECT} ${OBJECT_SWC} ${OBJECT_LOGGER} ${LIBPATH_32} ${LIBS} ${DYN_LINK}
	mv -f $(TARGET) ./bin
endif

#Rule
${SRC_DIR}%.o : ${SRC_DIR}%.cpp
ifeq (${OS_ARCH},64)
	${CC} ${INC} ${CFLAGS_64} -o $@ -c $<
else
	${CC} ${INC} ${CFLAGS_32} -o $@ -c $<
endif
${SRC_SWC_DIR}%.o : ${SRC_SWC_DIR}%.cpp
ifeq (${OS_ARCH},64)
	${CC} ${INC} ${CFLAGS_64} -o $@ -c $<
else
	${CC} ${INC} ${CFLAGS_32} -o $@ -c $<
endif
${SRC_LOGGER_DIR}%.o : ${SRC_LOGGER_DIR}%.cpp
ifeq (${OS_ARCH},64)
	${CC} ${INC} ${CFLAGS_64} -o $@ -c $<
else
	${CC} ${INC} ${CFLAGS_32} -o $@ -c $<
endif

###############################################################################
#Clean
clean : 
	@echo
	@echo ==============================================================================
	@echo Clean  Objects and Binaries ...                   
	@echo ==============================================================================
	@echo
	rm -rf ${OBJECT} ${OBJECT_SWC} ${OBJECT_LOGGER}

###############################################################################
#All 
all : clean ${TARGET} 

###############################################################################

