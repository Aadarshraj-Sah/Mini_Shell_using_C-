# Mini_Shell_using_C++

The command that has to be typed from command prompt to execute your program:
```
g++ 200101001.cpp -lreadline
./a.out
```

### Programme starts with a Welcome Note.

### Some Salient features of my programme are as follows : 

*	You can navigate between previously entered commands by using 
	upper and lower arrow keys.  
	(implemented using add_history(input) of readline header)

*	In each directory a history.txt file will be created storing 
	all your input commands while working in that directory. 

*	Internal commands such as:  
		'cd new'  - can be used to navigate to new directory.
			     (New directory created using 'mkdir new')  
		'cd'      - can be used return to home directory and 
			     will clear history.txt of previously working directory.  
		'history' - This will print all the inputs in that session.(using vectors)  
		'exit'    - You will exit the shell clearing history.txt of that session.  
		'help'    - will open My shell help.  
		other cmd - 'USER' , 'HOME' , 'PWD', 'PATH', 'SHELL' ,'TERM' 
				(can also be executed using 'echo $env' )

* 	All kinds of Environmental variable can be printed using cmd  'echo $env' . 
	

* 	You can also set any variable using cmd 'setenv Name Value'. 
	Print its value using 'printenv Name' or 'echo $Name' .

*	Single level piping (cmd1 | cmd2) can be performed both in 
	'external | external' and 'internal | external' .

*	input(<) redirection, output (>) redirection, append(>>) redirection 
	and combination of (< , >) and (< , >>) are implemented.  
	Eg-   
      cat <test.txt will open the file in read mode.  
	    cat > test.txt will open the file in write mode.  
	    cat >> test.txt will open the file in append mode.  
	    cat <input.txt > output.txt will copy contents of input.txt to output.txt  
	    sort <i.txt >>o.txt will append the sorted contents of i.txt to o.txt  

*	All external commands are implemented using execvp().  
	Eg - man, which, chsh, whereis, passwd, date, cal, clear,
	     sleep, alias, unalias, history, apropos, exit, logout, 
            shutdown, Is, cat, more, less, touch, cp, mv, rm, script,
	     find, mkdir, cd, pwd, rmdir, chmod, grep etc

*	Shell commands such as 'echo any_string' will output any_string.

*	All space (' ') sensitivity is well taken care. Using various self made functions. 
	like - spaceseperator , removeLeadingSpaces

* 	Error printing implemented in different places to identify which specific
	part of input has error.  
