Nathan Mailhot

# CS118 Project 1

This is the repo for winter22 cs118 project 1.

## Makefile

This provides a couple make targets for things.
By default (all target), it makes the `server` executables.

It provides a `clean` target, and `tarball` target to create the submission file as well.

You will need to modify the `Makefile` to add your userid for the `.tar.gz` turn-in at the top of the file.

## Academic Integrity Note

You are encouraged to host your code in private repositories on [GitHub](https://github.com/), [GitLab](https://gitlab.com), or other places.  At the same time, you are PROHIBITED to make your code for the class project public during the class or any time after the class.  If you do so, you will be violating academic honestly policy that you have signed, as well as the student code of conduct and be subject to serious sanctions.

## Provided Files

`server.c` is the entry points for the server part of the project.

## Testing

You can test your HTTP server directly using a browser, and/or a utility like `telnet` or `nc`. Your code will be graded using a script similar to the provided `test.sh`, and you should ensure that your code passes the provided tests. The final grading will use additional hidden tests to make sure your code follows the specification.

The output of `test.sh` would indicate which tests passed and failed along with the total number of passing tests. You can use this information to debug your code and make sure all the tests pass.

```
Checking HTTP status code ... pass
Checking content length ... pass
Checking if content is correct ... pass
Checking case insensitivity
Checking HTTP status code ... pass
Checking if content is correct ... pass
Checking GET without extension
Checking HTTP status code ... pass

Passed 6 tests, 0 failed
```

## TODO

    ###########################################################
    ##                                                       ##
    ## REPLACE CONTENT OF THIS FILE WITH YOUR PROJECT REPORT ##
    ##                                                       ##
    ###########################################################
