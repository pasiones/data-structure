// Datastructures.cc
//
// Student name: Dat Minh Lam
// Student email: datminh.lam@tuni.fi
// Student number: 151359774

#include "datastructures.hh"

#include <iostream>
#include <queue>
#include <random>
#include <set>
#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

inline double euclideanDistance(const Coord& c1, const Coord& c2) {
    return std::sqrt(std::pow(c2.x - c1.x, 2) + std::pow(c2.y - c1.y, 2));
}
template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{

}

Datastructures::~Datastructures()
{

}

unsigned int Datastructures::get_affiliation_count()
{
    return affiliations.size();
    //throw NotImplemented("get_affiliation_count()");
}

void Datastructures::clear_all()
{
    affiliations.clear();
    publications.clear();
    alphabeticalCache.clear();
    distanceIncreasingCache.clear();
    CoordCache.clear();
    NameExtract.clear();
    //throw NotImplemented("clear_all()");
}

std::vector<AffiliationID> Datastructures::get_all_affiliations()
{
    std::vector<AffiliationID> allAffiliationIDs;
    allAffiliationIDs.reserve(affiliations.size());

    for (const auto& affiliation : affiliations)
    {
        allAffiliationIDs.push_back(affiliation.first);
    }

    return allAffiliationIDs;
    //throw NotImplemented("get_all_affiliations()");
}

bool Datastructures::add_affiliation(AffiliationID id, const Name &name, Coord xy)
{
    // Check if an affiliation with the same ID already exists
    auto it = affiliations.find(id);
    if (it != affiliations.end())
    {
        return false;
    }
    // If not, add the new affiliation
    CoordCache.insert({xy, id});
    NameExtract.insert({name, id});
    affiliations.insert({id, {id, name, xy, {}, {}}});
    CoordChanged = true;
    NotUpdatedGraph = true;
    return true;
    //throw NotImplemented("add_affiliation()");
}

Name Datastructures::get_affiliation_name(AffiliationID id)
{
    auto it = affiliations.find(id);
    if (it != affiliations.end())
    {
        return it->second.name;
    }
    return NO_NAME; // No affiliation with the given ID found
    //throw NotImplemented("get_affiliation_name()");
}

Coord Datastructures::get_affiliation_coord(AffiliationID id)
{
    auto it = affiliations.find(id);
    if (it != affiliations.end())
    {
        return it->second.xy;
    }
    return NO_COORD;
    //throw NotImplemented("get_affiliation_coord()");
}

std::vector<AffiliationID> Datastructures::get_affiliations_alphabetically()
{
    // Sort the affiliations vector alphabetically by name
    if (alphabeticalCache.size() == affiliations.size())
    {
        return alphabeticalCache;
    }
    std::vector<AffiliationID> sortedIDs;
    sortedIDs.reserve(affiliations.size());
    for (const auto& pair : NameExtract)
    {
        sortedIDs.push_back(pair.second);
    }
    alphabeticalCache = sortedIDs;
    return sortedIDs;
    //throw NotImplemented("get_affiliations_alphabetically()");
}

std::vector<AffiliationID> Datastructures::get_affiliations_distance_increasing()
{
    if (!CoordChanged)
    {
        return distanceIncreasingCache;
    }
    std::vector<std::pair<Coord, AffiliationID>> tempVector;
    tempVector.reserve(CoordCache.size());
    tempVector.assign(CoordCache.begin(), CoordCache.end());

    // Sort the vector based on distance and name
    std::sort(tempVector.begin(), tempVector.end(), [&](const auto& a, const auto& b) {
        double distanceA = euclideanDistance({0, 0}, a.first);
        double distanceB = euclideanDistance({0, 0}, b.first);

        if (distanceA != distanceB) {
            return distanceA < distanceB;
        }

        // If distances are equal, sort by name
        return a.first.y < b.first.y;
    });

    // Extract the sorted IDs from the sorted vector
    std::vector<AffiliationID> sortedIDs;
    sortedIDs.reserve(tempVector.size());
    for (const auto& affiliation : tempVector) {
        sortedIDs.push_back(affiliation.second);
    }
    CoordChanged = false;
    distanceIncreasingCache = sortedIDs;
    return sortedIDs;
    //throw NotImplemented("get_affiliations_distance_increasing()");
}

AffiliationID Datastructures::find_affiliation_with_coord(Coord xy)
{
    auto it = CoordCache.find(xy);
    if (it != CoordCache.end())
    {
        return it->second;
    }
    return NO_AFFILIATION;
    //throw NotImplemented("find_affiliation_with_coord()");
}

