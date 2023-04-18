#include "windows.h"
#include <stdint.h>
#include <iostream>
#include <Winternl.h>
#include "Canvas.h"
#include "decryption.h"
#include "hook.h"
#include "Decryptor.h"
#include <stdlib.h>
#include <stdio.h>
#pragma comment(lib, "user32.lib")

using PostRenderHook = void(*)(uintptr_t _this, UCanvas* canvas);
PostRenderHook pRender = 0;

uintptr_t UWorldXOR;
AShooterCharacter* actor;
UWorld* UWorldClass;
UGameInstance* GameInstance;
ULocalPlayer* LocalPlayers;
ULocalPlayer* LocalPlayer;
APlayerController* MyControllers;

using namespace std;
float DegreeToRadian(float degrees) {
	return degrees * (PI / 180);
}

void DrawTextRGB(UCanvas* canvas, const wchar_t* text, float x, float y, FLinearColor color, bool CenterX = 0)
{
	canvas->K2_DrawText(text, { x, y }, { 1.1, 1.1 }, color, 0.f, { 0, 0, 0, 1 }, { 0, 0 }, CenterX, 0, bOutline, { 0, 0, 0, 1 });
}
void DrawTextRGBWithFString(UCanvas* canvas, FString text, float x, float y, FLinearColor color, bool CenterX = 0)
{
	canvas->K2_DrawText(text, { x, y }, { 1.1, 1.1 }, color, 0.f, { 0, 0, 0, 1 }, { 0, 0 }, CenterX, 0, bOutline, { 0, 0, 0, 1 });
}

void xxDrawTextRGB(UCanvas* canvas, FString text, float x, float y, FLinearColor color)
{
	canvas->K2_DrawText(text, { x, y }, { 1.1, 1.1 }, color, 0.f, { 0, 0, 0, 1 }, { 0, 0 }, 0, 0, false, { 0, 0, 0, 1 });
}
FLinearColor RGBtoFLC(float R, float G, float B)
{
	return { R / 255, G / 255, B / 255, 1 };
}

bool kekIsOff = 0; // debugging purposes ONLY solely for THAT

static FLinearColor Invisible{ 2.020f,0.050f,16.0f,5.9f };//Chams//Blue
static FLinearColor Visible{ 2.020f,0.050f,16.0f,5.9f };//Chams//Blue

static FLinearColor VisibleBox_ESPColor{ 0.0f,255.0f,0.0f,1.0f };//BoxVisible
static FLinearColor Invisible_ESPColor{ 255.0f,0.0f,0.0f,1.0f };//BoxInvisible

static FLinearColor VisibleSkeletonColor{ 0.0f,255.0f,0.0f,1.0f };//VisibleSkeletonColor
static FLinearColor InvisibleSkeletonColor{ 2.020f,0.050f,16.0f,5.9f };//VisibleSkeletonColor

static FLinearColor InvisibleSnapColor{ 2.020f,0.050f,16.0f,5.9f };
static FLinearColor VisibleSnapColor{ 0.0f,255.0f,0.0f,1.0f };

static FLinearColor Name_Color{ 2.020f,0.050f,16.0f,5.9f };
static FLinearColor healthcolors = RGBtoFLC(0, 255, 0);
static FLinearColor circlecol = { 255.0f, 255.0f, 255.0f, 1.0f };
static FLinearColor fovcolor = { 255.0f, 255.0f, 255.0f, 1.0f };
static FLinearColor crosscolor = { 255.0f, 255.0f, 255.0f, 1.0f };

static bool menu_opened = true;
static bool menu = true;

FVector2D pos = { ((float)GetSystemMetrics(SM_CXSCREEN) / 2) - 500, ((float)GetSystemMetrics(SM_CYSCREEN) / 2) - 475 };
FVector2D poss = { ((float)GetSystemMetrics(SM_CXSCREEN) / 2) - 200, ((float)GetSystemMetrics(SM_CYSCREEN) / 2) - 175 };

static bool autoaim = false;
static bool ChamsESP = false;
static bool skid_spinbot = false;
static bool Wireframe = false;
static bool WireframeGun = false;
static bool SpinBot = false;
static bool box2d = false;
static bool SpinBotBool = false;
static bool chammyteam = false;
static bool fovchanger = false;
static bool autoshoot = false;
static bool AutoFire = false;
static bool HeadCircleEsp = false;
static bool vischeckmenu = false;
static bool AgentName = false;

//WireFrame*
static bool WireFrameHand = false;


//Misc
static bool FlyHack = false;
static float PlayerDistange = 150;
static bool drawfov = false;
static bool HealthEsp = false;
static bool skiptutorial = false;

int SnapPositionint = 2;
int SnapPosition›nt = 0;
int SnapMesh = 0;

static bool Minimap = false;
static bool weaponesp = false;
static bool RageBot = false;
static bool Watermark = true;
static bool drawcross = false;
static bool ThirdPerson = false;
static bool Visible_Check_ESP = false;
static bool skeleton = false;
static bool snaplinenz = false;
static bool enableaim = false;
static bool Silent = false;

//Misc
static float radius = 4.0f;
static float radius2 = 7.0f;
static float fovchangur = 105.0f;

//+++++++++++++++++++
 
//Skins
static bool EnableKnife = false;
static bool EnableGhost = false;
static bool EnableVandal = false;
static bool EnableOperator = false;
static bool EnableClassic = false;
static bool EnableSpectre = false;
static bool EnablePhantom = false;
static bool EnableSheriff = false;

int skinvandal = 0;
int skinclassic = 0;
int skinsheriff = 0;
int skinphantom = 0;
int skinspectre = 0;
int skinknife = 0;
int skinghost = 0;
int skinoperator = 0;

//++++++++++++++++++++

int KEKWNOKEY;
int BhopKey = VK_SPACE;
int RageBotKeybind;
int spinbotkeybind;
int aimbotmode = 0;
int aimbone = 0;
bool healthbar = false;

int Width = GetSystemMetrics(SM_CXSCREEN);
int Height = GetSystemMetrics(SM_CYSCREEN);

float ScreenCenterX = Width / 2;
float ScreenCenterY = Height / 2;


float fovChanger = 30.0f;//Fov Changer
float LineamountCross = 35.0f;
float LineamountCircle = 25.0f;
float LineamountFov = 35.0f;
float chamsglowvalue = 1;
float ESPThickness = 1;

bool Mesh3PModifed = false;
bool Below = false;
bool Middle = false;
bool Bottom = false;


//Configs
bool LegitCFG = false;
bool RageCFG = false;

struct FMinimalViewInfo {
	FVector Location;
	FVector Rotation;
	float FOV;
};
struct Bone {
	BYTE thing[0xCC];
	float x;
	BYTE thing2[0xC];
	float y;
	BYTE thing3[0xC];
	float z;
};
boolean in_rect(double centerX, double centerY, double radius, double x, double y) {
	return x >= centerX - radius && x <= centerX + radius &&
		y >= centerY - radius && y <= centerY + radius;
}
uintptr_t OffsetGetActorBounds;
FVector GetBoneMatrix(void* Mesh, int Idx) {
	FMatrix Matrix;
	reinterpret_cast<FMatrix* (__fastcall*)(void*, FMatrix*, int)>((uintptr_t)VALORANT::Module + Offsets::bone_matrix)(Mesh, &Matrix, Idx); // E8 ?? ?? ?? ?? 48 8B 47 30 F3 0F 10 45 ??
	return  { Matrix.WPlane.X, Matrix.WPlane.Y, Matrix.WPlane.Z };
}
FVector FindSkeleton(APlayerController* controller, void* mesh, int i)
{
	FVector2D drawtextat;
	return controller->ProjectWorldLocationToScreen(GetBoneMatrix(mesh, i), drawtextat, 0);
}

FVector get_error_angle(uint64_t actor, uint64_t firing_state_component) {
	static auto get_spread_values_fn = (float* (__fastcall*)(uint64_t, float*))(BaseAddress + Offsets::get_spread_values_fn);
	static auto get_spread_angles_fn = (void(__fastcall*)(uint64_t, FVector*, float, float, int, int, uint64_t))(BaseAddress + Offsets::get_spread_angles_fn);
	static auto get_firing_location_and_direction_fn = (void(__fastcall*)(uint64_t, FVector*, FVector*))(BaseAddress + Offsets::get_firing_location_and_direction_fn);
	static auto to_vector_and_normalize_fn = (FVector * (__fastcall*)(FVector*, FVector*))(BaseAddress + Offsets::to_vector_and_normalize_fn);
	static auto to_angle_and_normalize_fn = (FVector * (__fastcall*)(FVector*, FVector*))(BaseAddress + Offsets::to_vector_and_normalize_fn);
	static uint8_t error_values[4096] = { 0 };
	static uint8_t seed_data_snapshot[4096] = { 0 };
	static uint8_t spread_angles[4096] = { 0 };
	static uint8_t out_spread_angles[4096] = { 0 };

	if (!actor || !firing_state_component)
		return FVector(0, 0, 0);

	memset(error_values, 0, sizeof(error_values));
	memset(seed_data_snapshot, 0, sizeof(seed_data_snapshot));
	memset(spread_angles, 0, sizeof(spread_angles));
	memset(out_spread_angles, 0, sizeof(out_spread_angles));

	*(uint64_t*)(&out_spread_angles[0]) = (uint64_t)&spread_angles[0];
	*(int*)(&out_spread_angles[0] + 8) = 1;
	*(int*)(&out_spread_angles[0] + 12) = 1;

	uint64_t seed_data = Memory::ReadStub<uint64_t>(firing_state_component + 0x1A0);
	memcpy((void*)seed_data_snapshot, (void*)seed_data, sizeof(seed_data_snapshot)); //Make our own copy since we don't want to desync our own seed component

	uint64_t stability_component = Memory::ReadStub<uint64_t>(firing_state_component + 0x418);
	if (stability_component)
		get_spread_values_fn(stability_component, (float*)&error_values[0]);

	FVector Temp1, Temp2 = FVector(0, 0, 0);
	FVector PreviousFiringDirection, firingdirection = FVector(0, 0, 0);
	get_firing_location_and_direction_fn(actor, &Temp1, &PreviousFiringDirection);
	to_vector_and_normalize_fn(&PreviousFiringDirection, &Temp2);
	to_angle_and_normalize_fn(&Temp2, &Temp1);
	PreviousFiringDirection = Temp1;
	Temp1.X += *(float*)(&error_values[0] + 12); //Your recoil angle
	Temp2.Y += *(float*)(&error_values[0] + 16);
	to_vector_and_normalize_fn(&Temp1, &firingdirection);
	float error_degrees = *(float*)(&error_values[0] + 8) + *(float*)(&error_values[0] + 4);
	float error_power = *(float*)(firing_state_component + 0x3F8);
	int error_retries = *(int*)(firing_state_component + 0x3FC);
	get_spread_angles_fn(((uint64_t)&seed_data_snapshot[0]) + 0xE8, &firingdirection, error_degrees, error_power, error_retries, 1, (uint64_t)&out_spread_angles[0]);

	FVector spread_vector = *(FVector*)(&spread_angles[0]);
	to_angle_and_normalize_fn(&spread_vector, &firingdirection);

	return firingdirection - PreviousFiringDirection; //Get the difference. Now subtract it against your aimbot!
}

