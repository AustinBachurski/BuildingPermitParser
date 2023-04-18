#include "main.hpp"
#include "gui.hpp"

wxIMPLEMENT_APP(Application);

bool Application::OnInit()
{
	Frame* frame = new Frame("Building Permit Parse Tool");
	frame->SetClientSize(350, 730);
	frame->Show();
	return true;
}
