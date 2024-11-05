srctree := $(CURDIR)

${flags} += -w

buildall:
#Build Scripts 
	${MAKE}  -C   $(srctree)/scripts

# Generate necessary headers
	bash  $(srctree)/scripts/gen_headers.sh

# Build libs first
	${MAKE}  -C   $(srctree)/libs

	${MAKE}  -C   $(srctree)/libs/net
	${MAKE}  -C   $(srctree)/libs/IPC

#listener
	${MAKE}  -C   $(srctree)/listener

# parser
#	${MAKE}  -C   $(srctree)/parser


clean:
	${MAKE}  -C   $(srctree)/parser  clean
	${MAKE}  -C   $(srctree)/libs  	clean
	${MAKE}  -C   $(srctree)/scripts  clean 
	
	