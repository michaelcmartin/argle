#include <iostream>
#include <map>
#include <set>
#include <string>
#include "module_map.h"
#include "scan.h"

typedef std::set<std::string> UsedTokens;
typedef std::map<std::string, UsedTokens> UseMap;

namespace {

bool verbose;

void
usage(const char *progname) {
    std::cerr << "Usage:" << std::endl << std::endl;
    std::cerr << "\t" << progname << " [-v] filenames" << std::endl << std::endl;
    std::cerr << "Where -v is verbose output and \"filenames\" is the files to analyze." << std::endl;
}

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
        int minversion = 11;
        std::set<std::string> exts;
        
        for (UseMap::iterator it = result_.begin(); it != result_.end(); ++it) {
            if (it->second.size() > 0) {
                // This is icky, and will collapse once we hit a major or
                // minor version greater than 9
                if (it->first.find("GL_VERSION_") == 0) {
                    int len = it->first.size();
                    int major = it->first[len-3] - '0';
                    int minor = it->first[len-1] - '0';
                    int version = major * 10 + minor;
                    if (version > minversion) {
                        minversion = version;
                    }
                } else {
                    exts.insert(it->first);
                }
            }
        }

        std::cout << "Minimum required version of OpenGL: ";
        std::cout << (minversion / 10) << "." << (minversion % 10) << std::endl;
        if (exts.size() > 0) {
            std::cout << "Required extensions: ";
            bool first = true;
            for (std::set<std::string>::iterator it = exts.begin();
                 it != exts.end(); ++it) {
                if (first) {
                    first = false;
                } else {
                    std::cout << ", ";
                }
                std::cout << *it;
            }
            std::cout << std::endl;
        }
        if (verbose && result_.size() > 0) {
            std::cout << std::endl << "Function usage" << std::endl;
            std::cout << "--------------" << std::endl << std::endl;
            
            bool first = true;
            for (UseMap::iterator it = result_.begin(); 
                 it != result_.end(); ++it) {
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
                    for (UsedTokens::iterator j = it->second.begin(); 
                         j != it->second.end(); ++j) {
                        std::cout << *j << std::endl;
                    }
                }
            }
        }
    }
private:
    ModuleMap modmap_;
    UseMap result_;
};

}

int
main(int argc, char **argv)
{
    TokenPrinter tp;
    bool did_work = false;
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "-v" || arg == "--verbose") {
            verbose = true;
        } else {
            did_work = true;
            FILE *f = fopen(argv[i], "r");
            if (f) {
                scan(f, tp);
                fclose(f);
            } else {
                std::cerr << "Could not open " << argv[i] << std::endl;
            }
        }
    }
    if (did_work) {
        tp.dumpResults();
    } else {
        usage(argv[0]);
        return 1;
    }
    return 0;
}
