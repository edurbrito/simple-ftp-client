C=gcc
CDEPS=ftp-client.c utils.c state.c

.PHONY: all

all: clean compile echo

clean:
	@rm -f download

compile: 
	@$(C) $(CDEPS) -o download
	@echo 'Compiled ftp-client'

echo:
	@echo 'Usage: ./download ftp://[username:password@]ftp.server/path/filename'

