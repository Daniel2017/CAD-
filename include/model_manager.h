#ifndef MODEL_MANAGER_H
#define MODEL_MANAGER_H

#include "geometry.h"
#include <unordered_map>
#include <memory>
#include <vector>

/**
 * @brief CAD模型管理器
 * 
 * 用于管理单个机械零件（如螺栓、垫片）的几何数据
 * 实现顶点、边、面的导入与存储，智能指针管理内存，
 * 以及快速查询功能
 */
class ModelManager {
public:
    /**
     * @brief 构造函数
     */
    ModelManager();
    
    /**
     * @brief 析构函数
     */
    ~ModelManager();
    
    /**
     * @brief 添加顶点
     * 
     * @param id 顶点ID
     * @param x x坐标
     * @param y y坐标
     * @param z z坐标
     * @return std::shared_ptr<Point3D> 添加的顶点智能指针
     */
    std::shared_ptr<Point3D> addVertex(int id, double x, double y, double z);
    
    /**
     * @brief 添加边
     * 
     * @param id 边ID
     * @param start_id 起始顶点ID
     * @param end_id 结束顶点ID
     * @return std::shared_ptr<Edge> 添加的边智能指针
     */
    std::shared_ptr<Edge> addEdge(int id, int start_id, int end_id);
    
    /**
     * @brief 添加面
     * 
     * @param id 面ID
     * @param edge_ids 面的边ID集合
     * @return std::shared_ptr<Face> 添加的面智能指针
     */
    std::shared_ptr<Face> addFace(int id, const std::vector<int>& edge_ids);
    
    /**
     * @brief 通过ID获取顶点
     * 
     * @param id 顶点ID
     * @return std::shared_ptr<Point3D> 顶点智能指针，如果不存在返回nullptr
     */
    std::shared_ptr<Point3D> getVertex(int id) const;
    
    /**
     * @brief 通过ID获取边
     * 
     * @param id 边ID
     * @return std::shared_ptr<Edge> 边智能指针，如果不存在返回nullptr
     */
    std::shared_ptr<Edge> getEdge(int id) const;
    
    /**
     * @brief 通过ID获取面
     * 
     * @param id 面ID
     * @return std::shared_ptr<Face> 面智能指针，如果不存在返回nullptr
     */
    std::shared_ptr<Face> getFace(int id) const;
    
    /**
     * @brief 获取所有顶点
     * 
     * @return const std::vector<std::shared_ptr<Point3D>>& 顶点集合
     */
    const std::vector<std::shared_ptr<Point3D>>& getVertices() const;
    
    /**
     * @brief 获取所有边
     * 
     * @return const std::vector<std::shared_ptr<Edge>>& 边集合
     */
    const std::vector<std::shared_ptr<Edge>>& getEdges() const;
    
    /**
     * @brief 获取所有面
     * 
     * @return const std::vector<std::shared_ptr<Face>>& 面集合
     */
    const std::vector<std::shared_ptr<Face>>& getFaces() const;
    
    /**
     * @brief 预分配顶点空间
     * 
     * @param size 预分配的顶点数量
     */
    void reserveVertices(size_t size);
    
    /**
     * @brief 预分配边空间
     * 
     * @param size 预分配的边数量
     */
    void reserveEdges(size_t size);
    
    /**
     * @brief 预分配面空间
     * 
     * @param size 预分配的面数量
     */
    void reserveFaces(size_t size);
    
private:
    std::unordered_map<int, std::shared_ptr<Point3D>> vertex_map; // 顶点ID到顶点对象的映射
    std::unordered_map<int, std::shared_ptr<Edge>> edge_map;     // 边ID到边对象的映射
    std::unordered_map<int, std::shared_ptr<Face>> face_map;     // 面ID到面对象的映射
    std::vector<std::shared_ptr<Point3D>> vertices; // 批量顶点存储
    std::vector<std::shared_ptr<Edge>> edges;      // 批量边存储
    std::vector<std::shared_ptr<Face>> faces;      // 批量面存储
};

#endif // MODEL_MANAGER_H
