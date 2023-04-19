#include "parser.hpp"

Parser::Parser(std::string lastYear) :
    key_TotalBuildingPermits{ "Total Building Permits Issued" },
    key_CommercialPermits{ "Commercial Permits" },
    key_ResidentialPermits{ "Residential Permits" },
    key_QuasiPermits{ "Quasi Permits" },
    key_HealthCarePermits{ "Health Care Permits" },
    key_SignificantCommercial{ "Significant Commercial" },
    key_NewResidentialUnits{ "New Residential Units" },
    key_NewSFR_TH_DuplexUnits{ "New SFR-TH-Duplex Units" },
    key_MultiFamilyUnits{ "Multi-family Units" },
    key_SignificantQuasi{ "Significant Quasi" },
    key_TotalValue{ "Total Value" },
    m_counts{
                {key_TotalBuildingPermits, 0},
                {key_CommercialPermits, 0},
                {key_ResidentialPermits, 0},
                {key_QuasiPermits, 0},
                {key_HealthCarePermits, 0},
                {key_SignificantCommercial, 0},
                {key_NewResidentialUnits, 0},
                {key_NewSFR_TH_DuplexUnits, 0},
                {key_MultiFamilyUnits, 0},
                {key_SignificantQuasi, 0}
    },
    m_totals{
                {key_SignificantCommercial, 0},
                {key_NewResidentialUnits, 0},
                {key_SignificantQuasi, 0},
                {key_TotalValue, 0}
    },
    m_firstDataRow{ 2 }, // Skip the header row.
    m_index{},
    m_populatedRows{},
    m_targetSheet{ lastYear }
{}

Columns column;

void Parser::addValue(const OpenXLSX::XLWorksheet& sheet, const std::string& key)
{ 
    // Add the values from the target cell to the totals map at specified key.
    if (isTypeInt(sheet, column.projectValue))
    {
        m_totals.at(key) += sheet.cell(column.projectValue + m_index).value().get<int>();
    }
    else if (isTypeFloat(sheet, column.projectValue))
    {
        m_totals.at(key) += sheet.cell(column.projectValue + m_index).value().get<float>();
    }
}

bool Parser::cellContains(const OpenXLSX::XLWorksheet& sheet, const std::string&& searchString) const
{
    // Return true if the cell contains the exact search string.
    return sheet.cell(column.buildingUse + m_index).value().get<std::string>() == searchString;
}

// Getter for the counts map.
std::string Parser::getCount(const std::string& key) const
{
    return std::to_string(m_counts.at(key));
}

// Getter for the values map.
std::string Parser::getValue(const std::string& key) const
{
    // Building department is not interested in the pennies.
    return std::to_string(std::llroundl(m_totals.at(key)));
}

bool Parser::isSignificant(const OpenXLSX::XLWorksheet& sheet) const
{
    // A permit is considered "Significant" if it's value is $250,000 or more.
    // Return true if the value in the cell is 250,000 or more.
    if (isTypeInt(sheet, column.projectValue))
    {
        return sheet.cell(column.projectValue + m_index).value().get<int>() >= 250000;
    }
    else if (isTypeFloat(sheet, column.projectValue))
    {
        return sheet.cell(column.projectValue + m_index).value().get<float>() >= 250000.0f;
    }
    else
    {
        return false;
    }
}

bool Parser::isNewConstruction(const OpenXLSX::XLWorksheet& sheet) const
{
    // Return true if the cell contains the string "NEW".
    return sheet.cell(column.classOfWork + m_index).value().get<std::string>().find("NEW") != std::string::npos;
}

/*************************************************************
 * OpenXLSX is very picky about the type that is extracted   *
 * from the cell. isTypeFloat() and isTypeInteger() check    *
 * the type prior to other methods accessing the cells data. *
 *************************************************************/
bool Parser::isTypeFloat(const OpenXLSX::XLWorksheet& sheet, const std::string& column) const
{
    // Return true if the cell contains a float.
    return sheet.cell(column + m_index).value().type() == OpenXLSX::XLValueType::Float;
}

bool Parser::isTypeInt(const OpenXLSX::XLWorksheet& sheet, const std::string& column) const
{
    // Return true if the cell contains an integer.
    return sheet.cell(column + m_index).value().type() == OpenXLSX::XLValueType::Integer;
}

/*************************************************************************************
 * parseSpreadsheet() is where the work gets done, I'm not a fan of all the nesting  *
 * and ifs but I felt that it was more efficient to iterate once and check all the   *
 * cases rather than iterate numerous times using a seperate function for each case. *
 *************************************************************************************/
