#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame
{
	wxPanel* InputOutputPanel;
	wxPanel* ControlsPanel1;
	wxPanel* ControlsPanel2;
	wxPanel* ControlsPanel3;
	wxPanel* ControlsPanel4;
    wxPanel* ControlsPanel5;
    wxPanel* ControlsPanel6;
	wxBoxSizer* InputOutputSizer;
	wxBoxSizer* ControlsSizer1;
	wxBoxSizer* ControlsSizer2;
	wxBoxSizer* ControlsSizer3;
	wxBoxSizer* ControlsSizer4;
    wxBoxSizer* ControlsSizer5;
    wxBoxSizer* ControlsSizer6;
	wxBoxSizer* MainSizer;
	
	wxStaticText* InputTextShow;
	wxTextCtrl* InputOutputTextCtrl;
	wxButton* MultiplyButton;
	wxButton* DivideButton;
	wxButton* AddButton;
	wxButton* SubtractButton;
	wxButton* ClearButton;
	wxButton* OpenBracketButton;
	wxButton* CloseBracketButton;
	wxButton* Button0;
	wxButton* Button1;
	wxButton* Button2;
	wxButton* Button3;
	wxButton* Button4;
	wxButton* Button5;
	wxButton* Button6;
	wxButton* Button7;
	wxButton* Button8;
	wxButton* Button9;
	wxButton* EqualButton;
	wxButton* SinButton;
    wxButton* CosButton;
    wxButton* TanButton;
    wxButton* CotButton;
    wxButton* SecButton;
    wxButton* CscButton;
	wxButton* DegButton;
	wxButton* LogButton;
    wxButton* LnButton;
	wxButton* ExponentButton;

	bool EqualPressed = false;
public:
	MainFrame(const wxString& title);
	void AddControls();
	void BindControls();
    void OnButtonClicked(wxCommandEvent& event);
	void OnType(wxCommandEvent& event);
    void CalculateAndShowOutput();
};


