#ifndef PARSER_HPP
#define PARSER_HPP

#include <chrono>
#include <cmath>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "OpenXLSX.hpp"

struct Columns
{ // Spreadsheet columns.
    std::string buildingUse { "D" };
    std::string classOfWork { "F" };
    std::string projectValue { "H" };
    std::string numberOfUnits { "E" };
};

class Parser
{
public:
    Parser(std::string& year);
    ~Parser();

    // Map Keys.
    const std::string key_TotalBuildingPermits;
    const std::string key_CommercialPermits;
    const std::string key_ResidentialPermits;
    const std::string key_QuasiPermits;
    const std::string key_HealthCarePermits;
    const std::string key_SignificantCommercial;
    const std::string key_NewResidentialUnits;
    const std::string key_NewSFR_TH_DuplexUnits;
    const std::string key_MultiFamilyUnits;
    const std::string key_SignificantQuasi;
    const std::string key_TotalValue;

    // Methods.
    std::string getCount(const std::string& key) const;
    std::string getValue(const std::string& key) const;
    void parseSpreadsheet();

private:
    // Maps.
    std::unordered_map<std::string, int> m_counts;
    std::unordered_map<std::string, long double> m_totals;

    // Members.
    constexpr int m_firstDataRow;
    int m_populatedRows;
    std::string m_targetYear;

    // Methods
    template <typename T>
    bool isType(const OpenXLSX::XLWorksheet& sheet, const Column& column, const int index) const;
    void sumValues(const OpenXLSX::XLWorksheet& sheet, const std::string& key, const int index);
    void setPopulatedRows(const OpenXLSX::XLWorksheet& sheet);

};















#endif