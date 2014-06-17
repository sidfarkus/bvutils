#include "include/bitvectors.h"
#include <stdio.h>


int test_create_destroy()
{
    printf("test_create_destroy()... ");fflush(NULL);
    cbitvector a = NULL;
    a = bitvector_create();
    if (a == NULL)
    {
        printf("\nERROR in test_create_destroy() creation seems to have failed\n");
        return 1;
    }
    bitvector_destroy(&a);
    if (a != NULL)
    {
        printf("\nERROR in test_create_destroy() destruction seems to have failed\n");
        return 1;
    }
    printf("success\n");
    return 0;
}

int test_create_from_array_destroy()
{
    printf("test_create_from_array_destroy()... ");fflush(NULL);
    cbitvector a = NULL;
    int a_indexes[] = {3, 85334, 1073741823};
    a = bitvector_create_from_array(sizeof(a_indexes)/sizeof(int), a_indexes);
    if (a == NULL)
    {
        printf("\nERROR in test_create_from_array_destroy() creation seems to have failed\n");
        return 1;
    }
    bitvector_destroy(&a);
    if (a != NULL)
    {
        printf("\nERROR in test_create_from_array_destroy() destruction seems to have failed\n");
        return 1;
    }
    printf("success\n");
    return 0;
}

int test_create_from_serialized_destroy()
{
    printf("test_create_from_serialized_destroy()...");fflush(NULL);
    cbitvector a = NULL;
    cbitvector b = NULL;
    int a_indexes[] = {2, 11, 32768, 1010101, 1073741823};
    SerializedBitvector *a_ser = NULL;
    SerializedBitvector *b_ser = NULL;
    int comp;


    a = bitvector_create_from_array(sizeof(a_indexes)/sizeof(int), a_indexes);
    bitvector_serialize(a, &a_ser);
    if (a_ser == NULL)
    {
        printf("\nERROR in test_create_from_serialized_destroy() serialization failed\n");
        return 1;
    }
    
    b = bitvector_create_from_serialized(*a_ser);
    if (b == NULL)
    {
        printf("\nERROR in test_create_from_serialized_destroy() failed to create from serialized\n");
        return 1;
    }
    bitvector_serialize(b, &b_ser);

    comp = bitvector_authorizeReqAll(b,*a_ser);
    if (comp == 0)
    {
        printf("\nERROR in test_create_from_serialized_destroy() authorizeReqAll(b,a_ser) == 0\n");
        return 1;
    }

    comp = bitvector_authorizeReqAll(a,*b_ser);
    if (comp == 0)
    {
        printf("\nERROR in test_create_from_serialized_destroy() authorizeReqAll(a,b_ser) == 0\n");
        return 1;
    }
    
    bitvector_destroy(&a);
    bitvector_destroy(&b);
    bitvector_serialized_free(&a_ser);
    bitvector_serialized_free(&b_ser);

    if (a != NULL)
    {
        printf("\nERROR in test_create_from_serialized_destroy() failed to destroy bitvector a\n");
        return 1;
    }
    if (b != NULL)
    {
        printf("\nERROR in test_create_from_serialized_destroy() failed to destroy bitvector b\n");
        return 1;
    }
    if (a_ser != NULL)
    {
        printf("\nERROR in test_create_from_serialized_destroy() failed to destroy bitvector a_ser\n");
        return 1;
    }
    if (b_ser != NULL)
    {
        printf("\nERROR in test_create_from_serialized_destroy() failed to destroy bitvector b_ser\n");
        return 1;
    }
    printf("success\n");
    return 0;
}


int test_serialization_destruction()
{
    printf("test_serialization_destruction()... ");fflush(NULL);
    cbitvector a = NULL;
    int a_indexes[] = {2, 11, 32768, 1010101, 1073741823};
    SerializedBitvector *a_ser = NULL;


    a = bitvector_create_from_array(sizeof(a_indexes)/sizeof(int), a_indexes);
    bitvector_serialize(a, &a_ser);
    if (a_ser == NULL)
    {
        printf("\nERROR in test_serialization_destruction() serialization failed\n");
        return 1;
    }
    bitvector_destroy(&a);
    bitvector_serialized_free(&a_ser);

    if (a != NULL)
    {
        printf("\nERROR in test_serialization_destruction() failed to destroy bitvector a\n");
        return 1;
    }
    if (a_ser != NULL)
    {
        printf("\nERROR in test_serialization_destruction() failed to destroy bitvector a_ser\n");
        return 1;
    }
    printf("success\n");
    return 0;
}

int test_identical()
{
    printf("test_identical()... ");fflush(NULL);
    cbitvector a = NULL;
    cbitvector b = NULL;
    int a_indexes[] = {3, 12346, 505569, 948593, 500000000, 1073741823};
    int b_indexes[] = {3, 12346, 505569, 948593, 500000000, 1073741823};
    SerializedBitvector *a_ser = NULL;
    SerializedBitvector *b_ser = NULL;
    int comp;

    a = bitvector_create_from_array(sizeof(a_indexes)/sizeof(int), a_indexes);
    bitvector_serialize(a, &a_ser);
    if (a_ser == NULL)
    {
        printf("\nERROR in test_identical() serialization failed\n");
        return 1;
    }

    b = bitvector_create_from_array(sizeof(b_indexes)/sizeof(int), b_indexes);
    bitvector_serialize(b, &b_ser);
    if (b_ser == NULL)
    {
        printf("\nERROR in test_identical() serialization failed\n");
        return 1;
    }
    
    comp = bitvector_authorizeReqAll(a, *b_ser);
    if (comp !=1)
    {
        printf("\nERROR in test_identical() bitvector_authorizeReqAll(a, *b_ser) != 1\n");
        return 1;
    }

    comp = bitvector_authorizeReqAll(b, *a_ser);
    if (comp !=1)
    {
        printf("\nERROR in test_identical() bitvector_authorizeReqAll(b, *a_ser) != 1\n");
        return 1;
    }

    printf("success\n");
    return 0;
}