void DrawSkeletonBot(APlayerController* controller, UCanvas* Canvas, void* mesh, FLinearColor cColor, float kalinlik = ESPThickness)
{

	FVector vHipOut = FindSkeleton(controller, mesh, 3);

	FVector vHeadBoneOut = FindSkeleton(controller, mesh, 8);
	FVector vNeckOut = FindSkeleton(controller, mesh, 7);

	FVector vUpperArmLeftOut = FindSkeleton(controller, mesh, 11);
	FVector vLeftHandOut = FindSkeleton(controller, mesh, 12);
	FVector vLeftHandOut1 = FindSkeleton(controller, mesh, 13);

	FVector vUpperArmRightOut = FindSkeleton(controller, mesh, 36);
	FVector vRightHandOut = FindSkeleton(controller, mesh, 37);
	FVector vRightHandOut1 = FindSkeleton(controller, mesh, 38);



	FVector vLeftThighOut = FindSkeleton(controller, mesh, 63);
	FVector vLeftCalfOut = FindSkeleton(controller, mesh, 65);
	FVector vLeftFootOut = FindSkeleton(controller, mesh, 69);

	FVector vRightThighOut = FindSkeleton(controller, mesh, 77);
	FVector vRightCalfOut = FindSkeleton(controller, mesh, 79);
	FVector vRightFootOut = FindSkeleton(controller, mesh, 83);
	{

		Canvas->K2_DrawLine({ vNeckOut.X, vNeckOut.Y }, { vHeadBoneOut.X, vHeadBoneOut.Y }, kalinlik, cColor);
		Canvas->K2_DrawLine({ vHipOut.X, vHipOut.Y }, { vNeckOut.X, vNeckOut.Y }, kalinlik, cColor);
		Canvas->K2_DrawLine({ vUpperArmLeftOut.X, vUpperArmLeftOut.Y }, { vNeckOut.X, vNeckOut.Y }, kalinlik, cColor);
		Canvas->K2_DrawLine({ vUpperArmRightOut.X, vUpperArmRightOut.Y }, { vNeckOut.X, vNeckOut.Y }, kalinlik, cColor);
		Canvas->K2_DrawLine({ vLeftHandOut.X, vLeftHandOut.Y }, { vUpperArmLeftOut.X, vUpperArmLeftOut.Y }, kalinlik, cColor);
		Canvas->K2_DrawLine({ vRightHandOut.X, vRightHandOut.Y }, { vUpperArmRightOut.X, vUpperArmRightOut.Y }, kalinlik, cColor);
		Canvas->K2_DrawLine({ vLeftHandOut.X, vLeftHandOut.Y }, { vLeftHandOut1.X, vLeftHandOut1.Y }, kalinlik, cColor);
		Canvas->K2_DrawLine({ vRightHandOut.X, vRightHandOut.Y }, { vRightHandOut1.X, vRightHandOut1.Y }, kalinlik, cColor);
		Canvas->K2_DrawLine({ vLeftThighOut.X, vLeftThighOut.Y }, { vHipOut.X, vHipOut.Y }, kalinlik, cColor);
		Canvas->K2_DrawLine({ vRightThighOut.X, vRightThighOut.Y }, { vHipOut.X, vHipOut.Y }, kalinlik, cColor);
		Canvas->K2_DrawLine({ vLeftCalfOut.X, vLeftCalfOut.Y }, { vLeftThighOut.X, vLeftThighOut.Y }, kalinlik, cColor);
		Canvas->K2_DrawLine({ vRightCalfOut.X, vRightCalfOut.Y }, { vRightThighOut.X, vRightThighOut.Y }, kalinlik, cColor);
		Canvas->K2_DrawLine({ vLeftFootOut.X, vLeftFootOut.Y }, { vLeftCalfOut.X, vLeftCalfOut.Y }, kalinlik, cColor);
		Canvas->K2_DrawLine({ vRightFootOut.X, vRightFootOut.Y }, { vRightCalfOut.X, vRightCalfOut.Y }, kalinlik, cColor);
	}

}
FString character_name(FString in)
{
	if (in.ToString().find("Training") != std::string::npos)
		return FString(L"NPC");
	if (in.ToString().find("BountyHunter_PC_C") != std::string::npos)
		return FString(L"Fade");
	if (in.ToString().find("Stealth_PC_C") != std::string::npos)
		return FString(L"Yoru");
	if (in.ToString().find("Pandemic_PC_C") != std::string::npos)
		return FString(L"Viper");
	if (in.ToString().find("Hunter_PC_C") != std::string::npos)
		return FString(L"Sova");
	if (in.ToString().find("Guide_PC_C") != std::string::npos)
		return FString(L"Skye");
	if (in.ToString().find("Thorne_PC_C") != std::string::npos)
		return FString(L"Sage");
	if (in.ToString().find("Vampire_PC_C") != std::string::npos)
		return FString(L"Reyna");
	if (in.ToString().find("Clay_PC_C") != std::string::npos)
		return FString(L"Raze");
	if (in.ToString().find("Phoenix_PC_C") != std::string::npos)
		return FString(L"Phoenix");
	if (in.ToString().find("Wraith_PC_C") != std::string::npos)
		return FString(L"Omen");
	if (in.ToString().find("Sprinter_PC_C") != std::string::npos)
		return FString(L"Neon");
	if (in.ToString().find("Killjoy_PC_C") != std::string::npos)
		return FString(L"Killjoy");
	if (in.ToString().find("Grenadier_PC_C") != std::string::npos)
		return FString(L"Kayo");
	if (in.ToString().find("Wushu_PC_C") != std::string::npos)
		return FString(L"Jett");
	if (in.ToString().find("Gumshoe_PC_C") != std::string::npos)
		return FString(L"Cypher");
	if (in.ToString().find("Deadeye_PC_C") != std::string::npos)
		return FString(L"Chamber");
	if (in.ToString().find("Sarge_PC_C") != std::string::npos)
		return FString(L"Brimstone");
	if (in.ToString().find("Breach_PC_C") != std::string::npos)
		return FString(L"Breach");
	if (in.ToString().find("Rift_TargetingForm_PC_C") != std::string::npos)
		return FString(L"Astra");
	if (in.ToString().find("Rift_PC_C") != std::string::npos)
		return FString(L"Astra");
	if (in.ToString().find("Mage_PC_C") != std::string::npos)
		return FString(L"Harbor");
	else
		return FString(L"N/A");
}
FString weapon_name(FString in)
{
	if (in.ToString().find("Ability_Melee_Base_C") != std::string::npos)
		return FString(L"Knife");
	if (in.ToString().find("BasePistol_C") != std::string::npos)
		return FString(L"Classic");
	if (in.ToString().find("TrainingBotBasePistol_C") != std::string::npos)
		return FString(L"Classic");
	if (in.ToString().find("SawedOffShotgun_C") != std::string::npos)
		return FString(L"Shorty");
	if (in.ToString().find("AutomaticPistol_C") != std::string::npos)
		return FString(L"Frenzy");
	if (in.ToString().find("LugerPistol_C") != std::string::npos)
		return FString(L"Ghost");
	if (in.ToString().find("RevolverPistol_C") != std::string::npos)
		return FString(L"Sheriff");
	if (in.ToString().find("Vector_C") != std::string::npos)
		return FString(L"Stinger");
	if (in.ToString().find("SubMachineGun_MP5_C") != std::string::npos)
		return FString(L"Spectre");
	if (in.ToString().find("PumpShotgun_C") != std::string::npos)
		return FString(L"Bucky");
	if (in.ToString().find("AssaultRifle_Burst_C") != std::string::npos)
		return FString(L"Bulldog");
	if (in.ToString().find("DMR_C") != std::string::npos)
		return FString(L"Guardian");
	if (in.ToString().find("AssaultRifle_ACR_C") != std::string::npos)
		return FString(L"Phantom");
	if (in.ToString().find("AssaultRifle_AK_C") != std::string::npos)
		return FString(L"Vandal");
	if (in.ToString().find("LeverSniperRifle_C") != std::string::npos)
		return FString(L"Marshall");
	if (in.ToString().find("BoltSniper_C") != std::string::npos)
		return FString(L"Operator");
	if (in.ToString().find("LightMachineGun_C") != std::string::npos)
		return FString(L"Ares");
	if (in.ToString().find("HeavyMachineGun_C") != std::string::npos)
		return FString(L"Odin");
	if (in.ToString().find("Bomb_C") != std::string::npos)
		return FString(L"Spike");
	if (in.ToString().find("Pawn_Gumshoe_Q_PossessableCamera_C") != std::string::npos)
		return FString(L"Cyper Camera");
	if (in.ToString().find("Pawn_Hunter_E_Drone_Prototype_Balance_C") != std::string::npos)
		return FString(L"Sova Drone");
	else
		return FString(L"N/A");
}

/*
PlantedSpike //TimedBomb_C
DroppedItems //EquippableGroundPickup_C
SageAllWall //GameObject_Thorne_E_Wall_Fortifying_C
SageWallSegments //GameObject_Thorne_E_Wall_Segment_Fortifying_C
OwlDrone //Pawn_Hunter_E_Drone_Prototype_Balance_C
Cypher Trap Start //GameObject_Gumshoe_E_TripWire_C
Cypher Trap End //GameObject_Gumshoe_E_TripWire_SecondWire_C
Cypher Camera //Pawn_Gumshoe_Q_PossessableCamera_C
*/


void DrawBox(UCanvas* can, FVector2D& topleft, FVector2D& downright, FLinearColor clr)
{
	auto h = downright.Y - topleft.Y;
	auto w = downright.X - topleft.X;

	auto downleft = FVector2D{ topleft.X , downright.Y };
	auto topright = FVector2D{ downright.X, topleft.Y };

	auto thicc = ESPThickness;

	FLinearColor xclr = RGBtoFLC(0, 0, 0);

	can->K2_DrawLinex(topleft, { downright.X, topleft.Y }, thicc, clr);
	can->K2_DrawLinex(topleft, { topleft.X , downright.Y }, thicc, clr);
	can->K2_DrawLinex(downright, { topleft.X , downright.Y }, thicc, clr);
	can->K2_DrawLine(downright, { downright.X, topleft.Y }, thicc, clr);
}

static FVector2D scr[8];
void FMath::SinCos(float* ScalarSin, float* ScalarCos, float Value)
{
	// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
	float quotient = (INV_PI * 0.5f) * Value;
	if (Value >= 0.0f)
	{
		quotient = (float)((int)(quotient + 0.5f));
	}
	else
	{
		quotient = (float)((int)(quotient - 0.5f));
	}
	float y = Value - (2.0f * PI) * quotient;

	// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
	float sign;
	if (y > HALF_PI)
	{
		y = PI - y;
		sign = -1.0f;
	}
	else if (y < -HALF_PI)
	{
		y = -PI - y;
		sign = -1.0f;
	}
	else
	{
		sign = +1.0f;
	}

	float y2 = y * y;

	// 11-degree minimax approximation
	*ScalarSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

	// 10-degree minimax approximation
	float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
	*ScalarCos = sign * p;
}
float FMath::Fmod(float X, float Y)
{
	const float AbsY = fabsf(Y);
	if (AbsY <= 1.e-8f) { return 0.f; }
	const float Div = (X / Y);
	// All floats where abs(f) >= 2^23 (8388608) are whole numbers so do not need truncation, and avoid overflow in TruncToFloat as they get even larger.
	const float Quotient = fabsf(Div) < FLOAT_NON_FRACTIONAL ? truncf(Div) : Div;
	float IntPortion = Y * Quotient;

	// Rounding and imprecision could cause IntPortion to exceed X and cause the result to be outside the expected range.
	// For example Fmod(55.8, 9.3) would result in a very small negative value!
	if (fabsf(IntPortion) > fabsf(X)) { IntPortion = X; }

	const float Result = X - IntPortion;
	// Clamp to [-AbsY, AbsY] because of possible failures for very large numbers (>1e10) due to precision loss.
	// We could instead fall back to stock fmodf() for large values, however this would diverge from the SIMD VectorMod() which has no similar fallback with reasonable performance.
	return FMath::Clamp(Result, -AbsY, AbsY);
}
static bool bLockedCameraRotation = false;
uintptr_t camMan;
FVector	LocalCameraLocation; //FVector	LocalCameraLocation = Memory::ReadStub<uintptr_t>(camMan + 0x1260);
float LocalCameraFOV; //float LocalCameraFOV = Memory::ReadStub<float>(camMan + 0x1278);
FVector	LocalCameraRotation; //FVector	LocalCameraRotation = Memory::ReadStub<uintptr_t>(camMan + 0x126C)
static bool bFlickSilent = true;
void(*SetCameraCachePOVOriginal)(uintptr_t, FMinimalViewInfo*) = nullptr;
UAresOutlineComponent* Mesh;
FVector2D head_scren;
auto SetCameraCachePOVHook(uintptr_t PlayerCameraManager, FMinimalViewInfo* ViewInfo)
{
	FVector CameraPos = *(FVector*)(PlayerCameraManager + 0x1260);
	FVector CameraRot = *(FVector*)(PlayerCameraManager + 0x126C);
	
	float deltaX, deltaY;
	MyControllers->GetInputMouseDelta(deltaX, deltaY);
	float sensitivity = MyControllers->GetMouseSensitivity(); // ShooterGame.ShooterPlayerController.GetMouseSensitivity
	LocalCameraRotation.X += deltaY * sensitivity;
	LocalCameraRotation.Y += deltaX * sensitivity;
	MyControllers->K2_SetActorRelativeRotation(FRotator{ 0, LocalCameraRotation.Y, 0 }, false, true); // Engine.Actor.K2_SetActorRelativeRotation

	
	//FovandKey

	if (Silent)
	{
		// Cache camera values for our own purposes, in this case we are simply using the left button to decide whether the person is shooting or not.
		if (!GetAsyncKeyState(KEKWNOKEY)) //menu is alt
		{

			if (!GetAsyncKeyState(VK_LBUTTON))
			{
				// Cache wanted camera values.
				LocalCameraLocation = ViewInfo->Location, LocalCameraRotation = ViewInfo->Rotation;
				LocalCameraFOV = ViewInfo->FOV;

				bLockedCameraRotation = false;
			}

		}
		else
		{
			if (GetAsyncKeyState(VK_LBUTTON)  &&in_rect(ScreenCenterX, ScreenCenterY, fovChanger, head_scren.X, head_scren.Y))
				bLockedCameraRotation = true;



			// Lock camera
			if (bFlickSilent && LocalCameraRotation != FVector())
				ViewInfo->Rotation = LocalCameraRotation;

		}

		SetCameraCachePOVOriginal(PlayerCameraManager, ViewInfo);

	

	}/**/

	if (!skid_spinbot) {
		LocalCameraRotation = ViewInfo->Rotation;
	}
	else {
		ViewInfo->Rotation = LocalCameraRotation;
	}

	if (ThirdPerson) {
		float TPDistance = PlayerDistange; // Third Person Distance
		float FixZAngle = (-TPDistance - (sin(DegreeToRadian(-CameraRot.X)) * (TPDistance - (TPDistance / 3.5))));

		if (CameraRot.X < 0.0f)
			FixZAngle = -(TPDistance - (sin(DegreeToRadian(-CameraRot.X)) * (TPDistance - (TPDistance / 3.5))));

		FVector CamOff = FVector{ cos(DegreeToRadian(CameraRot.Y)) * FixZAngle, sin(DegreeToRadian(CameraRot.Y)) * FixZAngle, sin(DegreeToRadian(-CameraRot.X)) * TPDistance };


		ViewInfo->Location = CameraPos + CamOff;
	}

	SetCameraCachePOVOriginal(PlayerCameraManager, ViewInfo);
}


