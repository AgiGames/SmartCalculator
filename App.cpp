#include "App.h"
#include "MainFrame.h"
#include <wx/wx.h>

wxIMPLEMENT_APP(App);

bool App::OnInit() {
	MainFrame* CalculatorMainFrame = new MainFrame("Calculator");
	CalculatorMainFrame->SetClientSize(400, 650);
	CalculatorMainFrame->Center();
	CalculatorMainFrame->SetBackgroundColour(wxColour(30, 30, 30));
	CalculatorMainFrame->Show();
	return true;
}

void App::DeleteDialog() {
}