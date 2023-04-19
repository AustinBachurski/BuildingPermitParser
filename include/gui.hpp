#ifndef GUI_HPP
#define GUI_HPP

#include <wx/wx.h>

class Frame : public wxFrame
{
public:
	Frame(const std::string& title);
private:
	void copyTotalPermits(wxCommandEvent& evt);
	void copyCommercialPermits(wxCommandEvent& evt);
	void copyResidentialPermits(wxCommandEvent& evt);
	void copyQuasiPermits(wxCommandEvent& evt);
	void copyHealthCare(wxCommandEvent& evt);
	void copySignificantCommercialCount(wxCommandEvent& evt);
	void copySignificantCommercialValue(wxCommandEvent& evt);
	void copyNewResidential(wxCommandEvent& evt);
	void copySFR_TH_Duplex(wxCommandEvent& evt);
	void copyMultiFamily(wxCommandEvent& evt);
	void copyNewResidentialValue(wxCommandEvent& evt);
	void copySignificantQuasiCount(wxCommandEvent& evt);
	void copySignificantQuasiValue(wxCommandEvent& evt);
	void copyTotalValue(wxCommandEvent& evt);
};

#endif