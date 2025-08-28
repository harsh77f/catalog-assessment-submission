// Online C++ compiler to run C++ program online
#include <bits/stdc++.h>
using namespace std;
long long convertFromBase(const string& value, int base) {
    long long result = 0;
    long long power = 1;
    
    // Process digits from right to left
    for (int i = value.length() - 1; i >= 0; i--) {
        char digit = value[i];
        int digitValue;
        
        if (digit >= '0' && digit <= '9') {
            digitValue = digit - '0';
        } else if (digit >= 'A' && digit <= 'Z') {
            digitValue = digit - 'A' + 10;
        } else if (digit >= 'a' && digit <= 'z') {
            digitValue = digit - 'a' + 10;
        } else {
            throw invalid_argument("Invalid character in number");
        }
        
        if (digitValue >= base) {
            throw invalid_argument("Invalid digit for given base");
        }
        
        result += digitValue * power;
        power *= base;
    }
    
    return result;
}

string extractStringValue(const string& json, const string& key) {
    string searchKey = "\"" + key + "\":";
    size_t pos = json.find(searchKey);
    if (pos == string::npos) return "";
    
    pos += searchKey.length();
    size_t startQuote = json.find("\"", pos);
    if (startQuote == string::npos) return "";
    
    size_t endQuote = json.find("\"", startQuote + 1);
    if (endQuote == string::npos) return "";
    
    return json.substr(startQuote + 1, endQuote - startQuote - 1);
}

int extractIntValue(const string& json, const string& key) {
    string searchKey = "\"" + key + "\":";
    size_t pos = json.find(searchKey);
    if (pos == string::npos) return -1;
    
    pos += searchKey.length();
    while (pos < json.length() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    
    string numStr;
    while (pos < json.length() && json[pos] >= '0' && json[pos] <= '9') {
        numStr += json[pos];
        pos++;
    }
    
    return numStr.empty() ? -1 : stoi(numStr);
}

string removeWhitespace(const string& str) {
    string result;
    for (char c : str) {
        if (c != ' ' && c != '\n' && c != '\t' && c != '\r') {
            result += c;
        }
    }
    return result;
}

vector<long long> helper(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Cannot open file: " + filename);
    }

    string jsonContent;
    string line;
    while (getline(file, line)) {
        jsonContent += line;
    }
    file.close();

    jsonContent = removeWhitespace(jsonContent);

    vector<long long> roots;

    // Find all root entries (numbered keys)
    size_t pos = 0;
    while ((pos = jsonContent.find("\"", pos)) != string::npos) {
        size_t endQuote = jsonContent.find("\"", pos + 1);
        if (endQuote == string::npos) break;

        string key = jsonContent.substr(pos + 1, endQuote - pos - 1);
        
        // Check if key is a number (root entry) and not "n" or "k"
        bool isNumeric = !key.empty() && all_of(key.begin(), key.end(), ::isdigit);
        
        if (isNumeric && key != "n" && key != "k") {
            // Find the object for this key
            size_t objStart = jsonContent.find("{", endQuote);
            size_t objEnd = jsonContent.find("}", objStart);
            
            if (objStart != string::npos && objEnd != string::npos) {
                string objContent = jsonContent.substr(objStart, objEnd - objStart + 1);
                
                string baseStr = extractStringValue(objContent, "base");
                string valueStr = extractStringValue(objContent, "value");
                
                if (!baseStr.empty() && !valueStr.empty()) {
                    int base = stoi(baseStr);
                    long long decodedValue = convertFromBase(valueStr, base);
                    
                    roots.push_back(decodedValue);
                    cout << "Root " << key << ": " << valueStr << " (base " << base << ") = " << decodedValue << " (decimal)" << endl;
                }
            }
        }
        pos = endQuote + 1;
    }

    return roots;
}

int main() {

        string filename = "input.json"; 
        vector<long long> roots = helper(filename);
        
        int n = roots.size();
        long long val = 1;
        for(int i = 0; i < n; i++){
            val = val * roots[i];
        }
        if(n & 1) val = val * -1;
        
        cout << "Secret c: " << val << endl;
        

    
    return 0;
}
