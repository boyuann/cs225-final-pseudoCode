#include <queue>
#include <stdexcept>

using namespace Algorithms;

template <typename T>
Algorithms::TraversalLabel Algorithms::get_label(Labels<T> const& labels,
                                                 T vertex) {
  if (labels.first.find(vertex) != labels.first.end())
    return labels.first.at(vertex);
  throw std::range_error("non-existent vertex");
}

template <typename T>
Algorithms::TraversalLabel Algorithms::get_label(Labels<T> const& labels,
                                                 T source, T destination) {
  if (labels.second.find(source) != labels.second.end() &&
      labels.second.at(source).find(destination) !=
          labels.second.at(source).end())
    return labels.second.at(source).at(destination);
  throw std::range_error("non-existent edge");
}

template <typename T>
Algorithms::TraversalLabel Algorithms::get_label(Labels<T> const& labels,
                                                 typename Graph<T>::Edge edge) {
  return get_label(labels, edge.source, edge.destination);
}

template <typename T>
void Algorithms::set_label(Algorithms::Labels<T>& labels, T vertex,
                           Algorithms::TraversalLabel label) {
  labels.first[vertex] = label;
}

template <typename T>
void Algorithms::set_label(Algorithms::Labels<T>& labels, T source,
                           T destination, Algorithms::TraversalLabel label) {
  if (labels.second.find(source) == labels.second.end())
    labels.second.insert({source, {}});
  // std::cout << "set_label(" << source << ", " << destination << ", " << label
  // << ")" << std::endl;
  labels.second[source][destination] = label;
}

template <typename T>
void Algorithms::set_label(Algorithms::Labels<T>& labels,
                           typename Graph<T>::Edge edge,
                           Algorithms::TraversalLabel label) {
  set_label(labels, edge.source, edge.destination, label);
}

std::ostream& Algorithms::operator<<(std::ostream& os,
                                     TraversalLabel const& label) {
  switch (label) {
  case TraversalLabel::UNEXPLORED:
    return os << "UNEXPLORED";
  case TraversalLabel::VISITED:
    return os << "VISITED";
  case TraversalLabel::DISCOVERY:
    return os << "DISCOVERY";
  case TraversalLabel::CROSS:
    return os << "CROSS";
  case TraversalLabel::BACK:
    return os << "BACK";
  default:
    return os << "UNKNOWN";
  }
}

template <typename T>
std::ostream& Algorithms::operator<<(std::ostream& os,
                                     Algorithms::Labels<T> const& labels) {
  os << "{" << std::endl;
  for (auto p : labels.first) {
    os << "  " << p.first << ": " << p.second << "," << std::endl;
  }
  for (auto p : labels.second) {
    auto left = p.first;
    for (auto p : p.second) {
      os << "  " << left << "->" << p.first << ": " << p.second << ","
         << std::endl;
    }
  }
  return os << "}" << std::endl;
}

template <typename T>
void bfs_init(Graph<T> const& g, Labels<T>& labels, std::vector<T>& vertices,
              std::vector<typename Graph<T>::Edge> edges) {
  g.get_all_vertices(vertices);
  g.get_all_edges(edges);
  for (T& v : vertices) {
    set_label(labels, v, TraversalLabel::UNEXPLORED);
  }
  for (auto& e : edges) {
    set_label(labels, e, TraversalLabel::UNEXPLORED);
  }
}

template <typename T>
void bfs_walk_impl(Graph<T> const& g, T const& v, Labels<T>& labels) {
  std::queue<T> q;
  set_label(labels, v, TraversalLabel::VISITED);
  q.push(v);

  while (!q.empty()) {
    T v = q.front();
    q.pop();
    std::cout << v << std::endl;
    for (T w : g.get_adjacent(v)) {
      if (get_label(labels, w) == TraversalLabel::UNEXPLORED) {
        set_label(labels, v, w, TraversalLabel::DISCOVERY);
        set_label(labels, w, TraversalLabel::VISITED);
        q.push(w);
      } else if (get_label(labels, v, w) == TraversalLabel::UNEXPLORED) {
        set_label(labels, v, w, TraversalLabel::CROSS);
      }
    }
  }
}

template <typename T>
void Algorithms::bfs_walk(Graph<T> const& g, T const& v, Labels<T>& labels) {

  std::vector<T> vertices;
  std::vector<typename Graph<T>::Edge> edges;

  bfs_init(g, labels, vertices, edges);

  bfs_walk_impl(g, v, labels);
}

template <typename T>
void Algorithms::bfs_walk(Graph<T> const& g, Labels<T>& labels) {
  std::vector<T> vertices;
  std::vector<typename Graph<T>::Edge> edges;

  bfs_init(g, labels, vertices, edges);

  for (T& v : vertices) {
    if (get_label(labels, v) == TraversalLabel::UNEXPLORED)
      bfs_walk_impl(g, v, labels);
  }
}
