#include "parser.hpp"

Columns column;

Parser::Parser(std::string& year) :
key_TotalBuildingPermits { "Total Building Permits Issued" },
key_CommercialPermits { "Commercial Permits" },
key_ResidentialPermits { "Residential Permits" },
key_QuasiPermits { "Quasi Permits" },
key_HealthCarePermits { "Health Care Permits" },
key_SignificantCommercial { "Significant Commercial" },
key_NewResidentialUnits { "New Residential Units" },
key_NewSFR_TH_DuplexUnits { "New SFR-TH-Duplex Units" },
key_MultiFamilyUnits { "Multi-family Units" },
key_SignificantQuasi { "Significant Quasi" },
key_TotalValue { "Total Value" },
m_counts {
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
m_totals {
            {key_SignificantCommercial, 0},
            {key_NewResidentialUnits, 0},
            {key_SignificantQuasi, 0},
            {key_TotalValue, 0}
},
m_firstDataRow { 2 },
m_index {},
m_populatedRows {},
m_targetYear { year }
{}

void Parser::addValue(const OpenXLSX::XLWorksheet& sheet, const std::string& key)
{
    if (isType<OpenXLSX::XLValueType::Integer>(sheet, column.projectValue))
    {
        m_totals[key] += sheet.cell(column.projectValue + m_index).value().get<int>();
    }
    else if (isType<OpenXLSX::XLValueType::Float>(sheet, column.projectValue))
    {
        m_totals[key] += sheet.cell(column.projectValue + m_index).value().get<float>();
    }
}

bool Parser::cellContains(const OpenXLSX::XLWorksheet& sheet, const std::string&& match) const
{
    return sheet.cell(column.buildingUse + m_index).value().get<std::string>() == match;
}

std::string Parser::getCount(const std::string& key) const
{
    return std::to_string(m_counts[key]);
}

std::string Parser::getValue(const std::string& key) const
{
    return std::to_string(std::llroundl(m_totals[key]));
}

bool Parser::isNewConstruction(const OpenXLSX::XLWorksheet& sheet) const
{
    return sheet.cell(column.classOfWork + m_index).value().get<std::string>().find("NEW") != std::string::npos;
}

template <typename T>
bool Parser::isType(const OpenXLSX::XLWorksheet& sheet, const std::string& column) const
{
    return sheet.cell(column + m_index).value().type() == T;
}

void Parser::parseSpreadsheet(const std::string&& fileName)
{
    const OpenXLSX::XLDocument spreadsheet(fileName);
    
    if (spreadsheet.isOpen())
    {
        auto sheet = spreadsheet.workbook().worksheet(m_targetYear);

        for (int i = m_firstDataRow; i <= m_populatedRows; ++i)
        {
            m_index = std::to_string(i);

            // TOTALS.
            ++m_counts[key_TotalBuildingPermits];
            addValue(sheet, key_TotalValue);

            // COMMERCIAL PERMITS.
            if (cellContains(sheet, "Commercial"))
            {
                ++m_counts[key_CommercialPermits];

                if (isNewConstruction(sheet))
                {
                    ++m_counts[key_SignificantCommercial];
                    addValue(sheet, key_SignificantCommercial);
                }
                else if (isType<OpenXLSX::XLValueType::Integer>(sheet, column.projectValue))
                {
                    if (valueIsGreaterThan250k(sheet))
                    {
                        ++m_counts[key_SignificantCommercial];
                        addValue(sheet, key_SignificantCommercial);
                    }
                }    
                else if (isType<OpenXLSX::XLValueType::Float>(sheet, column.projectValue))
                {
                    if (valueIsGreaterThan250k(sheet))
                    {
                        ++m_counts[key_SignificantCommercial];
                        addValue(sheet, key_SignificantCommercial);
                    }
                }                    
            }
            // RESIDENTIAL PERMITS.
            if (cellContains(sheet, "Single Family Residence")
                || cellContains(sheet, "Townhouse")
                || cellContains(sheet, "Duplex"))
            {
                ++m_counts[key_ResidentialPermits];

                if (isNewConstruction(sheet))
                {
                    ++m_counts[key_NewResidentialUnits];
                    ++m_counts[key_NewSFR_TH_DuplexUnits];
                    addValue(sheet, key_NewResidentialUnits);
                    if (cellContains(sheet, "Duplex"))
                    {
                        // Duplexes count as TWO units, so we hit it once during the initial "NEW" check above,
                        // then hit it again here if "Duplex".
                        ++m_counts[key_NewResidentialUnits];
                        ++m_counts[key_NewSFR_TH_DuplexUnits];
                    }
                }
            }
            // APARTMENTS
            if (cellContains(sheet, "Apartments"))
            {
                ++m_counts[key_ResidentialPermits];

                if (isType<OpenXLSX::XLValueType::Integer>(sheet, column.numberOfUnits))
                {
                    m_counts[key_NewResidentialUnits] += sheet.cell(column.numberOfUnits + m_index).value().get<int>();
                    m_counts[key_MultiFamilyUnits] += sheet.cell(column.numberOfUnits + m_index).value().get<int>();
                    addValue(sheet, key_NewResidentialUnits);
                }
            }
            // HEALTHCARE.
            if (cellContains(sheet, "Health Care"))
            {
                ++m_counts[key_HealthCarePermits];
                if (isNewConstruction(sheet))
                {
                    ++m_counts[key_SignificantQuasi];
                    addValue(sheet, key_SignificantQuasi);
                }
                else if (isType<OpenXLSX::XLValueType::Integer>(sheet, column.projectValue))
                {
                    if (valueIsGreaterThan250k(sheet))
                    {
                        ++m_counts[key_SignificantQuasi];
                        addValue(sheet, key_SignificantQuasi);    
                    }
                }
                else if (isType<OpenXLSX::XLValueType::Float>(sheet, column.projectValue))
                {
                    if (valueIsGreaterThan250k(sheet))
                    {
                        ++m_counts[key_SignificantQuasi];
                        addValue(sheet, key_SignificantQuasi);
                    }
                }
            }
            // QUASI.
            if (cellContains(sheet, "Quasi"))
            {
                ++m_counts[key_SignificantQuasi];
                if (isNewConstruction(sheet))
                {
                    ++m_counts[key_SignificantQuasi];
                    addValue(sheet, key_SignificantQuasi);
                }
                else if (isType<OpenXLSX::XLValueType::Integer>(sheet, column.projectValue))
                {
                    if (valueIsGreaterThan250k(sheet))
                    {
                        ++m_counts[key_SignificantQuasi];
                        addValue(sheet, key_SignificantQuasi);
                    }
                }
                else if (isType<OpenXLSX::XLValueType::Float>(sheet, column.projectValue))
                {
                    if (valueIsGreaterThan250k(sheet))
                    {
                        ++m_counts[key_SignificantQuasi];
                        addValue(sheet, key_SignificantQuasi);
                    }
                }
            }
        }
    }
}

void Parser::setPopulatedRows(const OpenXLSX::XLWorksheet& sheet)
{
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

bool Parser::valueIsGreaterThan250k(const OpenXLSX::XLWorksheet& sheet) const
{
    if (isType<OpenXLSX::XLValueType::Integer>(sheet, column.projectValue))
    {
        return sheet.cell(column.projectValue + m_index).value().get<int>() >= 250000;
    }
    else if (isType<OpenXLSX::XLValueType::Float>(sheet, column.projectValue))
    {
        return sheet.cell(column.projectValue + m_index).value().get<float>() >= 250000;
    }
}

Parser::~Parser()
{} // OpenXLSX objects use RAII, no need to explicitly close or destroy.
