###################################################
# Define Project name
###################################################
PRJ_NAME=get_tnh
DEBUG=0

SYSTEM_MSW=0
SYSTEM_LINUX=1
###################################################
# Cross-compiling setup
###################################################
CROSS_COMPILE=0

ifeq ($(CROSS_COMPILE), 0)
CROSS=
else
CROSS=/opt/arm-gcc-4.6.3-glibc-2.14.1-s3c6410/bin/arm-s3c6410-linux-gnueabi-
endif

###################################################
###################################################
OBJDIR=obj
SRCDIR=src
INCDIR=src
LSTDIR=lst

LIBDIR=../_lib

###################################################
###################################################
UNAME=$(shell uname -a)

OUT_FILE=$(PRJ_NAME)

###################################################
# Source file list
###################################################
C_SRC=$(notdir $(wildcard $(SRCDIR)/*.c))
CPP_SRC=$(notdir $(wildcard $(SRCDIR)/*.cpp))
A_SRC=$(notdir $(wildcard $(SRCDIR)/*.s))

###################################################
# Gen object file name automatically
###################################################
C_OBJ=$(addprefix $(OBJDIR)/, $(C_SRC:.c=.o))
CPP_OBJ=$(addprefix $(OBJDIR)/, $(CPP_SRC:.cpp=.o))
A_OBJ=$(addprefix $(OBJDIR)/, $(A_SRC:.s=.o))


###################################################
# Tools setup
###################################################
INCLUDES=-I$(INCDIR) -I$(LIBDIR)

DEFINES=-DDEBUG=$(DEBUG) -DCROSS_COMPILE=$(CROSS_COMPILE) -DPRJ_NAME=$(PRJ_NAME) \
		-DSYSTEM_MSW=$(SYSTEM_MSW) -DSYSTEM_LINUX=$(SYSTEM_LINUX)
		
CC=$(CROSS)g++
CFLAGS=-c -O -Wall $(DEFINES) $(INCLUDES)

CPP=$(CROSS)g++
CPPFLAGS=$(CFLAGS)

ASM=$(CROSS)g++
ASFLAGS=$(CFLAGS)

LINK=$(CROSS)g++
LFLAGS=-static

AR=$(CROSS)ar
ARFLAGS=-rc

###################################################
# Link file
###################################################
OBJS=$(A_OBJ) $(C_OBJ) $(CPP_OBJ)

$(OUT_FILE) : $(OBJS)
	@echo .
ifeq ($(CROSS_COMPILE), 0)
	@echo [$(OUT_FILE)]
else
	@echo "[$(OUT_FILE)]<CROSS-COMPILE>"
endif	
	@$(LINK) $(LFLAGS) -o $(OUT_FILE) $(OBJS) $(LIB)

###################################################
# Compile C++ files
###################################################
$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@echo [$<]
	@$(CPP) $(CPPFLAGS) $< -o $@ -Wa,-a=$(LSTDIR)/$(@F:.o=.lst)

###################################################
# Compile C files
###################################################
$(OBJDIR)/%.o : $(SRCDIR)/%.c
	@echo [$<]
	@$(CC) $(CFLAGS) $< -o $@ -Wa,-a=$(LSTDIR)/$(@F:.o=.lst)

###################################################
# Assembly ASM files
###################################################
$(OBJDIR)/%.o : $(SRCDIR)/%.s
	@echo [$<]
	@$(ASM) $(ASFLAGS) $< -o $@ -Wa,-a=$(LSTDIR)/$(@F:.o=.lst)

###################################################
# Clear all object file and output file
###################################################
.PHONY : clean
clean :
	@echo Delete all object file.
	@rm -f $(OBJDIR)/*
	@rm -f $(LSTDIR)/*
	@rm -f $(OUT_FILE)