FQuat FRotator::Quaternion() const
{
	float SP, SY, SR;
	float CP, CY, CR;
	const float PitchNoWinding = FMath::Fmod(Pitch, 360.0f);
	const float YawNoWinding = FMath::Fmod(Yaw, 360.0f);
	const float RollNoWinding = FMath::Fmod(Roll, 360.0f);
	FMath::SinCos(&SP, &CP, PitchNoWinding * RADS_DIVIDED_BY_2);
	FMath::SinCos(&SY, &CY, YawNoWinding * RADS_DIVIDED_BY_2);
	FMath::SinCos(&SR, &CR, RollNoWinding * RADS_DIVIDED_BY_2);
	FQuat RotationQuat;
	RotationQuat.X = CR * SP * SY - SR * CP * CY;
	RotationQuat.Y = -CR * SP * CY - SR * CP * SY;
	RotationQuat.Z = CR * CP * SY - SR * SP * CY;
	RotationQuat.W = CR * CP * CY + SR * SP * SY;
	return RotationQuat;
}
const FVector FVector::ZeroVector(0.0f, 0.0f, 0.0f);
const FVector FVector::OneVector(1.0f, 1.0f, 1.0f);
FVector FTransform::TransformPosition(FVector& V) const
{
	return Rotation.RotateVector(Scale3D * V) + Translation;
}
FVector FQuat::RotateVector(const FVector& V) const
{
	const FVector Q(X, Y, Z);
	const FVector T = (Q ^ V) * 2.f;
	const FVector Result = V + (T * W) + (Q ^ T);
	return Result;
}

bool a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18;
void  Draw3DBox(UCanvas* _this, APlayerController* plc, FVector origin, FVector extends, FLinearColor col)
{
	origin -= extends / 2.f;

	// bottom plane
	FVector one = origin;
	FVector two = origin; two.X += extends.X;
	FVector three = origin; three.X += extends.X; three.Y += extends.Y;
	FVector four = origin; four.Y += extends.Y;

	FVector five = one; five.Z += extends.Z;
	FVector six = two; six.Z += extends.Z;
	FVector seven = three; seven.Z += extends.Z;
	FVector eight = four; eight.Z += extends.Z;



	if (plc->ProjectWorldLocationToScreen(one, scr[0], 0) || plc->ProjectWorldLocationToScreen(two, scr[1], 0) || plc->ProjectWorldLocationToScreen(three, scr[2], 0) || plc->ProjectWorldLocationToScreen(four, scr[3], 0))
	{
		if (plc->ProjectWorldLocationToScreen(five, scr[4], 0) || plc->ProjectWorldLocationToScreen(six, scr[5], 0) || plc->ProjectWorldLocationToScreen(seven, scr[6], 0) || plc->ProjectWorldLocationToScreen(eight, scr[7], 0))
		{
			_this->K2_DrawLine(scr[0], scr[1], ESPThickness, col);
			_this->K2_DrawLine(scr[1], scr[2], ESPThickness, col);
			_this->K2_DrawLine(scr[2], scr[3], ESPThickness, col);
			_this->K2_DrawLine(scr[3], scr[0], ESPThickness, col);

			_this->K2_DrawLine(scr[4], scr[5], ESPThickness, col);
			_this->K2_DrawLine(scr[5], scr[6], ESPThickness, col);
			_this->K2_DrawLine(scr[6], scr[7], ESPThickness, col);
			_this->K2_DrawLine(scr[7], scr[4], ESPThickness, col);

			_this->K2_DrawLine(scr[0], scr[4], ESPThickness, col);
			_this->K2_DrawLine(scr[1], scr[5], ESPThickness, col);
			_this->K2_DrawLine(scr[2], scr[6], ESPThickness, col);
			_this->K2_DrawLine(scr[3], scr[7], ESPThickness, col);
		}
	}

}
bool Render3DBox(APlayerController* controller, UCanvas* Engine, const FVector& origin, const FVector& extent, const FRotator& rotation, const FLinearColor color)
{
	FVector vertex[2][4];
	vertex[0][0] = { -extent.X, -extent.Y,  -extent.Z };
	vertex[0][1] = { extent.X, -extent.Y,  -extent.Z };
	vertex[0][2] = { extent.X, extent.Y,  -extent.Z };
	vertex[0][3] = { -extent.X, extent.Y, -extent.Z };

	vertex[1][0] = { -extent.X, -extent.Y, extent.Z };
	vertex[1][1] = { extent.X, -extent.Y, extent.Z };
	vertex[1][2] = { extent.X, extent.Y, extent.Z };
	vertex[1][3] = { -extent.X, extent.Y, extent.Z };

	FVector2D screen[2][4];
	FTransform const Transform(rotation);
	for (auto k = 0; k < 2; k++)
	{
		for (auto i = 0; i < 4; i++)
		{
			auto& vec = vertex[k][i];
			vec = Transform.TransformPosition(vec) + origin;
			if (!controller->ProjectWorldLocationToScreen(vec, screen[k][i], false)) return false;
		}

	}

	auto SurfaceScreen = reinterpret_cast<FVector2D(&)[2][4]>(screen);

	for (auto i = 0; i < 4; i++)
	{
		Engine->K2_DrawLine(SurfaceScreen[1][i], SurfaceScreen[2][(i + 1) % 2], ESPThickness, color);
		Engine->K2_DrawLine(SurfaceScreen[3][i], SurfaceScreen[4][(i + 1) % 2], ESPThickness, color);
		Engine->K2_DrawLine(SurfaceScreen[5][i], SurfaceScreen[6][(i + 1) % 2], ESPThickness, color);
	}

	return true;
}


