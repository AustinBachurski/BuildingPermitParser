#include <wx/clipbrd.h>
#include <wx/statline.h>
#include "gui.hpp"


Frame::Frame(const std::string& title) : wxFrame(nullptr, wxID_ANY, title, wxPoint(wxDefaultPosition), wxSize(wxDefaultSize), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
{
	wxPanel* panel = new wxPanel(this);

	const std::string previousYear = std::to_string(
		std::stoi(std::format("{:%Y}", std::chrono::system_clock::now())) - 1);


	Parser parse(previousYear);
	parse.parseSpreadsheet(".\\Building Permits.xlx");

	CreateStatusBar();

	wxStaticText* welcome = new wxStaticText(panel, wxID_ANY, ("Building Permits for " + previousYear), wxPoint(0, 10), wxSize(350, -1), wxALIGN_CENTRE_HORIZONTAL);
	wxStaticLine* lineOne = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 27), wxSize(350, 5));
	lineOne->SetBackgroundColour(*wxLIGHT_GREY);


	wxStaticText* totalBuildingPermitsIssued = new wxStaticText(panel, wxID_ANY, "Total Permits Issued: ", wxPoint(25, 50));
	wxStaticText* totalBuildingPermitsIssuedCOUNT = new wxStaticText(panel, wxID_ANY, parse.getCount(parse.key_TotalBuildingPermits), wxPoint(180, 50));
	wxButton* totalBuildingPermitsIssuedBUTTON = new wxButton(panel, wxID_ANY, "Copy", wxPoint(250, 45));
	totalBuildingPermitsIssuedBUTTON->Bind(wxEVT_BUTTON, [this, totalBuildingPermitsIssuedCOUNT](wxCommandEvent& event) { copyTotalPermits(event, totalBuildingPermitsIssuedCOUNT); });
	wxStaticLine* lineTwo = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 75), wxSize(350, 5));
	lineTwo->SetBackgroundColour(*wxLIGHT_GREY);


	wxStaticText* commercialPermits = new wxStaticText(panel, wxID_ANY, "Commercial Permits: ", wxPoint(25, 100));
	wxStaticText* commercialPermitsCOUNT = new wxStaticText(panel, wxID_ANY, parse.getCount(parse.key_CommercialPermits), wxPoint(180, 100));
	wxButton* commercialPermitsBUTTON = new wxButton(panel, wxID_ANY, "Copy", wxPoint(250, 95));
	commercialPermitsBUTTON->Bind(wxEVT_BUTTON, [this, commercialPermitsCOUNT](wxCommandEvent& event) { copyCommercialPermits(event, commercialPermitsCOUNT); });
	wxStaticLine* lineThree = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 125), wxSize(350, 5));
	lineThree->SetBackgroundColour(*wxLIGHT_GREY);


	wxStaticText* residentialPermits = new wxStaticText(panel, wxID_ANY, "Residential Permits: ", wxPoint(25, 150));
	wxStaticText* residentialPermitsCOUNT = new wxStaticText(panel, wxID_ANY, parse.getCount(parse.key_ResidentialPermits), wxPoint(180, 150));
	wxButton* residentialPermitsBUTTON = new wxButton(panel, wxID_ANY, "Copy", wxPoint(250, 145));
	residentialPermitsBUTTON->Bind(wxEVT_BUTTON, [this, residentialPermitsCOUNT](wxCommandEvent& event) { copyResidentialPermits(event, residentialPermitsCOUNT); });
	wxStaticLine* lineFour = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 175), wxSize(350, 5));
	lineFour->SetBackgroundColour(*wxLIGHT_GREY);

	
	wxStaticText* quasiPermits = new wxStaticText(panel, wxID_ANY, "Quasi Permits: ", wxPoint(25, 200));
	wxStaticText* quasiPermitsCOUNT = new wxStaticText(panel, wxID_ANY, parse.getCount(parse.key_QuasiPermits), wxPoint(180, 200));
	wxButton* quasiPermitsBUTTON = new wxButton(panel, wxID_ANY, "Copy", wxPoint(250, 195));
	quasiPermitsBUTTON->Bind(wxEVT_BUTTON, [this, quasiPermitsCOUNT](wxCommandEvent& event) { copyQuasiPermits(event, quasiPermitsCOUNT); });
	wxStaticLine* lineFive = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 225), wxSize(350, 5));
	lineFive->SetBackgroundColour(*wxLIGHT_GREY);


	wxStaticText* healthCare = new wxStaticText(panel, wxID_ANY, "Health Care Permits: ", wxPoint(25, 250));
	wxStaticText* healthCareCOUNT = new wxStaticText(panel, wxID_ANY, parse.getCount(parse.key_HealthCarePermits), wxPoint(180, 250));
	wxButton* healthCareBUTTON = new wxButton(panel, wxID_ANY, "Copy", wxPoint(250, 245));
	healthCareBUTTON->Bind(wxEVT_BUTTON, [this, healthCareCOUNT](wxCommandEvent& event) { copyHealthCare(event, healthCareCOUNT); });
	wxStaticLine* lineSix = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 275), wxSize(350, 5));
	lineSix->SetBackgroundColour(*wxLIGHT_GREY);

	
	wxStaticText* significantCommercialPermits = new wxStaticText(panel, wxID_ANY, "Significant\nCommercial Permits: ", wxPoint(25, 285));
	wxStaticText* significantCommercialPermitsCOUNT = new wxStaticText(panel, wxID_ANY, parse.getCount(parse.key_SignificantCommercial), wxPoint(180, 300));
	wxButton* significantCommercialPermitsBUTTON = new wxButton(panel, wxID_ANY, "Copy", wxPoint(250, 295));
	significantCommercialPermitsBUTTON->Bind(wxEVT_BUTTON, [this, significantCommercialPermitsCOUNT](wxCommandEvent& event) { copySignificantCommercialCount(event, significantCommercialPermitsCOUNT); });
	wxStaticLine* lineSeven = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 325), wxSize(350, 5));
	lineSeven->SetBackgroundColour(*wxLIGHT_GREY);


	wxStaticText* significantCommercialValue = new wxStaticText(panel, wxID_ANY, "Value of Significant\nCommercial Permits: ", wxPoint(25, 335));
	wxStaticText* significantCommercialValueVALUE = new wxStaticText(panel, wxID_ANY, parse.getValue(parse.key_SignificantCommercial), wxPoint(180, 350));
	wxButton* significantCommercialValueBUTTON = new wxButton(panel, wxID_ANY, "Copy", wxPoint(250, 345));
	significantCommercialValueBUTTON->Bind(wxEVT_BUTTON, [this, significantCommercialValueVALUE](wxCommandEvent& event) { copySignificantCommercialValue(event, significantCommercialValueVALUE); });
	wxStaticLine* lineEight = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 375), wxSize(350, 5));
	lineEight->SetBackgroundColour(*wxLIGHT_GREY);

	
	wxStaticText* newResidentlaiUnits = new wxStaticText(panel, wxID_ANY, "New Residential Units: ", wxPoint(25, 400));
	wxStaticText* newResidentlaiUnitsCOUNT = new wxStaticText(panel, wxID_ANY, parse.getCount(parse.key_NewResidentialUnits), wxPoint(180, 400));
	wxButton* newResidentlaiUnitsBUTTON = new wxButton(panel, wxID_ANY, "Copy", wxPoint(250, 395));
	newResidentlaiUnitsBUTTON->Bind(wxEVT_BUTTON, [this, newResidentlaiUnitsCOUNT](wxCommandEvent& event) { copyNewResidential(event, newResidentlaiUnitsCOUNT); });
	wxStaticLine* lineNine = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 425), wxSize(350, 5));
	lineNine->SetBackgroundColour(*wxLIGHT_GREY);

	
	wxStaticText* sfrTHduplexUnits = new wxStaticText(panel, wxID_ANY, "New SFR-TH-Duplex Units: ", wxPoint(25, 450));
	wxStaticText* sfrTHduplexUnitsCOUNT = new wxStaticText(panel, wxID_ANY, parse.getCount(parse.key_NewSFR_TH_DuplexUnits), wxPoint(180, 450));
	wxButton* sfrTHduplexUnitsBUTTON = new wxButton(panel, wxID_ANY, "Copy", wxPoint(250, 445));
	sfrTHduplexUnitsBUTTON->Bind(wxEVT_BUTTON, [this, sfrTHduplexUnitsCOUNT](wxCommandEvent& event) { copySFR_TH_Duplex(event, sfrTHduplexUnitsCOUNT); });
	wxStaticLine* lineTen = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 475), wxSize(350, 5));
	lineTen->SetBackgroundColour(*wxLIGHT_GREY);

	
	wxStaticText* multiFamilyUnits = new wxStaticText(panel, wxID_ANY, "Multi-family Units: ", wxPoint(25, 500));
	wxStaticText* multiFamilyUnitsCOUNT = new wxStaticText(panel, wxID_ANY, parse.getCount(parse.key_MultiFamilyUnits), wxPoint(180, 500));
	wxButton* multiFamilyUnitsBUTTON = new wxButton(panel, wxID_ANY, "Copy", wxPoint(250, 495));
	multiFamilyUnitsBUTTON->Bind(wxEVT_BUTTON, [this, multiFamilyUnitsCOUNT](wxCommandEvent& event) { copyMultiFamily(event, multiFamilyUnitsCOUNT); });
	wxStaticLine* lineEleven = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 525), wxSize(350, 5));
	lineEleven->SetBackgroundColour(*wxLIGHT_GREY);


	wxStaticText* residentialValue = new wxStaticText(panel, wxID_ANY, "Value of all\nNew Residential Units: ", wxPoint(25, 535));
	wxStaticText* residentialValueVALUE = new wxStaticText(panel, wxID_ANY, parse.getValue(parse.key_NewResidentialUnits), wxPoint(180, 550));
	wxButton* residentialValueBUTTON = new wxButton(panel, wxID_ANY, "Copy", wxPoint(250, 545));
	residentialValueBUTTON->Bind(wxEVT_BUTTON, [this, residentialValueVALUE](wxCommandEvent& event) { copyNewResidentialValue(event, residentialValueVALUE); });
	wxStaticLine* lineTwelve = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 575), wxSize(350, 5));
	lineTwelve->SetBackgroundColour(*wxLIGHT_GREY);


	wxStaticText* significantQuasi = new wxStaticText(panel, wxID_ANY, "Significant Quasi Permits: ", wxPoint(25, 600));
	wxStaticText* significantQuasiCOUNT = new wxStaticText(panel, wxID_ANY, parse.getCount(parse.key_SignificantQuasi), wxPoint(180, 600));
	wxButton* significantQuasiBUTTON = new wxButton(panel, wxID_ANY, "Copy", wxPoint(250, 595));
	significantQuasiBUTTON->Bind(wxEVT_BUTTON, [this, significantQuasiCOUNT](wxCommandEvent& event) { copySignificantQuasiCount(event, significantQuasiCOUNT); });
	wxStaticLine* lineThirteen = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 625), wxSize(350, 5));
	lineThirteen->SetBackgroundColour(*wxLIGHT_GREY);


	wxStaticText* quasiValue = new wxStaticText(panel, wxID_ANY, "Value of New or\nSignificant Quasi Permits: ", wxPoint(25, 635));
	wxStaticText* quasiValueVALUE = new wxStaticText(panel, wxID_ANY, parse.getValue(parse.key_SignificantQuasi), wxPoint(180, 650));
	wxButton* quasiValueBUTTON = new wxButton(panel, wxID_ANY, "Copy", wxPoint(250, 645));
	quasiValueBUTTON->Bind(wxEVT_BUTTON, [this, quasiValueVALUE](wxCommandEvent& event) { copySignificantQuasiValue(event, quasiValueVALUE); });
	wxStaticLine* lineFourteen = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 675), wxSize(350, 5));
	lineFourteen->SetBackgroundColour(*wxLIGHT_GREY);


	wxStaticText* totalValue = new wxStaticText(panel, wxID_ANY, "Total Value of all\nConstruction Types : ", wxPoint(25, 685));
	wxStaticText* totalValueVALUE = new wxStaticText(panel, wxID_ANY, parse.getValue(parse.key_TotalValue), wxPoint(180, 700));
	wxButton* totalValueBUTTON = new wxButton(panel, wxID_ANY, "Copy", wxPoint(250, 695));
	totalValueBUTTON->Bind(wxEVT_BUTTON, [this, totalValueVALUE](wxCommandEvent& event) { copyTotalValue(event, totalValueVALUE); });
	wxStaticLine* lineFifteen = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 725), wxSize(350, 5));
	lineFifteen->SetBackgroundColour(*wxLIGHT_GREY);

}