bool Datastructures::change_affiliation_coord(AffiliationID id, Coord newcoord)
{
    auto it = affiliations.find(id);
    if (it != affiliations.end())
    {
        // Update coordinates
        Coord oldCoord = it->second.xy;
        it->second.xy = newcoord;
        CoordCache.erase(oldCoord);
        CoordCache[newcoord] = id;
        CoordChanged = true;
        return true;
    }
    return false;
    //throw NotImplemented("change_affiliation_coord()");
}

bool Datastructures::add_publication(PublicationID id, const Name &name, Year year, const std::vector<AffiliationID> &affiliations_id)
{
    // Check if a publication with the given ID already exists
    auto it = publications.find(id);

    if (it != publications.end())
    {
        return false; // Publication with the same ID already exists
    }
    publications.insert({id, {id, name, year, affiliations_id, nullptr, {}}});
    for (const auto& affiliation : affiliations_id)
    {
        affiliations[affiliation].publications_made.push_back(id);
    }
    NotUpdatedGraph = true;
    return true;
    //throw NotImplemented("add_publication()");
}

std::vector<PublicationID> Datastructures::all_publications()
{
    std::vector<PublicationID> allPublicationIDs = {};

    // Use iterators to traverse the publications vector
    for (const auto& entry : publications)
    {
        allPublicationIDs.push_back(entry.first);
    }
    return allPublicationIDs;
    //throw NotImplemented("all_publications()");
}

Name Datastructures::get_publication_name(PublicationID id)
{
    auto it = publications.find(id);
    if (it != publications.end())
    {
        return it->second.name;
    }
    return NO_NAME;
    //throw NotImplemented("get_publication_name()");
}

Year Datastructures::get_publication_year(PublicationID id)
{
    auto it = publications.find(id);
    if (it != publications.end())
    {
        return it->second.year;
    }
    return NO_YEAR; // Publication with the given ID not found
    //throw NotImplemented("get_publication_year()");
}

std::vector<AffiliationID> Datastructures::get_affiliations(PublicationID id)
{
    auto it = publications.find(id);

    if (it != publications.end())
    {
        return it->second.affiliations;
    }
    return {NO_AFFILIATION};
    //throw NotImplemented("get_affiliations()");
}

bool Datastructures::add_reference(PublicationID id, PublicationID parentid)
{
    auto it = publications.find(id);
    auto parent_it = publications.find(parentid);
    if (it != publications.end() && parent_it != publications.end())
    {
        // Find parent in the reference network roots or children
        it->second.parent = &(parent_it->second);

        // Add id address to the vector children of parentid
        parent_it->second.children.push_back(&(it->second));
        return true;
    }
    return false;
    //throw NotImplemented("add_reference()");
}

std::vector<PublicationID> Datastructures::get_direct_references(PublicationID id)
{
    auto it = publications.find(id);
    if (it == publications.end()) {
        return {NO_PUBLICATION};
    }
    std::vector<PublicationID> directReferences;
    for (const auto& child : it->second.children) {
        directReferences.push_back(child->id);
    }
    return directReferences;
    //throw NotImplemented("get_direct_references()");
}

bool Datastructures::add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid)
{
    auto pub_it = publications.find(publicationid);
    auto aff_it = affiliations.find(affiliationid);
    if (pub_it != publications.end() && aff_it != affiliations.end()) {
        // Find the affiliation in the affiliations vector
        pub_it->second.affiliations.push_back(affiliationid);
        aff_it->second.publications_made.push_back(publicationid);
        return true;
    }
    NotUpdatedGraph = true;
    return false;
    //throw NotImplemented("add_affiliation_to_publication()");
}

std::vector<PublicationID> Datastructures::get_publications(AffiliationID id)
{
    std::vector<PublicationID> result = {};

    // Find the affiliation in the affiliations vector
    auto it = affiliations.find(id);
    if (it != affiliations.end())
    {
        for (const auto &entry : publications)
        {
            // Check if the current AffiliationID is in the vector associated with the PublicationID
            if (std::find(entry.second.affiliations.begin(),
                          entry.second.affiliations.end(), id)
                != entry.second.affiliations.end())
            {
                // Add the PublicationID to the result
                result.push_back(entry.first);
            }
        }
        return result;
    }
    return result = {NO_PUBLICATION};
    //throw NotImplemented("get_publications()");
}

