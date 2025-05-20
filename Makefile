CC      = gcc
CFLAGS  = -Wall -Wextra -Werror
VFLAGS  = --leak-check=full --show-leak-kinds=all --track-origins=yes
SRCDIR  = srcs
INCDIR  = includes
BUILDDIR = build
EXEC = kv
DEBUG ?= 0

ifneq ($(DEBUG), 0)
        CFLAGS += -g3
endif

SRCS    := $(shell find $(SRCDIR) -type f -name "*.c")

OBJS    := $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRCS))

all:    ${EXEC}

${EXEC}: ${OBJS}
        ${CC} ${OBJS} -o ${EXEC}

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
        @mkdir -p $(dir $@)
        ${CC} ${CFLAGS} -I ${INCDIR} -c $< -o $@

clean:
        rm -rf ${BUILDDIR}

fclean: clean
        rm -f ${EXEC}

debug: fclean ${EXEC} run

run:
        clear
        valgrind ${VFLAGS} ./${EXEC}

re:     fclean all

.PHONY: all clean fclean re debug run