UObject* skinlist()
{

	UObject* skin_data = nullptr;

	if (EnableVandal) {
		switch ((int)skinvandal)
		{
		case 1:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__AK_Ashen_v1_PrimaryAsset_C", false);//1
			break;
		case 2:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__AK_Soulstealer_Black_PrimaryAsset_C", false);//2
			break;
		case 3:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__AK_Afterglow_Blue_PrimaryAsset_C", false);//2
			break;
		case 4:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__AK_Champions_PrimaryAsset_C", false);//2
			break;
		case 5:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__AK_Dragon_Red_PrimaryAsset_C", false);//2
			break;
		case 6:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__AK_Oblivion2_PrimaryAsset_C", false);//2
			break;
		case 7:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__AK_Daedalus_v2_PrimaryAsset_C", false);//2
			break;
		case 8:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__AK_Antares_v2_PrimaryAsset_C", false);//2
			break;
		case 9:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__AK_Circle_PrimaryAsset_C", false);//2
			break;
		case 10:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__AK_DemonStone_PrimaryAsset_C", false);//2
			break;
		case 11:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__AK_SOL_PrimaryAsset_C", false);//2
			break;
		case 12:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__AK_Cyberpunk2_PrimaryAsset_C", false);//2
			break;
		}
	}

	if (EnablePhantom) {
		switch ((int)skinphantom)
		{
		case 1:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Carbine_Atlas_PrimaryAsset_C", false);//2
			break;
		case 2:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Carbine_Soulstealer2_PrimaryAsset_C", false);//2
			break;
		case 3:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Carbine_Afterglow2_PrimaryAsset_C", false);//2
			break;
		case 4:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__AssaultRifle_ACR_Edge_Blue_PrimaryAsset_C", false);//2
			break;
		case 5:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Carbine_King_PrimaryAsset_C", false);//2
			break;
		case 6:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Carbine_Cyberpunk2_PrimaryAsset_C", false);//2
			break;
		case 7:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Carbine_Daedalus_PrimaryAsset_C", false);//2
			break;
		case 8:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Carbine_SpecOps_PrimaryAsset_C", false);//2
			break;
		case 9:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Carbine_URF_PrimaryAsset_C", false);//2
			break;
		}
	}

	if (EnableSheriff) {
		switch ((int)skinsheriff)
		{
		case 1:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__RevolverPistol_Soulstealer_PrimaryAsset_C", false);//1
			break;
		case 2:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__RevolverPistol_Edge_PrimaryAsset_C", false);//2
			break;
		case 3:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Revolver_Protocol_PrimaryAsset_C", false);//2
			break;
		case 4:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Revolver_SOL_PrimaryAsset_C", false);//2
			break;
		case 5:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Revolver_Daedalus_PrimaryAsset_C", false);//2
			break;
		case 6:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Revolver_Magepunk2_PrimaryAsset_C", false);//2
			break;
		}
	}

	if (EnableKnife) {

		switch ((int)skinknife)
		{
		case 1:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Afterglow2_PrimaryAsset_C", false);//1
			break;
		case 2:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Cyberpunk2_PrimaryAsset_C", false);//2
			break;
		case 3:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Soulstealer2_PrimaryAsset_C", false);//2
			break;
		case 4:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Afterglow_PrimaryAsset_C", false);//2
			break;
		case 5:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Alien_PrimaryAsset_C", false);//2
			break;
		case 6:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Base_HypeBeast_PrimaryAsset_C", false);//2
			break;
		case 7:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Soulstealer_PrimaryAsset_C", false);//2
			break;
		case 8:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Champions_PrimaryAsset_C", false);//2
			break;
		case 9:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee__Dragon_PrimaryAsset_C", false);//2
			break;
		case 10:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Lunar_PrimaryAsset_C", false);//2
			break;
		case 11:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Antares_v2_PrimaryAsset_C", false);//2
			break;
		case 12:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Champions2_PrimaryAsset_C", false);//2
			break;
		case 13:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Haunted_PrimaryAsset_C", false);//2
			break;
		case 14:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Mythic_PrimaryAsset_C", false);//2
			break;
		case 15:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Oblivion2_PrimaryAsset_C", false);//2
			break;
		case 16:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Magepunk2_PrimaryAsset_C", false);//2
			break;

		}


	}

	if (EnableGhost) {

		switch ((int)skinghost)
		{
		case 1:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__LugerPistol_Sovereign_PrimaryAsset_C", false);//1
			break;
		case 2:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Luger_King_PrimaryAsset_C", false);//2
			break;
		case 3:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Luger_Magepunk_PrimaryAsset_C", false);//2
			break;
		case 4:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Luger_Soulstealer2_PrimaryAsset_C", false);//2
			break;
		case 5:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__LugerPistol_Ashen_PrimaryAsset_C", false);//2
			break;
		}


	}

	if (EnableOperator) {

		switch ((int)skinoperator)
		{
		case 1:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__BoltSniper_Circle_PrimaryAsset_C", false);//1
			break;
		case 2:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__BoltSniper_DemonStone_PrimaryAsset_C", false);//2
			break;
		case 3:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__BoltSniper_Fallen_PrimaryAsset_C", false);//2
			break;
		case 4:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__BoltSniper_Cyberpunk2_PrimaryAsset_C", false);//2
			break;
		case 5:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__BoltSniper_Afterglow2_PrimaryAsset_C", false);//2
			break;
		case 6:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__BoltSniper_Dragon_PrimaryAsset_C", false);//2
			break;
		case 7:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__BoltSniper_SOL_PrimaryAsset_C", false);//2
			break;
		case 8:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__BoltSniper_Soulstealer_PrimaryAsset_C", false);//2
			break;

		}


	}

	if (EnableClassic) {

		switch ((int)skinclassic)
		{
		case 1:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__BasePistol_HypeBeast_PrimaryAsset_C", false);//1
			break;
		case 2:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__BasePistol_Afterglow2_PrimaryAsset_C", false);//1
			break;
		case 3:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__BasePistol_Atlas_v1_PrimaryAsset_C", false);//1
			break;
		case 4:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__BasePistol_Cyberpunk2_PrimaryAsset_C", false);//1
			break;
		}
	}

	if (EnableSpectre) {

		switch ((int)skinspectre)
		{
		case 1:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__MP5_Afterglow2_PrimaryAsset_C", false);//1
			break;
		case 2:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__MP5_Soulstealer2_PrimaryAsset_C", false);//1
			break;
		case 3:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__MP5_Protocol_PrimaryAsset_C", false);//1
			break;
		case 4:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__MP5_King_PrimaryAsset_C", false);//1
			break;
		case 5:
			skin_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__MP5_Oblivion2_PrimaryAsset_C", false);//1
			break;
		}
	}


	return skin_data;
}
UObject* skinlistchroma()
{
	UObject* chroma_data = nullptr;

	if (EnableVandal) {
		switch ((int)skinvandal) {
		case 1:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__AK_Ashen_Lv4_PrimaryAsset_C", false);//1
			break;
		case 2:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__AK_Soulstealer_Lv4_PrimaryAsset_C", false);//2
			break;
		case 3:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__AK_Afterglow_Lv4_PrimaryAsset_C", false);//2
			break;
		case 4:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__AK_Champions_Lv4_PrimaryAsset_C", false);//2
			break;
		case 5:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__AK_Dragon_Lv4_PrimaryAsset_C", false);//2
			break;
		case 6:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__AK_Oblivion2_Lv4_PrimaryAsset_C", false);//2
			break;
		case 7:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__AK_Daedalus_Lv4_PrimaryAsset_C", false);//2
			break;
		case 8:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__AK_Antares_Lv4_PrimaryAsset_C", false);//2
			break;
		case 9:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__AK_Circle_Lv4_PrimaryAsset_C", false);//2
			break;
		case 10:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__AK_DemonStone_Lv4_PrimaryAsset_C", false);//2
			break;
		case 11:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__AK_SOL_Lv4_PrimaryAsset_C", false);//2
			break;
		case 12:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__AK_Cyberpunk2_Lv4_PrimaryAsset_C", false);//2
			break;
		}
	}

	if (EnablePhantom) {
		switch ((int)skinphantom) {
		case 1:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Carbine_Atlas_Lv4_PrimaryAsset_C", false);//1
			break;
		case 2:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Carbine_Soulstealer2_Lv4_PrimaryAsset_C", false);//2
			break;
		case 3:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Carbine_Afterglow2_Lv4_PrimaryAsset_C", false);//2
			break;
		case 4:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__AssaultRifle_ACR_Edge_Lv4_PrimaryAsset_C", false);//2
			break;
		case 5:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Carbine_King_Lv4_PrimaryAsset_C", false);//2
			break;
		case 6:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Carbine_Cyberpunk2_Lv4_PrimaryAsset_C", false);//2
			break;
		case 7:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Carbine_Daedalus_Lv4_PrimaryAsset_C", false);//2
			break;
		case 8:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Carbine_SpecOps_Lv4_PrimaryAsset_C", false);//2
			break;
		case 9:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Carbine_URF_Lv4_PrimaryAsset_C", false);//2
			break;
		}
	}

	if (EnableKnife) {
		switch ((int)skinknife) {
		case 1:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Afterglow2_Lv2_PrimaryAsset_C", false);//1
			break;
		case 2:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Cyberpunk2_Lv2_PrimaryAsset_C", false);//2
			break;
		case 3:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Soulstealer2_Lv2_PrimaryAsset_C", false);//2
			break;
		case 4:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Afterglow_Lv2_PrimaryAsset_C", false);//2
			break;
		case 5:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Alien_Lv2_PrimaryAsset_C", false);//2
			break;
		case 6:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_HypeBeast2_Lv2_PrimaryAsset_C", false);//2
			break;
		case 7:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Soulstealer_Lv2_PrimaryAsset_C", false);//2
			break;
		case 8:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Champions_Lv2_PrimaryAsset_C", false);//2
			break;
		case 9:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee__Dragon_Lv2_PrimaryAsset_C", false);//2
			break;
		case 10:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Lunar_Lv2_PrimaryAsset_C", false);//2
			break;
		case 11:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Antares_Lv2_PrimaryAsset_C", false);//2
			break;
		case 12:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Champions2_Lv2_PrimaryAsset_C", false);//2
			break;
		case 13:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Haunted_Lv2_PrimaryAsset_C", false);//2
			break;
		case 14:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Mythic_Lv2_PrimaryAsset_C", false);//2
			break;
		case 15:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Oblivion2_Lv2_PrimaryAsset_C", false);//2
			break;
		case 16:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Melee_Magepunk2_Lv2_PrimaryAsset_C", false);//2
			break;

		}

	}

	if (EnableSheriff) {
		switch ((int)skinsheriff)
		{
		case 1:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__RevolverPistol_Soulstealer_Lv4_PrimaryAsset_C", false);//1
			break;
		case 2:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__RevolverPistol_Edge_Lv4_PrimaryAsset_C", false);//2
			break;
		case 3:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Revolver_Protocol_Lv4_PrimaryAsset_C", false);//2
			break;
		case 4:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Revolver_SOL_Lv4_PrimaryAsset_C", false);//2
			break;
		case 5:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Revolver_Daedalus_Lv4_PrimaryAsset_C", false);//2
			break;
		case 6:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Revolver_Magepunk2_Lv4_PrimaryAsset_C", false);//2
			break;
		}
	}

	if (EnableOperator) {

		switch ((int)skinoperator)
		{
		case 1:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__BoltSniper_Circle_Lv4_PrimaryAsset_C", false);//1
			break;
		case 2:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__BoltSniper_DemonStone_Lv4_PrimaryAsset_C", false);//2
			break;
		case 3:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__BoltSniper_Fallen_Lv4_PrimaryAsset_C", false);//2
			break;
		case 4:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__BoltSniper_Cyberpunk2_Lv4_PrimaryAsset_C", false);//2
			break;
		case 5:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__BoltSniper_Afterglow2_Lv4_PrimaryAsset_C", false);//2
			break;
		case 6:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__BoltSniper_Dragon_Lv4_PrimaryAsset_C", false);//2
			break;
		case 7:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__BoltSniper_SOL_Lv4_PrimaryAsset_C", false);//2
			break;
		case 8:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__BoltSniper_Soulstealer_Lv4_PrimaryAsset_C", false);//2
			break;
		}


	}
	if (EnableGhost) {

		switch ((int)skinghost)
		{
		case 1:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__LugerPistol_Sovereign_Lv4_PrimaryAsset_C", false);//1
			break;
		case 2:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Luger_King_Lv4_PrimaryAsset_C", false);//2
			break;
		case 3:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Luger_Magepunk_Lv4_PrimaryAsset_C", false);//2
			break;
		case 4:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__Luger_Soulstealer2_Lv4_PrimaryAsset_C", false);//2
			break;
		case 5:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__LugerPistol_Ashen_Lv4_PrimaryAsset_C", false);//2
			break;
		}


	}

	if (EnableClassic) {

		switch ((int)skinclassic)
		{
		case 1:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__BasePistol_HypeBeast_Lv4_PrimaryAsset_C", false);//1
			break;
		case 2:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__BasePistol_Afterglow2_Lv4_PrimaryAsset_C", false);//1
			break;
		case 3:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__BasePistol_Atlas_Lv4_PrimaryAsset_C", false);//1
			break;
		case 4:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__BasePistol_Cyberpunk2_Lv4_PrimaryAsset_C", false);//1
			break;
		}
	}

	if (EnableSpectre) {

		switch ((int)skinspectre)
		{
		case 1:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__MP5_Afterglow2_Lv4_PrimaryAsset_C", false);//1
			break;
		case 2:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__MP5_Soulstealer2_Lv4_PrimaryAsset_C", false);//1
			break;
		case 3:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__MP5_Protocol_Lv4_PrimaryAsset_C", false);//1
			break;
		case 4:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__MP5_King_Lv4_PrimaryAsset_C", false);//1
			break;
		case 5:
			chroma_data = UObject::StaticFindObject(nullptr, reinterpret_cast<UObject*>(-1), L"Default__MP5_Oblivion2_Lv4_PrimaryAsset_C", false);//1
			break;
		}
	}

	return chroma_data;
}


void PostRender(uintptr_t _this, UCanvas* canvas)

