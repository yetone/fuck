# unnamed programming language

Experimental programming language with intepreter written in C++

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