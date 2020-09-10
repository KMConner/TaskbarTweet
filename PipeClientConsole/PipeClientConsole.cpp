#include <iostream>
#include "PipeClient.h"

int main()
{
	std::cout << "Hello World!\n";
	TCHAR content[] = TEXT("hogehoge");
	TCHAR pipeName[] = TEXT("pipeName");

	Send(content, pipeName);
}
