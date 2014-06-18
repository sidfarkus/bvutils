from pymongo import MongoClient
import bitvectors
from bson.binary import Binary


def printres(cursor):
    for c in cursor:
        if 'visibilityBV' in c:
            del c['visibilityBV']
        if '_id' in c:
            del c['_id']
        print c

def deleteIfExists(client, name):
    dbs = client.database_names()
    if name in dbs:
        client.drop_database(name)

def printHex(str):
    hex_chars = map(hex,map(ord,str))
    print hex_chars


def main():
    dbname = 'bitvectorTest'
    client=MongoClient()
    deleteIfExists(client, dbname)
    db=client[dbname]

    # insert 3 records with visibility marking #1
    vis1 = [0,1,2]
    bv1 = bitvectors.Bitvector(vis1)
    bv1_ser = bv1.serialize()
    for x in range(3):
        np = {'author':'mike', 'val':'vis1_'+str(x), 'visibilityBV':Binary(bv1_ser)}
        db.posts.insert(np)

    # insert 3 records with visibility marking #2
    vis2 = [3, 12346, 505569, 948593, 500000000, 1073741823]
    bv2 = bitvectors.Bitvector(vis2)
    bv2_ser = bv2.serialize()
    for x in range(3):
        np = {'author':'mike', 'val':'vis2_'+str(x), 'visibilityBV':Binary(bv2_ser)}
        db.posts.insert(np)

    # insert 3 records with visibility marking #3
    vis3 = [9999, 99999, 999999, 9999999]
    bv3 = bitvectors.Bitvector(vis3)
    bv3_ser = bv3.serialize()
    for x in range(3):
        np = {'author':'mike', 'val':'vis3_'+str(x), 'visibilityBV':Binary(bv3_ser)}
        db.posts.insert(np)

    # set up an authorization that intersects with 1 and no others
    auth_intersects_1 = [2, 7562, 810604, 53450602]
    bv_auth_intersects_1 = bitvectors.Bitvector(auth_intersects_1)
    bv_auth_intersects_1_ser = bv_auth_intersects_1.serialize()

    # set up an authorization that is identical to visibility 2
    auth_identical_2 = [3, 12346, 505569, 948593, 500000000, 1073741823]
    bv_auth_identical_2 = bitvectors.Bitvector(auth_identical_2)
    bv_auth_identical_2_ser = bv_auth_identical_2.serialize()   

    # set up an authorization that intersects both visibility 1 and visibility 3
    auth_intersects_1_3 = [2, 9999, 888888, 8888888, 88888888]
    bv_auth_intersects_1_3 = bitvectors.Bitvector(auth_intersects_1_3)
    bv_auth_intersects_1_3_ser = bv_auth_intersects_1_3.serialize()

    # set up an authorization that intersects no visibilities
    auth_non_intersecting = [4, 6, 8, 1000000, 8475921, 94567146]
    bv_auth_non_intersecting = bitvectors.Bitvector(auth_non_intersecting)
    bv_auth_non_intersecting_ser = bv_auth_non_intersecting.serialize()


    res = db.posts.find({'author':'mike', 'visibilityBV':{'$bvAuthReqAny':Binary(bv_auth_intersects_1_ser)}})
    print "should see 3 results with the value prefix of vis1_"
    printres(res)
    print ''

    res = db.posts.find({'author':'mike', 'visibilityBV':{'$bvAuthReqAll':Binary(bv_auth_intersects_1_ser)}})
    print "should see none"
    printres(res)
    print ''

    
    res = db.posts.find({'author':'mike', 'visibilityBV':{'$bvAuthReqAny':Binary(bv_auth_identical_2_ser)}})
    print "should see 3 results with the value prefix of vis2_"
    printres(res)
    print ''

    res = db.posts.find({'author':'mike', 'visibilityBV':{'$bvAuthReqAll':Binary(bv_auth_identical_2_ser)}})
    print "should see 3 results with the value prefix of vis2_"
    printres(res)
    print ''

    res = db.posts.find({'author':'mike', 'visibilityBV':{'$bvAuthReqAny':Binary(bv_auth_intersects_1_3_ser)}})
    print "should see 6 results, 3 with the value prefix of vis1_, 3 with the value prefix of vis3_"
    printres(res)
    print ''

    res = db.posts.find({'author':'mike', 'visibilityBV':{'$bvAuthReqAll':Binary(bv_auth_intersects_1_3_ser)}})
    print "should see none"
    printres(res)
    print ''

    res = db.posts.find({'author':'mike', 'visibilityBV':{'$bvAuthReqAny':Binary(bv_auth_non_intersecting_ser)}})
    print "should see none"
    printres(res)
    print ''

    res = db.posts.find({'author':'mike', 'visibilityBV':{'$bvAuthReqAll':Binary(bv_auth_non_intersecting_ser)}})
    print "should see none"
    printres(res)
    print ''

if __name__ == "__main__":
    main()
