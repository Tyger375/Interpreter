compiler = g++


out.exe: main.cpp ./Variable/Variable.cpp ./Variable/Variable.h ./Interpreter/Interpreter.cpp ./Interpreter/Interpreter.h ./Other/Utilities/Utilities.cpp ./Other/Utilities/Utilities.h ./Function/Function.cpp ./Function/Function.h ./Other/DebugInterprete.cpp
	${compiler} main.cpp ./Variable/Variable.cpp ./Interpreter/Interpreter.cpp ./Other/Utilities/Utilities.cpp ./Function/Function.cpp ./Other/DebugInterprete.cpp