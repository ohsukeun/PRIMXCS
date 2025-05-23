###############################################################################
#                                                                             #
# Theme  : Makefile for 'crypto' test                                         #
# Author : Park Sungsoo                                                       #
# Date   : 2023. 06. 03                                                       #
#                                                                             #
###############################################################################

###############################################################################
#macro definitions
BUILD_DATE=$(shell date +"%Y-%m-%d")
BUILD_TIME=$(shell date +"%H:%M:%S")

PROJECT_NAME=crypto_test

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
            -DUNIX64 
CFLAGS_32 = -g -Wall -O2 -O0 -static \
            -D_REENTRANT 

###############################################################################
#Macro Definition
OS_ARCH:=$(shell getconf LONG_BIT)

###############################################################################
#Directory
SRC_DIR        = ./src/
SRC_CRYPTO_DIR = ../src/
SRC_SWC_DIR    = ../../swc/src/

###############################################################################
#Include Directory
INC = -I ./include/ \
      -I ../include/ \
	  -I ../../swc/include/ \
	  -I /PROJECT/gtest/include/

###############################################################################
#Library Path
LIBPATH_64 = -L /usr/lib64 \
			 -L /PROJECT/gtest
LIBPATH_32 = -L /usr/lib \
			 -L /PROJECT/gtest

###############################################################################
#Sub-Path

###############################################################################
#Target Archive

###############################################################################
#Libraries
LIBS = -lssl \
       -lcrypto \
       -lgtest

###############################################################################
#Object Files
OBJECT        = $(patsubst ${SRC_DIR}%.cpp, ${SRC_DIR}%.o, $(wildcard ${SRC_DIR}*.cpp))
OBJECT_CRYPTO = $(patsubst ${SRC_CRYPTO_DIR}%.cpp, ${SRC_CRYPTO_DIR}%.o, $(wildcard ${SRC_CRYPTO_DIR}*.cpp))
OBJECT_SWC    = $(patsubst ${SRC_SWC_DIR}%.cpp, ${SRC_SWC_DIR}%.o, $(wildcard ${SRC_SWC_DIR}*.cpp))

###############################################################################
#Target
TARGET = ${PROJECT_NAME}

###############################################################################
#Make
${TARGET} : ${OBJECT} ${OBJECT_CRYPTO} ${OBJECT_SWC}
	@echo
	@echo ==============================================================================
	@echo Make ${TARGET} [${OS_ARCH} bit mode] ...........
	@echo ==============================================================================
	@echo
ifeq (${OS_ARCH},64)
	${CC} -o ${TARGET} ${OBJECT} ${OBJECT_CRYPTO} ${OBJECT_SWC} ${LIBPATH_64} ${LIBS} ${DYN_LINK}
	mv -f $(TARGET) ./bin
else
	${CC} -o ${TARGET} ${OBJECT} ${OBJECT_CRYPTO} ${OBJECT_SWC} ${LIBPATH_32} ${LIBS} ${DYN_LINK}
	mv -f $(TARGET) ./bin
endif

#Rule
${SRC_DIR}%.o : ${SRC_DIR}%.cpp
ifeq (${OS_ARCH},64)
	${CC} ${INC} ${CFLAGS_64} -o $@ -c $<
else
	${CC} ${INC} ${CFLAGS_32} -o $@ -c $<
endif
${SRC_CRYPTO_DIR}%.o : ${SRC_CRYPTO_DIR}%.cpp
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

###############################################################################
#Clean
clean : 
	@echo
	@echo ==============================================================================
	@echo Clean  Objects and Binaries ...                   
	@echo ==============================================================================
	@echo
	rm -rf ${OBJECT} ${OBJECT_CRYPTO} ${OBJECT_SWC}

###############################################################################
#All 
all : clean ${TARGET} 

###############################################################################

