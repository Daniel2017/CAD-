#include "model_manager.h"

/**
 * @brief 构造函数
 */
ModelManager::ModelManager() {
    // 初始化时不需要特殊操作
}

/**
 * @brief 析构函数
 */
ModelManager::~ModelManager() {
    // 智能指针会自动管理内存，不需要手动释放
}

/**
 * @brief 添加顶点
 * 
 * @param id 顶点ID
 * @param x x坐标
 * @param y y坐标
 * @param z z坐标
 * @return std::shared_ptr<Point3D> 添加的顶点智能指针
 */
std::shared_ptr<Point3D> ModelManager::addVertex(int id, double x, double y, double z) {
    // 检查顶点是否已存在
    auto it = vertex_map.find(id);
    if (it != vertex_map.end()) {
        // 顶点已存在，返回现有顶点
        return it->second;
    }
    
    // 创建新顶点
    auto vertex = std::make_shared<Point3D>(id, x, y, z);
    
    // 添加到映射和向量中
    vertex_map[id] = vertex;
    vertices.push_back(vertex);
    
    return vertex;
}

/**
 * @brief 添加边
 * 
 * @param id 边ID
 * @param start_id 起始顶点ID
 * @param end_id 结束顶点ID
 * @return std::shared_ptr<Edge> 添加的边智能指针
 */
std::shared_ptr<Edge> ModelManager::addEdge(int id, int start_id, int end_id) {
    // 检查边是否已存在
    auto it = edge_map.find(id);
    if (it != edge_map.end()) {
        // 边已存在，返回现有边
        return it->second;
    }
    
    // 检查顶点是否存在
    if (!getVertex(start_id) || !getVertex(end_id)) {
        // 顶点不存在，返回nullptr
        return nullptr;
    }
    
    // 创建新边
    auto edge = std::make_shared<Edge>(start_id, end_id);
    
    // 添加到映射和向量中
    edge_map[id] = edge;
    edges.push_back(edge);
    
    return edge;
}

/**
 * @brief 添加面
 * 
 * @param id 面ID
 * @param edge_ids 面的边ID集合
 * @return std::shared_ptr<Face> 添加的面智能指针
 */
std::shared_ptr<Face> ModelManager::addFace(int id, const std::vector<int>& edge_ids) {
    // 检查面是否已存在
    auto it = face_map.find(id);
    if (it != face_map.end()) {
        // 面已存在，返回现有面
        return it->second;
    }
    
    // 检查边是否都存在
    for (int edge_id : edge_ids) {
        if (!getEdge(edge_id)) {
            // 边不存在，返回nullptr
            return nullptr;
        }
    }
    
    // 创建新面
    auto face = std::make_shared<Face>(edge_ids);
    
    // 添加到映射和向量中
    face_map[id] = face;
    faces.push_back(face);
    
    return face;
}

/**
 * @brief 通过ID获取顶点
 * 
 * @param id 顶点ID
 * @return std::shared_ptr<Point3D> 顶点智能指针，如果不存在返回nullptr
 */
std::shared_ptr<Point3D> ModelManager::getVertex(int id) const {
    auto it = vertex_map.find(id);
    if (it != vertex_map.end()) {
        return it->second;
    }
    return nullptr;
}

/**
 * @brief 通过ID获取边
 * 
 * @param id 边ID
 * @return std::shared_ptr<Edge> 边智能指针，如果不存在返回nullptr
 */
std::shared_ptr<Edge> ModelManager::getEdge(int id) const {
    auto it = edge_map.find(id);
    if (it != edge_map.end()) {
        return it->second;
    }
    return nullptr;
}

/**
 * @brief 通过ID获取面
 * 
 * @param id 面ID
 * @return std::shared_ptr<Face> 面智能指针，如果不存在返回nullptr
 */
std::shared_ptr<Face> ModelManager::getFace(int id) const {
    auto it = face_map.find(id);
    if (it != face_map.end()) {
        return it->second;
    }
    return nullptr;
}

/**
 * @brief 获取所有顶点
 * 
 * @return const std::vector<std::shared_ptr<Point3D>>& 顶点集合
 */
const std::vector<std::shared_ptr<Point3D>>& ModelManager::getVertices() const {
    return vertices;
}

/**
 * @brief 获取所有边
 * 
 * @return const std::vector<std::shared_ptr<Edge>>& 边集合
 */
const std::vector<std::shared_ptr<Edge>>& ModelManager::getEdges() const {
    return edges;
}

/**
 * @brief 获取所有面
 * 
 * @return const std::vector<std::shared_ptr<Face>>& 面集合
 */
const std::vector<std::shared_ptr<Face>>& ModelManager::getFaces() const {
    return faces;
}

/**
 * @brief 预分配顶点空间
 * 
 * @param size 预分配的顶点数量
 */
void ModelManager::reserveVertices(size_t size) {
    vertices.reserve(size);
}

/**
 * @brief 预分配边空间
 * 
 * @param size 预分配的边数量
 */
void ModelManager::reserveEdges(size_t size) {
    edges.reserve(size);
}

/**
 * @brief 预分配面空间
 * 
 * @param size 预分配的面数量
 */
void ModelManager::reserveFaces(size_t size) {
    faces.reserve(size);
}
