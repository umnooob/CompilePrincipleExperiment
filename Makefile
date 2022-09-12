main:main.cpp SyntacticAnalysis.o LexicalAnalysis.o
	g++ main.cpp SyntacticAnalysis.o LexicalAnalysis.o -o main
SyntacticAnalysis.o:SyntacticAnalysis.cpp
	g++ -c SyntacticAnalysis.cpp -o SyntacticAnalysis.o
LexicalAnalysis.o:LexicalAnalysis.cpp
	g++ -c LexicalAnalysis.cpp -o LexicalAnalysis.o
clean:
	rm *.o main