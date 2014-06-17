#include <netinet/in.h>
#include "EWAHBoolArray/headers/ewah.h"
#include "bitvectors.h"

class Bitvector {
    private:
        EWAHBoolArray<uint64_t> * _bv;
        void _init();
        void _reinit();
    public:
        Bitvector(std::vector<int> ascendingBitPositions);
        Bitvector();
        Bitvector(SerializedBitvector sbv);
        Bitvector(char * data, int length);
        void serialize_cpp(SerializedBitvector ** rtn);
        void serialize(char ** data, int * len);
        bool authorizeReqAny(SerializedBitvector auth);
        bool authorizeReqAny(char * auth_in, int auth_len);
        bool authorizeReqAll(SerializedBitvector auth);
        bool authorizeReqAll(char * auth_in, int auth_len);
        ~Bitvector();
};