PublicationID Datastructures::get_parent(PublicationID id)
{
    auto it = publications.find(id);
    if (it != publications.end() && it->second.parent != nullptr)
    {
        return it->second.parent->id;
    }
    return NO_PUBLICATION;
    //throw NotImplemented("get_parent()");
}

std::vector<std::pair<Year, PublicationID> > Datastructures::get_publications_after(AffiliationID affiliationid, Year year)
{
    auto it = affiliations.find(affiliationid);

    if (it == affiliations.end()) {
        return {{NO_YEAR, NO_PUBLICATION}};
    }

    std::vector<std::pair<Year, PublicationID>> result;

    for (auto& publication : it->second.publications_made)
    {
        auto it = publications.find(publication);
        if (it != publications.end() && it->second.year >= year)
        {
            result.emplace_back(it->second.year, publication);
        }
    }
    std::sort(result.begin(), result.end());
    return result;
    //throw NotImplemented("get_publications_after()");
}

std::vector<PublicationID> Datastructures::get_referenced_by_chain(PublicationID id)
{
    std::vector<PublicationID> chain;

    // Find the publication with the given id
    auto iter = publications.find(id);
    if (iter == publications.end())
    {
        return {NO_PUBLICATION};
    }
    while (iter != publications.end() && iter->second.parent != nullptr) {
        // Add the parent's id to the chain
        chain.push_back(iter->second.parent->id);

        // Move to the parent
        iter = publications.find(iter->second.parent->id);
    }
    return chain;
    //throw NotImplemented("get_referenced_by_chain()");
}

std::vector<PublicationID> Datastructures::get_all_references(PublicationID id)
{
    // Check if the publication with the given id exists
    auto it = publications.find(id);
    if (it == publications.end()) {
        return {NO_PUBLICATION}; // or throw an exception, depending on your design
    }

    // Use an unordered_set to ensure uniqueness of references
    std::unordered_set<PublicationID> references;

    // Helper function for recursive traversal
    std::function<void(const Publication&)> traverse_references = [&](const Publication& publication) {
        for (const auto& child : publication.children) {
            references.insert(child->id);
            auto childIt = publications.find(child->id);
            if (childIt != publications.end()) {
                traverse_references(childIt->second);
            }
        }
    };

    // Start the traversal with the given publication
    traverse_references(it->second);

    // Convert the unordered_set to a vector if needed
    return {references.begin(), references.end()};
    //throw NotImplemented("get_all_references()");
}

std::vector<AffiliationID> Datastructures::get_affiliations_closest_to(Coord xy)
{
    if (affiliations.empty()) {
        return {}; // Given affiliation not found, return empty vector
    }
    // Calculate distances and store in a vector of pairs (AffiliationID, distance)
    std::vector<std::pair<AffiliationID, double>> distances;
    for (const auto& affiliation : affiliations) {
        double distance = euclideanDistance(xy, affiliation.second.xy);
        distances.emplace_back(affiliation.first, distance);
    }

    // Sort the vector based on distances
    std::sort(distances.begin(), distances.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });

    // Extract up to three AffiliationIDs or all if less than three
    std::vector<AffiliationID> closestAffiliations;
    for (const auto& affiliation : distances) {
        closestAffiliations.push_back(affiliation.first);
        if (closestAffiliations.size() >= 3) {
            break;
        }
    }

    return closestAffiliations;
    //throw NotImplemented("get_affiliations_closest_to()");
}

bool Datastructures::remove_affiliation(AffiliationID id)
{
    auto affiliationIt = affiliations.find(id);

    if (affiliationIt == affiliations.end()) {
        return false; // Affiliation not found
    }

    // Update publications that reference the affiliation
    for (auto& publicationEntry : publications) {
        auto& affiliationsVec = publicationEntry.second.affiliations;
        affiliationsVec.erase(std::remove(affiliationsVec.begin(),
                                          affiliationsVec.end(), id), affiliationsVec.end());
    }
    // Remove the affiliation from the affiliations map
    affiliations.erase(id);

    return true;
    //throw NotImplemented("remove_affiliation()");
}

PublicationID Datastructures::get_closest_common_parent(PublicationID id1, PublicationID id2)
{
    if (publications.find(id1) == publications.end() || publications.find(id2) == publications.end()) {
        return NO_PUBLICATION;
    }

    // Get the nodes corresponding to id1 and id2
    Publication* node1 = &publications[id1];
    Publication* node2 = &publications[id2];

    // Find the ancestors of node1 and store them in a set for fast lookup
    std::unordered_set<PublicationID> ancestors;
    while (node1 != nullptr) {
        ancestors.insert(node1->id);
        node1 = node1->parent;
    }

    // Traverse the ancestors of node2 to find the lowest common ancestor
    while (node2 != nullptr) {
        if (ancestors.find(node2->id) != ancestors.end()) {
            // Found the lowest common ancestor
            if(node2->id != id1 && node2->id != id2)
            {
                return node2->id;
            }
        }
        node2 = node2->parent;
    }
    // If no common ancestor is found, throw an exception or return a default value
    return NO_PUBLICATION;
    //throw NotImplemented("get_closest_common_parent()");
}

