
#OPT := -O2

# Can I put this here?
export LD_LIBRARY_PATH := .

LIBNAME = libcworks
VERSION = 0.0.12
BASEVERSION = 0

LIB=$(LIBNAME).so
VLIB=$(LIB).$(VERSION)
BLIB=$(LIB).$(BASEVERSION)

LINKDIR=/usr/local/lib
ROOTLINKDIR=/usr/lib

WINLINKDIR ?= D:\Lib

all: listworks sortworks stringworks tableworks fileworks intworks
	gcc -shared -o $(VLIB) *.o
	-ln -sf $(VLIB) $(LIB)


listworks: arrlist.o nodelist.o bintree.o arrqueue.o
sortworks: compfunc.o search.o mergesort.o
stringworks: stringbuilder.o boyermoore.o
tableworks: table.o
fileworks: filefuncs.o
intworks: ranges.o


# Well, these are sorted alphabetically.  In need of better organization
arrlist.o:
	gcc -c $(OPT) -fpic arrlist.c

arrqueue.o:
	gcc -c $(OPT) -fpic arrqueue.c

bintree.o:
	gcc -c $(OPT) -fpic bintree.c

boyermoore.o:
	gcc -c $(OPT) -fpic boyermoore.c

compfunc.o:
	gcc -c $(OPT) -fpic compfunc.c

filefuncs.o:
	gcc -c $(OPT) -fpic filefuncs.c

mergesort.o:
	gcc -c $(OPT) -fpic mergesort.c

nodelist.o:
	gcc -c $(OPT) -fpic nodelist.c

ranges.o:
	gcc -c $(OPT) -fpic ranges.c

search.o:
	gcc -c $(OPT) -fpic search.c

stringbuilder.o:
	gcc -c $(OPT) -fpic stringbuilder.c

table.o:
	gcc -c $(OPT) -fpic table.c


clean:
	-rm *.o
	-rm $(LIB)
	-rm $(VLIB)

install:
	# (must be root for this one, apparently)
	cp $(VLIB) $(LINKDIR)
	ln -sf $(VLIB) $(LINKDIR)/$(LIB)
	ln -sf $(VLIB) $(LINKDIR)/$(BLIB)
	ln -sf $(LINKDIR)/$(VLIB) $(ROOTLINKDIR)/$(VLIB)
	ln -sf $(LINKDIR)/$(LIB) $(ROOTLINKDIR)/$(LIB)
	ln -sf $(LINKDIR)/$(BLIB) $(ROOTLINKDIR)/$(BLIB)
	
	-rm -r /usr/local/include/cworks
	mkdir /usr/local/include/cworks
	cp *.h /usr/local/include/cworks/

uninstall:
	# (this one, too)
	-rm $(LINKDIR)/$(VLIB)
	-rm $(LINKDIR)/$(LIB)
	-rm $(LINKDIR)/$(BLIB)
	-rm $(ROOTLINKDIR)/$(VLIB)
	-rm $(ROOTLINKDIR)/$(LIB)
	-rm $(ROOTLINKDIR)/$(BLIB)
	-rm -r /usr/local/include/cworks

wclean:
	-del *.o
	-del $(LIB)
	-del $(VLIB)

winstall:
	copy /Y $(VLIB) $(WINLINKDIR)\cworks\$(VLIB)
	copy /Y $(VLIB) $(WINLINKDIR)\cworks\$(LIBNAME).dll
	copy /Y $(VLIB) $(WINLINKDIR)\$(LIBNAME).dll
	copy /Y $(VLIB) C:\Windows\System32\\
	copy /Y $(VLIB) C:\Windows\SysWOW64\\
	copy /Y *.h $(WINLINKDIR)\cworks\\

wuninstall:
	del $(WINLINKDIR)\cworks\$(VLIB)
	del $(WINLINKDIR)\cworks\$(LIBNAME).dll
	del $(WINLINKDIR)\$(LIBNAME).dll
	del C:\Windows\System32\$(VLIB)
	del C:\Windows\SysWOW64\$(VLIB)
	del $(WINLINKDIR)\cworks\*.h

