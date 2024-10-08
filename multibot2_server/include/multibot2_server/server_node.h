#pragma once

#include <memory>

#include <QApplication>

#include <nav2_util/lifecycle_node.hpp>

#include "multibot2_server/instance_manager.h"
#include "multibot2_server/subgoal_generator/subgoal_generator.h"
#include "multibot2_server/subgoal_generator/subgoalgen_config.h"
#include "multibot2_server/server_panel.h"

#include "multibot2_util/panel_util.h"
#include "multibot2_util/Interface/Observer_Interface.h"

#include <geometry_msgs/msg/polygon.hpp>

#include "multibot2_msgs/msg/robot_state.hpp"

namespace multibot2_server
{
    class MultibotServer : public nav2_util::LifecycleNode, public Observer::ObserverInterface<multibot2_util::PanelUtil::Msg>
    {
    public:
        typedef std::unique_ptr<MultibotServer> UniquePtr;
        typedef std::shared_ptr<MultibotServer> SharedPtr;

    public:
        MultibotServer();

        ~MultibotServer();

    public:
        void run_server_panel(int argc, char *argv[]);

    public:
        void update(const multibot2_util::PanelUtil::Msg &_msg) override;

    protected:
        void init_variables();

    protected:
        void update_subgoals();

    protected:
        nav2_util::LifecycleNode::SharedPtr nh_;
        rclcpp::TimerBase::SharedPtr subgoal_update_timer_;

    protected:
        Instance_Manager::SharedPtr instance_manager_;

        SubgoalGenerator::Config::SharedPtr subgoalgen_config_;
        SubgoalGenerator::Generator::UniquePtr subgoal_generator_;

        Panel::UniquePtr serverPanel_;

        bool panel_is_running_{false};

    }; // class MultibotServer
} // namespace multibot2_server