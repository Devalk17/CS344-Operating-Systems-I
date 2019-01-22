Program Py – CS 344
Overview
In this assignment, you will be given ZERO instruction in HOW to accomplish it! The actual requirements are very simple to satisfy. This is a test of how well you can research the topic and satisfy the conditions of this Assignment on your own. This is how much of the real world works - you'll be thrown into situations that are full of unknowns, and you'll need to come up with a solution!

You are becoming a computer scientist, not a technician: you will learn how to handle ANY language thrown at you, not just the ones you are spoon-fed, by learning good research habits and techniques. The more problems you encounter and solve, the greater problem set you'll be able to infer solutions to from your background of knowledge.

This is by far the easiest assignment in this course - it's worth the least points, with the most time to accomplish it. You will use what you've learned here in later courses at OSU.

Specifications
For this assignment, you will create a script in the Python language. You can read more about Python here:

http://www.python.org/ (Links to an external site.)Links to an external site.

All execution, compiling, and testing of this script should ONLY be done in the bash prompt on our class server!

Your script must satisfy the following requirements:

Be contained in one single file, called mypython.py.
When executed, create 3 files in the same directory as your script, each named differently (the name of the files is up to you), which remain there after your script finishes executing. Each of these 3 files must contain exactly 10 random characters from the lowercase alphabet, with no spaces ("hoehdgwkdq", for example). The final (eleventh) character of each file should be a newline character.
When executed, print out on screen the contents of the 3 files it is creating in exactly the format given below.
When executed, after the file contents of all three files have been printed, print out two random integers (whose range is from 1 to 42, inclusive), and print out the product of the two numbers, again in exactly the format given below.
You do not have to parse and read the data back in from the files created in step 2 in order to complete step 3. For step 3, just dump the contents that you already randomly generated in your program directly onto the screen, if that's the easiest way for you. I recommend that you look into sys.stdout.write() for outputting text, as it will allow you to control newlines better.

Note that the visual format for all of this is completely up to you! The graders will simply be checking for the above requirements to assign your grade. Further, no help on this assignment will be provided by the Instructor or TAs at any time.

You can choose to use Python 2 or Python 3. The TAs will try all three of these commands to find one that works with your script:

$ python mypython.py
$ python2.7 mypython.py
$ python3 mypython.py
Python is NOT a topic that will be covered on the Final.

Example Output
Here's an example of the script being run; all output is random except for the last line.

$ python mypython.py
vwdzwuxwvh
nibqaackrp
pxeugdqnwc
8
10
80
Hints
I HIGHY recommend that you develop this program directly on our server, instead of one your personal machine. Doing so will prevent you from having problems transferring the program back and forth, which can cause compatibility issues.

If you do see ^M characters all over your files, try this command:

% dos2unix bustedFile
What to Turn In and When
Please submit simply your script.

The due date given below is the last minute that this can be turned in for full credit. The "available until" date is NOT the due date, but instead closes off submissions for this assignment automatically once 48 hours past the due date has been reached, in accordance with our Syllabus Grading policies.

Grading
Note that this assignment is PASS/FAIL, meaning all of the points are awarded or not awarded based on whether or not you accomplish ALL of the requirements! If you do not accomplish all of them, you will receive a zero. But of course you'll accomplish them, because this Assignment is really easy! It's practically extra credit!

Remember that this will be graded only on os1, in a bash prompt. Make sure your code works there!

The TAs will use this exact set of instructions: ProgramPy Grading.pdfPreview the document to grade your submission.