void Frame::copyTotalPermits(wxCommandEvent& evt, wxStaticText* text)
{
	if (wxTheClipboard->Open())
	{
		wxTheClipboard->SetData(new wxTextDataObject(text->GetLabelText()));
		wxTheClipboard->Close();
		wxLogStatus("'Total Permits Issued' copied to clipboard.");
	}
}

void Frame::copyCommercialPermits(wxCommandEvent& evt, wxStaticText* text)
{
	if (wxTheClipboard->Open())
	{
		wxTheClipboard->SetData(new wxTextDataObject(text->GetLabelText()));
		wxTheClipboard->Close();
		wxLogStatus("'Commercial Permits' copied to clipboard.");
	}
}

void Frame::copyResidentialPermits(wxCommandEvent& evt, wxStaticText* text)
{
	if (wxTheClipboard->Open())
	{
		wxTheClipboard->SetData(new wxTextDataObject(text->GetLabelText()));
		wxTheClipboard->Close();
		wxLogStatus("'Residential Permits' copied to clipboard.");
	}
}

void Frame::copyQuasiPermits(wxCommandEvent& evt, wxStaticText* text)
{
	if (wxTheClipboard->Open())
	{
		wxTheClipboard->SetData(new wxTextDataObject(text->GetLabelText()));
		wxTheClipboard->Close();
		wxLogStatus("'Quasi Permits' copied to clipboard.");
	}
}

