![logo](fulang-logo.png)
***

Experimental programming language with intepreter written in C++

__fulang__ is an acronym for **fuck you language** or **ful language**

## Arguments

| Argument    		| Description                           |
| --------    		| -----------                           |
| --verbose, -v   	| Prints what the intepreter is doing   |
| --colors    		| Colors stderr and verbose output      |

Every other argument is being parsed as input file

## Examples

### Declare normal method

```
method example1
  out "calling method"
end_method
```

### Declare method in namespace

```
namespace ns
  method example2
    out "calling method in namespace"
  end_method
end_namespace
```

### Invoke method

```
call example1
call ns.example2
```

### Printing

```
out "printing to stdout"
err "printing to stderr"

set var 10
out "printing $var"
out "actually printing the string \$var"
```

### Goto

```
:label
out "loop"
goto label
```

### Variables

```
set name "value"
set number 10
```

### Comment

```
# comment
// comment

# not valid
set var 10 # set variable to 10
```

### Think of the indentation when doing nested "blocks" (use tabs)

```
set var true

if $var
	if $var
		if $var
			out "ok"
		else
			out \$var is somehow false
		endif
	endif
endif
```

### While

```
while $var
	while $var
		out will only run once
		set var false
	end_while
end_while
```