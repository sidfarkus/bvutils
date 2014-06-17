#include "include/bitvectors.hpp"

int test_create_destroy()
{
    std::cout << "test_create_destroy()... " << std::flush;

    Bitvector * bv_a = new Bitvector();
    delete bv_a;

    std::cout << "success\n" << std::flush;
    return 0;
}

int test_create_from_array_destroy()
{
    std::cout << "test_create_from_array_destroy()... " << std::flush;
    Bitvector * a = NULL;
    std::vector<int> a_indexes;
    a_indexes.push_back(3);
    a_indexes.push_back(85334);
    a_indexes.push_back(1073741823);

    a = new Bitvector(a_indexes);
    if (a == NULL)
    {
        std::cout << "\nERROR in test_create_from_array_destroy() creation seems to have failed\n" << std::flush;;
        return 1;
    }

    delete a;
    std::cout << "success\n" << std::flush;
    return 0;
}

int test_create_from_serialized_destroy()
{
    std::cout << "test_create_from_serialized_destroy()..." << std::flush;
    Bitvector * a = NULL;
    Bitvector * b = NULL;
    std::vector<int> a_indexes;
    a_indexes.push_back(2);
    a_indexes.push_back(11);
    a_indexes.push_back(32768);
    a_indexes.push_back(1010101);
    a_indexes.push_back(1073741823);

    SerializedBitvector *a_ser = NULL;
    SerializedBitvector *b_ser = NULL;
    bool comp;


    a = new Bitvector(a_indexes);
    
    a->serialize_cpp(&a_ser);
    if (a_ser == NULL)
    {
        std::cout << "\nERROR in test_create_from_serialized_destroy() serialization failed\n" << std::flush;
        return 1;
    }

    b = new Bitvector(*a_ser);
    if (b == NULL)
    {
        std::cout << "\nERROR in test_create_from_serialized_destroy() failed to create from serialized\n" << std::flush;
        return 1;
    }
    b->serialize_cpp(&b_ser);

    comp = b->authorizeReqAll(*a_ser);
    if (comp == false)
    {
        std::cout << "\nERROR in test_create_from_serialized_destroy() authorizeReqAll(b,a_ser) == 0\n" << std::flush;
        return 1;
    }

    comp = a->authorizeReqAll(*b_ser);
    if (comp == false)
    {
        std::cout << "\nERROR in test_create_from_serialized_destroy() authorizeReqAll(a,b_ser) == 0\n" << std::flush;
        return 1;
    }
 
    delete a;
    delete b;

    bitvector_serialized_free(&a_ser);
    bitvector_serialized_free(&b_ser);

    if (a_ser != NULL)
    {
        std::cout << "\nERROR in test_create_from_serialized_destroy() failed to destroy bitvector a_ser\n" << std::flush;
        return 1;
    }
    if (b_ser != NULL)
    {
        std::cout << "\nERROR in test_create_from_serialized_destroy() failed to destroy bitvector b_ser\n" << std::flush;
        return 1;
    }
    std::cout << "success\n" << std::flush;
    return 0;
}

int test_serialization_destruction()
{
    std::cout << "test_serialization_destruction()... " << std::flush;
    Bitvector * a = NULL;
    std::vector<int> a_indexes;
    a_indexes.push_back(2);
    a_indexes.push_back(11);
    a_indexes.push_back(32768);
    a_indexes.push_back(1010101);
    a_indexes.push_back(1073741823);
    SerializedBitvector *a_ser = NULL;


    a = new Bitvector(a_indexes);
    a->serialize_cpp(&a_ser);
    if (a_ser == NULL)
    {
        std::cout << "\nERROR in test_serialization_destruction() serialization failed\n" << std::flush;
        return 1;
    }
    delete a;
    bitvector_serialized_free(&a_ser);

    if (a_ser != NULL)
    {
        std::cout << "\nERROR in test_serialization_destruction() failed to destroy bitvector a_ser\n" << std::flush;
        return 1;
    }
    std::cout << "success\n" << std::flush;
    return 0;
}


