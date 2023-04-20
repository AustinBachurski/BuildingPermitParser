#include "main.hpp"
#include "gui.hpp"

wxIMPLEMENT_APP(Application); // wxWidgets hides int main() so this is our entry point.

// Start application.
bool Application::OnInit()
{
	Frame* frame = new Frame("Building Permit Parse Tool");
	frame->SetClientSize(450, 730);
	frame->Show();
	return true;
}
