# Lmao Interpreter

An interpreter made in C++ just for fun.

- [Running the interpreter](#Running-the-interpreter)
- [Syntax](#Syntax)
  - [Variables](#Variables)
    - [Updating variables](#Updating-variables)
    - [Casting variables](#Casting-variables)
    - [Lists](#Lists)
    - [Dictionaries](#Dicts)
  - [Functions](#Functions)
  - [Internal functions](#Internal-functions)
    - [Out](#Out)
    - [Outl](#Outl)
    - [In](#In)
    - [Type](#Type)
  - [If statement](#If-statement)
  - [While loop](#While-loop)
- [Links and socials](#Links-and-socials)

## Interpreter

This interpreter has two modes: <br>
- Real-time
- File-reader<br>

***

## Running the interpreter
These are the program arguments:<br>
- {PATH}<br>
The file parameter must be the first argument.<br>
If there isn't any file path, the Real-time interpreter will start
- --debug={bool}<br>
At the end of the interpreter,<br>
it will show all the internal variable, workspace variables and functions
- --savelogs={bool}<br>
All data shown with --debug, will be saved in a file called logs.txt<br>
**--debug parameter is not required for this argument**

***

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
- Boolean
- List

#### Updating variables

```
var variable = 2
variable = variable + 2 //<- 4
variable = variable - 1 //<- 3
var var2 = variable + " hello" //<- "2 hello"

var2 = variable - 2 //<- error
```

#### Casting variables

```
//Variable to integer
var cast_int = 2
var to_int = int(cast_int)
outl("to_int", to_int, type(to_int))

//Variable to string
var cast_str = 5
var to_str = string(cast_str)
outl("to_str", to_str, type(to_str))

//Variable to boolean
var cast_bool = "true"
var to_bool = bool(cast_bool)
outl("to_bool", to_bool, type(to_bool))

//Variable to list
var cast_list = "[true, 2, 5]"
var to_list = list(cast_list)
outl("to_list", to_list, type(to_list))
```

#### Lists

```
var list = ["hi"]

outl(list[0])

list[0] = 2

list.add(["another list", 2])

list.remove(0)
list.remove(-1)

outl(list[0][0])
outl(list.length())
```

### Dicts

```
var dict = {"hi":"hello"}

outl(dict["hi"])

dict["hi"] = 2

dict["other things"] = ["a list", 2]

outl(dict["other things"][0])
outl(dict.keys())
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

##### Out
```
out("hello world!")
```
"Out" function allows you to print something in the output. <br>

##### Outl
```
outl("hello world!")
```
"Outl" function allows you to print something in the output with \n at the end. <br>

##### In
```
var input = in()
```
"In" function allows you to get an input from the user. <br>

##### Type
```
var input = in()
var type = type(input)
```
"Type" function allows you to get the type of variable. <br> 

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
"=<",
">",
"<"<br>
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

***

## Links and socials

- Discord: `tyger 375#4141`<br>
- Web site: https://tyger375.tk/ <br>
- Twitter: https://twitter.com/375tyger <br>