int test_identical()
{
    std::cout << "test_identical()... " << std::flush;
    Bitvector * a = NULL;
    Bitvector * b = NULL;
    std::vector<int> a_indexes;
    a_indexes.push_back(3);
    a_indexes.push_back(12346);
    a_indexes.push_back(505569);
    a_indexes.push_back(948593);
    a_indexes.push_back(500000000);
    a_indexes.push_back(1073741823);
    std::vector<int> b_indexes;
    b_indexes.push_back(3);
    b_indexes.push_back(12346);
    b_indexes.push_back(505569);
    b_indexes.push_back(948593);
    b_indexes.push_back(500000000);
    b_indexes.push_back(1073741823);

    SerializedBitvector *a_ser = NULL;
    SerializedBitvector *b_ser = NULL;
    bool comp;

    a = new Bitvector(a_indexes);
    a->serialize_cpp(&a_ser);
    if (a_ser == NULL)
    {
        std::cout << "\nERROR in test_identical() serialization failed\n" << std::flush;
        return 1;
    }

    b = new Bitvector(b_indexes);
    b->serialize_cpp(&b_ser);
    if (b_ser == NULL)
    {
        std::cout << "\nERROR in test_identical() serialization failed\n" << std::flush;
        return 1;
    }

    comp = a->authorizeReqAll(*b_ser);
    if (comp !=true)
    {
        std::cout << "\nERROR in test_identical() bitvector_authorizeReqAll(a, *b_ser) != 1\n" << std::flush;
        return 1;
    }

    comp = b->authorizeReqAll(*a_ser);
    if (comp !=true)
    {
        std::cout << "\nERROR in test_identical() bitvector_authorizeReqAll(b, *a_ser) != 1\n" << std::flush;
        return 1;
    }

    std::cout << "success\n" << std::flush;
    return 0;
}


int test_intersecting_auth_more()
{
    std::cout << "test_intersecting_auth_more()... " << std::flush;
    Bitvector * a = NULL;
    Bitvector * b = NULL;
    std::vector<int> a_indexes;
    a_indexes.push_back(2);
    a_indexes.push_back(12345);
    a_indexes.push_back(505568);
    a_indexes.push_back(948592);
    a_indexes.push_back(500000001);
    a_indexes.push_back(1073741822);
    std::vector<int> b_indexes;
    b_indexes.push_back(2);
    b_indexes.push_back(3);
    b_indexes.push_back(5);
    b_indexes.push_back(9);
    b_indexes.push_back(13);
    b_indexes.push_back(17);
    b_indexes.push_back(12345);
    b_indexes.push_back(505568);
    b_indexes.push_back(948592);
    b_indexes.push_back(500000001);
    b_indexes.push_back(1073741822);
    SerializedBitvector *a_ser = NULL;
    SerializedBitvector *b_ser = NULL;
    bool comp;

    a = new Bitvector(a_indexes);
    a->serialize_cpp(&a_ser);
    if (a_ser == NULL)
    {
        std::cout << "\nERROR in test_intersecting_auth_more() serialization failed\n" << std::flush;
        return 1;
    }

    b = new Bitvector(b_indexes);
    b->serialize_cpp(&b_ser);
    if (b_ser == NULL)
    {
        std::cout << "\nERROR in test_intersecting_auth_more() serialization failed\n" << std::flush;
        return 1;
    }

    comp = a->authorizeReqAny(*b_ser);
    if (comp !=true)
    {
        std::cout << "\nERROR in test_intersecting_auth_more() bitvector_authorizeReqAny(a, *b_ser) != 1\n" << std::flush;
        return 1;
    }

    comp = a->authorizeReqAll(*b_ser);
    if (comp !=true)
    {
        std::cout << "\nERROR in test_intersecting_auth_more() bitvector_authorizeReqAll(a, *b_ser) != 1\n" << std::flush;
        return 1;
    }

    std::cout << "success\n" << std::flush;
    return 0;
}


