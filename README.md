# fulang

Experimental programming language with intepreter written in C++

## Arguments

| Argument    		| Description                           |
| --------    		| -----------                           |
| --verbose, -v   	| Prints what the intepreter is doing   |
| --colors    		| Colors stderr and verbose output      |

Every other argument is being parsed as input file

## Methods

### Declare normal method

```
method example1
  out calling method
end_method
```

### Declare method in namespace

```
namespace ns
  method example2
    out calling method in namespace
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
out printing to stdout
err printing to stderr
```
