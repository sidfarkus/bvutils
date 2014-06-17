import bitvectors
import sys

def test_create_destroy():
    sys.stdout.write("test_create_destroy()... ")

    bv_a = bitvectors.Bitvector()
    print "success"
    return 0

def test_create_from_array_destroy():
    sys.stdout.write("test_create_from_array_destroy()... ")

    a = bitvectors.Bitvector([3,85334,1073741823])

    print "success" 
    return 0

def test_create_from_serialized_destroy():
    sys.stdout.write("test_create_from_serialized_destroy()...")

    a = bitvectors.Bitvector([2,11,32768,1010101,1073741823])
    a_ser = a.serialize()

    b = bitvectors.Bitvector(a_ser)
    b_ser = b.serialize()

    comp = b.authorizeReqAll(a_ser)
    if (comp == False):
        print "\nERROR in test_create_from_serialized_destroy() authorizeReqAll(b,a_ser) == 0"
        return 1

    comp = a.authorizeReqAll(b_ser)
    if (comp == False):
        print "\nERROR in test_create_from_serialized_destroy() authorizeReqAll(a,b_ser) == 0"
        return 1

    print "success"
    return 0

def test_serialization_destruction():
    sys.stdout.write("test_serialization_destruction()... ")

    a = bitvectors.Bitvector([2,11,32768,1010101,1073741823])
    a_ser = a.serialize()
    return 0

def test_identical():
    sys.stdout.write("test_identical()... ")
    initializer = [3, 12346, 505569, 948593, 500000000, 1073741823]

    a = bitvectors.Bitvector(initializer)
    a_ser = a.serialize()

    b = bitvectors.Bitvector(initializer)
    b_ser = b.serialize()

    comp = a.authorizeReqAll(b_ser)
    if (comp !=True):
        print "\nERROR in test_identical() bitvector_authorizeReqAll(a, *b_ser) != 1"
        return 1

    comp = b.authorizeReqAll(a_ser)
    if (comp !=True):
        print "\nERROR in test_identical() bitvector_authorizeReqAll(b, *a_ser) != 1"
        return 1

    print "success" 
    return 0

def test_intersecting_auth_more():
    sys.stdout.write("test_intersecting_auth_more()... ")
    
    a_indexes = [2, 12345, 505568, 948592, 500000001, 1073741822]
    b_indexes = [2, 3, 5, 9, 13, 17, 12345, 505568, 948592, 500000001, 1073741822]

    a = bitvectors.Bitvector(a_indexes)
    a_ser = a.serialize()

    b = bitvectors.Bitvector(b_indexes)
    b_ser = b.serialize();

    comp = a.authorizeReqAny(b_ser);
    if (comp !=True):
        print "\nERROR in test_intersecting_auth_more() bitvector_authorizeReqAny(a, *b_ser) != 1"
        return 1;

    comp = a.authorizeReqAll(b_ser);
    if (comp !=True):
        print "\nERROR in test_intersecting_auth_more() bitvector_authorizeReqAll(a, *b_ser) != 1" 
        return 1;

    print "success"
    return 0;

def test_intersecting_auth_fewer():
    sys.stdout.write("test_intersecting_auth_fewer()... ")

    a_indexes = [2, 12345, 505568, 948592, 500000001, 1073741822]
    b_indexes = [2, 505568, 948592, 500000001, 1073741822]

    a = bitvectors.Bitvector(a_indexes)
    a_ser = a.serialize()

    b = bitvectors.Bitvector(b_indexes)
    b_ser = b.serialize();

    comp = a.authorizeReqAny(b_ser);
    if (comp !=True):
        print "\nERROR in test_intersecting_auth_fewer() bitvector_authorizeReqAny(a, *b_ser) != 1"
        return 1;

    comp = a.authorizeReqAll(b_ser);
    if (comp ==True):
        print "\nERROR in test_intersecting_auth_fewer() bitvector_authorizeReqAll(a, *b_ser) == 1"
        return 1;

    print "success"
    return 0;

def test_non_intersecting():
    sys.stdout.write("test_non_intersecting()... ")

    a_indexes = [2, 12345, 505568, 948592, 500000001, 1073741822]
    b_indexes = [3, 12346, 505569, 948593, 500000000, 1073741823]

    a = bitvectors.Bitvector(a_indexes)
    a_ser = a.serialize()

    b = bitvectors.Bitvector(b_indexes)
    b_ser = b.serialize();

    comp = a.authorizeReqAny(b_ser);
    if (comp ==True):
        print "\nERROR in test_non_intersecting() bitvector_authorizeReqAny(a, *b_ser) == 1"
        return 1;

    comp = a.authorizeReqAll(b_ser);
    if (comp ==True):
        print "\nERROR in test_non_intersecting() bitvector_authorizeReqAll(a, *b_ser) == 1"
        return 1;

    comp = b.authorizeReqAny(a_ser);
    if (comp ==True):
        print "\nERROR in test_non_intersecting() bitvector_authorizeReqAny(b, *a_ser) == 1"
        return 1;

    comp = b.authorizeReqAll(a_ser);
    if (comp ==True):
        print "\nERROR in test_non_intersecting() bitvector_authorizeReqAll(b, *a_ser) == 1"
        return 1;

    print "success"
    return 0;


def main():
    errors = 0
    errors += test_create_destroy()
    errors += test_create_from_array_destroy()
    errors += test_create_from_serialized_destroy()
    errors += test_serialization_destruction()
    errors += test_identical()
    errors += test_intersecting_auth_fewer()
    errors += test_intersecting_auth_more()
    errors += test_non_intersecting()

    if errors > 0:
        print "FAILURE: %d errors" % (errors)
    else:
        print "SUCCESS"

if __name__ == "__main__":
    main()
