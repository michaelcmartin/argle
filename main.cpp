#include <iostream>
#include <map>
#include <set>
#include <string>
#include "module_map.h"
#include "scan.h"

typedef std::set<std::string> UsedTokens;
typedef std::map<std::string, UsedTokens> UseMap;

class TokenPrinter : public ScannerCallback {
public:
    TokenPrinter() { fill_map(modmap_); }
    virtual ~TokenPrinter() {}
    virtual void registerToken(const std::string &token) {
        ModuleMap::iterator it = modmap_.find(token);
        if (it != modmap_.end()) {
            result_[it->second].insert(it->first);
        }
    }
    void dumpResults() {
        bool first = true;
        for (UseMap::iterator it = result_.begin(); it != result_.end(); ++it) {
            if (it->second.size() > 0) {
                if (first) {
                    first = false;
                } else {
                    std::cout << std::endl;
                }
                std::cout << it->first << std::endl;
                for (int j = 0; j < it->first.size(); ++j) {
                    std::cout << "-";
                }
                std::cout << std::endl;
                for (UsedTokens::iterator j = it->second.begin(); j != it->second.end(); ++j) {
                    std::cout << *j << std::endl;
                }
            }
        }
        if (first) {
            std::cout << "No functions found requiring a version of OpenGL later than 1.1." << std::endl;
        }
    }
private:
    ModuleMap modmap_;
    UseMap result_;
};

int
main(int argc, char **argv)
{
    TokenPrinter tp;
    for (int i = 1; i < argc; ++i) {
        FILE *f = fopen(argv[i], "r");
        scan(f, tp);
        fclose(f);
    }
    tp.dumpResults();
    return 0;
}
