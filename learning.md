  /*A file descriptor (FD) is just an integer that represents an open resource in your program — such as:

files

sockets

pipes

Linux treats almost everything like a file, including network sockets.

🧠 Why 3?

In every program, the first three file descriptors are already in use:

FD	Meaning
0	stdin
1	stdout
2	stderr

So the next available file descriptor is 3.
Your socket is the fourth open resource, so it becomes FD 3.*/



To talk with aclient we will use  clientfd (client file discriptor)