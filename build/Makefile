VFSDIR	=	..

CC	=	gcc

CFLAGS	=	-g -I$(VFSDIR)/include -ansi -pedantic

OBJS	=	$(VFSDIR)/src/main.o $(VFSDIR)/src/block.o $(VFSDIR)/src/fileDescriptor.o $(VFSDIR)/src/mainHeader.o $(VFSDIR)/src/freeList.o $(VFSDIR)/src/nAryTree.o $(VFSDIR)/src/hashTable.o $(VFSDIR)/src/fileSystemOps.o $(VFSDIR)/src/fileDescriptor.o $(VFSDIR)/src/directoryOps.o

TARGET	=	myVfs_exe

$(TARGET):	$(OBJS)
	$(CC) -o $(VFSDIR)/bin/$@ $(OBJS)
	mv $(VFSDIR)/src/*.o $(VFSDIR)/bin


$(OBJS) : %.o : %.c
	$(CC) -c $(CFLAGS) -o $@ $<

all: myVfs_exe

clean:
	rm -f $(VFSDIR)/bin/*.o core $(VFSDIR)/src/*.o core $(VFSDIR)/bin/$(TARGET)

