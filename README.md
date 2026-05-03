*This project was created by `akkim` and `sejang` as part of the 42 curriculum.*

# Description
- **minishell** is a project to create a simple shell. In other words, the goal of this project is to create **your own small Bash**. 

- **The objective of the assignment** is to write a C program called `minishell` that functions as similarly to Bash as possible. At a minimum, it must satisfy the conditions specified in the assignment. 

# Instructions
- You must submit a `Makefile` to compile the source files. `relink` is not allowed.
- The `Makefile` must contain the rules $(NAME), all, clean, fclean, and re. 
- To submit the ‘bonus’, the ‘Makefile’ must include a **bonus** rule. 
- Files for the ‘bonus’ must have filenames in the format *_bonus.{c/h}. 
- To use libft, you must include the relevant files and a Makefile; the project's Makefile must compile the library using libft's Makefile before compiling the project. 
- Execution
    - Run the `make` command from the root directory.
    - Run the minishell program using various test cases.
    - Run the `make bonus` command to evaluate the bonus.

# References
- GNU Bash Manual (https://www.gnu.org/savannah-checkouts/gnu/bash/manual/bash.html): Referenced during implementation
- AI Reference: Generating test values during parsing tests
- AI Reference: Writing shell scripts for testing

# Features
- Built-in Commands
	**Command		Description**
	echo		Print arguments to standard output (-n flag supported)
	cd		Change the current working directorypwdPrint the current working directory
	export		Set or export environment variables
	unset		Remove environment variablesenvDisplay all environment variables
	exit		Exit the shell with an optional exit code

- Operators & Redirections
	**Syntax	Description**
	|		Pipe — connect stdout of one command to stdin of the next
	<		Input redirection
	>		Output redirection (overwrite)
	>>		Output redirection (append)
	<<		Here-document — read input until a delimiter is reached
	&&		Logical AND — execute next command only if previous succeeded (bonus)
	||		Logical OR — execute next command only if previous failed (bonus)

- Other Features
	-Environment variable expansion — $VAR and $? (last exit status)
	-Quote handling — single quotes preserve literal value; double quotes allow variable expansion
	-Signal handling — Ctrl-C interrupts current process, Ctrl-D exits the shell, Ctrl-\ is ignored in interactive mode
	-Interactive prompt with readline support and command history
	
# How to Run Tests
- Place the test files in the minishell directory and run: `test.sh`.
