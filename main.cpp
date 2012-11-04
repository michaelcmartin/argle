#include <iostream>
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
            std::cout << it->second << ": " << it->first << std::endl;
        }
    }
private:
    ModuleMap modmap_;
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
    return 0;
}