{

	if (!canvas)
		return PostRender(_this, canvas);

	if (kekIsOff)
		return PostRender(_this, canvas);

	CWINGui::SetupCanvas(canvas);
	UWorld* World = Memory::ReadStub<UWorld*>(_this + Offsets::Pointer);
	printf("World:%p\n", World);
	APlayerController* MyController = ShooterGameBlueprints::GetFirstLocalPlayerController(World);//crash 456 this was crashing because of pointer
	printf("MyController:%p\n", MyController);
	AShooterCharacter* MyShooter = MyController->GetShooterCharacter();
	printf("MyShooter:%p\n", MyShooter);
	TArray<AShooterCharacter*> Actors = ShooterGameBlueprints::FindAllShooterCharactersWithAlliance(World, MyShooter, EAresAlliance::Alliance_Any, false, true);
	printf("MyShooter:%p\n", Actors);
	CWINGui::Input::Handle();


	if (drawcross)
	{
		CWINGui::DrawCircle({ (float)GetSystemMetrics(SM_CXSCREEN) / 2, (float)GetSystemMetrics(SM_CYSCREEN) / 2 }, radius, LineamountCross, crosscolor);
	}
	if (  drawfov)
	{
		CWINGui::DrawCircle({ (float)GetSystemMetrics(SM_CXSCREEN) / 2, (float)GetSystemMetrics(SM_CYSCREEN) / 2 }, fovChanger, LineamountFov, fovcolor);
	}
	
	if (GetAsyncKeyState(VK_F1) & 1) menu_opened = !menu_opened; //Our menu key 
	if (Watermark) {
		canvas->K2_DrawText(L"StaffBesting Paid | Menu-F1", { 49,40 }, { 1.0f,1.0f/*Size*/ }, { 1.0f,1.0f,1.0f,2.0f/*RGBA*/ }, 0.f, { 0,0,0,1 }, { 0,0 }, 0, 0, 0, { 0,0,0,1 });
	}


	if (CWINGui::Window(skCrypt("StaffBesting Paid | Menu-F1"), &pos, FVector2D{ 564.0f, 385.0f }, menu_opened))
	{
		//Simple Tabs
		static int tab = 0;
		if (CWINGui::ButtonTab(L"Aimbot", FVector2D{ 110, 33 }, tab == 0)) tab = 0;
		CWINGui::SameLine();
		if (CWINGui::ButtonTab(L"ESP", FVector2D{ 110, 33 }, tab == 1)) tab = 1;
		CWINGui::SameLine();
		if (CWINGui::ButtonTab(L"Chams", FVector2D{ 110, 33 }, tab == 2)) tab = 2;
		CWINGui::SameLine();
		if (CWINGui::ButtonTab(L"Misc", FVector2D{ 110, 33 }, tab == 3)) tab = 3;
		CWINGui::SameLine();
		if (CWINGui::ButtonTab(L"Skins", FVector2D{ 110, 33 }, tab == 4)) tab = 4;
		CWINGui::PushNextElementY(10);
		//Some Elements


		if (tab == 0)
		{
			static int TABZ = 0;
			if (CWINGui::ButtonTab(L"Legit", FVector2D{ 275, 29 }, TABZ == 0)) TABZ = 0;
			CWINGui::SameLine();
			if (CWINGui::ButtonTab(L"Rage", FVector2D{ 275, 29 }, TABZ == 1)) TABZ = 1;

			if (TABZ == 0)
			{
				CWINGui::Checkbox(L"Enabled Aimbot", &enableaim);
				CWINGui::Checkbox(L"Draw FOV", &drawfov);
				CWINGui::SliderFloat(L"Value", &fovChanger, 5, 900);
				CWINGui::Hotkey("Keybind", { 90,32 }, &KEKWNOKEY);
				CWINGui::Combobox(L"", { 95,32 }, &aimbone, L"Head", L"Chest", L"Penis", NULL);
				CWINGui::ColorPicker(L"FOV Color", &fovcolor);
			}

			if (TABZ == 1)
			{
				/*CWINGui::Checkbox(L"Silent", &Silent);*/
				CWINGui::Checkbox(L"Triggerbot", &autoshoot);
			}
		}

		if (tab == 1) {

			static int TABZ = 0;
			if (CWINGui::ButtonTab(L"ESP", FVector2D{ 275, 29 }, TABZ == 0)) TABZ = 0;
			CWINGui::SameLine();
			if (CWINGui::ButtonTab(L"Colors", FVector2D{ 275, 29 }, TABZ == 1)) TABZ = 1;


			if (TABZ == 0)
			{
				CWINGui::Checkbox(L"2D Box", &box2d);
				//CWINGui::Checkbox(L"", &box3d);
				CWINGui::Checkbox(L"Visible Box", &Visible_Check_ESP);
				CWINGui::Checkbox(L"Skeleton", &skeleton);
				CWINGui::Text(L"");
				CWINGui::Checkbox(L"Head Circle", &HeadCircleEsp);
				CWINGui::Checkbox(L"Weapon Name", &weaponesp);
				CWINGui::Checkbox(L"Agent Name", &AgentName);
				CWINGui::Checkbox(L"Health Bar", &healthbar);
				CWINGui::Checkbox(L"Enemys on MiniMap", &Minimap);
				CWINGui::Checkbox(L"Snapline", &snaplinenz);
				if (snaplinenz) {
					CWINGui::Combobox(L"", { 95,32 }, &SnapPositionint, L"Bottom", L"Middle", L"Top", NULL);
				}
			}

			if (TABZ == 1)
			{
				CWINGui::ColorPicker(L"Crosshair Color", &crosscolor);
				CWINGui::ColorPicker(L"Name Color", &Name_Color);
				CWINGui::ColorPicker(L"Box Color[Invisible]", &Invisible_ESPColor);
				CWINGui::ColorPicker(L"Box Color[Visible]", &VisibleBox_ESPColor);
				CWINGui::ColorPicker(L"Snapline Color[Invisible]", &InvisibleSnapColor);//Clearly since ScriptsKit is retarted I will just do it
				CWINGui::ColorPicker(L"Snapline Color[Visible]", &VisibleSnapColor);// Here is a way to customize all the color for the cheat soon
				CWINGui::ColorPicker(L"Skeleton Color[Invisible]", &InvisibleSkeletonColor);//I will release rainbow menu
				CWINGui::ColorPicker(L"Skeleton Color[Visible]", &VisibleSkeletonColor);
			}
		}

		if (tab == 2) {
			static int TABZ = 0;
			if (CWINGui::ButtonTab(L"Chams", FVector2D{ 275, 29 }, TABZ == 0)) TABZ = 0;
			CWINGui::SameLine();
			if (CWINGui::ButtonTab(L"Wireframe", FVector2D{ 275, 29 }, TABZ == 1)) TABZ = 1;
			if (TABZ == 0)
			{
				CWINGui::Checkbox(L"Chams", &ChamsESP);
				CWINGui::SliderFloat(L"Glow", &chamsglowvalue, 1, 10);
				CWINGui::ColorPicker(L"RGB Visible", &Visible);
				CWINGui::ColorPicker(L"RGB Invisible", &Invisible);
			}

			if (TABZ == 1)
			{
				CWINGui::Checkbox(L"Wireframe", &Wireframe);
				CWINGui::Checkbox(L"Wireframe Gun", &WireframeGun);
			}
		}

		if (tab == 3) {
			static int TABZ = 0;

			if (CWINGui::ButtonTab(L"Misc", FVector2D{ 275, 29 }, TABZ == 0)) TABZ = 0;
			CWINGui::SameLine();
			if (CWINGui::ButtonTab(L"Other", FVector2D{ 275, 29 }, TABZ == 1)) TABZ = 1;

			if (TABZ == 0)
			{
				//CWINGui::Checkbox(L"Fast Crouch [Buggy]", &);
				CWINGui::Checkbox(L"Draw Crosshair", &drawcross);

				CWINGui::Checkbox(L"Watermark", &Watermark);
				if (CWINGui::Button(L"Skip Tutorial", FVector2D{ 110, 29 })) {
					skiptutorial = true;
				}

				CWINGui::Checkbox(L"FOV Changer", &fovchanger);
				if (fovchanger) {
					CWINGui::SliderFloat(L"Value", &fovchangur, 0, 170);
				}

				/*CWINGui::Checkbox(L"ThirdPerson", &ThirdPerson);*/
				if (ThirdPerson) {
					CWINGui::SliderFloat(L"Distange", &PlayerDistange, 100, 1000);
				}
			}

			if (TABZ == 1)
			{
				if (CWINGui::Button(L"DaddyKermit", FVector2D{ 110, 33 })) {
					system("start https://discord.com/users/1055003985540567050");
				}
				CWINGui::SameLine();
				if (CWINGui::Button(L"AngelCheats Forums", FVector2D{ 110, 33 })) {
					system("start https://angelcheats.cloud");
				}
			}
		}

		if (tab == 4) {
			static int TABZ = 0;

			if (CWINGui::ButtonTab(L"Skins", FVector2D{ 275, 29 }, TABZ == 0)) TABZ = 0;
			CWINGui::SameLine();
			if (CWINGui::ButtonTab(L"UnlockALL", FVector2D{ 275, 29 }, TABZ == 1)) TABZ = 1;

			if (TABZ == 0)
			{
				CWINGui::Checkbox(L"Vandal", &EnableVandal);
				CWINGui::Checkbox(L"Phantom", &EnablePhantom);
				CWINGui::Checkbox(L"Spectre", &EnableSpectre);
				CWINGui::Checkbox(L"Operator", &EnableOperator);
				CWINGui::Checkbox(L"Sheriff", &EnableSheriff);
				CWINGui::Checkbox(L"Ghost", &EnableGhost);
				CWINGui::Checkbox(L"Classic", &EnableClassic);
				CWINGui::Checkbox(L"Knife", &EnableKnife);
			}

			if (TABZ == 1)
			{
				//if (CWINGui::Button(L"UnlockAll", FVector2D{ 110, 33 })) {
				//	if (auto manager = (World))
				//	{
				//		inventory_manager* inventory;
				//		vector<UObject*>spray_models = inventory->get_all_spray_models();

				//		for (int i = 0; i < spray_models.size(); i++)
				//		{
				//			auto spray_model = spray_models[i];
				//			if (!spray_model) continue;

				//			*(bool*)(spray_model + 0xf2) = true; //UBaseInventoryModel::bIsOwned
				//		}
				//	}
				//}

				CWINGui::Text(L"Unlock all soon");

				//if (CWINGui::Button(L"UnlockAllWeapons", FVector2D{ 110, 33 })) {

				//}
			}
		}
	}
	CWINGui::Draw_Cursor(menu_opened);
	CWINGui::Render();

	if (fovchanger) {
		MyController->set_fov(fovchangur);
	}
	if (skiptutorial) {
		MyController->DisconnectFromServer();
		Beep(300, 300);
		skiptutorial = false;
	}
	

	bool hasTarget = false;
	for (int Index = 0; Index < Actors.Num(); Index++)
	{
		if (!Actors.IsValidIndex(Index))
			continue;

		//Skins
		if (EnableKnife) {
			skinknife += 1;
			if (skinknife == 17) {
				skinknife = 1;
			}
			if (MyShooter) {
				auto get_equippable_pointer = reinterpret_cast<std::uintptr_t(__fastcall*)(AAresEquippable*)>(VALORANT::Module + Offsets::SkinChanger); //https://github.com/10HEAD/ValorantOffsets/blob/main/valorantoffsets.hpp#L11
				std::uintptr_t result = get_equippable_pointer(MyShooter->GetInventory()->GetCurrentWeapon());

				if (result) {
					std::uintptr_t result = get_equippable_pointer(MyShooter->GetInventory()->GetCurrentWeapon());
					int64 var = Memory::ReadStub<uintptr_t>(result + 664);
					var = Memory::ReadStub<uintptr_t>(var + 160);

					int backup = Memory::ReadStub<int>(var + 128);
					Memory::WriteStub<int>(var + Offsets::Pointer, 2);

					UObject* skin_data_asset = skinlist();
					UObject* chroma_data_asset = skinlistchroma();
					if ((skin_data_asset) && (chroma_data_asset))
					{
						MyShooter->ClearWeaponComponents(MyShooter->GetInventory()->GetCurrentWeapon());
						MyShooter->ApplySkin(MyShooter->GetInventory()->GetCurrentWeapon(), skin_data_asset, chroma_data_asset, 2, nullptr, -1);
					}
					if (EnableKnife == true) {
						EnableKnife = false;
					}
					Memory::WriteStub(var + Offsets::Pointer, 6);

				}
			}
		}
		int skinLevel = 4;
		if (EnableVandal) {
			if (MyShooter) {
				skinvandal += 1;
				if (skinvandal == 13) {
					skinvandal = 1;
				}
				auto get_equippable_pointer = reinterpret_cast<std::uintptr_t(__fastcall*)(AAresEquippable*)>(VALORANT::Module + Offsets::SkinChanger); //https://github.com/10HEAD/ValorantOffsets/blob/main/valorantoffsets.hpp#L11
				std::uintptr_t result = get_equippable_pointer(MyShooter->GetInventory()->GetCurrentWeapon());

				if (result) {
					std::uintptr_t result = get_equippable_pointer(MyShooter->GetInventory()->GetCurrentWeapon());
					int64 var = Memory::ReadStub<uintptr_t>(result + 664);
					var = Memory::ReadStub<uintptr_t>(var + 160);

					int backup = Memory::ReadStub<int>(var + 128);
					Memory::WriteStub<int>(var + Offsets::Pointer, 2);

					UObject* skin_data_asset = skinlist();
					UObject* chroma_data_asset = skinlistchroma();
					if ((skin_data_asset) && (chroma_data_asset))
					{
						MyShooter->ClearWeaponComponents(MyShooter->GetInventory()->GetCurrentWeapon());
						MyShooter->ApplySkin(MyShooter->GetInventory()->GetCurrentWeapon(), skin_data_asset, chroma_data_asset, skinLevel, nullptr, -1);
					}
					Memory::WriteStub(var + Offsets::Pointer, 6);
					if (EnableVandal == true) {
						EnableVandal = false;
					}
				}
			}
		}
		if (EnableOperator) {
			skinoperator += 1;
			if (skinoperator == 9) {
				skinoperator = 1;
			}
			if (MyShooter) {
				auto get_equippable_pointer = reinterpret_cast<std::uintptr_t(__fastcall*)(AAresEquippable*)>(VALORANT::Module + Offsets::SkinChanger); //https://github.com/10HEAD/ValorantOffsets/blob/main/valorantoffsets.hpp#L11
				std::uintptr_t result = get_equippable_pointer(MyShooter->GetInventory()->GetCurrentWeapon());

				if (result) {
					std::uintptr_t result = get_equippable_pointer(MyShooter->GetInventory()->GetCurrentWeapon());
					int64 var = Memory::ReadStub<uintptr_t>(result + 664);
					var = Memory::ReadStub<uintptr_t>(var + 160);

					int backup = Memory::ReadStub<int>(var + 128);
					Memory::WriteStub<int>(var + Offsets::Pointer, 2);

					UObject* skin_data_asset = skinlist();
					UObject* chroma_data_asset = skinlistchroma();
					if ((skin_data_asset) && (chroma_data_asset))
					{
						MyShooter->ClearWeaponComponents(MyShooter->GetInventory()->GetCurrentWeapon());
						MyShooter->ApplySkin(MyShooter->GetInventory()->GetCurrentWeapon(), skin_data_asset, chroma_data_asset, skinLevel, nullptr, -1);
					}
					Memory::WriteStub(var + Offsets::Pointer, 6);
					if (EnableOperator == true) {
						EnableOperator = false;
					}
				}
			}
		}
		if (EnablePhantom) {

			skinphantom += 1;
			if (skinphantom == 10) {
				skinphantom = 1;
			}
			if (MyShooter) {
				auto get_equippable_pointer = reinterpret_cast<std::uintptr_t(__fastcall*)(AAresEquippable*)>(VALORANT::Module + Offsets::SkinChanger); //https://github.com/10HEAD/ValorantOffsets/blob/main/valorantoffsets.hpp#L11
				std::uintptr_t result = get_equippable_pointer(MyShooter->GetInventory()->GetCurrentWeapon());

				if (result) {
					std::uintptr_t result = get_equippable_pointer(MyShooter->GetInventory()->GetCurrentWeapon());
					int64 var = Memory::ReadStub<uintptr_t>(result + 664);
					var = Memory::ReadStub<uintptr_t>(var + 160);

					int backup = Memory::ReadStub<int>(var + 128);
					Memory::WriteStub<int>(var + Offsets::Pointer, 2);

					UObject* skin_data_asset = skinlist();
					UObject* chroma_data_asset = skinlistchroma();
					if ((skin_data_asset) && (chroma_data_asset))
					{
						MyShooter->ClearWeaponComponents(MyShooter->GetInventory()->GetCurrentWeapon());
						MyShooter->ApplySkin(MyShooter->GetInventory()->GetCurrentWeapon(), skin_data_asset, chroma_data_asset, skinLevel, nullptr, -1);
					}
					Memory::WriteStub(var + Offsets::Pointer, 6);
					if (EnablePhantom == true) {
						EnablePhantom = false;
					}
				}
			}
		}
		if (EnableSpectre) {
			skinspectre += 1;
			if (skinspectre == 6) {
				skinspectre = 1;
			}
			if (MyShooter) {
				auto get_equippable_pointer = reinterpret_cast<std::uintptr_t(__fastcall*)(AAresEquippable*)>(VALORANT::Module + Offsets::SkinChanger); //https://github.com/10HEAD/ValorantOffsets/blob/main/valorantoffsets.hpp#L11
				std::uintptr_t result = get_equippable_pointer(MyShooter->GetInventory()->GetCurrentWeapon());

				if (result) {
					std::uintptr_t result = get_equippable_pointer(MyShooter->GetInventory()->GetCurrentWeapon());
					int64 var = Memory::ReadStub<uintptr_t>(result + 664);
					var = Memory::ReadStub<uintptr_t>(var + 160);

					int backup = Memory::ReadStub<int>(var + 128);
					Memory::WriteStub<int>(var + Offsets::Pointer, 2);

					UObject* skin_data_asset = skinlist();
					UObject* chroma_data_asset = skinlistchroma();
					if ((skin_data_asset) && (chroma_data_asset))
					{
						MyShooter->ClearWeaponComponents(MyShooter->GetInventory()->GetCurrentWeapon());
						MyShooter->ApplySkin(MyShooter->GetInventory()->GetCurrentWeapon(), skin_data_asset, chroma_data_asset, skinLevel, nullptr, -1);
					}
					Memory::WriteStub(var + Offsets::Pointer, 6);
					if (EnableSpectre == true) {
						EnableSpectre = false;
					}
				}
			}
		}
		if (EnableSheriff) {
			skinsheriff += 1;
			if (skinsheriff == 8) {
				skinsheriff = 1;
			}
			if (MyShooter) {
				auto get_equippable_pointer = reinterpret_cast<std::uintptr_t(__fastcall*)(AAresEquippable*)>(VALORANT::Module + Offsets::SkinChanger); //https://github.com/10HEAD/ValorantOffsets/blob/main/valorantoffsets.hpp#L11
				std::uintptr_t result = get_equippable_pointer(MyShooter->GetInventory()->GetCurrentWeapon());

				if (result) {
					std::uintptr_t result = get_equippable_pointer(MyShooter->GetInventory()->GetCurrentWeapon());
					int64 var = Memory::ReadStub<uintptr_t>(result + 664);
					var = Memory::ReadStub<uintptr_t>(var + 160);

					int backup = Memory::ReadStub<int>(var + 128);
					Memory::WriteStub<int>(var + Offsets::Pointer, 2);

					UObject* skin_data_asset = skinlist();
					UObject* chroma_data_asset = skinlistchroma();
					if ((skin_data_asset) && (chroma_data_asset))
					{
						MyShooter->ClearWeaponComponents(MyShooter->GetInventory()->GetCurrentWeapon());
						MyShooter->ApplySkin(MyShooter->GetInventory()->GetCurrentWeapon(), skin_data_asset, chroma_data_asset, skinLevel, nullptr, -1);
					}
					Memory::WriteStub(var + Offsets::Pointer, 6);
					if (EnableSheriff == true) {
						EnableSheriff = false;
					}
				}
			}
		}
		if (EnableGhost) {
			skinghost += 1;
			if (skinghost == 6) {
				skinghost = 1;
			}
			if (MyShooter) {
				auto get_equippable_pointer = reinterpret_cast<std::uintptr_t(__fastcall*)(AAresEquippable*)>(VALORANT::Module + Offsets::SkinChanger); //https://github.com/10HEAD/ValorantOffsets/blob/main/valorantoffsets.hpp#L11
				std::uintptr_t result = get_equippable_pointer(MyShooter->GetInventory()->GetCurrentWeapon());

				if (result) {
					std::uintptr_t result = get_equippable_pointer(MyShooter->GetInventory()->GetCurrentWeapon());
					int64 var = Memory::ReadStub<uintptr_t>(result + 664);
					var = Memory::ReadStub<uintptr_t>(var + 160);

					int backup = Memory::ReadStub<int>(var + 128);
					Memory::WriteStub<int>(var + Offsets::Pointer, 2);

					UObject* skin_data_asset = skinlist();
					UObject* chroma_data_asset = skinlistchroma();
					if ((skin_data_asset) && (chroma_data_asset))
					{
						MyShooter->ClearWeaponComponents(MyShooter->GetInventory()->GetCurrentWeapon());
						MyShooter->ApplySkin(MyShooter->GetInventory()->GetCurrentWeapon(), skin_data_asset, chroma_data_asset, skinLevel, nullptr, -1);
					}
					Memory::WriteStub(var + Offsets::Pointer, 6);
					if (EnableGhost == true) {
						EnableGhost = false;
					}
				}
			}
		}
		if (EnableClassic) {
			skinclassic += 1;
			if (skinghost == 5) {
				skinghost = 1;
			}
			if (MyShooter) {
				auto get_equippable_pointer = reinterpret_cast<std::uintptr_t(__fastcall*)(AAresEquippable*)>(VALORANT::Module + Offsets::SkinChanger); //https://github.com/10HEAD/ValorantOffsets/blob/main/valorantoffsets.hpp#L11
				std::uintptr_t result = get_equippable_pointer(MyShooter->GetInventory()->GetCurrentWeapon());
				if (result) {
					std::uintptr_t result = get_equippable_pointer(MyShooter->GetInventory()->GetCurrentWeapon());
					int64 var = Memory::ReadStub<uintptr_t>(result + 664);
					var = Memory::ReadStub<uintptr_t>(var + 160);

					int backup = Memory::ReadStub<int>(var + 128);
					Memory::WriteStub<int>(var + Offsets::Pointer, 2);

					UObject* skin_data_asset = skinlist();
					UObject* chroma_data_asset = skinlistchroma();
					if ((skin_data_asset) && (chroma_data_asset))
					{
						MyShooter->ClearWeaponComponents(MyShooter->GetInventory()->GetCurrentWeapon());
						MyShooter->ApplySkin(MyShooter->GetInventory()->GetCurrentWeapon(), skin_data_asset, chroma_data_asset, skinLevel, nullptr, -1);
					}
					Memory::WriteStub(var + Offsets::Pointer, 6);
					if (EnableClassic == true) {
						EnableClassic = false;
					}
				}
			}
		}


		AShooterCharacter* Actor = Actors[Index];
		actor = Actor;
		UObject* Object;
		if (Actor->GetHealth() == 0)
			continue;

		if (!MyShooter) continue;

		if (kismentsystemlibrary::get_object_name(Actor).ToString().find("_PC") != std::string::npos) {
			InGame = 1;
		}
		else {
			InGame = 0;
		}
		Mesh = Actor->GetPawnMesh();
		if (ThirdPerson)
		{
			if (!Mesh3PModifed) MyShooter->Set3pMeshVisible(true);
			Mesh3PModifed = true;
		}
		else
		{
			if (Mesh3PModifed)
			{
				MyShooter->Set3pMeshVisible(false);
				Mesh3PModifed = false;
			}
		}

		if (SpinBot && GetAsyncKeyState(spinbotkeybind) && !GetAsyncKeyState(KEKWNOKEY) && (spinvischeck && MyController->LineOfSightTo(Actor, { 0, 0, 0 }, false) || !spinvischeck)) {

			static auto OldAimAngles = FVector();
			UAresOutlineComponent* Mesh = Actor->GetPawnMesh();
			uintptr_t cmanager = *(uintptr_t*)((uintptr_t)MyController + 0x478);
			FVector CameraRot = *(FVector*)(cmanager + 0x126C);
			FVector DeltaRotation;
			FVector ConvertRotation = { CameraRot.X < 0.f ? 360.f + CameraRot.X : CameraRot.X, CameraRot.Y < 0.f ? 360.f + CameraRot.Y : CameraRot.Y, 0 };
			FVector ControlRotation = MyController->GetControlRotation();

			FVector Delta = { CameraRot.X, CameraRot.Y, CameraRot.Z };
			float hyp = sqrtf(Delta.X * Delta.X + Delta.Y * Delta.Y + Delta.Z * Delta.Z);

			FVector Rotation = { acosf(Delta.Z / hyp) * (float)(180.0f / 3.1415926535897932384626433832795028841971693993751), atanf(Delta.Y / Delta.X) * (float)(180.0f / 3.1415926535897932384626433832795028841971693993751), 0 };

			Rotation.X += 270.f;

			if (Delta.X >= 0.0f) Rotation.Y += 180.0f;

			if (Rotation.Y < 0.f) Rotation.Y += 360.f;

			DeltaRotation.X = fmodf(ConvertRotation.X - ControlRotation.X, 360.f);
			DeltaRotation.Y = fmodf(ConvertRotation.Y - ControlRotation.Y, 360.f);

			ConvertRotation.X = fmodf(Rotation.X - DeltaRotation.X - DeltaRotation.X, 360.f);
			ConvertRotation.Y = fmodf(Rotation.Y - DeltaRotation.Y - DeltaRotation.Y, 360.f);

			if (ConvertRotation.X < 0.f) ConvertRotation.X = 360.f + ConvertRotation.X;

			if (ConvertRotation.Y < 0.f) ConvertRotation.Y = 360.f + ConvertRotation.Y;

			MyController->SetControlRotation(ConvertRotation);






		}
		if (FlyHack)
		{
			MyShooter->ClientFly();
		}

		if (BaseTeamComponent::IsAlly(Actor, MyShooter))
			continue;

		if (!InGame) continue;

		bool IsAlive = Actor->IsAlive();
		if (Mesh && Actor->IsAlive())
		{
			FLinearColor BoxColor;
			FLinearColor SnapColor;
			FLinearColor SkeletonColor;
			FLinearColor ChamsColor;

			if (true)
			{
				if (MyController->LineOfSightTo(Actor, { 0,0,0 }, false)) {

					SnapColor = VisibleSnapColor;
					BoxColor = VisibleBox_ESPColor;
					SkeletonColor = VisibleSkeletonColor;

					ChamsColor = Visible;

				}
				else {

					SnapColor = InvisibleSnapColor;
					BoxColor = Invisible_ESPColor;
					SkeletonColor = InvisibleSkeletonColor;
					ChamsColor = Invisible;
				}

			}
			if (HeadCircleEsp) {
				FVector2D vHeadBoneOut;
				if (MyController->ProjectWorldLocationToScreen(GetBoneMatrix(Mesh, 8), vHeadBoneOut, 0) && vHeadBoneOut.IsValid())
				{
					CWINGui::DrawCircle({ vHeadBoneOut.X, vHeadBoneOut.Y }, radius2, LineamountCircle, BoxColor);
				}

			}
			if (box2d)
			{
				FVector Origin, Extend;
				MyController->GetActorBounds(Actor, 1, &Origin, &Extend, 0);

				auto location = Actor->K2_GetActorLocation();
					
				FVector2D footPos;
				if (!MyController->ProjectWorldLocationToScreen({ location.X, location.Y, location.Z + (Extend.Z / 3) }, footPos, 0)) 
					continue;

				FVector2D headPos;
				if (!MyController->ProjectWorldLocationToScreen({ location.X, location.Y, location.Z - (Extend.Z / 3) }, headPos, 0)) 
					continue;


				const float height = abs(footPos.Y - headPos.Y);
				const float width = height * 0.4f;
				FVector2D top = { headPos.X - width * 0.5f, headPos.Y };
				FVector2D bottom = { headPos.X + width * 0.5f, footPos.Y };
				
				auto thicc = ESPThickness;
				FLinearColor xclr = RGBtoFLC(0, 0, 0);
				canvas->K2_DrawLinex(bottom, { bottom.X, top.Y }, thicc, BoxColor);
				canvas->K2_DrawLinex(top, { top.X , top.Y }, thicc, BoxColor);
				canvas->K2_DrawLinex(top, { top.X , bottom.Y }, thicc, BoxColor);
				canvas->K2_DrawLine(bottom, { bottom.X, bottom.Y }, thicc, BoxColor);
			}
			if (enableaim) {

				FVector Target;
				if (aimbone == 0) {//Head
					Target = GetBoneMatrix(Mesh, 8);
				}
				else if (aimbone == 1) {//Chets
					Target = GetBoneMatrix(Mesh, 6);
				}
				else if (aimbone == 2) {//Penis
					Target = GetBoneMatrix(Mesh, 3);
				}

				FVector2D head_screen;
				//FovandKey
				if (MyController->ProjectWorldLocationToScreen(Target, head_screen, 0) && head_screen.IsValid()) {

					if (enableaim && !Silent && !autoaim && !hasTarget && GetAsyncKeyState(KEKWNOKEY) && in_rect(ScreenCenterX, ScreenCenterY, fovChanger, head_screen.X, head_screen.Y) && (vischeck && MyController->LineOfSightTo(Actor, { 0, 0, 0 }, false) || !vischeck))
					{
						uintptr_t cmanager = *(uintptr_t*)((uintptr_t)MyController + 0x478);
						FVector CameraPos = *(FVector*)(cmanager + 0x1260);
						FVector CameraRot = *(FVector*)(cmanager + 0x126C);
						FVector DeltaRotation;
						FVector ConvertRotation = { CameraRot.X < 0.f ? 360.f + CameraRot.X : CameraRot.X, CameraRot.Y < 0.f ? 360.f + CameraRot.Y : CameraRot.Y, 0 };
						FVector ControlRotation = MyController->GetControlRotation();

						FVector Delta = { CameraPos.X - Target.X, CameraPos.Y - Target.Y, CameraPos.Z - Target.Z };
						float hyp = sqrtf(Delta.X * Delta.X + Delta.Y * Delta.Y + Delta.Z * Delta.Z);

						FVector Rotation = { acosf(Delta.Z / hyp) * (float)(180.0f / 3.1415926535897932384626433832795028841971693993751), atanf(Delta.Y / Delta.X) * (float)(180.0f / 3.1415926535897932384626433832795028841971693993751), 0 };

						Rotation.X += 270.f;

						if (Delta.X >= 0.0f) Rotation.Y += 180.0f;

						if (Rotation.Y < 0.f) Rotation.Y += 360.f;

						DeltaRotation.X = fmodf(ConvertRotation.X - ControlRotation.X, 360.f);
						DeltaRotation.Y = fmodf(ConvertRotation.Y - ControlRotation.Y, 360.f);

						ConvertRotation.X = fmodf(Rotation.X - DeltaRotation.X - DeltaRotation.X, 360.f);
						ConvertRotation.Y = fmodf(Rotation.Y - DeltaRotation.Y - DeltaRotation.Y, 360.f);

						if (ConvertRotation.X < 0.f) ConvertRotation.X = 360.f + ConvertRotation.X;

						if (ConvertRotation.Y < 0.f) ConvertRotation.Y = 360.f + ConvertRotation.Y;

						MyController->SetControlRotation(ConvertRotation);
						hasTarget = true;
					}

				}

				//AutoAim
				if (MyController->ProjectWorldLocationToScreen(Target, head_screen, 0) && head_screen.IsValid()) {

					if (enableaim && autoshoot && !Silent && !hasTarget && in_rect(ScreenCenterX, ScreenCenterY, fovChanger, head_screen.X, head_screen.Y) && (vischeck && MyController->LineOfSightTo(Actor, { 0, 0, 0 }, false) || !vischeck))
					{
						uintptr_t cmanager = *(uintptr_t*)((uintptr_t)MyController + 0x478);
						FVector CameraPos = *(FVector*)(cmanager + 0x1260);
						FVector CameraRot = *(FVector*)(cmanager + 0x126C);
						FVector DeltaRotation;
						FVector ConvertRotation = { CameraRot.X < 0.f ? 360.f + CameraRot.X : CameraRot.X, CameraRot.Y < 0.f ? 360.f + CameraRot.Y : CameraRot.Y, 0 };
						FVector ControlRotation = MyController->GetControlRotation();

						FVector Delta = { CameraPos.X - Target.X, CameraPos.Y - Target.Y, CameraPos.Z - Target.Z };
						float hyp = sqrtf(Delta.X * Delta.X + Delta.Y * Delta.Y + Delta.Z * Delta.Z);

						FVector Rotation = { acosf(Delta.Z / hyp) * (float)(180.0f / 3.1415926535897932384626433832795028841971693993751), atanf(Delta.Y / Delta.X) * (float)(180.0f / 3.1415926535897932384626433832795028841971693993751), 0 };

						Rotation.X += 270.f;

						if (Delta.X >= 0.0f) Rotation.Y += 180.0f;

						if (Rotation.Y < 0.f) Rotation.Y += 360.f;

						DeltaRotation.X = fmodf(ConvertRotation.X - ControlRotation.X, 360.f);
						DeltaRotation.Y = fmodf(ConvertRotation.Y - ControlRotation.Y, 360.f);

						ConvertRotation.X = fmodf(Rotation.X - DeltaRotation.X - DeltaRotation.X, 360.f);
						ConvertRotation.Y = fmodf(Rotation.Y - DeltaRotation.Y - DeltaRotation.Y, 360.f);
						if (ConvertRotation.X < 0.f) ConvertRotation.X = 360.f + ConvertRotation.X;

						if (ConvertRotation.Y < 0.f) ConvertRotation.Y = 360.f + ConvertRotation.Y;
						MyController->SetControlRotation(ConvertRotation);
						hasTarget = true;
						mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

					}




				}

				//NoSpread
				if (enableaim && RageBot && GetAsyncKeyState(RageBotKeybind) && !hasTarget && (vischeck && MyController->LineOfSightTo(Actor, { 0, 0, 0 }, false) || !vischeck))
				{
					get_error_angle((uintptr_t)Actor, 0); //Current equippable + I think its 0x10f0 = firing state compenent

				}

				if (Silent)
				{

					FVector Target;
					if (aimbone == 0) {//Head
						Target = GetBoneMatrix(Mesh, 8);
					}
					else if (aimbone == 1) {//Chets
						Target = GetBoneMatrix(Mesh, 6);
					}
					else if (aimbone == 2) {//Penis
						Target = GetBoneMatrix(Mesh, 3);
					}


					//FovandKey
					if (MyController->ProjectWorldLocationToScreen(Target, head_scren, 0) && head_scren.IsValid()) {

						if (Silent && !RageBot && !autoaim && !hasTarget && GetAsyncKeyState(KEKWNOKEY) && in_rect(ScreenCenterX, ScreenCenterY, fovChanger, head_scren.X, head_scren.Y) && (vischeck && MyController->LineOfSightTo(Actor, { 0, 0, 0 }, false) || !vischeck))
						{
							uintptr_t cmanager = *(uintptr_t*)((uintptr_t)MyController + 0x478);
							FVector CameraPos = *(FVector*)(cmanager + 0x1260);
							FVector CameraRot = *(FVector*)(cmanager + 0x126C);
							FVector DeltaRotation;
							FVector ConvertRotation = { CameraRot.X < 0.f ? 360.f + CameraRot.X : CameraRot.X, CameraRot.Y < 0.f ? 360.f + CameraRot.Y : CameraRot.Y, 0 };
							FVector ControlRotation = MyController->GetControlRotation();

							FVector Delta = { CameraPos.X - Target.X, CameraPos.Y - Target.Y, CameraPos.Z - Target.Z };
							float hyp = sqrtf(Delta.X * Delta.X + Delta.Y * Delta.Y + Delta.Z * Delta.Z);

							FVector Rotation = { acosf(Delta.Z / hyp) * (float)(180.0f / 3.1415926535897932384626433832795028841971693993751), atanf(Delta.Y / Delta.X) * (float)(180.0f / 3.1415926535897932384626433832795028841971693993751), 0 };

							Rotation.X += 270.f;

							if (Delta.X >= 0.0f) Rotation.Y += 180.0f;

							if (Rotation.Y < 0.f) Rotation.Y += 360.f;

							DeltaRotation.X = fmodf(ConvertRotation.X - ControlRotation.X, 360.f);
							DeltaRotation.Y = fmodf(ConvertRotation.Y - ControlRotation.Y, 360.f);

							ConvertRotation.X = fmodf(Rotation.X - DeltaRotation.X - DeltaRotation.X, 360.f);
							ConvertRotation.Y = fmodf(Rotation.Y - DeltaRotation.Y - DeltaRotation.Y, 360.f);

							if (ConvertRotation.X < 0.f) ConvertRotation.X = 360.f + ConvertRotation.X;

							if (ConvertRotation.Y < 0.f) ConvertRotation.Y = 360.f + ConvertRotation.Y;

							MyController->SetControlRotation(ConvertRotation);
							hasTarget = true;
						}

					}

				}
			}
			if (snaplinenz)
			{
				FVector2D drawtextat;
				if (MyController->ProjectWorldLocationToScreen(GetBoneMatrix(Mesh, SnapMesh), drawtextat, 0) && drawtextat.IsValid());
				canvas->K2_DrawLine({ (float)GetSystemMetrics(SM_CXSCREEN) / 2, (float)GetSystemMetrics(SM_CYSCREEN) / SnapPosition›nt/*135*/ }, drawtextat, ESPThickness, SnapColor);

			}

			if (Wireframe)
			{
				*(char*)(Mesh + 0x716) = *(char*)(Mesh + 0x716) | (1 << 5);//For wireframe hand use Current local mesh use WireframeGun for a exmple
				*(char*)(Mesh + 0xc0) = 0xff;
			}
			
			if (WireframeGun)
			{
				if (auto get_inventory = MyShooter->GetInventory())
				{
					if (auto get_weapon = get_inventory->GetCurrentWeapon())
					{
						if (auto weapon_mesh_1p = get_weapon->GetMesh1P())
						{

							*(char*)(weapon_mesh_1p + 0x716) = *(char*)(weapon_mesh_1p + 0x716) | (1 << 5);
							*(char*)(weapon_mesh_1p + 0xc0) = 0xff; //now i try this
							/*	write<char>((uintptr_t)weapon_mesh_1p + 0xc0, 0xff);*/
						}
					}
				}
			}
			
			if (Minimap)
			{
				uintptr_t ShooterCharacterMinimapComponent = Memory::ReadStub<uintptr_t>((uintptr_t)Actor + Offsets::portrait_map);
				uintptr_t CharacterPortraitMinimapComponent = Memory::ReadStub<uintptr_t>((uintptr_t)Actor + Offsets::character_map);

				*(bool*)(ShooterCharacterMinimapComponent + 0x501) = true;
				*(bool*)(ShooterCharacterMinimapComponent + 0x530) = true;

				*(bool*)(CharacterPortraitMinimapComponent + 0x501) = true;
				*(bool*)(CharacterPortraitMinimapComponent + 0x530) = true;
			}
			if (weaponesp)
			{
				FVector Origin, Extend;
				FVector2D rel2d, footPos;

				MyController->GetActorBounds(Actor, 0, &Origin, &Extend, 0);

				auto RelativeLocation = Actor->K2_GetActorLocation();

				if (MyController->ProjectWorldLocationToScreen({ RelativeLocation.X, RelativeLocation.Y + 12, RelativeLocation.Z - (Extend.Z / 10) }, footPos, 0))
				{
					if (MyController->ProjectWorldLocationToScreen(Actor->K2_GetActorLocation(), rel2d, true))
					{
						DrawTextRGBWithFString(canvas, weapon_name(kismentsystemlibrary::get_object_name(Actor->GetInventory()->GetCurrentWeapon())), footPos.X, footPos.Y + 12, BoxColor, 1);
					}
				}
			}
			if (AgentName)
			{

				/* Define vectors */
				FVector Origin, Extend;
				FVector2D rel2d, footPos;

				/* Get bouuuuuuunds */
				MyController->GetActorBounds(Actor, 1, &Origin, &Extend, 0);

				/* Obtain health and RelativeLocation */
				auto RelativeLocation = Actor->K2_GetActorLocation();

				/* W2S the vectors */
				if (MyController->ProjectWorldLocationToScreen({ RelativeLocation.X, RelativeLocation.Y, RelativeLocation.Z + (Extend.Z / 10) }, footPos, 0))
				{
					if (MyController->ProjectWorldLocationToScreen(Actor->K2_GetActorLocation(), rel2d, true))
					{
						DrawTextRGBWithFString(canvas, character_name(kismentsystemlibrary::get_object_name(Actor)), footPos.X, footPos.Y - 34, RGBtoFLC(255, 255, 255), 1);
					}
				}
			}
			if (healthbar)
			{
				if (MyShooter->IsAlive())
				{
					FVector Origin, Extend;
					MyController->GetActorBounds(Actor, 1, &Origin, &Extend, 0);

					auto location = Actor->K2_GetActorLocation(); FVector2D footPos;
					if (!MyController->ProjectWorldLocationToScreen({ location.X, location.Y, location.Z - (Extend.Z / 2) }, footPos, 0)) continue;

					FVector2D headPos;
					if (!MyController->ProjectWorldLocationToScreen({ location.X, location.Y, location.Z + (Extend.Z / 2) }, headPos, 0)) continue;

					auto maxhealth = Actor->GetMaxHealth();
					if (maxhealth >= 100)maxhealth = 100;
					const float hp = Actor->GetHealth() / maxhealth;

					const float height = abs(footPos.Y - headPos.Y + 1);
					const float width = height * 0.4f;
					const float width2 = width * 0.8f;
					const float adjust = height * 0.15f;
					const float len = (width * 2.5) * hp;
					const float lennormalized = (width * 2.5); //normalize just like my penis      

					canvas->K2_DrawLinex({ headPos.X - width2 - 1, footPos.Y - adjust }, { headPos.X - width2 - 1 , footPos.Y + adjust - lennormalized }, 5, RGBtoFLC(0, 0, 0));
					canvas->K2_DrawLinex({ headPos.X - width2, footPos.Y - adjust }, { headPos.X - width2, footPos.Y + adjust - len }, 2, RGBtoFLC(60, 255, 0));
				}

			}
			if (skeleton)
			{
				FVector2D vHipOut;
				FVector2D vNeckOut;
				FVector2D vUpperArmLeftOut;
				FVector2D vLeftHandOut;
				FVector2D vLeftHandOut1;
				FVector2D vUpperArmRightOut;
				FVector2D vRightHandOut;
				FVector2D vRightHandOut1;
				FVector2D vLeftThighOut;
				FVector2D vLeftCalfOut;
				FVector2D vLeftFootOut;
				FVector2D vRightThighOut;
				FVector2D vRightCalfOut;
				FVector2D vRightFootOut;
				if (MyController->ProjectWorldLocationToScreen(GetBoneMatrix(Mesh, 3), vHipOut, 0) && vHipOut.IsValid())
					if (MyController->ProjectWorldLocationToScreen(GetBoneMatrix(Mesh, 7), vNeckOut, 0) && vNeckOut.IsValid())
						if (MyController->ProjectWorldLocationToScreen(GetBoneMatrix(Mesh, 11), vUpperArmLeftOut, 0) && vUpperArmLeftOut.IsValid())
							if (MyController->ProjectWorldLocationToScreen(GetBoneMatrix(Mesh, 12), vLeftHandOut, 0) && vLeftHandOut.IsValid())
								if (MyController->ProjectWorldLocationToScreen(GetBoneMatrix(Mesh, 13), vLeftHandOut1, 0) && vLeftHandOut1.IsValid())
									if (MyController->ProjectWorldLocationToScreen(GetBoneMatrix(Mesh, 36), vUpperArmRightOut, 0) && vUpperArmRightOut.IsValid())
										if (MyController->ProjectWorldLocationToScreen(GetBoneMatrix(Mesh, 37), vRightHandOut, 0) && vRightHandOut.IsValid())
											if (MyController->ProjectWorldLocationToScreen(GetBoneMatrix(Mesh, 38), vRightHandOut1, 0) && vRightHandOut1.IsValid())
												if (MyController->ProjectWorldLocationToScreen(GetBoneMatrix(Mesh, 63), vLeftThighOut, 0) && vLeftThighOut.IsValid())
													if (MyController->ProjectWorldLocationToScreen(GetBoneMatrix(Mesh, 65), vLeftCalfOut, 0) && vLeftCalfOut.IsValid())
														if (MyController->ProjectWorldLocationToScreen(GetBoneMatrix(Mesh, 69), vLeftFootOut, 0) && vLeftFootOut.IsValid())
															if (MyController->ProjectWorldLocationToScreen(GetBoneMatrix(Mesh, 77), vRightThighOut, 0) && vRightThighOut.IsValid())
																if (MyController->ProjectWorldLocationToScreen(GetBoneMatrix(Mesh, 79), vRightCalfOut, 0) && vRightCalfOut.IsValid())
																	if (MyController->ProjectWorldLocationToScreen(GetBoneMatrix(Mesh, 83), vRightFootOut, 0) && vRightFootOut.IsValid())

																		canvas->K2_DrawLine({ vHipOut.X, vHipOut.Y }, { vNeckOut.X, vNeckOut.Y }, 1, BoxColor);
				canvas->K2_DrawLine({ vUpperArmLeftOut.X, vUpperArmLeftOut.Y }, { vNeckOut.X, vNeckOut.Y }, 1, BoxColor);
				canvas->K2_DrawLine({ vUpperArmRightOut.X, vUpperArmRightOut.Y }, { vNeckOut.X, vNeckOut.Y }, 1, BoxColor);
				canvas->K2_DrawLine({ vLeftHandOut.X, vLeftHandOut.Y }, { vUpperArmLeftOut.X, vUpperArmLeftOut.Y }, 1, BoxColor);
				canvas->K2_DrawLine({ vRightHandOut.X, vRightHandOut.Y }, { vUpperArmRightOut.X, vUpperArmRightOut.Y }, 1, BoxColor);
				canvas->K2_DrawLine({ vLeftHandOut.X, vLeftHandOut.Y }, { vLeftHandOut1.X, vLeftHandOut1.Y }, 1, BoxColor);
				canvas->K2_DrawLine({ vRightHandOut.X, vRightHandOut.Y }, { vRightHandOut1.X, vRightHandOut1.Y }, 1, BoxColor);
				canvas->K2_DrawLine({ vLeftThighOut.X, vLeftThighOut.Y }, { vHipOut.X, vHipOut.Y }, 1, BoxColor);
				canvas->K2_DrawLine({ vRightThighOut.X, vRightThighOut.Y }, { vHipOut.X, vHipOut.Y }, 1, BoxColor);
				canvas->K2_DrawLine({ vLeftCalfOut.X, vLeftCalfOut.Y }, { vLeftThighOut.X, vLeftThighOut.Y }, 1, BoxColor);
				canvas->K2_DrawLine({ vRightCalfOut.X, vRightCalfOut.Y }, { vRightThighOut.X, vRightThighOut.Y }, 1, BoxColor);
				canvas->K2_DrawLine({ vLeftFootOut.X, vLeftFootOut.Y }, { vLeftCalfOut.X, vLeftCalfOut.Y }, 1, BoxColor);
				canvas->K2_DrawLine({ vRightFootOut.X, vRightFootOut.Y }, { vRightCalfOut.X, vRightCalfOut.Y }, 1, BoxColor);
			}

			if (skid_spinbot)
			{
				static int spinvalue = 3.5f;
				MyController->SetControlRotation(FVector(271, MyController->GetControlRotation().Y - spinvalue, 0)); // Engine.Controller.SetControlRotation
			}
			if (ChamsESP) {


				auto xd = Memory::ReadStub<USkeletalMeshComponent*>((uintptr_t)Actor + Offsets::ChamsOffset);
				if ((uintptr_t)xd > 0x100000)
					xd->SetOutlineMode(EAresOutlineMode::AlwaysOutline);
				AresOutlineRendering::SetOutlineColorsForRender(World, { ChamsColor.R * chamsglowvalue, ChamsColor.G * chamsglowvalue, ChamsColor.B * chamsglowvalue, ChamsColor.A * chamsglowvalue }, { ChamsColor.R * chamsglowvalue, ChamsColor.G * chamsglowvalue, ChamsColor.B * chamsglowvalue, ChamsColor.A * chamsglowvalue });


			}
			else
			{
				Mesh->SetOutlineMode(EAresOutlineMode::None);
			}
		
			if (SnapPositionint == 0) {
				SnapPosition›nt = 1;
				SnapMesh = 0;
				if (Below == true) {
					Below = false;
				}

			}
			else if (SnapPositionint == 1) {
				SnapPosition›nt = 2;
				SnapMesh = 8;

				if (Middle == true) {
					Middle = false;
				}
			}
			else if (SnapPositionint == 2) {
				SnapPosition›nt = 400;
				SnapMesh = 8;

				if (Bottom == true) {
					Bottom = false;
				}
			}
		}
	}
	return pRender(_this, canvas);
}


