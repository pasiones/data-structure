// Datastructures.hh
//
// Student name: Dat Minh Lam
// Student email: datminh.lam@tuni.fi
// Student number: 151395774

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <map>
#include <stack>
#include <unordered_set>
#include <unordered_map>

// Types for IDs
using AffiliationID = std::string;
using PublicationID = unsigned long long int;
using Name = std::string;
using Year = unsigned short int;
using Weight = int;
using Distance = int;

// Return values for cases where required thing was not found
AffiliationID const NO_AFFILIATION = "---";
PublicationID const NO_PUBLICATION = -1;
Name const NO_NAME = "!NO_NAME!";
Year const NO_YEAR = -1;
Weight const NO_WEIGHT = -1;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};


// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};

// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: size() operation
    // for a vector is a constant-time operation.
    unsigned int get_affiliation_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: The clear functions for standard C++
    // containers like vectors and maps typically have linear time complexity,
    // but the actual time may depend on the size of the containers.
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: the function creates a new vector
    // (allAffiliationIDs) to store the affiliation IDs, and the size of
    // this vector is directly proportional to the number of affiliations
    // in the original vector.
    std::vector<AffiliationID> get_all_affiliations();

    // Estimate of performance: O(1)
    // Short rationale for estimate: The function iterates through unordered map and
    // add elements with key type int.
    bool add_affiliation(AffiliationID id, Name const& name, Coord xy);

    // Estimate of performance: O(1)
    // Short rationale for estimate: iterates through unordered map with key type int.
    Name get_affiliation_name(AffiliationID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: iterates through unordered map with key type int.
    Coord get_affiliation_coord(AffiliationID id);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n * log(n))
    // Short rationale for estimate: the iteration through a map and add element to the return
    // value dominates the time complexity
    // of the function.
    std::vector<AffiliationID> get_affiliations_alphabetically();

    // Estimate of performance: O(n * log(n))
    // Short rationale for estimate: std::sort dominates time complexity of the function for
    // the average and worst-case scenarios.
    std::vector<AffiliationID> get_affiliations_distance_increasing();

    // Estimate of performance: O(1)
    // Short rationale for estimate: the iteration through a map dominates the time complexity
    // of the function.
    AffiliationID find_affiliation_with_coord(Coord xy);

    // Estimate of performance: O(1)
    // Short rationale for estimate: the iteration through a unordered map dominates the time complexity
    // of the function.
    bool change_affiliation_coord(AffiliationID id, Coord newcoord);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(1)
    // Short rationale for estimate: adding element to unordered map and access value of key.
    bool add_publication(PublicationID id, Name const& name, Year year, const std::vector<AffiliationID> & affiliations);

    // Estimate of performance:  O(n)
    // Short rationale for estimate: iterate through all the publications to collect their IDs.
    std::vector<PublicationID> all_publications();

    // Estimate of performance:  O(1)
    // Short rationale for estimate: access value of unordered map.
    Name get_publication_name(PublicationID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: access value of unordered map.
    Year get_publication_year(PublicationID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: access value of unordered map.
    std::vector<AffiliationID> get_affiliations(PublicationID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: access value of unordered map.
    bool add_reference(PublicationID id, PublicationID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: n is the number of direct references.
    std::vector<PublicationID> get_direct_references(PublicationID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: access value of unordered map.
    bool add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Iterate through a vector nested in an unordered map.
    std::vector<PublicationID> get_publications(AffiliationID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: access value of unordered map.
    PublicationID get_parent(PublicationID id);

    // Estimate of performance: O(n * log(n))
    // Short rationale for estimate: std::sort dominates the time complexity of the function.
    std::vector<std::pair<Year, PublicationID>> get_publications_after(AffiliationID affiliationid, Year year);

    // Estimate of performance: O(n)
    // Short rationale for estimate: n is the length of the chain.
    std::vector<PublicationID> get_referenced_by_chain(PublicationID id);


    // Non-compulsory operations

    // Estimate of performance: O(n)
    // Short rationale for estimate: n is the number of references.
    std::vector<PublicationID> get_all_references(PublicationID id);

    // Estimate of performance: O(n * log(n))
    // Short rationale for estimate: The overall time complexity is dominated by the sorting
    // operation, where n is the number of affiliations.
    std::vector<AffiliationID> get_affiliations_closest_to(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: n is the number of publications produced by the affiliation.
    bool remove_affiliation(AffiliationID id);

    // Estimate of performance: O(m + n)
    // Short rationale for estimate: m and n are the length of ancestor chains for id1 and id2
    // respectively.
    PublicationID get_closest_common_parent(PublicationID id1, PublicationID id2);

    // Estimate of performance: O(m + n)
    // Short rationale for estimate: m is the number of affiliation associated with the publication
    // and n is the number of children is one node.
    bool remove_publication(PublicationID publicationid);


private:
    struct Affiliation {
        AffiliationID id;
        Name name;
        Coord xy;
        std::vector<PublicationID> publications_made;
    };
    struct Publication {
        PublicationID id;
        Name name;
        Year year;
        std::vector<AffiliationID> affiliations;
        Publication* parent = nullptr;
        std::vector<Publication*> children;
    };
    bool CoordChanged = false;
    std::unordered_map<AffiliationID, Affiliation> affiliations;
    std::unordered_map<PublicationID, Publication> publications;
    std::vector<AffiliationID> alphabeticalCache = {};
    std::vector<AffiliationID> distanceIncreasingCache;
    std::map<Coord, AffiliationID> CoordCache;
    std::map<Name, AffiliationID> NameExtract;
};

#endif // DATASTRUCTURES_HH
