shell : main.c current_path.c echo.c ls.c pinfo.c pwd.c remindme.c clock.c setenv.c redirect.c\
	 pipe.c jobs.c kjob.c overkill.c  bg.c
	gcc -o shell main.c current_path.c echo.c ls.c pinfo.c pwd.c remindme.c clock.c setenv.c redirect.c\
	 pipe.c jobs.c kjob.c overkill.c  bg.c 

clean : 
	rm shell
	