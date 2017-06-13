# $Id: Makefile,v 1.9 2015-02-20 18:18:19-08 - - $

MKFILE    = Makefile
DEPSFILE  = ${MKFILE}.deps
NOINCLUDE = ci clean spotless
NEEDINCL  = ${filter ${NOINCLUDE}, ${MAKECMDGOALS}}
GMAKE     = gmake --no-print-directory

GCC       = gcc -g -O0 -Wall -Wextra -std=gnu11
MKDEPS    = gcc -MM

CSOURCE   = debug.c hashset.c strhash.c spellchk.c
CHEADER   = debug.h hashset.h strhash.h yyextern.h
OBJECTS   = ${CSOURCE:.c=.o} scanner.o
EXECBIN   = spellchk
SUBMITS   = ${CHEADER} ${CSOURCE} scanner.l ${MKFILE}
SOURCES   = ${SUBMITS}
LISTING   = Listing.ps
PROJECT   = cmps012b-wm.w15 asg5

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${GCC} -o $@ ${OBJECTS}

scanner.o : scanner.l
	flex -oscanner.c scanner.l
	gcc -g -O0 -std=gnu11 -c scanner.c

%.o : %.c
	${GCC} -c $<

ci : ${SOURCES}
	cid + ${SOURCES}
	checksource ${SUBMITS}

lis : ${SOURCES} ${DEPSFILE}
	mkpspdf ${LISTING} ${SOURCES} ${DEPSFILE}

clean :
	- rm ${OBJECTS} ${DEPSFILE} core scanner.c ${EXECBIN}.errs

spotless : clean
	- rm ${EXECBIN} ${LISTING} ${LISTING:.ps=.pdf}

submit : ${SUBMITS}
	submit ${PROJECT} ${SUBMITS}


deps : ${CSOURCE} ${CHEADER}
	@ echo "# ${DEPSFILE} created `date`" >${DEPSFILE}
	${MKDEPS} ${CSOURCE} >>${DEPSFILE}

${DEPSFILE} :
	@ touch ${DEPSFILE}
	${GMAKE} deps

again :
	${GMAKE} spotless deps ci all lis

ifeq "${NEEDINCL}" ""
include ${DEPSFILE}
endif

