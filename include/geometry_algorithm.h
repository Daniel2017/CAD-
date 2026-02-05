#ifndef GEOMETRY_ALGORITHM_H
#define GEOMETRY_ALGORITHM_H

#include "geometry.h"
#include "model_manager.h"
#include <vector>
#include <memory>

/**
 * @brief 几何算法类
 * 
 * 提供几何计算、变换、特征建模等核心算法
 * 实现拉伸/旋转基础特征建模
 */
class GeometryAlgorithm {
public:
    /**
     * @brief 计算两点之间的距离
     * 
     * @param p1 第一个点
     * @param p2 第二个点
     * @return double 两点之间的距离
     */
    static double calculateDistance(const Point3D& p1, const Point3D& p2);
    
    /**
     * @brief 计算点到面的投影
     * 
     * @param point 点
     * @param face 面
     * @param manager 模型管理器
     * @return Point3D 投影点
     */
    static Point3D projectPointToFace(const Point3D& point, const Face& face, const ModelManager& manager);
    
    /**
     * @brief 计算面的法向量
     * 
     * @param face 面
     * @param manager 模型管理器
     * @return double* 法向量数组 [x, y, z]
     */
    static double* calculateFaceNormal(const Face& face, const ModelManager& manager);
    
    /**
     * @brief 平移变换
     * 
     * @param point 点
     * @param dx x方向平移量
     * @param dy y方向平移量
     * @param dz z方向平移量
     * @return Point3D 平移后的点
     */
    static Point3D translate(const Point3D& point, double dx, double dy, double dz);
    
    /**
     * @brief 旋转变换（绕Z轴）
     * 
     * @param point 点
     * @param angle 旋转角度（弧度）
     * @return Point3D 旋转后的点
     */
    static Point3D rotateZ(const Point3D& point, double angle);
    
    /**
     * @brief 缩放变换
     * 
     * @param point 点
     * @param scale 缩放因子
     * @return Point3D 缩放后的点
     */
    static Point3D scale(const Point3D& point, double scale);
    
    /**
     * @brief 拉伸特征建模
     * 
     * @param manager 模型管理器
     * @param profile_vertices 轮廓顶点
     * @param distance 拉伸距离
     * @return bool 是否成功
     */
    static bool extrude(ModelManager& manager, const std::vector<Point3D>& profile_vertices, double distance);
    
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
    static bool revolve(ModelManager& manager, const std::vector<Point3D>& profile_vertices, 
                       const Point3D& axis_point, const double* axis_direction, double angle);
};

#endif // GEOMETRY_ALGORITHM_H