int test_intersecting_auth_fewer()
{
    std::cout << "test_intersecting_auth_fewer()... " << std::flush;
    Bitvector * a = NULL;
    Bitvector * b = NULL;
    std::vector<int> a_indexes;
    a_indexes.push_back(2);
    a_indexes.push_back(12345);
    a_indexes.push_back(505568);
    a_indexes.push_back(948592);
    a_indexes.push_back(500000001);
    a_indexes.push_back(1073741822);
    std::vector<int> b_indexes;
    b_indexes.push_back(2);
    b_indexes.push_back(505568);
    b_indexes.push_back(948592);
    b_indexes.push_back(500000001);
    b_indexes.push_back(1073741822);
    SerializedBitvector *a_ser = NULL;
    SerializedBitvector *b_ser = NULL;
    bool comp;

    a = new Bitvector(a_indexes);
    a->serialize_cpp(&a_ser);
    if (a_ser == NULL)
    {
        std::cout << "\nERROR in test_intersecting_auth_fewer() serialization failed\n" << std::flush;
        return 1;
    }

    b = new Bitvector(b_indexes);
    b->serialize_cpp(&b_ser);
    if (b_ser == NULL)
    {
        std::cout << "\nERROR in test_intersecting_auth_fewer() serialization failed\n" << std::flush;
        return 1;
    }

    comp = a->authorizeReqAny(*b_ser);
    if (comp !=true)
    {
        std::cout << "\nERROR in test_intersecting_auth_fewer() bitvector_authorizeReqAny(a, *b_ser) != 1\n" << std::flush;
        return 1;
    }

    comp = a->authorizeReqAll(*b_ser);
    if (comp ==true)
    {
        std::cout << "\nERROR in test_intersecting_auth_fewer() bitvector_authorizeReqAll(a, *b_ser) != 1\n" << std::flush;
        return 1;
    }

    std::cout << "success\n" << std::flush;
    return 0;
}

int test_non_intersecting()
{
    std::cout << "test_non_intersecting()... " << std::flush;
    Bitvector * a = NULL;
    Bitvector * b = NULL;
    std::vector<int> a_indexes;
    a_indexes.push_back(2);
    a_indexes.push_back(12345);
    a_indexes.push_back(505568);
    a_indexes.push_back(948592);
    a_indexes.push_back(500000001);
    a_indexes.push_back(1073741822);
    std::vector<int> b_indexes;
    b_indexes.push_back(3);
    b_indexes.push_back(12346);
    b_indexes.push_back(505569);
    b_indexes.push_back(948593);
    b_indexes.push_back(500000000);
    b_indexes.push_back(1073741823);
    SerializedBitvector *a_ser = NULL;
    SerializedBitvector *b_ser = NULL;
    bool comp;

    a = new Bitvector(a_indexes);
    a->serialize_cpp(&a_ser);
    if (a_ser == NULL)
    {
        std::cout << "\nERROR in test_non_intersecting() serialization failed\n" << std::flush;
        return 1;
    }

    b = new Bitvector(b_indexes);
    b->serialize_cpp(&b_ser);
    if (b_ser == NULL)
    {
        std::cout << "\nERROR in test_non_intersecting() serialization failed\n" << std::flush;
        return 1;
    }

    comp = a->authorizeReqAny(*b_ser);
    if (comp ==true)
    {
        std::cout << "\nERROR in test_non_intersecting() bitvector_authorizeReqAny(a, *b_ser) == 1\n" << std::flush;
        return 1;
    }

    comp = b->authorizeReqAny(*a_ser);
    if (comp ==true)
    {
        std::cout << "\nERROR in test_non_intersecting() bitvector_authorizeReqAny(b, *a_ser) == 1\n" << std::flush;
        return 1;
    }

    comp = a->authorizeReqAll(*b_ser);
    if (comp ==true)
    {
        std::cout << "\nERROR in test_non_intersecting() bitvector_authorizeReqAll(a, *b_ser) == 1\n" << std::flush;
        return 1;
    }

    comp = b->authorizeReqAll(*a_ser);
    if (comp ==true)
    {
        std::cout << "\nERROR in test_non_intersecting() bitvector_authorizeReqAll(b, *a_ser) == 1\n" << std::flush;
        return 1;
    }


    std::cout << "success\n" << std::flush;
    return 0;
}

int main (void)
{
    int errors = 0;
    errors += test_create_destroy();
    errors += test_create_from_array_destroy();
    errors += test_create_from_serialized_destroy();
    errors += test_serialization_destruction();
    errors += test_identical();
    errors += test_intersecting_auth_fewer();
    errors += test_intersecting_auth_more();
    errors += test_non_intersecting();

    if (errors > 0)
    {
        std::cout <<  "FAILURE: " << errors << " errors\n";
    }
    else
    {
        std::cout << "SUCCESS\n";
    }
}
