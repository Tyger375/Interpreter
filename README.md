# Interpreter

An interpreter made in C++ just for fun.

## Compiler

This compiler has two modes: <br>
- Real-time
- File-compiler

## Syntax

### Variables
```
var int_variable = 2
var str_variable = "string variable"
```

Available types:<br>
- Integer
- String

#### Updating variables

```
var variable = 2
variable = variable + 2 //<- 4
variable = variable - 1 //<- 1
var var2 = variable + " hello" //<- "2 hello"

var2 = variable - 2 //<- error
```

### Functions

```
func function_name(param)
{
    var newval = param + 2
    return newval
}

//calling the function
var returned = function_name("hi") //<- "hi2"
```

#### Internal functions

```
out("hello world!")
```

"Out" function allows you to print something in the output. <br>

```
var input = in()
```

"In" function allows you to get an input from the user. <br>

```
var input = in()
var type = type(input)
```

"Type" function allows you to get the type of a variable. <br> 

### If statement

```
if (1 == 2)
{
    //code
}
```

Now, only "==" is available <br>
else/else-if statements will be added soon.

### While loop

```
var num = 0
while (num < 10)
{
    //code
    num = num + 1
}
```

## Links and socials

- Discord: `tyger 375#4141`<br>
- Web site: https://tyger375.tk/ <br>
- Twitter: https://twitter.com/375tyger <br>
