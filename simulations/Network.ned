network Network{
    parameters: 
        int n;
	submodules:   
	    antenna: Antenna;
	    user[n]: User; 
	    
}