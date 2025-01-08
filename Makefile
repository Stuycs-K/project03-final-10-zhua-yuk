CC := gcc
CFLAGS := -I./include

OBJDIR := build
INCDIR := include
SRCDIR := src

IGNR := shared_memory.c fdmcalc.c subprocess.c 
IGNR := $(addprefix $(SRCDIR)/, $(IGNR))
SRCS := $(wildcard $(SRCDIR)/*.c)
SRCS := $(filter-out $(IGNR), $(SRCS))
OBJS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

TARGET = main

all: $(TARGET)

$(TARGET): $(OBJS)
	@$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/*.h
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJDIR) $(TARGET)

