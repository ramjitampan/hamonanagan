#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

struct Variable
{ // struktur variabelnya
    enum Type
    {
        INT,
        BOOL,
        STRING,
        ARRAY_INT,
        ARRAY_STRING
    } type;
    int intValue;
    bool boolValue;
    string strValue;
    vector<int> arrInt;
    vector<string> arrStr;
};

// Nah ini untuk Map Variabel
unordered_map<string, Variable> vars;

// Fungsi print nya leku
void apakandulu(const Variable &var)
{
    switch (var.type)
    {
    case Variable::INT:
        cout << "Nilai nyo: " << var.intValue << endl;
        break;
    case Variable::BOOL:
        cout << "Bool nyo: " << (var.boolValue ? "Benar lah" : "Salah lah") << endl;
        break;
    case Variable::STRING:
        cout << "Isi nyo: " << var.strValue << endl;
        break;
    case Variable::ARRAY_INT:
        cout << "Array int nyo: ";
        for (int v : var.arrInt)
            cout << v << " ";
        cout << endl;
        break;
    case Variable::ARRAY_STRING:
        cout << "Array string nyo: ";
        for (auto &s : var.arrStr)
            cout << s << " ";
        cout << endl;
        break;
    }
}

// Parsing integer sederhana
int parseInt(const string &s)
{
    stringstream ss(s);
    int val = 0;
    ss >> val;
    return val;
}

// Parsing bool sederhana
bool parseBool(const string &s)
{
    return s == "true" || s == "benar";
}

// Parsing string (hapus tanda petik)
string parseString(const string &s)
{
    if (s.size() >= 2 && s[0] == '"' && s[s.size() - 1] == '"')
        return s.substr(1, s.size() - 2);
    return s;
}

// Fungsi interpreter mini bahasa Medan
void run(const string &code)
{
    istringstream iss(code);
    string line;

    while (getline(iss, line))
    {
        // Trim spasi awal
        while (!line.empty() && line[0] == ' ')
            line.erase(0, 1);
        if (line.empty())
            continue;

        // ====== Variabel ======
        if (line.find("vr ") == 0)
        {
            string name, eq, val;
            stringstream ss(line.substr(3));
            ss >> name >> eq >> val;

            // Array int [1,2,3]
            if (val.size() >= 2 && val[0] == '[' && val.back() == ']')
            {
                vector<int> arr;
                string arrStr = val.substr(1, val.size() - 2);
                stringstream sa(arrStr);
                string item;
                while (getline(sa, item, ','))
                    arr.push_back(parseInt(item));
                vars[name] = {Variable::ARRAY_INT, 0, false, "", arr, {}};
            }
            else if (val == "true" || val == "false" || val == "benar" || val == "salah")
            {
                vars[name] = {Variable::BOOL, 0, parseBool(val), ""};
            }
            else if (val[0] == '"')
            {
                vars[name] = {Variable::STRING, 0, false, parseString(val)};
            }
            else
            {
                vars[name] = {Variable::INT, parseInt(val), false, ""};
            }
        }

        // ====== Output ======
        else if (line.find("apakandulu(") == 0)
        {
            size_t start = line.find("(") + 1;
            size_t end = line.find(")");
            string varName = line.substr(start, end - start);
            if (vars.count(varName))
                apakandulu(vars[varName]);
        }

        // ====== Operator dasar untuk x ======
        else if (line.find("x + ") == 0 && vars.count("x"))
        {
            vars["x"].intValue += parseInt(line.substr(4));
            apakandulu(vars["x"]);
        }
        else if (line.find("x - ") == 0 && vars.count("x"))
        {
            vars["x"].intValue -= parseInt(line.substr(4));
            apakandulu(vars["x"]);
        }
        else if (line.find("x * ") == 0 && vars.count("x"))
        {
            vars["x"].intValue *= parseInt(line.substr(4));
            apakandulu(vars["x"]);
        }
        else if (line.find("x / ") == 0 && vars.count("x"))
        {
            int v = parseInt(line.substr(4));
            if (v != 0)
                vars["x"].intValue /= v;
            else
                cout << "Error: Pembagian nol lah!" << endl;
            apakandulu(vars["x"]);
        }

        // ====== Conditional ======
        else if (line.find("jika (") == 0)
        {
            cout << "Ketemu jika: " << line << " (blok dieksekusi nanti)" << endl;
        }
        else if (line.find("else") == 0)
        {
            cout << "Ketemu else: " << line << " (blok dieksekusi nanti)" << endl;
        }

        // ====== Loop ======
        else if (line.find("selama (") == 0)
        {
            cout << "Ketemu selama: " << line << " (loop dieksekusi nanti)" << endl;
        }
        else if (line.find("ulangi (") == 0)
        {
            cout << "Ketemu ulangi: " << line << " (loop dieksekusi nanti)" << endl;
        }

        // ====== Fungsi ======
        else if (line.find("fungsi ") == 0)
        {
            cout << "Ketemu fungsi: " << line << " (eksekusi fungsi nanti)" << endl;
        }
    }
}

// runer untuk si hamonangan hamonangan itu
int main()
{
    ifstream file("hamonangan.main");
    if (!file.is_open())
    {
        cerr << "Gagal membuka file hamonangan.main" << endl;
        return 1;
    }

    string code((istreambuf_iterator<char>(file)),
                istreambuf_iterator<char>());

    run(code);
    return 0;
}