#CROSS_COMPILE = 

AS			= $(CROSS_COMPILE)as
LD			= $(CROSS_COMPILE)ld
CC			= $(CROSS_COMPILE)gcc
CPP			= $(CC) -E
AR			= $(CROSS_COMPILE)ar
NM			= $(CROSS_COMPILE)nm
STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump

TOPDIR := $(CURDIR)

export TOPDIR
export AS LD CC CPP AR NM STRIP OBJCOPY OBJDUMP

# 编译器在编译时的参数设置
CFLAGS := -Wall -O2 -g
# 添加头文件路径，不添加的话include目录下的头文件编译时找不到
CFLAGS += -I $(TOPDIR)/include -I.

# 链接器的链接参数设置
LDFLAGS :=

export CFLAGS LDFLAGS

# 定义将来编译生成的可执行程序的名字
TARGET := sm4
ifeq ("$(origin target)", "command line")	#make target=app_name指定生成的目标名
TARGET := $(target)
endif

# 添加项目中所有用到的源文件，有顶层目录下的.c文件，和子文件夹
# 添加顶层目录下的.c文件
obj-y += test_sample.o

#添加顶层目录下的子文件夹（注意目录名后面加一个/
DIR_REMOVE = $(TARGET) LICENSE include Makefile
DIR_REMOVE += %.o %.c %.build %.md
DIRS = $(notdir $(wildcard $(TOPDIR)/*))
dirs = $(filter-out $(DIR_REMOVE), $(DIRS))
obj-y += $(addsuffix /, $(dirs))

all: 
	@make --no-print-directory -C ./ -f $(TOPDIR)/Makefile.build
	@echo "   CC     $@.o"
	@$(CC) $(LDFLAGS) -o $(TARGET) built-in.o

clean:
	@rm -f $(shell find -name "*.o")
	@rm -f $(TARGET)

distclean:
	@rm -f $(shell find -name "*.o")
	@rm -f $(shell find -name "*.d")
	@rm -f $(TARGET)
	
