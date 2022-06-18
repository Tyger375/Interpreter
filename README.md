# Interpreter

An interpreter made in C++ just for fun.

- [Syntax](#Syntax)
  - [Variables](#Variables)
    - [Updating variables](#Updating-variables)
    - [Lists](#Lists)
  - [Functions](#Functions)
    - [Internal functions](#Internal-functions)
  - [If statement](#If-statement)
  - [While loop](#While-loop)
- [Links and socials](#Links-and-socials)

## Compiler

This compiler has two modes: <br>
- Real-time
- File-compiler

## Syntax

### Variables
```
var int_variable = 2
var str_variable = "string variable"
var bool_variable = true
var list_variable = [int_variable, str_variable, bool_variable]
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

#### Lists

```
var list = ["hi"]

out(list[0])

list[0] = 2

list.add(["another list", 2])

list.remove(0)
list.remove(-1)

out(list[0][0])
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
else
{
    //code
}
```

"==",
"!=",
">=",
"=>",
"<=",
"=<" <br>
These are the available comparators<br>
else-if statements will be added soon.

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