void Parser::parseSpreadsheet(const std::string&& fileName)
{
    OpenXLSX::XLDocument spreadsheet(fileName);

    if (spreadsheet.isOpen())
    {
        OpenXLSX::XLWorksheet sheet = spreadsheet.workbook().worksheet(m_targetSheet);
        setPopulatedRows(sheet);

        for (int i = m_firstDataRow; i <= m_populatedRows; ++i)
        {
            m_index = std::to_string(i);

            /***************************
             * Total Counts and Values *
             ***************************/
            ++m_counts.at(key_TotalBuildingPermits);
            addValue(sheet, key_TotalValue);

            /**********************
             * Commercial Permits *
             **********************/
            if (cellContains(sheet, "Commercial"))
            {
                ++m_counts.at(key_CommercialPermits);

                if (isNewConstruction(sheet))
                {
                    ++m_counts.at(key_SignificantCommercial);
                    addValue(sheet, key_SignificantCommercial);
                }
                else if (isTypeInt(sheet, column.projectValue))
                {
                    if (isSignificant(sheet))
                    {
                        ++m_counts.at(key_SignificantCommercial);
                        addValue(sheet, key_SignificantCommercial);
                    }
                }
                else if (isTypeFloat(sheet, column.projectValue))
                {
                    if (isSignificant(sheet))
                    {
                        ++m_counts.at(key_SignificantCommercial);
                        addValue(sheet, key_SignificantCommercial);
                    }
                }
            }
            /***********************
             * Residential Permits *
             ***********************/
            else if (cellContains(sheet, "Single Family Residence")
                  || cellContains(sheet, "Townhouse")
                  || cellContains(sheet, "Duplex"))
            {
                ++m_counts.at(key_ResidentialPermits);

                if (isNewConstruction(sheet))
                {
                    ++m_counts.at(key_NewResidentialUnits);
                    ++m_counts.at(key_NewSFR_TH_DuplexUnits);
                    addValue(sheet, key_NewResidentialUnits);
                    if (cellContains(sheet, "Duplex"))
                    {  
                        // Duplexes count as TWO units, so we increment once during the
                        // initial "NEW" check above, then increment again here if "Duplex".
                        ++m_counts.at(key_NewResidentialUnits); 
                        ++m_counts.at(key_NewSFR_TH_DuplexUnits); 
                    } 
                }     
            }
            /**************
             * Apartments *
             **************/
            else if (cellContains(sheet, "Apartments"))
            {
                ++m_counts.at(key_ResidentialPermits);

                if (isTypeInt(sheet, column.numberOfUnits))
                {
                    // The building department wants a unit count for multi-family structures.
                    m_counts.at(key_NewResidentialUnits) += sheet.cell(column.numberOfUnits + m_index).value().get<int>();
                    m_counts.at(key_MultiFamilyUnits) += sheet.cell(column.numberOfUnits + m_index).value().get<int>();
                    addValue(sheet, key_NewResidentialUnits);
                }
            }
            /**********************
             * Healthcare Permits *
             **********************/
            else if (cellContains(sheet, "Health Care"))
            {
                ++m_counts.at(key_HealthCarePermits);
                if (isNewConstruction(sheet))
                {
                    ++m_counts.at(key_SignificantQuasi);
                    addValue(sheet, key_SignificantQuasi);
                }
                else if (isTypeInt(sheet, column.projectValue))
                {
                    if (isSignificant(sheet))
                    {
                        ++m_counts.at(key_SignificantQuasi);
                        addValue(sheet, key_SignificantQuasi);
                    }
                }
                else if (isTypeFloat(sheet, column.projectValue))
                {
                    if (isSignificant(sheet))
                    {
                        ++m_counts.at(key_SignificantQuasi);
                        addValue(sheet, key_SignificantQuasi);
                    }
                }
            }
            /*****************
             * Quasi Permits *
             *****************/
            else if (cellContains(sheet, "Quasi"))
            {
                ++m_counts.at(key_QuasiPermits);
                if (isNewConstruction(sheet))
                {
                    ++m_counts.at(key_SignificantQuasi);
                    addValue(sheet, key_SignificantQuasi);
                }
                else if (isTypeInt(sheet, column.projectValue))
                {
                    if (isSignificant(sheet))
                    {
                        ++m_counts.at(key_SignificantQuasi);
                        addValue(sheet, key_SignificantQuasi);
                    }
                }
                else if (isTypeFloat(sheet, column.projectValue))
                {
                    if (isSignificant(sheet))
                    {
                        ++m_counts.at(key_SignificantQuasi);
                        addValue(sheet, key_SignificantQuasi);
                    }
                }
            }
        }
    }
}

void Parser::setPopulatedRows(const OpenXLSX::XLWorksheet& sheet)
{
    // Iterate through spreadsheet rows incrementing the count until an empty cell is found.
    auto range = sheet.range(OpenXLSX::XLCellReference("C1"), OpenXLSX::XLCellReference(OpenXLSX::MAX_ROWS, 2));

    for (const auto& cell : range)
    {
        if (cell.value().type() == OpenXLSX::XLValueType::Empty)
        {
            break;
        }
        ++m_populatedRows;
    }
}

Parser::~Parser()
{}