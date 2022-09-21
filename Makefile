CC = g++
CFLAGS = -g -Wno-int-to-pointer-cast
EHASH = Hash/ExtensibleHash
EHASH_DB = students_ehash

ehash:
	$(CC) $(CFLAGS) $(EHASH).cpp -o $(EHASH).out
ehash-rerun: ehash-drop ehash-clean ehash-run
ehash-run: ehash
	./$(EHASH).out
ehash-drop:
	$(RM) $(EHASH_DB).*
ehash-clean:
	$(RM) $(EHASH).out
clean: ehash-drop ehash-clean
