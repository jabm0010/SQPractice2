
	# PROGRAM INSTRUCTIONS
	
	The program works with a command based interface in which we have to give the following
	arguments:
	1- The patch to the folder of the instance that is going to be executed, which should have
	the files dom.txt, var.txt and ctr.txt
	2- If the search of the files has to be in upper case or lower case.
		-uc (UpperCase). For the instance Scen
		-lc (LowerCase). For the instance Graph.
	3- The seed, which will have to be an integer
	4- The model to use, choose one of the followings:  [generacional, estacionario, am1010, am1001, am1001Mej]
	5- The cross to use, choose one of the folowings: [blx, 2puntos]
	6- Finally choose how the results will be showed:
		    -t      Show in the terminal.
            -o      Save in a file
	

	Example
	
	./mi_fap_hb instancias-modificadas/graph05 -lc 77377914    am1010 blx -o

