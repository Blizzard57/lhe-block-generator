# Report

## Comments
The code was generated in C++ with the LHEF parser used for parsing the data.

I have some thoughts regarding use of any parser. The questions were more focussed on just getting the data and breaking it up into chucks and storing them again without any analysis (except checking if the \<init> tags are same). 

I feel that it could have been done much faster had the events simply been copied using XML parser directly. Both the libraries wrap XML parser into their own parser. Both the parser as soon as the file is given to them, will initialize their variables and store information extracted from the LHE file into it. This takes time which is not required as the information will be directly dumped back into another file.

So I feel that doing it directly without a parser would be faster.

## Question 1
Why did you choose the parser that you did? 

## Answer
The choices of the parser where LHEF in c++ or pylhe in python. 

The pylhe parser did not contain any method to directly write files to the LHE format with any layer of abstraction. But LHEF contained a Writer which would directly write in a generated LHE file, so I chose that one.

## Question 2

### Part 1
How would you design the user interface of a set of command-line tools to perform tasks of this sort? 

### Answer
The easiest interface for using a bunch of tasks of this sort would be to either use a single ELF binary with multiple flags defining the feature required or a shell like prompt.

Either of these user interfaces have their merits and their usage depends on the granularity of the task that is to be performed.

For example, if in the second question the requirement was to have the 3rd block of 350 events to be merged with 10th block of 650 events into a new file while keeping the header and init information from the second file,  command line tool would look something like this :

> `./main_command merge-files --file1 file1 --file2 file2 --block1 350 --block2 650 --loc1 3 --loc2 10 --keep-headers 1`

Whereas in an shell like prompt, we can point the user to a config file when entering the command to perform. It would look something like this :

> `merge-files`

This command in the prompt leads to config.txt, such that

>config.txt <br>
// This is a default config file, entering file1 and file2 locations are compulsory, everything else can be kept at default values <br> <br>
`file1 = enter_file1_location` <br>
`file2 = enter_file2_location` <br>
`block1 = 500` <br>
`block2 = 500` <br>
`loc1 = 1` <br>
`loc2 = 1`<br>
`keep-headers = False` <br>
// Enter only if keep-headers is true <br>
`header-file = 1`

This provides with a much more granular control of what to do in a particular command. 

A prompt also helps in expanding the help command, where extensive documentation can be given for a particular sub command. 

This form of help would be much easier fot the user to digest than a man page which would dump all the information about all the sub commands at once.

On the other hand, if such granularity is not required, then a single command would be much more easier to work with and can be used.

### Part 2
What tools would you use to parse the command-line arguments and what are their pros and cons?

### Answer
Parsers like getopt and argp would be the tools I would use (in C++) as they are robust enough to understand and perform complex tasks.

Similar packages exist for other languages like python, where argparse can be used to perform a similar task.

### Part 3
What optimisations would you apply to make the operations efficient?

### Answer
For the operations to be efficient, I would do not use any LHE parser as I explained earlier to make the code more efficent. 

One of the best optimization that can be done is lazy programming where the parser is used and all the data it generates is stored on the stack. When the second file is loaded onto the program, the previous data (that was on the stack) is not removed but rather kept till the time that it is accessed, and will only change if the information present in the new one is different than the old one.

Some other house keeping optimizations can also be done, but the main ones are either not storing the data at all or keeping it stored till different information is accessed depending on the amount of tasks to be perfromed (A rough estimate can be taken, or a variable enabling `long_tasks` can be used)