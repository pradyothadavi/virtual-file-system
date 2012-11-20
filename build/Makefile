VFSDIR	=	..

CC	=	gcc

CFLAGS	=	-g -I$(VFSDIR)/include -ansi -pedantic

OBJS	=	$(VFSDIR)/src/freeList.o $(VFSDIR)/src/hashTable.o $(VFSDIR)/src/nAryTree.o $(VFSDIR)/src/binarySearchTree.o $(VFSDIR)/src/fileSystemOps.o $(VFSDIR)/src/fileDescriptor.o $(VFSDIR)/src/makedir.o $(VFSDIR)/src/main.o

TARGET	=	myVfs_exe

$(TARGET):	$(OBJS)
	$(CC) -o $(VFSDIR)/bin/$@ $(OBJS)
	mv $(VFSDIR)/src/*.o $(VFSDIR)/bin


$(OBJS) : %.o : %.c
	$(CC) -c $(CFLAGS) -o $@ $<

all: myVfs_exe

clean:
	rm -f $(VFSDIR)/bin/*.o core $(VFSDIR)/src/*.o core $(VFSDIR)/bin/$(TARGET) $(VFSDIR)/bin/*

