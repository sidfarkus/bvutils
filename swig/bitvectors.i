%module bitvectors

%{
#include "include/bitvectors.hpp"
%}

/*struct SerializedBitvector{
        char * bitvector;
            uint64_t numbytes;
} SerializedBitvector;*/

%include "std_vector.i"
%include "cstring.i"
namespace std {
    %template(IntVector) vector<int>;
}


class Bitvector {
    public:

        Bitvector(std::vector<int> ascendingBitPositions);
        Bitvector();
        Bitvector(SerializedBitvector sbv);
%apply (char *STRING, int LENGTH) { (char * data, int length) };
        Bitvector(char * data, int length);
%cstring_output_allocate_size(char ** data, int * length, free(*$1));
        void serialize(char ** data, int * length);
%apply (char *STRING, int LENGTH) { (char * auth_in, int auth_len) };
        bool authorizeReqAny(char * auth_in, int auth_len);
%apply (char *STRING, int LENGTH) { (char * auth_in, int auth_len) };
        bool authorizeReqAll(char * auth_in, int auth_len);
        ~Bitvector();
};

