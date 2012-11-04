#include <string>
#include <cstdio>

class ScannerCallback {
public:
    ScannerCallback() {}
    virtual ~ScannerCallback() {} 
    virtual void registerToken(const std::string &token) = 0;
};

void scan(FILE *f, ScannerCallback &cn);