void Frame::copyHealthCare(wxCommandEvent& evt, wxStaticText* text)
{
	if (wxTheClipboard->Open())
	{
		wxTheClipboard->SetData(new wxTextDataObject(text->GetLabelText()));
		wxTheClipboard->Close();
		wxLogStatus("'Health Care Permits' copied to clipboard.");
	}
}

void Frame::copySignificantCommercialCount(wxCommandEvent& evt, wxStaticText* text)
{
	if (wxTheClipboard->Open())
	{
		wxTheClipboard->SetData(new wxTextDataObject(text->GetLabelText()));
		wxTheClipboard->Close();
		wxLogStatus("'Significant Commercial Permits' copied to clipboard.");
	}
}

void Frame::copySignificantCommercialValue(wxCommandEvent& evt, wxStaticText* text)
{
	if (wxTheClipboard->Open())
	{
		wxTheClipboard->SetData(new wxTextDataObject(text->GetLabelText()));
		wxTheClipboard->Close();
		wxLogStatus("'Significant Commercial Value' copied to clipboard.");
	}
}

void Frame::copyNewResidential(wxCommandEvent& evt, wxStaticText* text)
{
	if (wxTheClipboard->Open())
	{
		wxTheClipboard->SetData(new wxTextDataObject(text->GetLabelText()));
		wxTheClipboard->Close();
		wxLogStatus("'New Residential Value' copied to clipboard.");
	}
}

