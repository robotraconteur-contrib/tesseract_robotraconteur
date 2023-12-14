/**
 * @file tesseract_robotics_impl.h
 *
 * @author John Wason, PhD
 *
 * @copyright Copyright 2023 Wason Technology, LLC
 *
 * @par License
 * Software License Agreement (Apache License)
 * @par
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * @par
 * http://www.apache.org/licenses/LICENSE-2.0
 * @par
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef TESSERACT_ROBOTRAOUNTEUR_TESSERACT_ROBOTICS_IMPL_H
#define TESSERACT_ROBOTRAOUNTEUR_TESSERACT_ROBOTICS_IMPL_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <RobotRaconteur.h>
#include <RobotRaconteurCompanion/StdRobDef/StdRobDefAll.h>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include "environment_impl.h"
#include "tasks_impl.h"

#include "robotraconteur_generated.h"

namespace rr_tesseract = experimental::tesseract_robotics;

namespace tesseract_robotraconteur
{
    class TaskExecutorImpl;
    class EnvironmentImpl;
    class TesseractRoboticsImpl : public virtual rr_tesseract::TesseractRobotics_default_impl, public RR_ENABLE_SHARED_FROM_THIS<TesseractRoboticsImpl>
    {
    public:
        TesseractRoboticsImpl(tesseract_environment::Environment::Ptr default_env, tesseract_planning::TaskComposerExecutor::Ptr default_executor,
            std::shared_ptr<tesseract_planning::TaskComposerPluginFactory> executor_factory);

        void Init();

        com::robotraconteur::device::DeviceInfoPtr get_device_info() override;
        RobotRaconteur::RRMapPtr<std::string,rr_env::EnvironmentInfo  > get_environments_info() override;
        RobotRaconteur::RRMapPtr<std::string,rr_tasks::TaskPipelineInfo  > get_task_pipelines_info() override;
        RobotRaconteur::RRMapPtr<std::string,rr_tasks::TaskExecutorInfo  >  get_task_executors_info() override;
        std::string load_environment(const std::string& environment_resource_name, const std::string& new_environment_name) override;

        std::string clone_environment(const std::string& environment_name, const std::string& new_environment_name) override;

        rr_env::EnvironmentPtr get_environments(const std::string& ind) override;

        rr_tasks::TaskExecutorPtr get_task_executors(const std::string& ind) override;

        boost::shared_ptr<EnvironmentImpl> GetEnvironmentImpl(const std::string &name);

        std::shared_ptr<tesseract_planning::TaskComposerPluginFactory> GetExecutorFactory();

    protected:
        tesseract_environment::Environment::Ptr default_env;
        tesseract_planning::TaskComposerExecutor::Ptr default_executor;
        std::shared_ptr<tesseract_planning::TaskComposerPluginFactory> executor_factory;
        std::map<std::string, RR_SHARED_PTR<EnvironmentImpl> > environments;
        std::map<std::string, RR_SHARED_PTR<TaskExecutorImpl> > executors;
        boost::mutex this_lock;
    };

} // namespace tesseract_robotraconteur

#endif // TESSERACT_ROBOTRAOUNTEUR_TESSERACT_ROBOTICS_IMPL_H
