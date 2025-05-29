###############################################################################
#                                                                             # 
# Theme  : Makefile for logger library                                        #
# Author : Park Sungsoo                                                       #
# Date   : 2023. 06. 18                                                       #
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
#DYN_LINK= -Wl,-rpath,/PRIMX/lib

###############################################################################
#Compiler Define
CC = g++

###############################################################################
#Complie Option
CFLAGS_64 = -g -Wall -O2 -m64 -O0 -static 
CFLAGS_32 = -g -Wall -O2 -O0 -static 

###############################################################################
#Macro Definition
OS_ARCH:=$(shell getconf LONG_BIT)

###############################################################################
#Directory
SRC_DIR     = ./src/

###############################################################################
#Include Directory
INC = -I ./include/ 

###############################################################################
#Library Path

LIBPATH_64 = -L /usr/lib64
LIBPATH_32 = -L /usr/lib 

###############################################################################
#Sub-Path

###############################################################################
#Target Archive

###############################################################################
#Libraries
#LIBS = -lpthread \
#	   -lssl \
#	   -lcrypto \
#	   -ldl \

###############################################################################
#Object Files
OBJECT     = $(patsubst ${SRC_DIR}%.cpp, ${SRC_DIR}%.o, $(wildcard ${SRC_DIR}*.cpp))

###############################################################################
#Target
TARGET = lib${PROJECT_NAME}.a

###############################################################################
#Make
${TARGET} : ${OBJECT} 
	@echo
	@echo -e "\E[1m==============================================================================\E[0m"
	@echo -e "\E[1mMake ${TARGET} [${OS_ARCH} bit mode] ...........\E[0m"
	@echo -e "\E[1m==============================================================================\E[0m"
	@echo
ifeq (${OS_ARCH},64)
	ar rc ${TARGET} ${OBJECT}
	rm -f ./lib/${TARGET}
	cp ${TARGET} ../../lib/
	mv ${TARGET} ./lib/
else
	ar rc ${TARGET} ${OBJECT}
	rm -f ./lib/${TARGET}
	cp ${TARGET} ../../lib/
	mv ${TARGET} ./lib/
endif

#Rule
${SRC_DIR}%.o : ${SRC_DIR}%.cpp
ifeq (${OS_ARCH},64)
	${CC} ${INC} ${CFLAGS_64} -o $@ -c $<
else
	${CC} ${INC} ${CFLAGS_32} -o $@ -c $<
endif

###############################################################################
#Clean
clean : 
	@echo
	@echo -e "\E[1m==============================================================================\E[0m"
	@echo -e "\E[1mClean  Objects and Binaries ...\E[0m"
	@echo -e "\E[1m==============================================================================\E[0m"
	@echo
	rm -rf ${OBJECT} ${TARGET} core* 

###############################################################################
#All 
all : clean ${TARGET} 

###############################################################################


