VFSDIR	=	..

CC	=	gcc

CFLAGS	=	-g -I$(VFSDIR)/include -ansi -pedantic

OBJS	=	$(VFSDIR)/src/main.o $(VFSDIR)/src/fileDescriptor.o $(VFSDIR)/src/freeList.o $(VFSDIR)/src/nAryTree.o $(VFSDIR)/src/hashTable.o $(VFSDIR)/src/binarySearchTree.o $(VFSDIR)/src/fileSystemOps.o $(VFSDIR)/src/directoryOps.o $(VFSDIR)/src/fileOps.o

TARGET	=	myVfs_exe

$(TARGET):	$(OBJS)
	$(CC) -o $(VFSDIR)/bin/$@ $(OBJS)
	mv $(VFSDIR)/src/*.o $(VFSDIR)/bin


$(OBJS) : %.o : %.c
	$(CC) -c $(CFLAGS) -o $@ $<

all: myVfs_exe

clean:
	rm -f $(VFSDIR)/bin/*.o core $(VFSDIR)/src/*.o core $(VFSDIR)/bin/$(TARGET)

