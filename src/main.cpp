#include "model_manager.h"
#include "geometry_algorithm.h"
#include "topology_checker.h"
#include <iostream>
#include <vector>

/**
 * @brief 测试螺栓建模
 * 
 * 使用拉伸和旋转特征创建螺栓模型
 */
void testBoltModeling() {
    std::cout << "\n=== 测试螺栓建模 ===" << std::endl;
    
    ModelManager manager;
    
    // 螺栓头部轮廓（六边形）
    std::vector<Point3D> head_profile;
    double radius = 1.0;
    for (int i = 0; i < 6; ++i) {
        double angle = 2 * M_PI / 6 * i;
        double x = radius * std::cos(angle);
        double y = radius * std::sin(angle);
        head_profile.push_back(Point3D(i + 1, x, y, 0.0));
    }
    
    // 拉伸头部
    std::cout << "拉伸螺栓头部..." << std::endl;
    bool success = GeometryAlgorithm::extrude(manager, head_profile, 0.5);
    if (success) {
        std::cout << "螺栓头部拉伸成功!" << std::endl;
    } else {
        std::cout << "螺栓头部拉伸失败!" << std::endl;
    }
    
    // 螺栓杆部轮廓（圆形，简化为四边形）
    std::vector<Point3D> shaft_profile;
    double shaft_radius = 0.5;
    shaft_profile.push_back(Point3D(101, -shaft_radius, -shaft_radius, 0.0));
    shaft_profile.push_back(Point3D(102, shaft_radius, -shaft_radius, 0.0));
    shaft_profile.push_back(Point3D(103, shaft_radius, shaft_radius, 0.0));
    shaft_profile.push_back(Point3D(104, -shaft_radius, shaft_radius, 0.0));
    
    // 拉伸杆部
    std::cout << "拉伸螺栓杆部..." << std::endl;
    success = GeometryAlgorithm::extrude(manager, shaft_profile, 3.0);
    if (success) {
        std::cout << "螺栓杆部拉伸成功!" << std::endl;
    } else {
        std::cout << "螺栓杆部拉伸失败!" << std::endl;
    }
    
    // 检测拓扑错误
    std::cout << "检测螺栓模型拓扑错误..." << std::endl;
    TopologyChecker::detectAllTopologyErrors(manager);
    
    // 输出模型信息
    std::cout << "螺栓模型信息:" << std::endl;
    std::cout << "顶点数量: " << manager.getVertices().size() << std::endl;
    std::cout << "边数量: " << manager.getEdges().size() << std::endl;
    std::cout << "面数量: " << manager.getFaces().size() << std::endl;
}

/**
 * @brief 测试垫片建模
 * 
 * 使用拉伸特征创建垫片模型
 */
void testWasherModeling() {
    std::cout << "\n=== 测试垫片建模 ===" << std::endl;
    
    ModelManager manager;
    
    // 垫片外轮廓（圆形，简化为八边形）
    std::vector<Point3D> outer_profile;
    double outer_radius = 1.5;
    for (int i = 0; i < 8; ++i) {
        double angle = 2 * M_PI / 8 * i;
        double x = outer_radius * std::cos(angle);
        double y = outer_radius * std::sin(angle);
        outer_profile.push_back(Point3D(i + 1, x, y, 0.0));
    }
    
    // 拉伸垫片
    std::cout << "拉伸垫片..." << std::endl;
    bool success = GeometryAlgorithm::extrude(manager, outer_profile, 0.2);
    if (success) {
        std::cout << "垫片拉伸成功!" << std::endl;
    } else {
        std::cout << "垫片拉伸失败!" << std::endl;
    }
    
    // 检测拓扑错误
    std::cout << "检测垫片模型拓扑错误..." << std::endl;
    TopologyChecker::detectAllTopologyErrors(manager);
    
    // 输出模型信息
    std::cout << "垫片模型信息:" << std::endl;
    std::cout << "顶点数量: " << manager.getVertices().size() << std::endl;
    std::cout << "边数量: " << manager.getEdges().size() << std::endl;
    std::cout << "面数量: " << manager.getFaces().size() << std::endl;
}

/**
 * @brief 测试几何算法
 * 
 * 测试距离计算、变换等几何算法
 */
void testGeometryAlgorithms() {
    std::cout << "\n=== 测试几何算法 ===" << std::endl;
    
    // 测试距离计算
    Point3D p1(1, 0.0, 0.0, 0.0);
    Point3D p2(2, 1.0, 1.0, 1.0);
    double distance = GeometryAlgorithm::calculateDistance(p1, p2);
    std::cout << "两点距离: " << distance << std::endl;
    
    // 测试平移变换
    Point3D translated = GeometryAlgorithm::translate(p1, 1.0, 2.0, 3.0);
    std::cout << "平移变换后: (" << translated.x << ", " << translated.y << ", " << translated.z << ")" << std::endl;
    
    // 测试旋转变换
    Point3D rotated = GeometryAlgorithm::rotateZ(p2, M_PI / 2);
    std::cout << "旋转变换后: (" << rotated.x << ", " << rotated.y << ", " << rotated.z << ")" << std::endl;
    
    // 测试缩放变换
    Point3D scaled = GeometryAlgorithm::scale(p1, 2.0);
    std::cout << "缩放变换后: (" << scaled.x << ", " << scaled.y << ", " << scaled.z << ")" << std::endl;
}

/**
 * @brief 主函数
 * 
 * 测试CAD模型管理器的功能，包括螺栓/垫片建模、几何算法和拓扑检测
 */
int main() {
    std::cout << "=== CAD模型管理器测试 ===" << std::endl;
    
    // 测试几何算法
    testGeometryAlgorithms();
    
    // 测试螺栓建模
    testBoltModeling();
    
    // 测试垫片建模
    testWasherModeling();
    
    std::cout << "\n=== 所有测试完成 ===" << std::endl;
    
    return 0;
}
