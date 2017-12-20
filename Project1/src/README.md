PROJECT DESCRIPTION:
Project is based on client/server network finger program
The client (i.e., the client executable file name is fingerclient) runs in command line like this: fingerclient username@hostname:server_port,
The server program is a multi-client server which can accept multiple client connections and server sends back the finger ouput to the client. 


HOW TO COMPILE:
Open the terminal
Go to the project folder 
Run the command 'make'

HOW TO RUN THE SERVER:
Open the terminal 
Go to the folder in which finger server executable is present 
Run the command "$./fingerserver" to start the server or
Optionally add the executable fingerserver to your system path or $PATH environmental variable
and run "$fingerserver" to start the server. 


HOW TO RUN THE CLIENT:
Open the terminal 
GO to the folder in which finger server executable is present.
Run the command "$./fingerclient username@hostname:10222" eg:./fingerclient satyavad@seattleu.edu:10222 or 
Optionally add the executable fingerclient to your system path or $PATH environmental variable 
and run "$fingerclient username@hostname:10222" to initate client requests to the server located at hostname on portnname.

 
  



