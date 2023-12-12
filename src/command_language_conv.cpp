#include "tesseract_robotraconteur/conv/command_language_conv.h"
#include "tesseract_robotraconteur/conv/common_conv.h"
#include "RobotRaconteurCompanion/Converters/EigenConverters.h"


#include <boost/range/algorithm.hpp>

namespace RR=RobotRaconteur;
namespace RRC_Eigen=RobotRaconteur::Companion::Converters::Eigen;

#define RR_COMMAND_LANG_PREFIX "experimental.tesseract_robotics.command_language"

namespace tesseract_robotraconteur
{
namespace conv
{

rr_command::JointWaypointPtr JointWaypointToRR(const tesseract_planning::JointWaypointPoly& joint_waypoint)
{
    rr_command::JointWaypointPtr ret(new rr_command::JointWaypoint());
    ret->names = RR::stringVectorToRRList(joint_waypoint.getNames());
    ret->position = RRC_Eigen::EigenToRRArray<double>(joint_waypoint.getPosition());
    ret->upper_tolerance = RRC_Eigen::EigenToRRArray<double>(joint_waypoint.getUpperTolerance());
    ret->lower_tolerance = RRC_Eigen::EigenToRRArray<double>(joint_waypoint.getLowerTolerance());
    ret->is_constrained = joint_waypoint.isConstrained();
    ret->name = joint_waypoint.getName();
    return ret;
}

tesseract_planning::JointWaypoint JointWaypointFromRR(const rr_command::JointWaypointPtr& joint_waypoint)
{
    RR_NULL_CHECK(joint_waypoint);
    tesseract_planning::JointWaypoint ret;
    ret.setNames(RR::RRListToStringVector(joint_waypoint->names));
    ret.setPosition(RRC_Eigen::RRArrayToEigen<Eigen::VectorXd>(joint_waypoint->position));
    ret.setUpperTolerance(RRC_Eigen::RRArrayToEigen<Eigen::VectorXd>(joint_waypoint->upper_tolerance));
    ret.setLowerTolerance(RRC_Eigen::RRArrayToEigen<Eigen::VectorXd>(joint_waypoint->lower_tolerance));
    ret.setIsConstrained(joint_waypoint->is_constrained.value != 0);
    ret.setName(joint_waypoint->name);
    return ret;
}

rr_command::CartesianWaypointPtr CartesianWaypointToRR(const tesseract_planning::CartesianWaypointPoly& cartesian_waypoint)
{
    rr_command::CartesianWaypointPtr ret(new rr_command::CartesianWaypoint());
    ret->transform = RRC_Eigen::ToTransform(cartesian_waypoint.getTransform());
    ret->upper_tolerance = RRC_Eigen::EigenToRRArray<double>(cartesian_waypoint.getUpperTolerance());
    ret->lower_tolerance = RRC_Eigen::EigenToRRArray<double>(cartesian_waypoint.getLowerTolerance());
    ret->seed = JointStateToRR(cartesian_waypoint.getSeed());
    ret->name = cartesian_waypoint.getName();
    return ret;
}

tesseract_planning::CartesianWaypoint CartesianWaypointFromRR(const rr_command::CartesianWaypointPtr& cartesian_waypoint)
{
    RR_NULL_CHECK(cartesian_waypoint);
    tesseract_planning::CartesianWaypoint ret;
    ret.setTransform(RRC_Eigen::ToIsometry(cartesian_waypoint->transform));
    ret.setUpperTolerance(RRC_Eigen::RRArrayToEigen<Eigen::VectorXd>(cartesian_waypoint->upper_tolerance));
    ret.setLowerTolerance(RRC_Eigen::RRArrayToEigen<Eigen::VectorXd>(cartesian_waypoint->lower_tolerance));
    if (cartesian_waypoint->seed)
        ret.setSeed(JointStateFromRR(cartesian_waypoint->seed));
    ret.setName(cartesian_waypoint->name);
    return ret;
}

rr_command::StateWaypointPtr StateWaypointToRR(const tesseract_planning::StateWaypointPoly& state_waypoint)
{
    rr_command::StateWaypointPtr ret(new rr_command::StateWaypoint());
    ret->joint_names = RR::stringVectorToRRList(state_waypoint.getNames());
    ret->position = RRC_Eigen::EigenToRRArray<double>(state_waypoint.getPosition());
    ret->velocity = RRC_Eigen::EigenToRRArray<double>(state_waypoint.getVelocity());
    ret->acceleration = RRC_Eigen::EigenToRRArray<double>(state_waypoint.getAcceleration());
    ret->effort = RRC_Eigen::EigenToRRArray<double>(state_waypoint.getEffort());
    ret->time = state_waypoint.getTime();
    ret->name = state_waypoint.getName();
    return ret;
}

tesseract_planning::StateWaypoint StateWaypointFromRR(const rr_command::StateWaypointPtr& state_waypoint)
{
    RR_NULL_CHECK(state_waypoint);
    tesseract_planning::StateWaypoint ret;
    ret.setNames(RR::RRListToStringVector(state_waypoint->joint_names));
    ret.setPosition(RRC_Eigen::RRArrayToEigen<Eigen::VectorXd>(state_waypoint->position));
    ret.setVelocity(RRC_Eigen::RRArrayToEigen<Eigen::VectorXd>(state_waypoint->velocity));
    ret.setAcceleration(RRC_Eigen::RRArrayToEigen<Eigen::VectorXd>(state_waypoint->acceleration));
    ret.setEffort(RRC_Eigen::RRArrayToEigen<Eigen::VectorXd>(state_waypoint->effort));
    ret.setName(state_waypoint->name);
    ret.setTime(state_waypoint->time);
    return ret;
}

// TODO: Move into companion library
static com::robotraconteur::uuid::UUID UuidToRR(const boost::uuids::uuid& uuid)
{
    com::robotraconteur::uuid::UUID ret;
    memcpy(ret.s.uuid_bytes.data(), &uuid, 16);
    return ret;
}

static boost::uuids::uuid UuidFromRR(const com::robotraconteur::uuid::UUID& uuid)
{
    boost::uuids::uuid ret;
    memcpy(&ret, uuid.s.uuid_bytes.data(), 16);
    return ret;
}

rr_command::MoveInstructionPtr MoveInstructionToRR(const tesseract_planning::MoveInstructionPoly& move_instruction)
{
    rr_command::MoveInstructionPtr ret(new rr_command::MoveInstruction());
    ret->uuid = UuidToRR(move_instruction.getUUID());
    ret->parent_uuid = UuidToRR(move_instruction.getParentUUID());
    ret->waypoint = WaypointPolyToRR(move_instruction.getWaypoint());
    ret->manipulator_info = ManipulatorInfoToRR(move_instruction.getManipulatorInfo());
    ret->profile = move_instruction.getProfile();
    
    // TODO: overrides
    // ret->path_profile = ...
    // ret->profile_overrides = ...

    ret->move_type = (rr_command::MoveInstructionType::MoveInstructionType)move_instruction.getMoveType();
    ret->description = move_instruction.getDescription();
    
    return ret;
}

tesseract_planning::MoveInstructionPoly MoveInstructionFromRR(const rr_command::MoveInstructionPtr& move_instruction)
{
    RR_NULL_CHECK(move_instruction);
    tesseract_planning::MoveInstruction ret;
    ret.setUUID(UuidFromRR(move_instruction->uuid));
    ret.setParentUUID(UuidFromRR(move_instruction->parent_uuid));
    RR_NULL_CHECK(move_instruction->waypoint);
    if (move_instruction->waypoint->RRType() == (RR_COMMAND_LANG_PREFIX "." "JointWaypoint"))
    {
        ret.assignJointWaypoint(JointWaypointFromRR(RR::rr_cast<rr_command::JointWaypoint>(move_instruction->waypoint)));
    }
    else if (move_instruction->waypoint->RRType() == (RR_COMMAND_LANG_PREFIX "." "CartesianWaypoint"))
    {
        ret.assignCartesianWaypoint(CartesianWaypointFromRR(RR::rr_cast<rr_command::CartesianWaypoint>(move_instruction->waypoint)));
    }
    else if (move_instruction->waypoint->RRType() == (RR_COMMAND_LANG_PREFIX "." "StateWaypoint"))
    {
        ret.assignStateWaypoint(StateWaypointFromRR(RR::rr_cast<rr_command::StateWaypoint>(move_instruction->waypoint)));
    }
    else
    {
        throw RR::InvalidArgumentException("Unknown waypoint type");
    }
    if (move_instruction->manipulator_info)        
        ret.setManipulatorInfo(ManipulatorInfoFromRR(move_instruction->manipulator_info));
    ret.setProfile(move_instruction->profile);

    // TODO: overrides
    // ret.setPathProfile(...
    // ret.setProfileOverrides(...

    ret.setMoveType((tesseract_planning::MoveInstructionType)move_instruction->move_type);
    ret.setDescription(move_instruction->description);
    return ret;
}

rr_command::CompositeInstructionPtr CompositeInstructionToRR(const tesseract_planning::CompositeInstruction& composite_instruction)
{
    rr_command::CompositeInstructionPtr ret(new rr_command::CompositeInstruction());
    ret->order = (rr_command::CompositeInstructionOrder::CompositeInstructionOrder)composite_instruction.getOrder();
    ret->uuid = UuidToRR(composite_instruction.getUUID());
    ret->parent_uuid = UuidToRR(composite_instruction.getParentUUID());
    ret->description = composite_instruction.getDescription();
    ret->profile = composite_instruction.getProfile();

    // TODO: overrides
    // ret->profile_overrides = ...
    ret->manipulator_info = ManipulatorInfoToRR(composite_instruction.getManipulatorInfo());
    auto& instructions = composite_instruction.getInstructions();
    ret->instructions = RR::AllocateEmptyRRList<RR::RRValue>();
    boost::range::transform(instructions, std::back_inserter(*ret->instructions), InstructionPolyToRR);
    return ret;
}

tesseract_planning::CompositeInstruction CompositeInstructionFromRR(const rr_command::CompositeInstructionPtr& composite_instruction)
{
    RR_NULL_CHECK(composite_instruction);
    tesseract_planning::CompositeInstruction ret;

    // TODO: setOrder ?
    //ret.setOrder((tesseract_planning::CompositeInstructionOrder)composite_instruction->order);
    ret.setUUID(UuidFromRR(composite_instruction->uuid));
    ret.setParentUUID(UuidFromRR(composite_instruction->parent_uuid));
    ret.setDescription(composite_instruction->description);
    ret.setProfile(composite_instruction->profile);

    // TODO: overrides
    // ret.setProfileOverrides(...
    ret.setManipulatorInfo(ManipulatorInfoFromRR(composite_instruction->manipulator_info));
    {
    std::vector<tesseract_planning::InstructionPoly> instructions;
    RR_NULL_CHECK(composite_instruction->instructions);
    //boost::range::transform(*composite_instruction->instructions, std::back_inserter(instructions), InstructionPolyFromRR);
    for (auto& i : *composite_instruction->instructions)
    {
        tesseract_planning::InstructionPoly instr = InstructionPolyFromRR(i);
        instructions.push_back(instr);
    }
    ret.setInstructions(instructions);
    }
    // ret.print();
    return ret;
}

RR::RRValuePtr WaypointPolyToRR(const tesseract_planning::WaypointPoly& waypoint)
{
    if (waypoint.isJointWaypoint())
    {
        return JointWaypointToRR(waypoint.as<tesseract_planning::JointWaypointPoly>());
    }
    else if (waypoint.isCartesianWaypoint())
    {
        return CartesianWaypointToRR(waypoint.as<tesseract_planning::CartesianWaypointPoly>());
    }
    else if (waypoint.isStateWaypoint())
    {
        return StateWaypointToRR(waypoint.as<tesseract_planning::StateWaypointPoly>());
    }
    else
    {
        throw RR::InvalidArgumentException("Unknown waypoint type");
    }
}

RR::RRValuePtr InstructionPolyToRR(const tesseract_planning::InstructionPoly& instruction)
{
    if (instruction.isMoveInstruction())
    {
        return MoveInstructionToRR(instruction.as<tesseract_planning::MoveInstructionPoly>());
    }
    else if (instruction.isCompositeInstruction())
    {
        return CompositeInstructionToRR(instruction.as<tesseract_planning::CompositeInstruction>());
    }
    else
    {
        throw RR::InvalidArgumentException("Unknown instruction type");
    }
}

tesseract_planning::InstructionPoly InstructionPolyFromRR(const RR::RRValuePtr& instruction)
{
    RR_NULL_CHECK(instruction);
    if (instruction->RRType() == (RR_COMMAND_LANG_PREFIX "." "MoveInstruction"))
    {
        return MoveInstructionFromRR(RR::rr_cast<rr_command::MoveInstruction>(instruction));
    }
    else if (instruction->RRType() == (RR_COMMAND_LANG_PREFIX "." "CompositeInstruction"))
    {
        return CompositeInstructionFromRR(RR::rr_cast<rr_command::CompositeInstruction>(instruction));
    }
    else
    {
        throw RR::InvalidArgumentException("Unknown instruction type");
    }
}

tesseract_planning::WaypointPoly WaypointPolyFromRR(const RR::RRValuePtr& waypoint)
{
    RR_NULL_CHECK(waypoint);
    if (waypoint->RRType() == (RR_COMMAND_LANG_PREFIX "." "JointWaypoint"))
    {
        return JointWaypointFromRR(RR::rr_cast<rr_command::JointWaypoint>(waypoint));
    }
    else if (waypoint->RRType() == (RR_COMMAND_LANG_PREFIX "." "CartesianWaypoint"))
    {
        return CartesianWaypointFromRR(RR::rr_cast<rr_command::CartesianWaypoint>(waypoint));
    }
    else if (waypoint->RRType() == (RR_COMMAND_LANG_PREFIX "." "StateWaypoint"))
    {
        return StateWaypointFromRR(RR::rr_cast<rr_command::StateWaypoint>(waypoint));
    }
    else
    {
        throw RR::InvalidArgumentException("Unknown waypoint type");
    }
}

} // namespace conv
} // namespace tesseract_robotraconteur
