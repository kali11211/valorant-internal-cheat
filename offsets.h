#pragma once
namespace Offsets
{
	constexpr auto
        State = 0x8C38CC0, //uworld_state
        Key = State + 0x38,//stade + key
        relative_rotation = 0x170,
        ProcessEvent = 0x3366f10,
        StaticFindObject = 0x337b4d0,
        gobj_state = 0x8ABAA80,
		gobj_key = gobj_state + 0x38,
        Pointer = 0x80,
        SkinChanger = 0x23A1460,
        StaticLoadObject = 0x337bf80,
        TriggerVEH = 0x1a89ed0,//pakman offset
        bone_matrix = 0x4a70a70,
        PlayerCameraManager = 0x478,//aPlayerCameraManager 
        ChamsOffset = 0x1190,
        portrait_map = 0x1120, //UCharacterPortraitMinimapComponent_C
        character_map = portrait_map + 0x8, // UCharacterCharacterMinimapComponent_C
        Inventory = 0x950,  //struct UAresInventory* Inventory; 
        get_spread_values_fn = 0x237fb30,//Needs new offset
        get_spread_angles_fn = 0x2a4fed0,//Needs new offset
        get_firing_location_and_direction_fn = 0x288d9a0,//Needs new offset
        to_vector_and_normalize_fn = 0x309f290,//Needs new offset
        to_angle_and_normalize_fn = 0x3098ff0,//Needs new offset
		FiringStateComp = 0x1010,
		StabilityComponent = 0x418,
        CurrentEquippable = 0x238;  //struct AAresEquippable* CurrentEquippable;
}