bool Datastructures::remove_publication(PublicationID publicationid)
{
    auto it = publications.find(publicationid);
    if (it == publications.end()) {
        // Publication not found
        return false;
    }

    // Get the publication to be removed
    Publication& publicationToRemove = it->second;

    // Remove references from parent (if exists)
    if (publicationToRemove.parent != nullptr) {
        auto& parentChildren = publicationToRemove.parent->children;
        parentChildren.erase(std::remove(parentChildren.begin(), parentChildren.end(), &publicationToRemove), parentChildren.end());
    }

    // Remove references from children
    for (Publication* child : publicationToRemove.children) {
        child->parent = nullptr;
    }

    // Remove affiliations
    publicationToRemove.affiliations.clear();

    // Remove the publication from the publications map
    publications.erase(it);

    return true;
    //throw NotImplemented("remove_publication()");
}

std::vector<Connection> Datastructures::get_connected_affiliations(AffiliationID id)
{
    std::vector<Connection> result;
    if (NotUpdatedGraph)
    {
        updateGraph();
        NotUpdatedGraph = false;
    }
    auto it = affiliations.find(id);
    if (it == affiliations.end())
    {
        return result;
    }
    if (it->second.to_neighbours.empty())
    {
        return result;
    }

    for (const auto& neighbour : it->second.to_neighbours)
    {
        Connection NewConnection;
        NewConnection.aff1 = it->first;
        NewConnection.aff2 = neighbour.first->id;
        NewConnection.weight = neighbour.second.first;
        result.push_back(NewConnection);
    }
    return result;
}

std::vector<Connection> Datastructures::get_all_connections()
{
    std::vector<Connection> result;
    std::set<std::pair<AffiliationID, AffiliationID>> addedConnections;

    if (NotUpdatedGraph)
    {
        updateGraph();
        NotUpdatedGraph = false;
    }

    for (auto& affPair : affiliations)
    {
        for (const auto& neighbour : affPair.second.to_neighbours)
        {
            AffiliationID aff1 = std::min(affPair.first, neighbour.first->id);
            AffiliationID aff2 = std::max(affPair.first, neighbour.first->id);

            if (addedConnections.find({aff1, aff2}) == addedConnections.end())
            {
                Connection NewConnection;
                NewConnection.aff1 = aff1;
                NewConnection.aff2 = aff2;
                NewConnection.weight = neighbour.second.first;
                result.push_back(NewConnection);

                addedConnections.insert({aff1, aff2});
            }
        }
    }

    return result;
}

Path Datastructures::get_any_path(AffiliationID source, AffiliationID target)
{
    Path result;
    std::unordered_set<AffiliationID> visited;
    if (NotUpdatedGraph)
    {
        updateGraph();
        NotUpdatedGraph = false;
    }

    // Perform DFS to find a path
    dfs(source, target, visited, result);

    return result;
}

Path Datastructures::get_path_with_least_affiliations(AffiliationID source, AffiliationID target)
{
    Path path;
    if (NotUpdatedGraph)
    {
        updateGraph();
        NotUpdatedGraph = false;
    }
    auto it_source = affiliations.find(source);
    auto it_target = affiliations.find(target);
    if (it_target == affiliations.end() || it_source == affiliations.end())
    {
        return path;
    }
    std::unordered_set<AffiliationID> visited;
    std::queue<std::pair<AffiliationID, Path>> queue;
    visited.insert(source);
    queue.push({source, {}});
    while (!queue.empty())
    {
        auto current = queue.front();
        queue.pop();

        AffiliationID current_id = current.first;
        Path current_path = current.second;

        if (current_id == target)
        {
            path = current_path;
            break;
        }

        auto it_current = affiliations.find(current_id);
        if (it_current == affiliations.end())
        {
            continue;
        }

        for (const auto& neighbour : it_current->second.to_neighbours)
        {
            AffiliationID next_aff_id = neighbour.first->id;
            if (visited.find(next_aff_id) == visited.end())
            {
                visited.insert(next_aff_id);

                Path next_path = current_path;
                Connection newConnection;
                newConnection.aff1 = current_id;
                newConnection.aff2 = next_aff_id;
                newConnection.weight = neighbour.second.first;
                next_path.push_back(newConnection);

                queue.push({next_aff_id, next_path});
            }
        }
    }
    return path;
}

