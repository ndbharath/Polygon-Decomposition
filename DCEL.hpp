#include <bits/stdc++.h>
using namespace std;


/**
 * @brief Class representing a vertex with its x and y coordinates and a key value
 */
class vertex
{
  public:
    /* data */
    float x, y;
    int key;
};
/**
 * @brief Class representing a half-edge with its origin vertex, end vertex, twin half-edge, and incident face
 */
class half_edge
{
  public:
    int origin_v, end_v;
    class vertex *origin, *end;
    class half_edge *twin;
};
/**
 * @brief Class representing a face with its key value, outer component half-edge, and inner components vector of half-edges
 */
class face
{
  public:
    int key = -1;
};
/**
 * @brief Class representing a vertex table with a vertex and its incident half-edge
 */
class vertex_table
{
  public:
    class vertex *v;
    class half_edge *e;
};
/**
 * @brief Class representing a face table with a face, outer component half-edge, inner components vector of half-edges, and area value
 */
class face_table
{
  public:
    class face *face = NULL;
    vector<half_edge *> inner_components;
    class half_edge *outer_component = NULL;
    float area = -1;
};
/**
 * @brief Class representing a half-edge table with a half-edge, next half-edge, previous half-edge, and incident face
 */
class half_edge_table
{
  public:
    class half_edge *half_edge, *next, *prev;
    class face *incident_face = NULL;
};

#define INF 10000
/**
 * @brief Class representing a point in 2D with its x and y coordinates
 */
class Point
{
  public:
    float x;
    float y;
};



/**
 * @brief Print the details of each half-edge in the vector of half-edges with its origin and end vertex and twin half-edge
 * @param a Vector of half-edges
 * @param v Vector of vertices
 * @param l Number of half-edges in the vector
 */
void print_half_edge(vector<half_edge> &a, vector<vertex> &v, int l) {
  for (int i = 0; i < l; i++) {
    int s, e;
    half_edge *temp;
    s = a[i].origin_v;
    e = a[i].end_v;
    temp = a[i].twin;
    cout << "edge :" << s << "-" << e << " ";
    cout << "coordinates : (" << v[s].x << " , " << v[s].y << ") , (" << v[e].x
         << " , " << v[e].y << ") ";
    cout << "Twin edge :" << temp->origin_v << "-" << temp->end_v << "\n";
  }
}

/**
 * @brief Searches for the index of a half-edge with the given start and end vertices
 * 
 * @param s Start vertex index of the half-edge
 * @param e End vertex index of the half-edge
 * @param h Vector of half-edges
 * @return Index of the half-edge in the vector, or -1 if not found
 */
int search_half_edge(int s, int e, vector<half_edge> &h) {
  int l = h.size();
  for (int i = 0; i < l; i++) {
    if (h[i].origin_v == s && h[i].end_v == e) {
      return i;
    }
  }
  cout << "Error in serching half edge"
       << "\n";
  return -1;
}

/**
 * @brief Searches for the index of a half-edge in the half-edge table
 * 
 * @param half_edge Pointer to the half-edge to be found
 * @param half_edge_table Vector of half-edge table entries
 * @return Index of the half-edge in the table, or 0 if not found
 */
int search_half_edge_table(half_edge *half_edge,
                           vector<half_edge_table> half_edge_table) {
  int l = half_edge_table.size();
  for (int i = 0; i < l; i++) {
    if (half_edge_table[i].half_edge == half_edge)
      return i;
  }
  return 0;
}

/**
 * @brief Fills in the vertex table with entries for each vertex
 * 
 * @param ver_tab Vector of vertex table entries
 * @param l Number of vertices
 * @param adj Adjacency matrix for the graph
 * @param h Vector of half-edges
 * @param v Vector of vertices
 */
void fill_vertex_table(vector<vertex_table> &ver_tab, int l,
                       vector<vector<float>> adj, vector<half_edge> &h,
                       vector<vertex> &v) {
  for (int i = 0; i < l; i++) {
    int s = v[i].key;
    int e = adj[s][0]; // Assuming no stand alone vertex
    ver_tab[i].v = &v[i];
    ver_tab[i].e = &h[search_half_edge(s, e, h)];
  }
}

/**
 * @brief Computes the angle between three points
 * 
 * @param x1 X-coordinate of the first point
 * @param y1 Y-coordinate of the first point
 * @param x2 X-coordinate of the second point (the vertex of the angle)
 * @param y2 Y-coordinate of the second point (the vertex of the angle)
 * @param x3 X-coordinate of the third point
 * @param y3 Y-coordinate of the third point
 * @return Angle between the three points in degrees
 */
