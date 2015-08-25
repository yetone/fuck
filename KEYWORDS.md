# Keywords & Examples

This is the default keywords in fulang. You will be able to modify all these without recompiling

| Type                  	| Keywords
| -------------         	| :-------------  |
| Iteration Statements  	| [for foreach](examples/for.fu) [while](examples/while.fu)  |
| Selection Statements  	| [if elif else](examples/if.fu) |
| Jump Statements       	| [goto label](examples/goto.fu) [continue](examples/while.fu) [break](examples/while.fu) |
| Functions             	| [method return](examples/method.fu) |
| Namespaces            	| [namespace](examples/namespace.fu) |
| End Statement         	| [end](examples/conds.fu) |
| Using stdout          	| [out err](examples/hello-world.fu) |
| Using stdin           	| [read](examples/read.fu) |
| Call function         	| [call](examples/method.fu) |
| Boolean Constants     	| [true false](examples/bools.fu) |
| Boolean Operations   		| [and or xor](examples/conds.fu) |
| Used in for loops    		| [from to do in](examples/for.fu) |
| Variables					| [set front back incr decr unset](examples/vars.fu) |
| Arrays					| [array in ni](examples/arrays.fu) |
| Import files & functions 	| [use from](examples/use.fu) |

***

## Booleans

```go
true
false
```

### Multiple boolean conditions

```
$var := (true) && (false) || (false) || (true)

if (true) && (false) || (false) || (true)
...
end
```

#### Setting variable with boolean value

```go
set var false
set var true
$var := false
$var := true
```

## Pushing variable to stack

```set back``` is the default mode

### Synopsis

```set <position> <variable name> <expression>```

### Examples

```go
set
set front
set back
```

#### Setting variable

```go
set variable_name statement
```

is equal to

```go
set back variable_name statement
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

### Synopsis

```cpp
namespace <name>

end namespace
```

### Example

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

### Synopsis

```go
if <expr>

elif <expr>

else <expr

end
```

### Example

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

### Synopsis

```c
while <condition>
  ...
end
```

### Example

```c
$var := true
$another := false

while ($var) and ($another == false)
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

you can also loop an array

```php
array $arr [ "first", "second", "third" ]
foreach $key => $value in $arr
...
end
```

```i``` is the variable

```from 2``` declares the start value

```to 30``` declares the end value

```do $i + 1``` is the increase value (equal to ```$i := $i + 1```)

```in $arr``` loops all keys/values in array

### Example
```go
for $i; from 2; to 30; do $i + 1;
  out $i
end
```

## Increase or decrease

### Synopsis

```
decr $var [expr]
incr $var [expr]
```

**expr** will default to 1 if not found

### Example

```
$var := 10
# Will print "10"
out "$var" 
incr $var
# Will print 11
out "$var"
```

## Create array

### Synopsis

```php
array $var [item1, item2, key > expr]
```

### Example

```php
array $var [item1, key > item2]

out "$var"
out "$var(0)"
out "$var(key)"
```

## Import from another file

### Synopsis

```
use <file>
use <method> from <file>
```

### Example

```
use method1 from examples/file.fu
call method1

use examples/file.fu
call method2
```