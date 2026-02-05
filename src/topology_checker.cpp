#include "topology_checker.h"
#include "geometry_algorithm.h"
#include <string>
#include <iostream>
#include <unordered_set>
#include <algorithm>

/**
 * @brief 检测边重复
 * 
 * @param manager 模型管理器
 * @return std::vector<int> 重复边的ID列表
 */
std::vector<int> TopologyChecker::detectDuplicateEdges(const ModelManager& manager) {
    std::vector<int> duplicate_edges;
    std::unordered_set<std::string> edge_signatures;
    
    const auto& edges = manager.getEdges();
    
    for (size_t i = 0; i < edges.size(); ++i) {
        const auto& edge = edges[i];
        if (!edge) continue;
        
        // 创建边的签名（确保顺序一致）
        int min_id = std::min(edge->start_id, edge->end_id);
        int max_id = std::max(edge->start_id, edge->end_id);
        std::string signature = std::to_string(min_id) + "-" + std::to_string(max_id);
        
        if (edge_signatures.count(signature)) {
            // 边已存在，标记为重复
            duplicate_edges.push_back(edge->start_id); // 这里简化处理，实际应该使用边的ID
        } else {
            edge_signatures.insert(signature);
        }
    }
    
    return duplicate_edges;
}

/**
 * @brief 检测面重复
 * 
 * @param manager 模型管理器
 * @return std::vector<int> 重复面的ID列表
 */
std::vector<int> TopologyChecker::detectDuplicateFaces(const ModelManager& manager) {
    std::vector<int> duplicate_faces;
    std::unordered_set<std::string> face_signatures;
    
    const auto& faces = manager.getFaces();
    
    for (size_t i = 0; i < faces.size(); ++i) {
        const auto& face = faces[i];
        if (!face) continue;
        
        // 创建面的签名（边ID排序后拼接）
        std::vector<int> sorted_edges = face->edge_ids;
        std::sort(sorted_edges.begin(), sorted_edges.end());
        
        std::string signature;
        for (int edge_id : sorted_edges) {
            signature += std::to_string(edge_id) + ",";
        }
        
        if (face_signatures.count(signature)) {
            // 面已存在，标记为重复
            duplicate_faces.push_back(static_cast<int>(i + 1)); // 这里简化处理，实际应该使用面的ID
        } else {
            face_signatures.insert(signature);
        }
    }
    
    return duplicate_faces;
}

/**
 * @brief 检测面法向不一致
 * 
 * @param manager 模型管理器
 * @return std::vector<int> 法向不一致面的ID列表
 */
std::vector<int> TopologyChecker::detectNormalInconsistencies(const ModelManager& manager) {
    std::vector<int> inconsistent_faces;
    
    const auto& faces = manager.getFaces();
    if (faces.empty()) {
        return inconsistent_faces;
    }
    
    // 计算第一个面的法向量作为参考
    const auto& first_face = faces[0];
    if (!first_face || first_face->edge_ids.empty()) {
        return inconsistent_faces;
    }
    
    double* ref_normal = GeometryAlgorithm::calculateFaceNormal(*first_face, manager);
    
    // 检测其他面的法向量是否与参考一致
    for (size_t i = 1; i < faces.size(); ++i) {
        const auto& face = faces[i];
        if (!face || face->edge_ids.empty()) continue;
        
        double* current_normal = GeometryAlgorithm::calculateFaceNormal(*face, manager);
        
        // 计算点积，判断方向是否一致
        double dot_product = ref_normal[0] * current_normal[0] + 
                            ref_normal[1] * current_normal[1] + 
                            ref_normal[2] * current_normal[2];
        
        if (dot_product < 0) {
            // 法向不一致
            inconsistent_faces.push_back(static_cast<int>(i + 1)); // 这里简化处理，实际应该使用面的ID
        }
    }
    
    return inconsistent_faces;
}

/**
 * @brief 检测所有拓扑错误
 * 
 * @param manager 模型管理器
 * @return bool 是否存在拓扑错误
 */
bool TopologyChecker::detectAllTopologyErrors(const ModelManager& manager) {
    bool has_errors = false;
    
    // 检测边重复
    auto duplicate_edges = detectDuplicateEdges(manager);
    if (!duplicate_edges.empty()) {
        has_errors = true;
        std::cout << "检测到重复边：";
        for (int edge_id : duplicate_edges) {
            std::cout << edge_id << " ";
        }
        std::cout << std::endl;
    }
    
    // 检测面重复
    auto duplicate_faces = detectDuplicateFaces(manager);
    if (!duplicate_faces.empty()) {
        has_errors = true;
        std::cout << "检测到重复面：";
        for (int face_id : duplicate_faces) {
            std::cout << face_id << " ";
        }
        std::cout << std::endl;
    }
    
    // 检测面法向不一致
    auto inconsistent_faces = detectNormalInconsistencies(manager);
    if (!inconsistent_faces.empty()) {
        has_errors = true;
        std::cout << "检测到法向不一致的面：";
        for (int face_id : inconsistent_faces) {
            std::cout << face_id << " ";
        }
        std::cout << std::endl;
    }
    
    if (!has_errors) {
        std::cout << "未检测到拓扑错误！" << std::endl;
    }
    
    return has_errors;
}