Path Datastructures::get_path_of_least_friction(AffiliationID /*source*/, AffiliationID /*target*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("get_path_of_least_friction()");
}

PathWithDist Datastructures::get_shortest_path(AffiliationID source, AffiliationID target)
{
    if (NotUpdatedGraph)
    {
        updateGraph();
        NotUpdatedGraph = false;
    }
    std::priority_queue<std::pair<Distance, AffiliationID>,
        std::vector<std::pair<Distance, AffiliationID>>,
                        std::greater<std::pair<Distance, AffiliationID>>> pq;
    std::unordered_map<AffiliationID, Distance> distances;
    for (const auto & [id, affiliation] : affiliations)
    {
        distances[id] = std::numeric_limits<Distance>::max();
    }

    distances[source] = 0;
    pq.push({0, source});

    std::unordered_map<AffiliationID, Connection> prev;

    while (!pq.empty())
    {
        auto [distance, current] = pq.top();
        pq.pop();

        if (current == target)
        {
            break;
        }
        for (const auto& [neighbour, data] : affiliations[current].to_neighbours)
        {
            Distance distance = data.second;
            if (distances[current] + distance < distances[neighbour->id])
            {
                distances[neighbour->id] = distances[current] + distance;
                pq.push({distances[neighbour->id], neighbour->id});
                Connection newConnection;
                newConnection.aff1 = current;
                newConnection.aff2 = neighbour->id;
                newConnection.weight = data.first;
                prev[neighbour->id] = newConnection;
            }
        }
    }

    PathWithDist shortest_path;
    AffiliationID current = target;
    while (prev.find(current) != prev.end())
    {
        Connection connection = prev[current];
        Distance distance = euclideanDistance(affiliations[connection.aff1].xy,
                                              affiliations[current].xy);
        shortest_path.push_back({connection, distance});
        current = connection.aff1;
    }
    std::reverse(shortest_path.begin(), shortest_path.end());
    return shortest_path;
}

void Datastructures::updateGraph()
{
    // Reset the weights to zero
    for (auto& pair : affiliations)
    {
        Affiliation& aff = pair.second;
        if (!aff.to_neighbours.empty())
        {
            for (auto& neighbour : aff.to_neighbours)
            {
                neighbour.second.first = 0;
            }
        }
    }

    // Create a map from publications to their affiliations
    std::unordered_map<PublicationID, std::vector<Affiliation*>> publicationToAffiliations;
    for (auto& pair : affiliations)
    {
        Affiliation& aff = pair.second;
        for (const auto& pub : aff.publications_made)
        {
            publicationToAffiliations[pub].push_back(&aff);
        }
    }

    // Update the graph
    for (auto& pair : affiliations)
    {
        Affiliation& aff1 = pair.second;
        for (const auto& pub : aff1.publications_made)
        {
            for (Affiliation* aff2 : publicationToAffiliations[pub])
            {
                if (aff1.id != aff2->id)
                {
                    Distance distance = euclideanDistance(aff1.xy, aff2->xy);
                    auto& neighbour = aff1.to_neighbours[aff2];
                    if (std::find(aff2->publications_made.begin(), aff2->publications_made.end(), pub) != aff2->publications_made.end())
                    {
                        neighbour.first++;  // Increase the weight
                    }
                    neighbour.second = distance;
                }
            }
        }
    }
}

void Datastructures::dfs(AffiliationID current, AffiliationID target, std::unordered_set<AffiliationID>& visited, Path& result)
{
    visited.insert(current);

    // Get connections for the current affiliation
    std::vector<Connection> connections = get_connected_affiliations(current);

    for (const Connection& connection : connections)
    {
        AffiliationID nextAffiliation = connection.aff2;

        if (visited.find(nextAffiliation) == visited.end())
        {
            // Add the connection to the result path
            result.push_back(connection);

            if (nextAffiliation == target)
            {
                // Found a path to the target
                return;
            }

            // Continue the DFS with the next affiliation
            dfs(nextAffiliation, target, visited, result);

            // If a path to the target is found, return
            if (!result.empty() && result.back().aff2 == target)
            {
                return;
            }

            // If no path to the target is found, backtrack
            result.pop_back();
        }
    }
}



