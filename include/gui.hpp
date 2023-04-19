#ifndef GUI_HPP
#define GUI_HPP

#include <wx/wx.h>
#include "parser.hpp"

class Frame : public wxFrame
{
public:
    // Application Window.
	Frame(const std::string& title);
private:
    // Clipboard methods.
	void copyTotalPermits(wxCommandEvent& evt, wxStaticText* text);
	void copyCommercialPermits(wxCommandEvent& evt, wxStaticText* text);
	void copyResidentialPermits(wxCommandEvent& evt, wxStaticText* text);
	void copyQuasiPermits(wxCommandEvent& evt, wxStaticText* text);
	void copyHealthCare(wxCommandEvent& evt, wxStaticText* text);
	void copySignificantCommercialCount(wxCommandEvent& evt, wxStaticText* text);
	void copySignificantCommercialValue(wxCommandEvent& evt, wxStaticText* text);
	void copyNewResidential(wxCommandEvent& evt, wxStaticText* text);
	void copySFR_TH_Duplex(wxCommandEvent& evt, wxStaticText* text);
	void copyMultiFamily(wxCommandEvent& evt, wxStaticText* text);
	void copyNewResidentialValue(wxCommandEvent& evt, wxStaticText* text);
	void copySignificantQuasiCount(wxCommandEvent& evt, wxStaticText* text);
	void copySignificantQuasiValue(wxCommandEvent& evt, wxStaticText* text);
	void copyTotalValue(wxCommandEvent& evt, wxStaticText* text);
};

#endif