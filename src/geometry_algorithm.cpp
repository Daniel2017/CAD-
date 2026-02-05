#include "geometry_algorithm.h"
#include <cmath>

/**
 * @brief 计算两点之间的距离
 * 
 * @param p1 第一个点
 * @param p2 第二个点
 * @return double 两点之间的距离
 */
double GeometryAlgorithm::calculateDistance(const Point3D& p1, const Point3D& p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    double dz = p2.z - p1.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

/**
 * @brief 计算点到面的投影
 * 
 * @param point 点
 * @param face 面
 * @param manager 模型管理器
 * @return Point3D 投影点
 */
Point3D GeometryAlgorithm::projectPointToFace(const Point3D& point, const Face& face, const ModelManager& manager) {
    // 简化实现：取面的第一个顶点作为投影点
    // 实际应用中需要计算点到平面的垂直投影
    if (face.edge_ids.empty()) {
        return point;
    }
    
    auto edge = manager.getEdge(face.edge_ids[0]);
    if (!edge) {
        return point;
    }
    
    auto vertex = manager.getVertex(edge->start_id);
    if (!vertex) {
        return point;
    }
    
    return *vertex;
}

/**
 * @brief 计算面的法向量
 * 
 * @param face 面
 * @param manager 模型管理器
 * @return double* 法向量数组 [x, y, z]
 */
double* GeometryAlgorithm::calculateFaceNormal(const Face& face, const ModelManager& manager) {
    static double normal[3];
    
    // 简化实现：假设面是三角形，使用前三个顶点计算法向量
    if (face.edge_ids.size() < 3) {
        normal[0] = 0.0;
        normal[1] = 0.0;
        normal[2] = 1.0;
        return normal;
    }
    
    // 获取边的顶点
    auto edge1 = manager.getEdge(face.edge_ids[0]);
    auto edge2 = manager.getEdge(face.edge_ids[1]);
    
    if (!edge1 || !edge2) {
        normal[0] = 0.0;
        normal[1] = 0.0;
        normal[2] = 1.0;
        return normal;
    }
    
    auto v1 = manager.getVertex(edge1->start_id);
    auto v2 = manager.getVertex(edge1->end_id);
    auto v3 = manager.getVertex(edge2->end_id);
    
    if (!v1 || !v2 || !v3) {
        normal[0] = 0.0;
        normal[1] = 0.0;
        normal[2] = 1.0;
        return normal;
    }
    
    // 计算向量
    double v1v2[3] = {v2->x - v1->x, v2->y - v1->y, v2->z - v1->z};
    double v1v3[3] = {v3->x - v1->x, v3->y - v1->y, v3->z - v1->z};
    
    // 叉乘计算法向量
    normal[0] = v1v2[1] * v1v3[2] - v1v2[2] * v1v3[1];
    normal[1] = v1v2[2] * v1v3[0] - v1v2[0] * v1v3[2];
    normal[2] = v1v2[0] * v1v3[1] - v1v2[1] * v1v3[0];
    
    // 归一化
    double length = std::sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
    if (length > 1e-6) {
        normal[0] /= length;
        normal[1] /= length;
        normal[2] /= length;
    }
    
    return normal;
}

/**
 * @brief 平移变换
 * 
 * @param point 点
 * @param dx x方向平移量
 * @param dy y方向平移量
 * @param dz z方向平移量
 * @return Point3D 平移后的点
 */
Point3D GeometryAlgorithm::translate(const Point3D& point, double dx, double dy, double dz) {
    return Point3D(point.id, point.x + dx, point.y + dy, point.z + dz);
}

/**
 * @brief 旋转变换（绕Z轴）
 * 
 * @param point 点
 * @param angle 旋转角度（弧度）
 * @return Point3D 旋转后的点
 */
Point3D GeometryAlgorithm::rotateZ(const Point3D& point, double angle) {
    double cos_theta = std::cos(angle);
    double sin_theta = std::sin(angle);
    
    double x = point.x * cos_theta - point.y * sin_theta;
    double y = point.x * sin_theta + point.y * cos_theta;
    
    return Point3D(point.id, x, y, point.z);
}

/**
 * @brief 缩放变换
 * 
 * @param point 点
 * @param scale 缩放因子
 * @return Point3D 缩放后的点
 */
Point3D GeometryAlgorithm::scale(const Point3D& point, double scale) {
    return Point3D(point.id, point.x * scale, point.y * scale, point.z * scale);
}

/**
 * @brief 拉伸特征建模
 * 
 * @param manager 模型管理器
 * @param profile_vertices 轮廓顶点
 * @param distance 拉伸距离
 * @return bool 是否成功
 */
bool GeometryAlgorithm::extrude(ModelManager& manager, const std::vector<Point3D>& profile_vertices, double distance) {
    if (profile_vertices.empty()) {
        return false;
    }
    
    // 预分配空间
    size_t vertex_count = profile_vertices.size();
    manager.reserveVertices(vertex_count * 2);
    manager.reserveEdges(vertex_count * 4);
    manager.reserveFaces(vertex_count + 2);
    
    // 添加原始轮廓顶点
    int base_id = 1;
    for (size_t i = 0; i < vertex_count; ++i) {
        const auto& v = profile_vertices[i];
        manager.addVertex(base_id + i, v.x, v.y, v.z);
    }
    
    // 添加拉伸后的顶点
    for (size_t i = 0; i < vertex_count; ++i) {
        const auto& v = profile_vertices[i];
        manager.addVertex(base_id + vertex_count + i, v.x, v.y, v.z + distance);
    }
    
    // 添加边
    int edge_id = 1;
    // 原始轮廓边
    for (size_t i = 0; i < vertex_count; ++i) {
        int next = (i + 1) % vertex_count;
        manager.addEdge(edge_id++, base_id + i, base_id + next);
    }
    // 拉伸后的轮廓边
    for (size_t i = 0; i < vertex_count; ++i) {
        int next = (i + 1) % vertex_count;
        manager.addEdge(edge_id++, base_id + vertex_count + i, base_id + vertex_count + next);
    }
    // 拉伸方向边
    for (size_t i = 0; i < vertex_count; ++i) {
        manager.addEdge(edge_id++, base_id + i, base_id + vertex_count + i);
    }
    
    // 添加面
    int face_id = 1;
    // 原始轮廓面
    std::vector<int> base_edges;
    for (size_t i = 1; i <= vertex_count; ++i) {
        base_edges.push_back(static_cast<int>(i));
    }
    manager.addFace(face_id++, base_edges);
    
    // 拉伸后的轮廓面
    std::vector<int> top_edges;
    for (size_t i = vertex_count + 1; i <= vertex_count * 2; ++i) {
        top_edges.push_back(static_cast<int>(i));
    }
    manager.addFace(face_id++, top_edges);
    
    // 拉伸侧面
    for (size_t i = 0; i < vertex_count; ++i) {
        std::vector<int> side_edges;
        side_edges.push_back(vertex_count * 2 + 1 + i);
        side_edges.push_back(vertex_count + 1 + (i + 1) % vertex_count);
        side_edges.push_back(vertex_count * 2 + 1 + (i + 1) % vertex_count);
        side_edges.push_back(i + 1);
        manager.addFace(face_id++, side_edges);
    }
    
    return true;
}

/**
 * @brief 旋转特征建模
 * 
 * @param manager 模型管理器
 * @param profile_vertices 轮廓顶点
 * @param axis_point 旋转轴点
 * @param axis_direction 旋转轴方向
 * @param angle 旋转角度（弧度）
 * @return bool 是否成功
 */
bool GeometryAlgorithm::revolve(ModelManager& manager, const std::vector<Point3D>& profile_vertices, 
                               const Point3D& /*axis_point*/, const double* /*axis_direction*/, double angle) {
    if (profile_vertices.empty()) {
        return false;
    }
    
    // 简化实现：绕Z轴旋转90度
    const int steps = 4; // 旋转步数
    const double step_angle = angle / steps;
    
    size_t vertex_count = profile_vertices.size();
    manager.reserveVertices(vertex_count * (steps + 1));
    manager.reserveEdges(vertex_count * (steps * 2 + 1));
    manager.reserveFaces(vertex_count * steps + 2);
    
    // 添加旋转顶点
    int base_id = 1;
    for (int step = 0; step <= steps; ++step) {
        double current_angle = step_angle * step;
        for (size_t i = 0; i < vertex_count; ++i) {
            const auto& v = profile_vertices[i];
            Point3D rotated = rotateZ(v, current_angle);
            manager.addVertex(base_id + step * vertex_count + i, rotated.x, rotated.y, rotated.z);
        }
    }
    
    // 添加边
    int edge_id = 1;
    // 每个步骤的轮廓边
    for (int step = 0; step <= steps; ++step) {
        for (size_t i = 0; i < vertex_count; ++i) {
            int next = (i + 1) % vertex_count;
            manager.addEdge(edge_id++, base_id + step * vertex_count + i, base_id + step * vertex_count + next);
        }
    }
    // 旋转方向边
    for (size_t i = 0; i < vertex_count; ++i) {
        for (int step = 0; step < steps; ++step) {
            manager.addEdge(edge_id++, base_id + step * vertex_count + i, base_id + (step + 1) * vertex_count + i);
        }
    }
    
    // 添加面
    int face_id = 1;
    // 端面
    std::vector<int> end_edges;
    for (size_t i = 0; i < vertex_count; ++i) {
        end_edges.push_back(i + 1);
    }
    manager.addFace(face_id++, end_edges);
    
    // 侧面
    for (int step = 0; step < steps; ++step) {
        for (size_t i = 0; i < vertex_count; ++i) {
            std::vector<int> side_edges;
            int base = step * vertex_count + 1;
            int next_base = (step + 1) * vertex_count + 1;
            side_edges.push_back(base + i);
            side_edges.push_back(base + (i + 1) % vertex_count);
            side_edges.push_back(next_base + (i + 1) % vertex_count);
            side_edges.push_back(next_base + i);
            manager.addFace(face_id++, side_edges);
        }
    }
    
    return true;
}