float angle(float x1, float y1, float x2, float y2, float x3, float y3) {
  x1 = x1 - x2;
  y1 = y1 - y2;
  x3 = x3 - x2;
  y3 = y3 - y2;
  float dot = x1 * x3 + y1 * y3;
  float det = x1 * y3 - y1 * x3;
  float result = atan2(det, dot);
  return ((result < 0) ? (result * 180 / 3.141592) + 360
                       : (result * 180 / 3.141592));
}

/**
 * @brief Given a half edge, finds the next half edge by selecting the adjacent vertex with the maximum angle.
 *
 * @param current The index of the current half edge in the half_edge vector.
 * @param adj The adjacency list of the input graph.
 * @param vertex The list of vertices in the input graph.
 * @param half_edge The list of half edges in the input graph.
 * @return The index of the next half edge in the half_edge vector.
 */
int next_half_edge(int current, vector<vector<float>> adj,
                   vector<vertex> vertex, vector<half_edge> &half_edge) {
  int s, e;
  float max_angle = 0, temp_angle;
  int next_vertex;
  s = half_edge[current].origin_v;
  e = half_edge[current].end_v;
  float x1, x2, y1, y2;
  next_vertex = s;
  x1 = vertex[s].x;
  y1 = vertex[s].y;
  x2 = vertex[e].x;
  y2 = vertex[e].y;
  for (auto k : adj[e]) {
    if (k == e)
      continue;
    else {
      temp_angle = angle(x1, y1, x2, y2, vertex[k].x, vertex[k].y);
      if (max_angle < temp_angle) {
        max_angle = temp_angle;
        next_vertex = k;
      }
    }
  }
  return search_half_edge(e, next_vertex, half_edge);
}


/**
 * @brief Fills in the face information in the face table based on the information in the half edge table.
 *
 * @param half_edge_table The list of half edges and their corresponding relationships in the input graph.
 * @param face The list of faces in the input graph.
 * @param vertex The list of vertices in the input graph.
 * @param face_table The table of faces in the input graph, which includes information about the face's area and outer component.
 */
void fill_half_edge_face(vector<half_edge_table> &half_edge_table,
                         vector<face> &face, vector<vertex> vertex,
                         vector<face_table> &face_table) {
  int l = half_edge_table.size();
  int face_key = 0;
  int head = 0;
  float x2, y2;
  float signedArea = 0;
  for (int i = 0; i < l; i++) {
    float x1 = vertex[half_edge_table[i].half_edge->origin_v].x;
    float y1 = vertex[half_edge_table[i].half_edge->origin_v].y;
    if (half_edge_table[i].next == half_edge_table[head].half_edge) {
      x2 = vertex[half_edge_table[head].half_edge->origin_v].x;
      y2 = vertex[half_edge_table[head].half_edge->origin_v].y;
      signedArea += (x1 * y2 - x2 * y1);
      if (signedArea > 0) // Assuming no standalone edge
      {
        face_table[face_key].area = abs(signedArea / 2);
        face[face_key].key = face_key;
        face_table[face_key].face = &face[face_key];
        face_table[face_key].outer_component = half_edge_table[head].half_edge;
        for (int j = head; j <= i; j++)
          half_edge_table[j].incident_face = &face[face_key];
        face_key++;
      }
      signedArea = 0;
      head = i + 1;
    } else {
      x2 = vertex[half_edge_table[i].half_edge->end_v].x;
      y2 = vertex[half_edge_table[i].half_edge->end_v].y;
      signedArea += (x1 * y2 - x2 * y1);
    }
  }
  face[face_key].key = -1;
  face_table[face_key].face = &face[face_key];
}

/**
 * @brief Fills in the half edge table for the input graph.
 *
 * @param half_edge_table The list of half edges and their corresponding relationships in the input graph.
 * @param half_edge The list of half edges in the input graph.
 * @param unvisited_half_edge A vector that indicates whether a given half edge has been visited or not.
 * @param vertex The list of vertices in the input graph.
 * @param adj The adjacency list of the input graph.
 * @param face The list of faces in the input graph.
 * @param face_table The table of faces in the input graph, which includes information about the face's area and outer component.
 */
