#
# Makefile.c++
#
# Instructions:
#
# 1. Edit Makefile using vi or emacs:
#    a. rename target, if desired
#    b. add list of source files at SRCS = ...
# 2. % touch Makefile.dep
# 3. % make depend
# 4. % make
#

TARGET = borne
CC = gcc
CFLAGS = -Wall -O -g 
#Version materielle
INCLDIRS = -I . -I /shared/TCii/include
LIBDIRS = -L /shared/TCii/lib

#dependance librairie libsmartcard_borne (voir git)
LIBS64 = -lsmartcard_borne_x64 -lpcsclite

SRCS = lecteurcarte.c borne.c baseclient.c voyants.c

OBJS = ${SRCS:.c=.o} 

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLDIRS) $(LIBDIRS) -o $@ $(OBJS) $(LIBS64)

$(OBJS):
	$(CC) $(CFLAGS) $(INCLDIRS) $(LIBDIRS) -c $*.c

depend:
	$(CC) -MM $(SRCS) $(INCLDIRS) > Makefile.dep

clean:
	rm -f $(OBJS) core

veryclean: clean
	rm -f $(TARGET) a.out *.*~

include Makefile.dep

