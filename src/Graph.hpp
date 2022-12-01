#include "Graph.h"
#include <stdexcept>
#include <map>
#include <queue>
#include <vector>
template <typename T>
void Graph<T>::add_vertex(T v) {
  if (!contains_vertex(v))
    _adj_list[v] = {};
}

template <typename T>
void Graph<T>::remove_vertex(T v) {
  if (!contains_vertex(v))
    return;

  for (auto p : _adj_list[v])
    p->second.erase(v);

  _adj_list.erase(v);
}

template <typename T>
void Graph<T>::add_edge(T source, T destination, double weight) {
  add_vertex(source);
  // _adj_list[source][destination] = Edge(source, destination, weight);
  _adj_list[source].insert({destination, {source, destination, weight}});
}

template <typename T>
void Graph<T>::remove_edge(T source, T destination) {
  if (contains_edge(source, destination))
    _adj_list[source].erase(destination);
}

template <typename T>
double Graph<T>::get_edge_weight(T source, T destination) {
  if (contains_edge(source, destination))
    return _adj_list[source][destination].weight;
  throw std::range_error("non-existent edge");
}

template <typename T>
bool Graph<T>::contains_edge(T source, T destination) {
  return (contains_vertex(source) &&
          _adj_list[source].find(destination) != _adj_list[source].end());
}

template <typename T>
bool Graph<T>::contains_vertex(T v) {
  return _adj_list.find(v) != _adj_list.end();
}

template <typename T>
std::vector<T> Graph<T>::get_adjacent(T v) {
  if (!contains_vertex(v))
    return {};

  std::vector<T> adj;
  for (auto p : _adj_list[v])
    adj.push_back(p.first);

  return adj;
}

template <typename T>
vector<T> Graph<T>::bfs_walk(T node, const Graph &graph) {
  vector<T> airports;
  queue<T> BFS_queue;
  map<T, bool> visited;
  visited[node] = true;
  BFS_queue.push(node);
  while(!BFS_queue.empty()) {
    node = BFS_queue.front();
    vector<T> adj = graph.get_adjacent(node);
    if (adj.empty())
      break;
    airports.push_back(node);
    BFS_queue.pop();
    for(T it : adj) {
      if (visited.find(it.first) == visited.end()){
        visited[it.first] = true;
        BFS_queue.push(it.first);
      }
    }
  }
  return airports;
}