int test_intersecting_auth_more()
{
    printf("test_intersecting_auth_more()... ");fflush(NULL);
    cbitvector a = NULL;
    cbitvector b = NULL;
    int a_indexes[] = {2, 12345, 505568, 948592, 500000001, 1073741822};
    int b_indexes[] = {2, 3, 5, 9, 13, 17, 12345, 505568, 948592, 500000001, 1073741822};
    SerializedBitvector *a_ser = NULL;
    SerializedBitvector *b_ser = NULL;
    int comp;

    a = bitvector_create_from_array(sizeof(a_indexes)/sizeof(int), a_indexes);
    bitvector_serialize(a, &a_ser);
    if (a_ser == NULL)
    {
        printf("\nERROR in test_intersecting_auth_more() serialization failed\n");
        return 1;
    }

    b = bitvector_create_from_array(sizeof(b_indexes)/sizeof(int), b_indexes);
    bitvector_serialize(b, &b_ser);
    if (b_ser == NULL)
    {
        printf("\nERROR in test_intersecting_auth_more() serialization failed\n");
        return 1;
    }
    
    comp = bitvector_authorizeReqAny(a, *b_ser);
    if (comp !=1)
    {
        printf("\nERROR in test_intersecting_auth_more() bitvector_authorizeReqAny(a, *b_ser) != 1\n");
        return 1;
    }

    comp = bitvector_authorizeReqAll(a, *b_ser);
    if (comp !=1)
    {
        printf("\nERROR in test_intersecting_auth_more() bitvector_authorizeReqAll(a, *b_ser) != 1\n");
        return 1;
    }

    printf("success\n");
    return 0;
}

int test_intersecting_auth_fewer()
{
    printf("test_intersecting_auth_fewer()... ");fflush(NULL);
    cbitvector a = NULL;
    cbitvector b = NULL;
    int a_indexes[] = {2, 12345, 505568, 948592, 500000001, 1073741822};
    int b_indexes[] = {2, 505568, 948592, 500000001, 1073741822};
    SerializedBitvector *a_ser = NULL;
    SerializedBitvector *b_ser = NULL;
    int comp;

    a = bitvector_create_from_array(sizeof(a_indexes)/sizeof(int), a_indexes);
    bitvector_serialize(a, &a_ser);
    if (a_ser == NULL)
    {
        printf("\nERROR in test_intersecting_auth_fewer() serialization failed\n");
        return 1;
    }

    b = bitvector_create_from_array(sizeof(b_indexes)/sizeof(int), b_indexes);
    bitvector_serialize(b, &b_ser);
    if (b_ser == NULL)
    {
        printf("\nERROR in test_intersecting_auth_fewer() serialization failed\n");
        return 1;
    }
    
    comp = bitvector_authorizeReqAny(a, *b_ser);
    if (comp !=1)
    {
        printf("\nERROR in test_intersecting_auth_fewer() bitvector_authorizeReqAny(a, *b_ser) != 1\n");
        return 1;
    }

    comp = bitvector_authorizeReqAll(a, *b_ser);
    if (comp ==1)
    {
        printf("\nERROR in test_intersecting_auth_fewer() bitvector_authorizeReqAll(a, *b_ser) == 1\n");
        return 1;
    }
    printf("success\n");
    return 0;
}


int test_non_intersecting()
{
    printf("test_non_intersecting()... ");fflush(NULL);
    cbitvector a = NULL;
    cbitvector b = NULL;
    int a_indexes[] = {2, 12345, 505568, 948592, 500000001, 1073741822};
    int b_indexes[] = {3, 12346, 505569, 948593, 500000000, 1073741823};
    SerializedBitvector *a_ser = NULL;
    SerializedBitvector *b_ser = NULL;
    int comp;

    a = bitvector_create_from_array(sizeof(a_indexes)/sizeof(int), a_indexes);
    bitvector_serialize(a, &a_ser);
    if (a_ser == NULL)
    {
        printf("\nERROR in test_non_intersecting() serialization failed\n");
        return 1;
    }

    b = bitvector_create_from_array(sizeof(b_indexes)/sizeof(int), b_indexes);
    bitvector_serialize(b, &b_ser);
    if (b_ser == NULL)
    {
        printf("\nERROR in test_non_intersecting() serialization failed\n");
        return 1;
    }
    
    comp = bitvector_authorizeReqAny(a, *b_ser);
    if (comp ==1)
    {
        printf("\nERROR in test_non_intersecting() bitvector_authorizeReqAny(a, *b_ser) == 1\n");
        return 1;
    }

    comp = bitvector_authorizeReqAny(b, *a_ser);
    if (comp ==1)
    {
        printf("\nERROR in test_non_intersecting() bitvector_authorizeReqAny(b, *a_ser) == 1\n");
        return 1;
    }

    comp = bitvector_authorizeReqAll(a, *b_ser);
    if (comp ==1)
    {
        printf("\nERROR in test_non_intersecting() bitvector_authorizeReqAll(a, *b_ser) == 1\n");
        return 1;
    }

    comp = bitvector_authorizeReqAll(b, *a_ser);
    if (comp ==1)
    {
        printf("\nERROR in test_non_intersecting() bitvector_authorizeReqAll(b, *a_ser) == 1\n");
        return 1;
    }


    printf("success\n");
    return 0;
}


int main(void)
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
        printf("FAILURE: %d errors\n",errors);
    }
    else
    {
        printf("SUCCESS\n");
    }
}
