  #include <iostream>
  #include <bits/stdc++.h>
  #include "DCEL.hpp"
  #include <algorithm>
  #include <chrono>

  using namespace std;
  using namespace std::chrono;



// N D Bharath 2019A7PS0166H
// Ritvik Upadhyay 2019A7PS0163H
// Tushar Chattopdhyay 2020A7PS0158H
// Varad Asawa 2020A7PS0217H

  /**
   * @brief   Computes the next vertex in the polygon
   * @param   v The current vertex
   * @param   P The polygon in which the current vertex belongs
   * @return  The next vertex in the polygon
   */
  vertex nextVertex(vertex v, const vector<vertex>& P) {
    int i = 0;
    for (; i < P.size(); i++) {
      if (P[i].key == v.key) break;
    }
    return P[(i + 1) % P.size()];
  }


  /**
   * @brief   Computes the orientation of 3 points
   * @param   p1 The first point
   * @param   p2 The second point
   * @param   p3 The third point
   * @return  The orientation of the 3 points
   */
  double orientation(vertex p1, vertex p2, vertex p3){
      double crossproduct = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
      return crossproduct;
  }


  /**
   * @brief   Computes the cross product of two vectors
   * @param   p1 The first vector
   * @param   p2 The second vector
   * @param   p3 The third vector
   * @return  The cross product of the two vectors
   */
  float cross_product(vertex p1, vertex p2, vertex p3) {
    float x1 = p2.x - p1.x;
    float y1 = p2.y - p1.y;
    float x2 = p3.x - p2.x;
    float y2 = p3.y - p2.y;
    return x1 * y2 - x2 * y1;
  }

  /**
   * @brief   Subtracts one polygon from another
   * @param   P The polygon from which to subtract
   * @param   L The polygon to subtract
   * @return  The resulting polygon
   */
  vector<vertex> subtractPolygons(vector<vertex> P, vector<vertex> L) {
      vector<vertex> result;
      for (vertex p : P) {
          bool is_in_L = false;
          for (vertex l : L) {
              if (p.key == l.key) {
                  is_in_L = true;
                  break;
              }
          }
          if (!is_in_L) {
              result.push_back(p);
              
          }
      }
      return result;
  }

  /**
   * @brief   Checks if a point is inside a convex polygon
   * @param   point The point to check
   * @param   polygon The polygon to check
   * @return  Whether the point is inside the polygon
   */
  bool is_point_inside_convex_polygon(vertex point, vector<vertex> polygon) {
    int n = polygon.size();
    if (n < 3) {
      return false;
    }

    // Check if the point is inside the polygon.
    int sign = 0;
    for (int i = 0; i < n; i++) {
      float cp = cross_product(polygon[i], polygon[(i + 1) % n], point);
      if (cp != 0) {
        if (sign == 0) {
          sign = (cp > 0) ? 1 : -1;
        } else if (sign != ((cp > 0) ? 1 : -1)) {
          return false;
        }
      }
    }
    return true;
  }
  /**
   * @brief Compares two vertices by their key (i.e., identifier).
   * 
   * @param v1 The first vertex.
   * @param v2 The second vertex.
   * @return true if the key of v1 is less than the key of v2, false otherwise.
   */
  bool compareVertex(vertex v1, vertex v2)
  {
      return (v1.key < v2.key);
  }
    
  /**
   * @brief Determines which side of the line through two points a third point lies on.
   * 
   * @param p1 The first point.
   * @param p2 The second point.
   * @param p3 The third point.
   * @return The value of the orientation function for the three points.
   */
  double sideOfLine(const vertex& p1, const vertex& p2, const vertex& p3) {
      return orientation(p2, p3, p1);
  }

  /**
   * @brief Computes the notches of a given polygon.
   * 
   * A notch is a vertex in the polygon where there is a counterclockwise turn.
   * 
   * @param polygon The polygon.
   * @return The notches of the polygon.
   */
  vector<vertex> getNotches(const vector<vertex>& polygon) {
      vector<vertex> notches;

      for (int i = 0; i < polygon.size(); ++i) {
          const vertex& v0 = polygon[i];
          const vertex& v1 = polygon[(i + 1) % polygon.size()];
          const vertex& v2 = polygon[(i + 2) % polygon.size()];

          double crossProduct = orientation(v0, v1, v2);


          if (crossProduct > 0) {  // counterclockwise turn
              notches.push_back(v1);
          }
      }

      return notches;
  }

  /**
   * @brief Computes the minimum number of convex polygons needed to cover a given polygon using the Monotone Partitioning algorithm.
   * 
   * @param P The polygon.
   * @return The minimum number of convex polygons needed to cover P.
   */
  vector<vector<int>> MP1(vector<vertex>& P){
    vector<vector<int>> polygonid;
    int t = 0;
    if(P.size() == 3){
    cout << "Already a convex polygon" ;
    for(auto point: P){
          cout <<"("<<point.x<<","<<point.y<<")";
          if(point.key!=P.back().key){
            cout << ",";
          }  
          }
    }
    vector<vertex> L;
    int size = P.size();
    unordered_set<int> count;
    int m = 1;
    L.push_back(P[0]);
    vertex v1 = L[0];
    //section 3.1
    while(P.size() > 3 && count.size() != size){
      vertex v2 = nextVertex(v1, P);
    //section 3.2 Add v2 to L
      L.push_back(v2);
      vertex vi = v2;
      vertex vin = nextVertex(v2, P);
      vertex vip = v1;
    //section 3.3 Checking conditions for vertex to be added to L
      while(L.size() < P.size() && count.size() != size){
        if((orientation(vip, vi, vin) <= 0) & (orientation(vi, vin, v1) <= 0) & (orientation(vin, v1, v2) <= 0)){
        //cout <<"ori"<<vi.key<<orientation(vi, vin, v1);
        L.push_back(vin);
        
        }
        else {
          break;
        }
        
        vip = vi;
        vi = nextVertex(vi, P);
        vin = nextVertex(vi, P);
      }
      //section 3.4
      if(L.size() != P.size() ){
        vector<vertex> PmL = subtractPolygons(P, L);
        vector<vertex> notches = getNotches(P);
        vector<vertex> LPVS;
        unordered_set<int> nt;

      
        for(auto point: notches){
          nt.insert(point.key);
        }
        //section 3.4.1
        for(auto point: PmL){
          if(nt.count(point.key)){
            LPVS.push_back(point);
          }
        }
        //section 3.4.2 Creating rectangle for filtering
        while(LPVS.size() > 0){
          float xmax = INT_MIN;
          float xmin = INT_MAX;
          float ymax = INT_MIN;
          float ymin = INT_MAX;
          for(auto point: L){
            if(point.x > xmax){
              xmax = point.x;
            }
            if(point.x < xmin){
              xmin = point.x;
            }
            if(point.y > ymax){
              ymax = point.y;
            }
            if(point.y < ymin){
              ymin = point.y;
            }
          

          }
          vector<vertex> rectangle = {{xmin, ymin}, {xmin, ymax}, {xmax, ymax}, {xmax, ymin} };
          
          bool backwards = false;
          vertex v = LPVS[0];
          while(!backwards &&  LPVS.size() > 0){
              if(!is_point_inside_convex_polygon(v, rectangle)){
                LPVS.pop_back();
              }
              else if((is_point_inside_convex_polygon(v, rectangle)) || (LPVS.size() == 0)){
                break;
              }
              if(LPVS.size() != 0){
              v = nextVertex(v, LPVS);
              }
          }
          //Removing vertices from L if there is a notch inside it
          if(LPVS.size() > 0){
            if(is_point_inside_convex_polygon(v, L)){
              vertex vk = L.back();
              double sideOfVk = sideOfLine(vk, v1, v);
              if (sideOfVk != 0) {
                for (auto it = L.begin(); it != L.end(); ) {
                  if (sideOfLine(*it, v1, v)*sideOfVk > 0) {
                    
                    vertex e = *it;
                    count.erase(e.key);
                    PmL.push_back(e);
                    it = L.erase(it);
                  } else {
                    it++;
                  }
                }
                
        }
        
              LPVS.pop_back();
              backwards = true;
            }
          }
          break;
        }
        //section 3.5, If last vertex in L is not V, add first and last vertices of L into PmL
        if(L.back().key != v2.key){
          bool Lm = true;
          reverse(PmL.begin(), PmL.end());
          PmL.insert(PmL.end(), L.front());
          PmL.insert(PmL.end(), L.back());
          sort(PmL.begin(), PmL.end(), compareVertex);
          P = PmL;
          v1 = P[0];
        
        
        }
        // if only 2 vertices in L, consider next vertex as v1
        else if(L.back().key == v2.key){
        // vector<vertex> L;
          L.clear();
          v1 = nextVertex(v1, P);
          L.push_back(v1);
          

          continue;
        }

      }
      
      for(auto point: L){
          polygonid.push_back(vector<int>());
          polygonid[t].push_back(point.key);
          count.insert(point.key);
        }
        t++;
      for(auto point: L){
          cout <<"("<<point.x<<","<<point.y<<")";
          if(point.key!=L.back().key){
            cout << ",";
          }  
          }
        cout << endl;
      if(P.size() == 3){
          for(auto point: P){
          cout <<"("<<point.x<<","<<point.y<<")";
          if(point.key!=P.back().key){
            cout << ",";
          }
        
          polygonid.push_back(vector<int>());
          polygonid[t].push_back(point.key);
          count.insert(point.key);
          }


          }
      L.clear();
      L.push_back(v1);
    }
    return polygonid;
  }

  int main(){
  vector<Point> P0;

    //P0= {{0,0}, {0,1}, {1,1}, {0.75, 0.5}, {1,0}}; //2513 ms
    //P0 = {{-1,0}, {-0.5,1}, {0.5,1}, {1,0}, {0.5,-1},{0,0} ,{-0.5,-1}}; //6146 ms
    //P0 = {{-1,0}, {-0.5,1},{0,-0.1}, {0.5,1}, {0.2,-0.1}, {0.5,-1},{0,-0.5} ,{-0.5,-1}}; //6922 ms //7362ms
    //P0 = {{0, 0}, {1, 1}, {2, 1}, {3, 0}, {3, -2}, {2, -3}, {0, -3}, {-2, -2}, {-1, 1}}; //6089 ms
    P0 = {{0, 0}, {1, 1}, {2, 0}, {3, 1}, {4, 0}, {4, -2}, {3, -3}, {1, -3}, {0, -2}, {-1, -3}, {-3, -3}}; //8 3 5 6 ms
    //P0 = {{0, 0}, {1, 5}, {3, 10}, {6, 13}, {9, 14}, {12, 12}, {14, 9}, {14, -9}, {12, -12}, {9, -14}, {6, -13}, {3, -10}, {1, -5}, {-1, -5}, {-3, -3}, {-5, -1}, {-5, 1}};
    //P0 = {{0, 0}, {1, 5}, {3, 11}, {6, 15}, {9, 17}, {12, 16}, {15, 12}, {17, 9}, {17, -9}, {15, -12}, {12, -16}, {9, -17}, {6, -15}};
    //P0 = {{0, 4}, {1, 1}, {3, 3}, {1, 0}, {4, 0}, {3, -3}, {0, -4}, {-3, -3}, {-4, 0}, {-3, 3}};
    
    // P0 = {{0, 4}, {1, 1}, {3, 3}, {4, 0}, {3, -3}, {0, -4}, {-3, -3}, {-4, 0}, {-3, 3}}; //3622 microseconds
    // P0 = {{0, 4}, {1, 1}, {3, 3}, {1, 0}, {4, 0}, {3, -3}, {0, -4}, {-3, -3}, {-4, 0}, {-3, 3}}; //5625 microseconds
    //P0 = {{0, 4}, {1, 1}, {3, 3}, {1, 0}, {4, 0}, {1, -1}, {3, -3}, {0, -4}, {-3, -3}, {-4, 0}, {-3, 3}}; // 8720 microseconds
    // P0 = {{0, 4}, {1, 1}, {3, 3}, {1, 0}, {4, 0}, {1, -1}, {3, -3}, {0, -1}, {0, -4}, {-3, -3}, {-4, 0}, {-3, 3}}; //9369 microseconds
    // P0 = {{0, 4}, {1, 1}, {3, 3}, {1, 0}, {4, 0}, {1, -1}, {3, -3}, {0, -1}, {0, -4}, {-1, -1}, {-3, -3}, {-4, 0}, {-3, 3}}; //11362
    // P0 = {{0, 4}, {1, 1}, {3, 3}, {1, 0}, {4, 0}, {1, -1}, {3, -3}, {0, -1}, {0, -4}, {-1, -1}, {-3, -3}, {-1, 0}, {-4, 0}, {-3, 3}}; //13662
    // P0 = {{0, 4}, {1, 1}, {3, 3}, {1, 0}, {4, 0}, {1, -1}, {3, -3}, {0, -1}, {0, -4}, {-1, -1}, {-3, -3}, {-1, 0}, {-4, 0}, {-1, 1}, {-3, 3}}; //14516
    // P0 = {{0, 4}, {1, 1}, {3, 3}, {1, 0}, {4, 0}, {1, -1}, {3, -3}, {0, -1}, {0, -4}, {-1, -1}, {-3, -3}, {-1, 0}, {-4, 0}, {-1, 1}, {-3, 3}, {0, 1}}; //16243 



    int nodes, edges;
    nodes = P0.size();

    edges = nodes;
    float x, y;
    // int edges, nodes;
    // float x, y;
    // cout << "No. of vertices : ";
    // cin >> nodes;
    // cout << "No. of edges : ";
    // cin >> edges;
    vector<vertex> P(nodes);
    

    for (int i = 0; i < nodes; i++) {
      //cout << "Coordinates of " << i << " vertex : ";
      //cin >> x >> y;
      P[i].key = i;
      P[i].x = P0[i].x;
      P[i].y = P0[i].y;
    }
    cout << "polygon " << endl;
    for(auto point: P){
      cout <<"("<<point.x<<","<<point.y<<")";
      if(point.key!=P.back().key){
          cout << ",";
        }
    }
    cout << endl << endl;
    vector<vertex> Poriginal = P;
    vector<vector<int>> polygonid;
    auto start = high_resolution_clock::now();
    polygonid = MP1(P);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);  
      cout << endl<<"Time taken by function: "
    << duration.count() << " microseconds" << endl;
    for (int i = 0; i < polygonid.size(); ) {
      if (polygonid[i].size() == 0) {
          polygonid.erase(polygonid.begin() + i);
      } else ++i;
    }

    //adding to list of edges
    edges = edges + polygonid.size();
    vector<Point> edgelist(edges);
    for(int i = 0; i < edges-polygonid.size(); i++){
      edgelist[i].x = i%(nodes);
      edgelist[i].y = (i+1)%(nodes);
    }

    for(int i = edges-polygonid.size(); i < edges; i++){
      edgelist[i].x = polygonid[i-edges+polygonid.size()].front();
      edgelist[i].y = polygonid[i-edges+polygonid.size()].back();
    }


   
    
    //removing redundant edges
    edgelist.erase(std::unique(edgelist.begin(), edgelist.end(), [](const auto& a, const auto& b) {
    return a.x == b.x && a.y == b.y;}), edgelist.end());
    for(int i = 0; i < edgelist.size(); i++){
      for(int j = i+1; j < edgelist.size(); j++){
      if(edgelist[i].x == edgelist[j].y && edgelist[i].y == edgelist[j].x){
        edgelist.erase(edgelist.begin()+j);
        j--;
        edges--;
      }
    }
    }

    vector<half_edge> h(2 * edges);
    vector<vertex_table> ver_tab(nodes);
    vector<half_edge_table> half_edge_table(2 * edges);
    vector<vector<float>> adj(nodes);
    vector<bool> unvisited_half_edge(2 * edges);
    vector<face> face(nodes);
    vector<face_table> face_table(nodes);


    

    for (int i = 0; i < edges; i++) {

      x = edgelist[i].x;
      y = edgelist[i].y;
      adj[x].push_back(y);
      adj[y].push_back(x);
      h[2 * i].origin_v = x;
      h[2 * i].end_v = y;
      h[2 * i].origin = &Poriginal[x];
      h[2 * i].end = &Poriginal[y];
      h[2 * i + 1].origin_v = y;
      h[2 * i + 1].end_v = x;
      h[2 * i + 1].origin = &Poriginal[y];
      h[2 * i + 1].end = &Poriginal[x];
      h[2 * i].twin = &h[2 * i + 1];
      h[2 * i + 1].twin = &h[2 * i];
    }
    
    // fill_vertex_table(ver_tab, nodes, adj, h, Poriginal);
    // fill_half_edge_table(half_edge_table, h, unvisited_half_edge, Poriginal, adj,face, face_table);
    // print_vertex_table(ver_tab, nodes);
    // print_half_edge_table(half_edge_table, h);
   
  }