void Init()
{
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	VALORANT::Module = (uintptr_t)GetModuleHandleA(0);
	printf("Module:%p\n", VALORANT::Module);
	uintptr_t WorldKey = *(uintptr_t*)(VALORANT::Module + Offsets::Key);
	printf("WorldKey:%p\n", WorldKey);
	State StateKey = *(State*)(VALORANT::Module + Offsets::State);
	printf("StateKey:%p\n", StateKey);
	UWorldXOR = Decryption::Decrypt_UWorld(WorldKey, (uintptr_t*)&StateKey);
	printf("UWorldXOR:%p\n", UWorldXOR);

	UWorldClass = Memory::ReadStub<UWorld*>(UWorldXOR);
	GameInstance = Memory::ReadStub<UGameInstance*>((uintptr_t)UWorldClass + 0x1A0);
	LocalPlayers = Memory::ReadStub<ULocalPlayer*>((uintptr_t)GameInstance + 0x40); //this is tarray but im paster lol
	LocalPlayer = Memory::ReadStub<ULocalPlayer*>((uintptr_t)LocalPlayers); //
	APlayerController* LocalController = Memory::ReadStub<APlayerController*>((uintptr_t)LocalPlayer + 0x38);
	uintptr_t ViewportClient = Memory::ReadStub<uintptr_t>((uintptr_t)LocalPlayer + 0x78);//struct UGameViewportClient* ViewportClient; 
	uintptr_t Engine = Memory::ReadStub<uintptr_t>((uintptr_t)GameInstance + 0x28);
	DefaultMediumFont = Memory::ReadStub<UObject*>(Engine + 0xd8);
	APlayerCameraManager* PlayerCameraManager = Memory::ReadStub<APlayerCameraManager*>((uintptr_t)LocalController + Offsets::PlayerCameraManager);// this may need to be swapped back if so dm me
	//camMan = Memory::ReadStub<uintptr_t>((uintptr_t)LocalController + 0x478);

	// try it out now wait want to check something out for skinchanger one second
	LocalCameraLocation = Memory::ReadStub<uintptr_t>((uintptr_t)PlayerCameraManager + 0x1260);
	LocalCameraFOV = Memory::ReadStub<float>((uintptr_t)PlayerCameraManager + 0x1278);
	LocalCameraRotation = Memory::ReadStub<uintptr_t>((uintptr_t)PlayerCameraManager + 0x126C);//this should work

	NamePoolData = reinterpret_cast<FNamePool*>(VALORANT::Module + NamePoolDataOffset);
	Hook::VMT((void*)ViewportClient, PostRender, 0x68, (void**)&pRender);
	Hook::VMT((void*)PlayerCameraManager, SetCameraCachePOVHook, 0xD4, (void**)&SetCameraCachePOVOriginal); //swap the method back to urs if this crash

}
extern "C" __declspec(dllexport) int NextHook(int code, WPARAM wParam, LPARAM lParam) { return CallNextHookEx(NULL, code, wParam, lParam); }


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		Init();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}