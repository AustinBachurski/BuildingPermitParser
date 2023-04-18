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
m_populatedRows {},
m_targetYear { year }
{}

std::string Parser::getCount(const std::string& key) const
{
    return std::to_string(m_counts[key]);
}

std::string Parser::getValue(const std::string& key) const
{
    return std::to_string(std::llroundl(m_totals[key]));
}

template <typename T>
bool Parser::isType(const OpenXLSX::XLWorksheet& sheet, const Column& column, const int index) const
{
    return worksheet.cell(column + std::to_string(i)).value().type() == T;
}

void Parser::parseSpreadsheet(std::string&& fileName)
{
    const OpenXLSX::XLDocument spreadsheet(fileName);
    
    if (spreadsheet.is_open())
    {
        auto worksheet = spreadsheet.workbook().worksheet(m_targetYear);

        for (int i = m_firstDataRow; i <= m_populatedRows; ++i)
        {
            // TOTALS.
            ++m_counts[key_TotalBuildingPermits];
            sumValues(worksheet, key_TotalValue, i);

            // COMMERCIAL PERMITS.
            if (worksheet.cell(
                column.buildingUse + std::to_string(i)).value().get<std::string>() == "Commercial")
            {
                ++m_counts[key_CommercialPermits];

                if (worksheet.cell(column.classOfWork + std::to_string(i)).value().get<std::string>().find("NEW") != std::string::npos)
                {
                    ++m_counts[key_SignificantCommercialPermits];
                    sumValues(worksheet, key_SignificantCommercial, i);
                }
// WORKING HERE!                else if (isType<OpenXLSX::XLValueType::Integer>(worksheet, column.projectValue, i)
                    worksheet.cell(column.projectValue + std::to_string(i)).value().type() == OpenXLSX::XLValueType::Integer
                    && worksheet.cell(column.projectValue + std::to_string(i)).value().get<int>() >= 250000)
                {
                    ++m_counts[key_SignificantCommercialPermits];
                    sumValues(worksheet, key_SignificantCommercial, i);
                }
                else if (worksheet.cell(column.projectValue + std::to_string(i)).value().type() == OpenXLSX::XLValueType::Float
                    && worksheet.cell(column.projectValue + std::to_string(i)).value().get<float>() >= 250000)
                {
                    ++m_counts[key_SignificantCommercialPermits];
                    sumValues(worksheet, key_SignificantCommercial, i);
                }
            }

            // RESIDENTIAL PERMITS.
            if (worksheet.cell(column.buildingUse + std::to_string(i)).value().get<std::string>() == "Single Family Residence"
                || worksheet.cell(column.buildingUse + std::to_string(i)).value().get<std::string>() == "Townhouse"
                || worksheet.cell(column.buildingUse + std::to_string(i)).value().get<std::string>() == "Duplex")
            {
                ++m_counts[key_ResidentialPermits];

                if (worksheet.cell(column.classOfWork + std::to_string(i)).value().get<std::string>().find("NEW") != std::string::npos)
                {
                    ++m_counts[key_NewResidentialUnits];
                    ++m_counts[key_NewSFR_TH_DuplexUnits];
                    sumValues(worksheet, key_NewResidentialUnits, i);
                    if (worksheet.cell(column.buildingUse + std::to_string(i)).value().get<std::string>() == "Duplex")
                    {
                        // Duplexes count as TWO units, so we hit it once during the initial "NEW" check above,
                        // then hit it again here if "Duplex".
                        ++m_counts[key_NewResidentialUnits];
                        ++m_counts[key_NewSFR_TH_DuplexUnits];
                    }
                }
            }

            if (worksheet.cell(column.buildingUse + std::to_string(i)).value().get<std::string>() == "Apartments")
            {
                ++m_counts[key_ResidentialPermits];

            }
                if (worksheet.cell(column.numberOfUnits + std::to_string(i)).value().type() == OpenXLSX::XLValueType::Integer)
                {
                    m_counts[key_NewResidentialUnits] += worksheet.cell(column.numberOfUnits + std::to_string(i)).value().get<int>();
                    m_counts[key_MultiFamilyUnits] += worksheet.cell(column.numberOfUnits + std::to_string(i)).value().get<int>();
                    sumValues(worksheet, key_NewResidentialUnits, i);
                }

            // HEALTHCARE.
            if (worksheet.cell(column.buildingUse + std::to_string(i)).value().get<std::string>() == "Health Care")
            {
                ++m_counts[key_HealthCarePermits];
                if (worksheet.cell(column.classOfWork + std::to_string(i)).value().get<std::string>().find("NEW") != std::string::npos)
                {
                    ++m_counts[key_SignificantQuasi];
                    sumValues(worksheet, key_SignificantQuasi, i);
                }
                else if (worksheet.cell(column.projectValue + std::to_string(i)).value().type() == OpenXLSX::XLValueType::Integer
                    && worksheet.cell(column.projectValue + std::to_string(i)).value().get<int>() >= 250000)
                {
                    ++m_counts[key_SignificantQuasi];
                    sumValues(worksheet, key_SignificantQuasi, i);
                }
                else if (worksheet.cell(column.projectValue + std::to_string(i)).value().type() == OpenXLSX::XLValueType::Float
                    && worksheet.cell(column.projectValue + std::to_string(i)).value().get<float>() >= 250000)
                {
                    ++m_counts[key_SignificantQuasi];
                    sumValues(worksheet, key_SignificantQuasi, i);
                }
            }

            // QUASI.
            if (worksheet.cell(column.buildingUse + std::to_string(i)).value().get<std::string>() == "Quasi")
            {
                ++m_counts[key_SignificantQuasi];
                if (worksheet.cell(column.classOfWork + std::to_string(i)).value().get<std::string>().find("NEW") != std::string::npos)
                {
                    ++m_counts[key_SignificantQuasi];
                    sumValues(worksheet, key_SignificantQuasi, i);
                }
                else if (worksheet.cell(column.projectValue + std::to_string(i)).value().type() == OpenXLSX::XLValueType::Integer
                    && worksheet.cell(column.projectValue + std::to_string(i)).value().get<int>() >= 250000)
                {
                    ++m_counts[key_SignificantQuasi];
                    sumValues(worksheet, key_SignificantQuasi, i);
                }
                else if (worksheet.cell(column.projectValue + std::to_string(i)).value().type() == OpenXLSX::XLValueType::Float
                    && worksheet.cell(column.projectValue + std::to_string(i)).value().get<float>() >= 250000)
                {
                    ++m_counts[key_SignificantQuasi];
                    sumValues(worksheet, key_SignificantQuasi, i);
                }
            }
        }
        spreadsheet.close();
    }
}

void Totals::sumValues(const OpenXLSX::XLWorksheet& sheet, const std::string& key, const int index)
{
    if (sheet.cell(column.projectValue + std::to_string(index)).value().type() == OpenXLSX::XLValueType::Integer)
    {
        m_totals[key] += sheet.cell(column.projectValue + std::to_string(index)).value().get<int>();
    }
    else if (sheet.cell(column.projectValue + std::to_string(index)).value().type() == OpenXLSX::XLValueType::Float)
    {
        m_totals[key] += sheet.cell(column.projectValue + std::to_string(index)).value().get<float>();
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

Parser::~Parser()
{} // OpenXLSX objects use RAII, no need to explicitly close or destroy.
