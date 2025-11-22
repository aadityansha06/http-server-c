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



It allows your server to reuse the port immediately, even if it’s in the TIME_WAIT state.

Normally, when you close a server on port 7070 and restart it quickly, you get:

Bind failed: Address already in use


Because the port enters TIME_WAIT for ~60 seconds.

SO_REUSEADDR fixes this.

✔️ After enabling it:

You can restart your server instantly.

No waiting.

No errors.

🧠 Why TIME_WAIT happens?

When a TCP connection closes, the OS keeps the port reserved for a bit to avoid old packets being mixed with new ones.

This is why port reuse is blocked by default.

🛠 Use it like this:
int opt = 1;
setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));


Place it right after creating the socket and before bind():

int sockfd = socket(AF_INET, SOCK_STREAM, 0);

int opt = 1;
setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

bind(sockfd, ...);
