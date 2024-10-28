#pragma once
#include <cstdint>
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <type_traits>

namespace llvm {

class Cell {
protected:
public:
    Cell() { }
    virtual ~Cell() = default;
    virtual std::string toString() = 0;
};

class IntCell : public Cell {
private:
    int64_t data;
public:
    IntCell(int64_t _data) : data(_data) { }
    virtual std::string toString() override {
        return std::to_string(data);
    }
};

class FltCell : public Cell {
private:
    double data;
public:
    FltCell(double _data) : data(_data) { }
    virtual std::string toString() override {
        return std::to_string(data);
    }
};

class StrCell : public Cell{
private:
    std::string data;
public:
    StrCell(const std::string& _data) : data(_data) { }
    virtual std::string toString() override {
        return data;
    }
};


class Col {
private:
std::vector<std::shared_ptr<Cell>> cells;
public:
    Col& appendCell(std::shared_ptr<Cell> cell) {
        cells.push_back(cell);
        return *this;
    }

    Col& appendData(std::string data) {
        cells.push_back(std::make_shared<StrCell>(data));
        return *this;
    }

    Col& appendData(int data) {
        cells.push_back(std::make_shared<IntCell>(data));
        return *this;
    }

    Col& appendData(int64_t data) {
        cells.push_back(std::make_shared<IntCell>(data));
        return *this;
    }

    Col& appendData(double data) {
        cells.push_back(std::make_shared<FltCell>(data));
        return *this;
    }
};

class Row {
private:
    std::vector<std::shared_ptr<Cell>> cells;
public:
    Row() { }
    Row(const std::vector<std::string>& _data) {
        for (const std::string& data : _data) {
            cells.push_back(std::make_shared<StrCell>(data));
        }
    }

    Row& appendCell(std::shared_ptr<Cell> cell) {
        cells.push_back(cell);
        return *this;
    }

    Row& appendData(std::string data) {
        cells.push_back(std::make_shared<StrCell>(data));
        return *this;
    }

    Row& appendData(int data) {
        cells.push_back(std::make_shared<IntCell>(data));
        return *this;
    }

    Row& appendData(int64_t data) {
        cells.push_back(std::make_shared<IntCell>(data));
        return *this;
    }

    Row& appendData(uint64_t data) {
        cells.push_back(std::make_shared<IntCell>(data));
        return *this;
    }

    Row& appendData(double data) {
        cells.push_back(std::make_shared<FltCell>(data));
        return *this;
    }

    std::string toString() {
        if (cells.empty()) return "";
        std::string result = cells[0]->toString();
        for (int i = 1; i < cells.size(); ++i) result += "," + cells[i]->toString();
        return result;
    }
};


class CSV {
private:
    std::string name;
    std::shared_ptr<Row> headLine;
    std::vector<std::shared_ptr<Row>> rows;
    int currentRow = 0;
    static CSV* instance;
public:
    CSV() { }
    CSV(const std::string& _name) : name(_name) { }
    CSV(const std::string& _name, std::shared_ptr<Row> _headLine) : name(_name), headLine(_headLine) { }
    CSV(const std::string& _name, const std::vector<std::string>& _data) : name(_name) {
        headLine = std::make_shared<Row>(_data);
    }

    static CSV& getRef() {
        if (!instance) instance = new CSV();
        return *instance;
    }

    static CSV* get() {
        if (!instance) instance = new CSV();
        return instance;
    }

    static bool create(const std::string& name) {
        if (!instance) {
            instance = new CSV(name);
            return true;
        }
        return false;
    }

    CSV& clear() {
        headLine = std::make_shared<Row>();
        rows.clear();
        return *this;
    }

    CSV& setName(const std::string& name) {
        this->name = name;
        return *this;
    }

    CSV& setHead(std::shared_ptr<Row> head) {
        headLine = head;
        return *this;
    }

    CSV& appendRow(std::shared_ptr<Row> row) {
        rows.push_back(row);
        return *this;
    }

    // append rest rows into the file
    CSV& flush() {
        std::string file = name.length() ? name : "data.csv";
        std::ofstream out;
        if (currentRow == 0) {
            write();
            return *this;
        }
        out.open(file, std::ios::app);
        while (currentRow < rows.size()) {
            auto row = rows[currentRow];
            out << row->toString() << "\n";
            currentRow++;
        }
        out.close();
        return *this;
    }

    CSV& write() {
        std::string file = name.length() ? name : "data.csv";
        printf("INFO: Writing csv file ... %s, num of rows is %d\n", file.c_str(), rows.size());
        std::ofstream out;
        out.open(file);
        out << toString();
        out.close();
        currentRow = rows.size();
        return *this;
    }

    std::string toString() {
        std::string result = "";
        if (headLine) {
            result += headLine->toString() + "\n";
        }
        for (std::shared_ptr<Row> row : rows) {
            result += row->toString() + "\n";
        }
        printf("INFO: The content of the table is:\n%s", result.c_str());
        return result;
    }
};

} // end of namespace llvm