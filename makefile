compiler = g++


out.exe: main.cpp Interpreter/Interpreter.cpp Function/Function.cpp Function/Function.h Other/DebugInterpreter.cpp Other/Utilities/Utilities.cpp Other/Utilities/Utilities.h Variable/Variable.cpp Variable/Variable.h While/While.cpp While/While.h Interpreter/Interpreter.h Operations/operations.cpp Variable/internal-functions.cpp
	${compiler} main.cpp Interpreter/Interpreter.cpp Function/Function.cpp Other/DebugInterpreter.cpp Other/Utilities/Utilities.cpp Variable/Variable.cpp While/While.cpp Operations/operations.cpp Variable/internal-functions.cpp