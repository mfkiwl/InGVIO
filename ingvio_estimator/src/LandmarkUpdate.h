#pragma once

#include <memory>

#include "IngvioParams.h"
#include "MapServer.h"
#include "Update.h"

namespace ingvio
{
    class State;
    class Triangulator;
    
    class LandmarkUpdate : public UpdateBase
    {
    public:
        
        using UpdateBase::UpdateBase;
        
        LandmarkUpdate(const IngvioParams& filter_params) :
        UpdateBase(filter_params._chi2_max_dof, filter_params._chi2_thres),
        _noise(filter_params._visual_noise)
        {}
        
        LandmarkUpdate(const LandmarkUpdate&) = delete;
        LandmarkUpdate operator=(const LandmarkUpdate&) = delete;
        
        virtual ~LandmarkUpdate() {}
        
        void updateLandmarkMono(std::shared_ptr<State> state,
                                std::shared_ptr<MapServer> map_server);
        
        void initNewLandmarkMono(std::shared_ptr<State> state,
                                 std::shared_ptr<MapServer> map_server,
                                 std::shared_ptr<Triangulator> tri);
        
        void changeLandmarkAnchor(std::shared_ptr<State> state, 
                                  std::shared_ptr<MapServer> map_server);
        
    protected:
        
        double _noise;
        
        void calcResJacobianSingleLandmarkMono(const std::shared_ptr<FeatureInfo> feature_info,
                                               const std::shared_ptr<State> state,
                                               Eigen::Vector2d& res,
                                               Eigen::Matrix<double, 2, 9>& H_fj_epose,
                                               Eigen::Matrix<double, 2, 6>& H_fj_ext,
                                               Eigen::Matrix<double, 2, 6>& H_fj_anch,
                                               Eigen::Matrix<double, 2, 3>& H_fj_pf);
        
        void calcResJacobianSingleFeatAllMonoObs(
            const std::shared_ptr<FeatureInfo> feature_info,
            const std::map<double, std::shared_ptr<SE3>>& sw_poses,
            const std::vector<std::shared_ptr<SE3>>& sw_var_order,
            const std::map<std::shared_ptr<SE3>, int>& sw_index_map,
            Eigen::VectorXd& res_block,
            Eigen::MatrixXd& Hx_block,
            Eigen::MatrixXd& Hf_block);
        
        void generateSwVarOrder(const std::shared_ptr<State> state,
                                std::vector<std::shared_ptr<SE3>>& sw_var_order,
                                std::map<std::shared_ptr<SE3>, int>& sw_index,
                                std::vector<std::shared_ptr<Type>>& sw_var_type);
    };
}