void fill_half_edge_table(vector<half_edge_table> &half_edge_table,
                          vector<half_edge> &half_edge,
                          vector<bool> &unvisited_half_edge,
                          vector<vertex> vertex, vector<vector<float>> adj,
                          vector<face> &face, vector<face_table> &face_table) {
  int l = unvisited_half_edge.size();
  int current, next, previous;
  int head, j = 0, head_index;
  for (int i = 0; i < l; i++) {
    if (unvisited_half_edge[i] == 0) {
      unvisited_half_edge[i] = 1;
      head = i;
      head_index = j;
      half_edge_table[j].half_edge = &half_edge[i];
      next = next_half_edge(i, adj, vertex, half_edge);
      half_edge_table[j].next = &half_edge[next];
      j++;
      previous = i;
      current = next;
      while (1) {
        unvisited_half_edge[current] = 1;
        half_edge_table[j].half_edge = &half_edge[current];
        next = next_half_edge(current, adj, vertex, half_edge);
        half_edge_table[j].next = &half_edge[next];
        half_edge_table[j].prev = &half_edge[previous];
        j++;
        previous = current;
        current = next;
        if (next == head) {
          break;
        }
      }
      half_edge_table[head_index].prev = &half_edge[previous];
    }
  }
  fill_half_edge_face(half_edge_table, face, vertex, face_table);
}



/**
 * @brief Prints the vertex table for the input graph.
 *
 * @param ver_tab The table of vertices in the input graph.
 * @param l The number of vertices in the input graph.
 */
void print_vertex_table(vector<vertex_table> &ver_tab, int l) {
  vertex *temp_v;
  half_edge *temp_e;
  cout << "\n"
       << "********** Vertex Table ***********"
       << "\n";
  cout << "vertex "
       << " Coordinates "
       << "Incident Edge "
       << "\n";
  for (int i = 0; i < l; i++) {
    temp_v = ver_tab[i].v;
    temp_e = ver_tab[i].e;
    cout << temp_v->key << "\t(" << temp_v->x << " , " << temp_v->y << ") ";
    cout << "\t" << temp_e->origin_v << "-" << temp_e->end_v << "\n";
  }
}

/**
 * Prints the Half Edge Table.
 * 
 * @param half_edge_table The half edge table to print.
 * @param half_edge The half edge to use for printing.
 */
void print_half_edge_table(vector<half_edge_table> &half_edge_table,
                           vector<half_edge> &half_edge) {
  int l = half_edge_table.size();
  class half_edge_table temp;
  cout << "\n"
       << "********** Half Edge Table **********"
       << "\n";
  cout << "Half-edge "
       << "Origin "
       << "Twin Incident_Face Next Previous"
       << "\n";
  for (int i = 0; i < l; i++) {
    temp = half_edge_table[i];
    cout << temp.half_edge->origin_v << "-" << temp.half_edge->end_v << "\t   ";
    cout << temp.half_edge->origin_v << "\t";
    cout << temp.half_edge->twin->origin_v << "-" << temp.half_edge->twin->end_v
         << "\t";
    if (temp.incident_face != NULL)
      cout << "   F" << temp.incident_face->key << "\t    ";
    else
      cout << "   NULL"
           << "     ";
    cout << temp.next->origin_v << "-" << temp.next->end_v << "   ";
    cout << temp.prev->origin_v << "-" << temp.prev->end_v << "\n";
  }
}


/**
 * Prints the Face Table.
 * 
 * @param face_table The face table to print.
 */
void print_face_table(vector<face_table> face_table) {
  int l = face_table.size();
  class face_table temp;
  cout << "*************** Face_Table **************"
       << "\n";
  cout << "Face   Outer_component"
       << "  Inner_Components"
       << "\n";
  for (int i = 0; i < l; i++) {
    temp = face_table[i];
    if (temp.face == NULL)
      break;
    if (temp.outer_component == NULL) {
      cout << "Finf" << setw(3) << "|" << setw(8) << "NULL" << setw(9) << "|"
           << setw(5);
      int n = temp.inner_components.size();
      for (int j = 0; j < n; j++)
        cout << temp.inner_components[j]->origin_v << "-"
             << temp.inner_components[j]->end_v << "  ";
      if (n == 0)
        cout << setw(5) << "NULL\t";
      cout << "\n";
    } else {
      cout << "F" << temp.face->key << setw(5) << "|" << setw(5)
           << temp.outer_component->origin_v << "-"
           << temp.outer_component->end_v;
      cout << setw(10) << "|" << setw(5);
      int n = temp.inner_components.size();
      for (int j = 0; j < n; j++)
        cout << temp.inner_components[j]->origin_v << "-"
             << temp.inner_components[j]->end_v << "\t";
      if (n == 0)
        cout << "    NULL";
      cout << "\n";
    }
  }
}




 

