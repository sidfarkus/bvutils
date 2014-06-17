#include "include/bitvectors.hpp"
#include <stdio.h>
#include <iostream>
#include <fstream>
void Bitvector::_init()
{
  _bv = new EWAHBoolArray<uint64_t>();
}

void Bitvector::_reinit()
{
  delete _bv;
  _bv = new EWAHBoolArray<uint64_t>();
}

Bitvector::Bitvector()
{
  _init();
}

Bitvector::Bitvector(std::vector<int> ascendingBitPositions)
{
    _init();
    for (std::vector<int>::iterator it = ascendingBitPositions.begin(); it != ascendingBitPositions.end(); ++it)
    {
        _bv->set(*it);
    }
    //_bv->debugprintout();
}

// uses the char * in sbv -- ensure that is not dealloced during the
// lifecycle of this object
Bitvector::Bitvector(SerializedBitvector sbv)
{
    _init();
    stringstream stream_bv;
    stream_bv.rdbuf()->pubsetbuf(sbv.bitvector, sbv.numbytes);
    _bv->readJava(stream_bv);
}

Bitvector::Bitvector(char * data, int length)
{
    _init();
    stringstream stream_bv;
    stream_bv.rdbuf()->pubsetbuf(data, length);
    _bv->readJava(stream_bv);
}

Bitvector::~Bitvector()
{
    delete(_bv);
}

/* for direct c++ usage and wrapped for C below. To take advantage of built-in
 * swig facilities, other languages should use the version that accepts
 * char** s, int * slen
 *
 * use the C api function to free the structure:
 * void bitvector_serialized_free(SerializedBitVector * sbv)
 * */
void Bitvector::serialize_cpp(SerializedBitvector ** rtn)
{
    *rtn = (SerializedBitvector *) malloc(sizeof(SerializedBitvector));
    serialize(&((*rtn)->bitvector), &((*rtn)->numbytes));
}

/* This version is really only used for swig, use the above version in native
 * c++ */
void Bitvector::serialize(char ** data, int * length )
{
    stringstream stream_bv;
    int nbytes = 0;
    streambuf * streambuf_bv;

    //_bv->debugprintout();
    _bv->writeJava(stream_bv);

    streambuf_bv = stream_bv.rdbuf();
    
    nbytes = streambuf_bv->pubseekoff(0,ios_base::end,ios_base::out);
    streambuf_bv->pubseekoff(0,ios_base::beg,ios_base::out);
    *data = (char *) malloc(nbytes);
    *length = nbytes;

    for (int i=0; i< nbytes; i++)
    {
        (*data)[i] = stream_bv.rdbuf()->sbumpc();
    }
}

 
bool Bitvector::authorizeReqAny(char * auth_in, int auth_len)
{
    EWAHBoolArray<uint64_t> * vis = _bv;
    EWAHBoolArray<uint64_t> * auth = new EWAHBoolArray<uint64_t>();
    EWAHBoolArray<uint64_t> * intersection = new EWAHBoolArray<uint64_t>();

    stringstream stream_bv;
    stream_bv.rdbuf()->pubsetbuf(auth_in, auth_len);
    auth->readJava(stream_bv);

    // treats this instance as the visibility, argument as authorization,
    // finds the intersection, returns true if any bits are set, otherwise
    // false
    //vis->debugprintout();
    // auth->debugprintout();
    vis->logicaland(*auth, *intersection);
    //intersection->debugprintout();
    if (intersection->numberOfOnes() > 0)
    {
        return true;
    }
    return false;
}

bool Bitvector::authorizeReqAny(SerializedBitvector auth_in)
{
    char * auth_arr = auth_in.bitvector;
    int  auth_len = auth_in.numbytes;
    return authorizeReqAny(auth_arr, auth_len);
}


bool Bitvector::authorizeReqAll(char * auth_in, int auth_len)
{
    EWAHBoolArray<uint64_t> * vis = _bv;
    EWAHBoolArray<uint64_t> * auth = new EWAHBoolArray<uint64_t>();
    EWAHBoolArray<uint64_t> * intersection = new EWAHBoolArray<uint64_t>();
    EWAHBoolArray<uint64_t> * in_vis_not_auth = new EWAHBoolArray<uint64_t>();

    //_bv->debugprintout();

    stringstream stream_bv;
    stream_bv.rdbuf()->pubsetbuf(auth_in, auth_len);
    auth->readJava(stream_bv);

    //auth->debugprintout();    
    // treats this instance as the visibility, argument as authorization,
    // finds the intersection, returns true if intersection is identical 
    // to visibility
    vis->logicaland(*auth, *intersection);
    vis->logicalxor(*intersection, *in_vis_not_auth);
    if (in_vis_not_auth->numberOfOnes() > 0)
    {
        return false;
    }
    return true;
}

bool Bitvector::authorizeReqAll(SerializedBitvector auth_in)
{
    char * auth_arr = auth_in.bitvector;
    int auth_len = auth_in.numbytes;
    return authorizeReqAll(auth_arr, auth_len);
}


extern "C" {
    cbitvector bitvector_create()
    {
        return (cbitvector) new Bitvector();
    }

    cbitvector bitvector_create_from_array(int length, int * ascendingBitPositions)
    {
        /*std::vector<int> abp(ascendingBitPositions,
                             ascendingBitPositions + length);*/
        std::vector<int> abp;
        for (int i = 0; i < length; i++)
        {
          abp.push_back(ascendingBitPositions[i]);
        }
        return (cbitvector) new Bitvector(abp);
    }

    cbitvector bitvector_create_from_serialized(SerializedBitvector sbv)
    {
        return (cbitvector) new Bitvector((SerializedBitvector)sbv);
    }

    void * bitvector_serialize(cbitvector bv, SerializedBitvector ** sbv)
    {
        ((Bitvector*)bv)->serialize_cpp(sbv);
    }

    void * bitvector_serialized_free(SerializedBitvector ** sbv)
    {
        free((*sbv)->bitvector);
        free(*sbv);
        *sbv = NULL;
    }

    unsigned int bitvector_authorizeReqAny(cbitvector vis, SerializedBitvector auth)
    {
        bool res;
        res = ((Bitvector*) vis)->authorizeReqAny(auth);
        if (res)
        {
          return (unsigned int) 1;
        }
        return (unsigned int) 0;
    }

    unsigned int bitvector_authorizeReqAll(cbitvector vis, SerializedBitvector auth)
    {
        bool res;
        res = ((Bitvector*) vis)->authorizeReqAll(auth);
        if (res)
        {
          return (unsigned int) 1;
        }
        return (unsigned int) 0;
    }

    void bitvector_destroy(cbitvector * bv)
    {
        delete ((Bitvector*) *bv);
        *bv = NULL;
    }
}