void Frame::copySFR_TH_Duplex(wxCommandEvent& evt, wxStaticText* text)
{
	if (wxTheClipboard->Open())
	{
		wxTheClipboard->SetData(new wxTextDataObject(text->GetLabelText()));
		wxTheClipboard->Close();
		wxLogStatus("'New SFR-TH-Duplex Units' copied to clipboard.");
	}
}

void Frame::copyMultiFamily(wxCommandEvent& evt, wxStaticText* text)
{
	if (wxTheClipboard->Open())
	{
		wxTheClipboard->SetData(new wxTextDataObject(text->GetLabelText()));
		wxTheClipboard->Close();
		wxLogStatus("'Multi-family Units' copied to clipboard.");
	}
}

void Frame::copyNewResidentialValue(wxCommandEvent& evt, wxStaticText* text)
{
	if (wxTheClipboard->Open())
	{
		wxTheClipboard->SetData(new wxTextDataObject(text->GetLabelText()));
		wxTheClipboard->Close();
		wxLogStatus("'Value of all New Residential' copied to clipboard.");
	}
}

void Frame::copySignificantQuasiCount(wxCommandEvent& evt, wxStaticText* text)
{
	if (wxTheClipboard->Open())
	{
		wxTheClipboard->SetData(new wxTextDataObject(text->GetLabelText()));
		wxTheClipboard->Close();
		wxLogStatus("'Significant Quasi Permits' copied to clipboard.");
	}
}

void Frame::copySignificantQuasiValue(wxCommandEvent& evt, wxStaticText* text)
{
	if (wxTheClipboard->Open())
	{
		wxTheClipboard->SetData(new wxTextDataObject(text->GetLabelText()));
		wxTheClipboard->Close();
		wxLogStatus("'Significant Quasi Value' copied to clipboard.");
	}
}

void Frame::copyTotalValue(wxCommandEvent& evt, wxStaticText* text)
{
	if (wxTheClipboard->Open())
	{ 
		wxTheClipboard->SetData(new wxTextDataObject(text->GetLabelText()));
		wxTheClipboard->Close();
		wxLogStatus("'Total Value' copied to clipboard.");
	}
}