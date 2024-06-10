#
# 
# 	    ItIs - ITRON Implementation by Sakamura Lab
# 
# Copyright (C) 1989-1996 by Sakamura Lab, the University of Tokyo, JAPAN
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. Neither the name of the University nor the names of the laboratory
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE UNIVERSITY OR THE LABORATORY BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
# GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
# IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 
#  @(#) $Id: Makefile,v 1.9 1996/02/17 09:35:25 hiro Exp $
# 

CPU = tron

include ../$(CPU)/Makefile.config

INCLUDES = -I../include -I../include/$(CPU)
CFLAGS := -DITIS -msoft-float $(INCLUDES) $(CFLAGS)

OBJS = perror.o panic.o itis_perror.o itis_panic.o \
	strerror.o assert_fail.o \
	syslog.o setlogmask.o sysstat.o

all : libitis.a

libitis.a : $(OBJS)
	rm -f libitis.a
	$(AR) q libitis.a `lorder $(OBJS) | tsort`

clean : 
	rm -f *.a *.o *~

install:
	cp libitis.a $(LIBDIR)/
	$(RANLIB) $(LIBDIR)/libitis.a
