#include "NouEngine.h"

NouEngine::NouEngine()
	:
	window(640, 480, "NouEnginge")
{

}

int NouEngine::Run()
{

	while (true)
	{
		
		if (NouWindow::ProcessMessage())
		{
			return NouWindow::LastQuitCode;
		}

		ExecuteFrame();
	}

	return -1;
}

void NouEngine::ExecuteFrame()
{
	static std::string b;
	static int c;
	c++;
	if (c >= 1000)
	{
		b.push_back('l');
		c = 0;
	}
	if (b.length() >= 50)
	{
		b.clear();
	}
	window.SetTitle(b.c_str());
}