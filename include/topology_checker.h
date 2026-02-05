#ifndef TOPOLOGY_CHECKER_H
#define TOPOLOGY_CHECKER_H

#include "model_manager.h"
#include <vector>

/**
 * @brief 拓扑错误检测类
 * 
 * 检测几何模型中的拓扑错误，如边面重复、面法向不一致等
 */
class TopologyChecker {
public:
    /**
     * @brief 检测边重复
     * 
     * @param manager 模型管理器
     * @return std::vector<int> 重复边的ID列表
     */
    static std::vector<int> detectDuplicateEdges(const ModelManager& manager);
    
    /**
     * @brief 检测面重复
     * 
     * @param manager 模型管理器
     * @return std::vector<int> 重复面的ID列表
     */
    static std::vector<int> detectDuplicateFaces(const ModelManager& manager);
    
    /**
     * @brief 检测面法向不一致
     * 
     * @param manager 模型管理器
     * @return std::vector<int> 法向不一致面的ID列表
     */
    static std::vector<int> detectNormalInconsistencies(const ModelManager& manager);
    
    /**
     * @brief 检测所有拓扑错误
     * 
     * @param manager 模型管理器
     * @return bool 是否存在拓扑错误
     */
    static bool detectAllTopologyErrors(const ModelManager& manager);
};

#endif // TOPOLOGY_CHECKER_H
