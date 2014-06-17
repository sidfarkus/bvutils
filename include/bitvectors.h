#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct SBV{
        char * bitvector;
            int numbytes;
} SerializedBitvector;

typedef void* cbitvector;

cbitvector bitvector_create();

cbitvector bitvector_create_from_array(int length, int * ascendingBitPositions);

cbitvector bitvector_create_from_serialized(SerializedBitvector sbv);

// caller responsible for calling bitvector_serialized_free() on the returned *
void * bitvector_serialize(cbitvector bv, SerializedBitvector ** sbv);
void * bitvector_serialized_free(SerializedBitvector ** sbv);

unsigned int bitvector_authorizeReqAny(cbitvector vis, SerializedBitvector auth);

unsigned int bitvector_authorizeReqAll(cbitvector vis, SerializedBitvector auth);

void bitvector_destroy(cbitvector*);

#ifdef __cplusplus
}
#endif
