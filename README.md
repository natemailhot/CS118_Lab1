Nathan Mailhot
UID: 105165575

High Level Design:
My server creates a socket that will fork a new child when requests come in.
If a request is a GET request, the server will parse the file out of it,
check if that file exists with or without extensions, open and read the data
within the file, and respond with the proper status. The socket is closed once
the file is opened.

Problems:
It took me a while to figure out how to account for files missing their extensions.
This ended up boiling down to an allocation error on my part with not using a constant char buffer.

It was also a bit difficult getting the proper responses.

Additional Resources:
Discussion Section B Recordings
https://www.php.net/manual/en/function.strcasecmp.php
https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
