// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef VREXPANSIONPLUGIN_VRStereoWidgetComponent_generated_h
#error "VRStereoWidgetComponent.generated.h already included, missing '#pragma once' in VRStereoWidgetComponent.h"
#endif
#define VREXPANSIONPLUGIN_VRStereoWidgetComponent_generated_h

#define TableTopSat_Plugins_VRExpansionPlugin_mordentral_vrexpansionplugin_36a63860c688_VRExpansionPlugin_Source_VRExpansionPlugin_Public_VRStereoWidgetComponent_h_22_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execGetPriority) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(int32*)Z_Param__Result=P_THIS->GetPriority(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetPriority) \
	{ \
		P_GET_PROPERTY(UIntProperty,Z_Param_InPriority); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		P_THIS->SetPriority(Z_Param_InPriority); \
		P_NATIVE_END; \
	}


#define TableTopSat_Plugins_VRExpansionPlugin_mordentral_vrexpansionplugin_36a63860c688_VRExpansionPlugin_Source_VRExpansionPlugin_Public_VRStereoWidgetComponent_h_22_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execGetPriority) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(int32*)Z_Param__Result=P_THIS->GetPriority(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetPriority) \
	{ \
		P_GET_PROPERTY(UIntProperty,Z_Param_InPriority); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		P_THIS->SetPriority(Z_Param_InPriority); \
		P_NATIVE_END; \
	}


#define TableTopSat_Plugins_VRExpansionPlugin_mordentral_vrexpansionplugin_36a63860c688_VRExpansionPlugin_Source_VRExpansionPlugin_Public_VRStereoWidgetComponent_h_22_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUVRStereoWidgetComponent(); \
	friend struct Z_Construct_UClass_UVRStereoWidgetComponent_Statics; \
public: \
	DECLARE_CLASS(UVRStereoWidgetComponent, UWidgetComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/VRExpansionPlugin"), NO_API) \
	DECLARE_SERIALIZER(UVRStereoWidgetComponent)


#define TableTopSat_Plugins_VRExpansionPlugin_mordentral_vrexpansionplugin_36a63860c688_VRExpansionPlugin_Source_VRExpansionPlugin_Public_VRStereoWidgetComponent_h_22_INCLASS \
private: \
	static void StaticRegisterNativesUVRStereoWidgetComponent(); \
	friend struct Z_Construct_UClass_UVRStereoWidgetComponent_Statics; \
public: \
	DECLARE_CLASS(UVRStereoWidgetComponent, UWidgetComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/VRExpansionPlugin"), NO_API) \
	DECLARE_SERIALIZER(UVRStereoWidgetComponent)


#define TableTopSat_Plugins_VRExpansionPlugin_mordentral_vrexpansionplugin_36a63860c688_VRExpansionPlugin_Source_VRExpansionPlugin_Public_VRStereoWidgetComponent_h_22_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UVRStereoWidgetComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UVRStereoWidgetComponent) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UVRStereoWidgetComponent); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UVRStereoWidgetComponent); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UVRStereoWidgetComponent(UVRStereoWidgetComponent&&); \
	NO_API UVRStereoWidgetComponent(const UVRStereoWidgetComponent&); \
public:


#define TableTopSat_Plugins_VRExpansionPlugin_mordentral_vrexpansionplugin_36a63860c688_VRExpansionPlugin_Source_VRExpansionPlugin_Public_VRStereoWidgetComponent_h_22_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UVRStereoWidgetComponent(UVRStereoWidgetComponent&&); \
	NO_API UVRStereoWidgetComponent(const UVRStereoWidgetComponent&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UVRStereoWidgetComponent); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UVRStereoWidgetComponent); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UVRStereoWidgetComponent)


#define TableTopSat_Plugins_VRExpansionPlugin_mordentral_vrexpansionplugin_36a63860c688_VRExpansionPlugin_Source_VRExpansionPlugin_Public_VRStereoWidgetComponent_h_22_PRIVATE_PROPERTY_OFFSET
#define TableTopSat_Plugins_VRExpansionPlugin_mordentral_vrexpansionplugin_36a63860c688_VRExpansionPlugin_Source_VRExpansionPlugin_Public_VRStereoWidgetComponent_h_19_PROLOG
#define TableTopSat_Plugins_VRExpansionPlugin_mordentral_vrexpansionplugin_36a63860c688_VRExpansionPlugin_Source_VRExpansionPlugin_Public_VRStereoWidgetComponent_h_22_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	TableTopSat_Plugins_VRExpansionPlugin_mordentral_vrexpansionplugin_36a63860c688_VRExpansionPlugin_Source_VRExpansionPlugin_Public_VRStereoWidgetComponent_h_22_PRIVATE_PROPERTY_OFFSET \
	TableTopSat_Plugins_VRExpansionPlugin_mordentral_vrexpansionplugin_36a63860c688_VRExpansionPlugin_Source_VRExpansionPlugin_Public_VRStereoWidgetComponent_h_22_RPC_WRAPPERS \
	TableTopSat_Plugins_VRExpansionPlugin_mordentral_vrexpansionplugin_36a63860c688_VRExpansionPlugin_Source_VRExpansionPlugin_Public_VRStereoWidgetComponent_h_22_INCLASS \
	TableTopSat_Plugins_VRExpansionPlugin_mordentral_vrexpansionplugin_36a63860c688_VRExpansionPlugin_Source_VRExpansionPlugin_Public_VRStereoWidgetComponent_h_22_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define TableTopSat_Plugins_VRExpansionPlugin_mordentral_vrexpansionplugin_36a63860c688_VRExpansionPlugin_Source_VRExpansionPlugin_Public_VRStereoWidgetComponent_h_22_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	TableTopSat_Plugins_VRExpansionPlugin_mordentral_vrexpansionplugin_36a63860c688_VRExpansionPlugin_Source_VRExpansionPlugin_Public_VRStereoWidgetComponent_h_22_PRIVATE_PROPERTY_OFFSET \
	TableTopSat_Plugins_VRExpansionPlugin_mordentral_vrexpansionplugin_36a63860c688_VRExpansionPlugin_Source_VRExpansionPlugin_Public_VRStereoWidgetComponent_h_22_RPC_WRAPPERS_NO_PURE_DECLS \
	TableTopSat_Plugins_VRExpansionPlugin_mordentral_vrexpansionplugin_36a63860c688_VRExpansionPlugin_Source_VRExpansionPlugin_Public_VRStereoWidgetComponent_h_22_INCLASS_NO_PURE_DECLS \
	TableTopSat_Plugins_VRExpansionPlugin_mordentral_vrexpansionplugin_36a63860c688_VRExpansionPlugin_Source_VRExpansionPlugin_Public_VRStereoWidgetComponent_h_22_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID TableTopSat_Plugins_VRExpansionPlugin_mordentral_vrexpansionplugin_36a63860c688_VRExpansionPlugin_Source_VRExpansionPlugin_Public_VRStereoWidgetComponent_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
