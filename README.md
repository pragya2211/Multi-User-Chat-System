## Description of the code:
Folder consists of:
- Server code
- Client Code
- Makefile


The communication method between the server and various clients chosen by me is
Domain Sockets. The communication between the server and client takes place by
giving the same path to both of them.
### Server Code:
In this code, a socket is opened using AF_UNIX protocol. The code has
an array of type client(a struct) which stores every new client requesting to
connect to the server. There is an array of type pthread_t which stores
every thread created to execute the connect_clients function. This function
handles all the inputs given by the user. For reading the message, I have
used recv() function to receive the message. Then, I compared if the string
was ‘1’ or ‘2’ or ‘3’ or ‘4’. After this, the further sub parsing according to the
menu takes place.
### Client Code:
This code also creates a socket using AF_UNIX protocol. Then the client
is connected to server through connect() function. After this input is taken
from the user based on the menu provided to them.
### Makefile:
Makefile is created to compile server and client’s code.
## Compiling:
Use the make command on your Linux Terminal to compile the program using
Makefile. Run the server code on one terminal, and all client code on different
terminals.
## Testing and Inputs to be given:
The user is displayed the menu as follows after successful socket creation,
binding and listening:
- Send a message to other client through client number
- Send the message to all the clients
- See available clients
- Exit from the client
The user should give input as:
-“1 (Client Number) (Message to be sent)” to send the message to
individual client through client number.
- “2 (Message)” to send the message to all the clients
- “3” to see the list of available clients
- “4” to Exit from the current client
Expected Outputs:
- If input was given as “1 (Client Number) (Message to be sent)”, the user
can see their message on the terminal of client whose client number is
given.
- If input was given as “2 (Message)”, the user can see message on all
terminals of clients as well as a display on server that message has been
sent to all the clients.
- If input was given as “3”, the user can see all the available clients along
with their IDs on server’s terminal.
- If input was given as “4”, the code for client would terminate on that
terminal with “Exiting” message and on the server’s terminal, display
message on Connection broke as well as removal of client from the array
takes place.
## Error values and Interpretation:
### Input errors:
- If an incorrect input is given by the user, an error message of “Incorrect
Input given” is displayed.
- If the user enters the Client Number which is not available, “Incorrect
Client number entered” is displayed.
### Other errors:
- If there is an error in socket creation it is displayed.
- If there is an error in binding, it is displayed.
- If there is an error in listening in the sockets.
- If the file exists or not on which the socket is opened for the client
side code.
- If there is an error in sending or receiving among the clients and
server.
