# 定义交叉编译器，用于编译针对 aarch64 架构的 Linux 系统的程序
CC := aarch64-linux-gnu-gcc

# 使用 shell 命令 find 查找 src 目录下所有扩展名为 .c 的源文件，并将结果赋值给变量 SRC
SRC := $(shell find src -name "*.c")

# 定义头文件搜索路径（include 目录），包括多个目录
INC := ./inc  \
			./3rd/usr/local/include \
			./3rd/usr/include \
			./3rd/usr/include/python3.10 \
			./3rd/usr/include/aarch64-linux-gnu/python3.10  \
			./3rd/usr/include/aarch64-linux-gnu  

# 将源文件列表中的每个文件名由 src/ 替换为 obj/，并将扩展名 .c 改为 .o，得到目标对象文件列表 OBJ
OBJ := $(subst src/,obj/,$(SRC:.c=.o))

# 定义最终可执行文件名及路径，存放在 obj/ 目录下
TARGET=obj/smartproject

# 通过 foreach 循环遍历 INC 中的每个路径，为每个路径添加 -I 前缀，生成 C 编译选项列表 CFLAGS
CFLAGS := $(foreach item, $(INC), -I$(item)) # -I./inc -I. /3rd/usr/local/include 

# 定义库文件搜索路径（lib 目录），包括多个目录
LIBS_PATH := ./3rd/usr/local/lib   \
							./3rd/lib/aarch64-linux-gnu  \
							./3rd/usr/lib/aarch64-linux-gnu  \
							./3rd/usr/lib/python3.10  

# 通过 foreach 循环遍历 LIBS_PATH 中的每个路径，为每个路径添加 -L 前缀，生成链接选项列表 LDFLAGS
LDFLAGS := $(foreach item, $(LIBS_PATH), -L$(item)) # -L./3rd/usr/local/LIBS

# 定义链接所需的库文件，包括 wiringPi、Python 3.10、线程支持（pthread）、expat、zlib、crypt 等
LIBS := -lwiringPi -lpython3.10 -pthread -lexpat -lz -lcrypt

# 规则：编译单个源文件（src/*.c）生成目标对象文件（obj/*.o）
obj/%.o:src/%.c
	mkdir -p obj
	$(CC) -o $@ -c $< $(CFLAGS) 

#$@ 被展开为该规则所要创建或更新的目标文件的完整名字 例如 obj/main.o
#$< 被展开为该规则所依赖的第一个文件的完整名字 例如 src/main.c
#在规则的命令行中，$^ 被展开为该规则所依赖的所有文件的完整名字，这些名字之间以空格分隔。它常用于需要引用所有依赖文件的场景，如静态库的链接。

# 规则：链接所有目标对象文件（OBJ）生成最终可执行文件（TARGET）
$(TARGET): $(OBJ)
# 使用交叉编译器链接目标对象文件，生成最终可执行文件，同时指定链接选项 LDFLAGS 和所需库 LIBS
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

# 别名：编译目标（等同于 make $(TARGET））
compile:$(TARGET)

# 清理规则：删除最终可执行文件（TARGET）和所有目标对象文件（OBJ）
clean:
	rm -rf $(TARGET) $(OBJ)

# 别名：调试（打印各变量的值）
debug:
	echo $(CC)
	echo $(SRC)
	echo $(INC)
	echo $(OBJ)
	echo $(TARGET)
	echo $(CFLAGS)
	echo $(LDFLAGS)
	echo $(LIBS)

# 声明 clean、compile、debug 为伪目标（无需对应文件即可执行）
.PHONY: clean compile debug



