CC = g++
CFLAGS = -g -Wno-int-to-pointer-cast
EHASH = Hash/ExtensibleHash
EHASH_DB = DB/db_ehash
EHASH_CMD = ehash.cmd
EHASH_EXE = ehash
SEQF = Sequential/main
SEQF_DB = DB/db_sequential
SEQF_CMD = ehash.cmd
SEQF_EXE = seqf
ADD_EHASH = ./add_ehash.sh
ADD_SEQF = ./add_seqf.sh
START_DATA = InputFiles/datatest_kaggle_100.csv

build: ehash seqf
ehash:
	$(CC) $(CFLAGS) $(EHASH).cpp -o $(EHASH_EXE).out
seqf:
	$(CC) $(CFLAGS) $(SEQF).cpp -o $(SEQF_EXE).out
csv-clean:
	$(RM) CSV/*

ehash-drop:
	$(RM) $(EHASH_DB)*.dat
	$(RM) $(EHASH_DB)*.ind
ehash-clean:
	$(RM) $(EHASH_EXE).out
ehash-start: ehash-drop ehash
	$(ADD_EHASH) $(START_DATA)

seqf-drop:
	$(RM) $(SEQF_DB)*.bin
seqf-clean:
	$(RM) $(SEQF_EXE).out
seqf-start: seqf-drop seqf
	$(ADD_SEQF) $(START_DATA)

start: ehash-start seqf-start
drop: ehash-drop seqf-drop
clean: ehash-clean seqf-clean csv-clean
clean-drop: clean drop

