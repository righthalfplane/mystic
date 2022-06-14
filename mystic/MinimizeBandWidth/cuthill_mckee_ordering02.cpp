//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//          Doug Gregor, D. Kevin McGrath
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include <boost/config.hpp>
#include <vector>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cuthill_mckee_ordering.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/bandwidth.hpp>


 // c++ -o cuthill_mckee_ordering02 -I/opt/local/include cuthill_mckee_ordering02.cpp
 
/*
  Sample Output
  original bandwidth: 8
  Reverse Cuthill-McKee ordering starting at: 6
    8 3 0 9 2 5 1 4 7 6
    bandwidth: 4
  Reverse Cuthill-McKee ordering starting at: 0
    9 1 4 6 7 2 8 5 3 0
    bandwidth: 4
  Reverse Cuthill-McKee ordering:
    0 8 5 7 3 6 4 2 1 9
    bandwidth: 4
 */
int main(int, char*[])
{
    using namespace boost;
    using namespace std;
    typedef adjacency_list< vecS, vecS, undirectedS,
        property< vertex_color_t, default_color_type,
            property< vertex_degree_t, int > > >
        Graph;
    typedef graph_traits< Graph >::vertex_descriptor Vertex;
    typedef graph_traits< Graph >::vertices_size_type size_type;

    typedef std::pair< std::size_t, std::size_t > Pair;
    Pair edges[52] = { 
        Pair(0, 1), // a-d
        Pair(1, 2), // a-f
        Pair(2, 3), // b-c
        Pair(3, 4), // b-e
        Pair(4, 5), // b-g
        Pair(5, 6), // b-j
        Pair(6, 7), // c-d
        Pair(7, 8), // c-e
        Pair(8, 9), // d-f
        Pair(9, 10), // d-f
        
        Pair(11, 12), // a-d
        Pair(12, 13), // a-f
        Pair(13, 14), // b-c
        Pair(14, 15), // b-e
        Pair(15, 16), // b-g
        Pair(16, 17), // b-j
        Pair(17, 18), // c-d
        Pair(18, 19), // c-e
        Pair(19, 20), // d-f
        Pair(20, 21), // d-i

        Pair(0, 11), // a-d
        Pair(1, 12), // a-f
        Pair(2, 13), // b-c
        Pair(3, 14), // b-e
        Pair(4, 15), // b-g
        Pair(5, 16), // b-j
        Pair(6, 17), // c-d
        Pair(7, 18), // c-e
        Pair(8, 19), // d-f
        Pair(9, 20), // d-f
        Pair(10, 21), // d-f
        
        Pair(22, 23), // a-d
        Pair(23, 24), // a-f
        Pair(24, 25), // b-c
        Pair(25, 26), // b-e
        Pair(26, 27), // b-g
        Pair(27, 28), // b-j
        Pair(28, 29), // c-d
        Pair(29, 30), // c-e
        Pair(30, 31), // d-f
        Pair(31, 32), // d-i

        Pair(11, 22), // a-d
        Pair(12, 23), // a-f
        Pair(13, 24), // b-c
        Pair(14, 25), // b-e
        Pair(15, 26), // b-g
        Pair(16, 27), // b-j
        Pair(17, 28), // c-d
        Pair(18, 29), // c-e
        Pair(19, 30), // d-f
        Pair(20, 31), // d-f
        Pair(21, 32), // d-f
       
        
        
    };

    Graph G(33);
    for (int i = 0; i < 52; ++i)
        add_edge(edges[i].first, edges[i].second, G);

    graph_traits< Graph >::vertex_iterator ui, ui_end;

    property_map< Graph, vertex_degree_t >::type deg = get(vertex_degree, G);
    for (boost::tie(ui, ui_end) = vertices(G); ui != ui_end; ++ui)
        deg[*ui] = degree(*ui, G);

    property_map< Graph, vertex_index_t >::type index_map
        = get(vertex_index, G);

    std::cout << "original bandwidth: " << bandwidth(G) << std::endl;

    std::vector< Vertex > inv_perm(num_vertices(G));
    
    std::vector< size_type > perm(num_vertices(G));
    
    {
        Vertex s = vertex(11, G);
        // reverse cuthill_mckee_ordering
        cuthill_mckee_ordering(G, s, inv_perm.rbegin(), get(vertex_color, G),
            get(vertex_degree, G));
        cout << "Reverse Cuthill-McKee ordering starting at: " << s << endl;
        cout << "  ";
        for (std::vector< Vertex >::const_iterator i = inv_perm.begin();
             i != inv_perm.end(); ++i)
            cout << index_map[*i] << " ";
        cout << endl;

        for (size_type c = 0; c != inv_perm.size(); ++c)
            perm[index_map[inv_perm[c]]] = c;
        std::cout << "  bandwidth: "
                  << bandwidth(G,
                         make_iterator_property_map(
                             &perm[0], index_map, perm[0]))
                  << std::endl;
    }
    
    
 
    {
        // reverse cuthill_mckee_ordering
        cuthill_mckee_ordering(
            G, inv_perm.rbegin(), get(vertex_color, G), make_degree_map(G));

        cout << "Reverse Cuthill-McKee ordering:" << endl;
        cout << "  ";
        for (std::vector< Vertex >::const_iterator i = inv_perm.begin();
             i != inv_perm.end(); ++i)
            cout << index_map[*i] << " ";
        cout << endl;

        for (size_type c = 0; c != inv_perm.size(); ++c)
            perm[index_map[inv_perm[c]]] = c;
        std::cout << "  bandwidth: "
                  << bandwidth(G,
                         make_iterator_property_map(
                             &perm[0], index_map, perm[0]))
                  << std::endl;
    }
    return 0;
}