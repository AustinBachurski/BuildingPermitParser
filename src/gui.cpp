#include <wx/clipbrd.h>
#include <wx/popupwin.h>
#include <wx/statline.h>
#include "gui.hpp"

// Application Window.
Frame::Frame(const std::string& title) : wxFrame(nullptr, wxID_ANY, title, wxPoint(wxDefaultPosition), wxSize(wxDefaultSize), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
{
	wxPanel* panel = new wxPanel(this);

    // We're only interested in permits for the previous calendar year.
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    int currentYear = std::gmtime(&now)->tm_year + 1900;
	const std::string lastYear = std::to_string(currentYear - 1);

	CreateStatusBar();

	Parser parse(lastYear);

    try
    {
	    parse.parseSpreadsheet("./Building Permits.xlsx");
    }
    catch(...)
    {
        wxMessageDialog* noSheet = new wxMessageDialog(NULL,
            "'Building Permits.xlsx' should be in the same folder as the executable.",
            "Spreadsheet error.", wxOK | wxICON_ERROR);
	    noSheet->ShowModal();
        wxLogStatus("Spreadsheet error, data is invalid!");
    }


	[[maybe_unused]] wxStaticText* welcome = new wxStaticText(panel, wxID_ANY, ("Building Permits for " + lastYear), wxPoint(0, 10), wxSize(450, -1), wxALIGN_CENTRE);
	wxStaticLine* lineOne = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 27), wxSize(450, 5));
	lineOne->SetBackgroundColour(*wxLIGHT_GREY);

    /*********************************************************************************
     * Display each value with a 'Copy' button to load the data into the clipboard.  *
     * Data will be used in a year end report, thousands separation commas as well   *
     * as cents are not desired, as such they are omitted from the display and copy. *
     *********************************************************************************/

    // Total Quantity of Permits.
	[[maybe_unused]] wxStaticText* totalBuildingPermitsIssued = new wxStaticText(panel, wxID_ANY, "Total Permits Issued: ", wxPoint(25, 50));
	wxStaticText* totalBuildingPermitsIssuedCOUNT = new wxStaticText(panel, wxID_ANY, parse.getCount(parse.key_TotalBuildingPermits), wxPoint(230, 50));
	wxButton* totalBuildingPermitsIssuedBUTTON = new wxButton(panel, wxID_ANY, "Copy", wxPoint(350, 45));
	totalBuildingPermitsIssuedBUTTON->Bind(wxEVT_BUTTON, [this, totalBuildingPermitsIssuedCOUNT](wxCommandEvent& event) { copyTotalPermits(event, totalBuildingPermitsIssuedCOUNT); });
	wxStaticLine* lineTwo = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 75), wxSize(450, 5));
	lineTwo->SetBackgroundColour(*wxLIGHT_GREY);

    // Quantity of Commercial Permits.
	[[maybe_unused]] wxStaticText* commercialPermits = new wxStaticText(panel, wxID_ANY, "Commercial Permits: ", wxPoint(25, 100));
	wxStaticText* commercialPermitsCOUNT = new wxStaticText(panel, wxID_ANY, parse.getCount(parse.key_CommercialPermits), wxPoint(230, 100));
	wxButton* commercialPermitsBUTTON = new wxButton(panel, wxID_ANY, "Copy", wxPoint(350, 95));
	commercialPermitsBUTTON->Bind(wxEVT_BUTTON, [this, commercialPermitsCOUNT](wxCommandEvent& event) { copyCommercialPermits(event, commercialPermitsCOUNT); });
	wxStaticLine* lineThree = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 125), wxSize(450, 5));
	lineThree->SetBackgroundColour(*wxLIGHT_GREY);

    // Quantity of Residential Permits.
	[[maybe_unused]] wxStaticText* residentialPermits = new wxStaticText(panel, wxID_ANY, "Residential Permits: ", wxPoint(25, 150));
	wxStaticText* residentialPermitsCOUNT = new wxStaticText(panel, wxID_ANY, parse.getCount(parse.key_ResidentialPermits), wxPoint(230, 150));
	wxButton* residentialPermitsBUTTON = new wxButton(panel, wxID_ANY, "Copy", wxPoint(350, 145));
	residentialPermitsBUTTON->Bind(wxEVT_BUTTON, [this, residentialPermitsCOUNT](wxCommandEvent& event) { copyResidentialPermits(event, residentialPermitsCOUNT); });
	wxStaticLine* lineFour = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 175), wxSize(450, 5));
	lineFour->SetBackgroundColour(*wxLIGHT_GREY);

	// Quantity of Quasi Permits.
	[[maybe_unused]] wxStaticText* quasiPermits = new wxStaticText(panel, wxID_ANY, "Quasi Permits: ", wxPoint(25, 200));
	wxStaticText* quasiPermitsCOUNT = new wxStaticText(panel, wxID_ANY, parse.getCount(parse.key_QuasiPermits), wxPoint(230, 200));
	wxButton* quasiPermitsBUTTON = new wxButton(panel, wxID_ANY, "Copy", wxPoint(350, 195));
	quasiPermitsBUTTON->Bind(wxEVT_BUTTON, [this, quasiPermitsCOUNT](wxCommandEvent& event) { copyQuasiPermits(event, quasiPermitsCOUNT); });
	wxStaticLine* lineFive = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 225), wxSize(450, 5));
	lineFive->SetBackgroundColour(*wxLIGHT_GREY);

    // Quantity of Health Care Permits.
	[[maybe_unused]] wxStaticText* healthCare = new wxStaticText(panel, wxID_ANY, "Health Care Permits: ", wxPoint(25, 250));
	wxStaticText* healthCareCOUNT = new wxStaticText(panel, wxID_ANY, parse.getCount(parse.key_HealthCarePermits), wxPoint(230, 250));
	wxButton* healthCareBUTTON = new wxButton(panel, wxID_ANY, "Copy", wxPoint(350, 245));
	healthCareBUTTON->Bind(wxEVT_BUTTON, [this, healthCareCOUNT](wxCommandEvent& event) { copyHealthCare(event, healthCareCOUNT); });
	wxStaticLine* lineSix = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 275), wxSize(450, 5));
	lineSix->SetBackgroundColour(*wxLIGHT_GREY);

	// Quantity of Commercial Permits Over $250k.
	[[maybe_unused]] wxStaticText* significantCommercialPermits = new wxStaticText(panel, wxID_ANY, "Significant\nCommercial Permits: ", wxPoint(25, 285));
	wxStaticText* significantCommercialPermitsCOUNT = new wxStaticText(panel, wxID_ANY, parse.getCount(parse.key_SignificantCommercial), wxPoint(230, 300));
	wxButton* significantCommercialPermitsBUTTON = new wxButton(panel, wxID_ANY, "Copy", wxPoint(350, 295));
	significantCommercialPermitsBUTTON->Bind(wxEVT_BUTTON, [this, significantCommercialPermitsCOUNT](wxCommandEvent& event) { copySignificantCommercialCount(event, significantCommercialPermitsCOUNT); });
	wxStaticLine* lineSeven = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 325), wxSize(450, 5));
	lineSeven->SetBackgroundColour(*wxLIGHT_GREY);

    // Total Value of Commercial Permits Over $250k.
	[[maybe_unused]] wxStaticText* significantCommercialValue = new wxStaticText(panel, wxID_ANY, "Value of Significant\nCommercial Permits: ", wxPoint(25, 335));
	wxStaticText* significantCommercialValueVALUE = new wxStaticText(panel, wxID_ANY, parse.getValue(parse.key_SignificantCommercial), wxPoint(230, 350));
	wxButton* significantCommercialValueBUTTON = new wxButton(panel, wxID_ANY, "Copy", wxPoint(350, 345));
	significantCommercialValueBUTTON->Bind(wxEVT_BUTTON, [this, significantCommercialValueVALUE](wxCommandEvent& event) { copySignificantCommercialValue(event, significantCommercialValueVALUE); });
	wxStaticLine* lineEight = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 375), wxSize(450, 5));
	lineEight->SetBackgroundColour(*wxLIGHT_GREY);

	// Quantity of New Residential Units.
	[[maybe_unused]] wxStaticText* newResidentlaiUnits = new wxStaticText(panel, wxID_ANY, "New Residential Units: ", wxPoint(25, 400));
	wxStaticText* newResidentlaiUnitsCOUNT = new wxStaticText(panel, wxID_ANY, parse.getCount(parse.key_NewResidentialUnits), wxPoint(230, 400));
	wxButton* newResidentlaiUnitsBUTTON = new wxButton(panel, wxID_ANY, "Copy", wxPoint(350, 395));
	newResidentlaiUnitsBUTTON->Bind(wxEVT_BUTTON, [this, newResidentlaiUnitsCOUNT](wxCommandEvent& event) { copyNewResidential(event, newResidentlaiUnitsCOUNT); });
	wxStaticLine* lineNine = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 425), wxSize(450, 5));
	lineNine->SetBackgroundColour(*wxLIGHT_GREY);

	// Quantity of New Single Family, Townhouse, or Duplex Units.
	[[maybe_unused]] wxStaticText* sfrTHduplexUnits = new wxStaticText(panel, wxID_ANY, "New SFR-TH-Duplex Units: ", wxPoint(25, 450));
	wxStaticText* sfrTHduplexUnitsCOUNT = new wxStaticText(panel, wxID_ANY, parse.getCount(parse.key_NewSFR_TH_DuplexUnits), wxPoint(230, 450));
	wxButton* sfrTHduplexUnitsBUTTON = new wxButton(panel, wxID_ANY, "Copy", wxPoint(350, 445));
	sfrTHduplexUnitsBUTTON->Bind(wxEVT_BUTTON, [this, sfrTHduplexUnitsCOUNT](wxCommandEvent& event) { copySFR_TH_Duplex(event, sfrTHduplexUnitsCOUNT); });
	wxStaticLine* lineTen = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 475), wxSize(450, 5));
	lineTen->SetBackgroundColour(*wxLIGHT_GREY);

	// Quantity of Multifamily Units.
	[[maybe_unused]] wxStaticText* multiFamilyUnits = new wxStaticText(panel, wxID_ANY, "Multi-family Units: ", wxPoint(25, 500));
	wxStaticText* multiFamilyUnitsCOUNT = new wxStaticText(panel, wxID_ANY, parse.getCount(parse.key_MultiFamilyUnits), wxPoint(230, 500));
	wxButton* multiFamilyUnitsBUTTON = new wxButton(panel, wxID_ANY, "Copy", wxPoint(350, 495));
	multiFamilyUnitsBUTTON->Bind(wxEVT_BUTTON, [this, multiFamilyUnitsCOUNT](wxCommandEvent& event) { copyMultiFamily(event, multiFamilyUnitsCOUNT); });
	wxStaticLine* lineEleven = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 525), wxSize(450, 5));
	lineEleven->SetBackgroundColour(*wxLIGHT_GREY);

    // Total Value of All New Residential Units.
	[[maybe_unused]] wxStaticText* residentialValue = new wxStaticText(panel, wxID_ANY, "Value of all\nNew Residential Units: ", wxPoint(25, 535));
	wxStaticText* residentialValueVALUE = new wxStaticText(panel, wxID_ANY, parse.getValue(parse.key_NewResidentialUnits), wxPoint(230, 550));
	wxButton* residentialValueBUTTON = new wxButton(panel, wxID_ANY, "Copy", wxPoint(350, 545));
	residentialValueBUTTON->Bind(wxEVT_BUTTON, [this, residentialValueVALUE](wxCommandEvent& event) { copyNewResidentialValue(event, residentialValueVALUE); });
	wxStaticLine* lineTwelve = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 575), wxSize(450, 5));
	lineTwelve->SetBackgroundColour(*wxLIGHT_GREY);

    // Quantity of Quasi Permits over $250k.
	[[maybe_unused]] wxStaticText* significantQuasi = new wxStaticText(panel, wxID_ANY, "Significant Quasi Permits: ", wxPoint(25, 600));
	wxStaticText* significantQuasiCOUNT = new wxStaticText(panel, wxID_ANY, parse.getCount(parse.key_SignificantQuasi), wxPoint(230, 600));
	wxButton* significantQuasiBUTTON = new wxButton(panel, wxID_ANY, "Copy", wxPoint(350, 595));
	significantQuasiBUTTON->Bind(wxEVT_BUTTON, [this, significantQuasiCOUNT](wxCommandEvent& event) { copySignificantQuasiCount(event, significantQuasiCOUNT); });
	wxStaticLine* lineThirteen = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 625), wxSize(450, 5));
	lineThirteen->SetBackgroundColour(*wxLIGHT_GREY);

    // Total Value of Quasi Permits over $250k.
	[[maybe_unused]] wxStaticText* quasiValue = new wxStaticText(panel, wxID_ANY, "Value of New or\nSignificant Quasi Permits: ", wxPoint(25, 635));
	wxStaticText* quasiValueVALUE = new wxStaticText(panel, wxID_ANY, parse.getValue(parse.key_SignificantQuasi), wxPoint(230, 650));
	wxButton* quasiValueBUTTON = new wxButton(panel, wxID_ANY, "Copy", wxPoint(350, 645));
	quasiValueBUTTON->Bind(wxEVT_BUTTON, [this, quasiValueVALUE](wxCommandEvent& event) { copySignificantQuasiValue(event, quasiValueVALUE); });
	wxStaticLine* lineFourteen = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 675), wxSize(450, 5));
	lineFourteen->SetBackgroundColour(*wxLIGHT_GREY);

    // Total Value of All Building Permits.
	[[maybe_unused]] wxStaticText* totalValue = new wxStaticText(panel, wxID_ANY, "Total Value of all\nConstruction Types : ", wxPoint(25, 685));
	wxStaticText* totalValueVALUE = new wxStaticText(panel, wxID_ANY, parse.getValue(parse.key_TotalValue), wxPoint(230, 700));
	wxButton* totalValueBUTTON = new wxButton(panel, wxID_ANY, "Copy", wxPoint(350, 695));
	totalValueBUTTON->Bind(wxEVT_BUTTON, [this, totalValueVALUE](wxCommandEvent& event) { copyTotalValue(event, totalValueVALUE); });
	wxStaticLine* lineFifteen = new wxStaticLine(panel, wxID_ANY, wxPoint(0, 725), wxSize(450, 5));
	lineFifteen->SetBackgroundColour(*wxLIGHT_GREY);

}

/***************************************************************************************************************
 * Load the displayed value for the section into the clipboard and update the status bar on copy button click. *
 ***************************************************************************************************************/
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
