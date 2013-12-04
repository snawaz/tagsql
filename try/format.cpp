
#include <iostream>
#include <fstream>
#include <string>

void test1()
{
    std::ifstream file("templates/from_expression.template++");
    std::ofstream out("from_.h++");
    std::string line;
    std::string space(12, ' ');
    while( std::getline(file, line) )
    {
	std::cout << space << line << std::endl;
	out << space << line << std::endl;	
    }
}
void test2()
{
    std::ifstream file("templates/from_expression.template++"); //, std::ios::binary);
    std::ofstream out("from_.h++");
    std::string space(12, ' ');
    char ch;
    std::cout << space;
    out << space;
    int braces = 0;
    while( file.read(&ch,1) )
    {
#if 0	
	std::cout << ch;
	out << ch;
	if ( ch == '\n' )
	{
	    std::cout << space;
	    out << space;
	}
#else
	if ( ch == '{' ) braces++;
	else if ( ch == '}' ) braces--;
#endif	
	if ( braces == 2 )
	{
	    std::cout <<  "(" << ch << ")";
	    out <<  "(" << ch << ")";
	}
    }
}

int main()
{
    //test1();
    test2();
}
