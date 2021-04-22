### 基本参数定义
BIN  := istcp_server

SRC  += istcp_server.c
SRC  += islog.c
SRC  += istcp.c

# 定义基本路径
OBJDIR := obj
# makefile 查找路径
vpath %.c   src
vpath %.h   src
RM = rm -i

#LIBS    := -lm -lpthread
CFLAGS  += -Wall -std=c99 -fPIC -g

#CFLAGS  += -I$(OBJDIR)/include
LDFLAGS += -shared
EXEFLAGS +=

.SUFFIXES: .c .so .o
### 操作系统特殊定义
#find the OS
uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')

ifeq ($(uname_S), Linux)

else

endif

OBJ  := $(patsubst %.c,$(OBJDIR)/%.o,$(SRC))

.PHONY: all
all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(EXEFLAGS) -o $@ $^ $(LIBS)


.PHONY: clean
clean:
	$(RM) -f $(BIN) obj/* test/$(BIN)

$(OBJDIR):
	@mkdir -p $@

$(OBJDIR)/%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<


