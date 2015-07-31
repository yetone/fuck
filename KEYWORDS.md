# Keywords & Examples

This is the default keywords in fulang. You will be able to modify all these without recompiling

| Type                  | Keywords
| -------------         | :-------------  |
| Iteration Statements  | [for](examples/for.fu) [while](examples/while.fu)  |
| Selection Statements  | [if elif else](examples/if.fu) |
| Jump Statements       | [goto label](examples/goto.fu) [continue](examples/while.fu) [break](examples/while.fu) |
| Functions             | [method return](examples/method.fu) |
| Namespaces            | [namespace](examples/namespace.fu) |
| End Statement         | [end](examples/conds.fu) |
| Using stdout          | [out err](examples/hello-world.fu) |
| Using stdin           | [read](examples/read.fu) |
| Call function         | [call](examples/method.fu) |
| Boolean Constants     | [true false](examples/bools.fu) |
| Boolean Operations    | [and or xor](examples/conds.fu) |
| Used in for loops     | [from to do](examples/for.fu) |
| Pause execution		| [sleep](examples/sleep.fu) | 

***

## Booleans

```go
true
false
```

#### Setting variable

```go
push var false
push var true
$var := false
$var := true
```

## Pushing variable to stack

```push back``` is the default mode

```go
push
push front
push back
```

#### Setting variable

```go
push variable_name statement
```

is equal to

```go
push back variable_name statement
```

and

```go
$variable_name := statement
```

##### Setting variable to 10

```go
$var := 5 + 5
```

##### Setting variable to string

```go
$var := "string"
```

## Declaring new namespace

```cpp
namespace ns
  method name
    // do things
  end method
end namespace
```

#### Calling method in namespace

```go
call ns.name()
```

#### Printing return value

```go
out "returned value is ns.name()"
```

## If constructs

```go
$var := 5

if ($var == 5)
  out "\$var is 5"
elif ($var < 5)
  out "\$var is below 5"
else
  out "\$var is $var"
endif
```

## While loop

```c
$var := true

while $var
  out "\$var is true"
end
```

## For loop

### Synopsis

```c
for <init>; from <start value>; to <end value>; do <change value>;
...
end
```

```i``` is the variable

```from 2``` declares the start value

```to 30``` declares the end value

```do $i + 1``` is the increase value (equal to ```$i := $i + 1```)

### Example
```go
for $i; from 2; to 30; do $i + 1;
  out $i
end
```

## Sleeping

### Synopsis

```sleep <ms>```

### Example:

```sleep 1000``` Will sleep for one second

Requires ```windows.h``` on Windows and ```unistd.h``` on *nix
