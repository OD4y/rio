librio.so:	rio.c riob.c
	@echo -n 'building shared library...  '
	@gcc -shared -fPIC -o librio.so rio.c riob.c
	@echo 'done!'
.PHONY:	clean
clean:
	@echo -n 'cleaning...  '
	@rm -f librio.so *.o *.a
	@echo 'done!'
# vim: noexpandtab
