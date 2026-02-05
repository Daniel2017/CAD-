#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <memory>

/**
 * @brief 三维顶点结构
 * 
 * 表示三维空间中的一个点，包含x、y、z三个坐标值和唯一ID
 * 贴合工业机械零件的三维特征
 */
struct Point3D {
    int id;    // 顶点唯一ID
    double x; // x坐标
    double y; // y坐标
    double z; // z坐标
    
    /**
     * @brief 构造函数
     * 
     * @param id 顶点唯一ID
     * @param x x坐标值
     * @param y y坐标值
     * @param z z坐标值
     */
    Point3D(int id = 0, double x = 0.0, double y = 0.0, double z = 0.0) 
        : id(id), x(x), y(y), z(z) {}
};

/**
 * @brief 边结构
 * 
 * 表示两个顶点之间的连线，存储关联顶点ID
 */
struct Edge {
    int start_id; // 起始顶点ID
    int end_id;   // 结束顶点ID
    
    /**
     * @brief 构造函数
     * 
     * @param start_id 起始顶点ID
     * @param end_id 结束顶点ID
     */
    Edge(int start_id, int end_id)
        : start_id(start_id), end_id(end_id) {}
};

/**
 * @brief 面结构
 * 
 * 表示由多个边组成的面，包含关联边ID和面法向量
 */
struct Face {
    std::vector<int> edge_ids; // 面的边ID集合
    double normal[3];          // 面法向量 (x, y, z)
    
    /**
     * @brief 构造函数
     * 
     * @param edge_ids 面的边ID集合
     */
    Face(const std::vector<int>& edge_ids)
        : edge_ids(edge_ids) {
        // 初始化法向量为零向量
        normal[0] = 0.0;
        normal[1] = 0.0;
        normal[2] = 0.0;
    }
};

#endif // GEOMETRY_H
