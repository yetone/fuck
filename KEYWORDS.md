# Keywords

This is the default keywords in fulang. You will be able to modify all these without recompiling

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

#### If constructs

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
