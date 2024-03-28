// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Subsystems/WorldSubsystem.h"
#include "Subsystems/EngineSubsystem.h"
#include "ImGui_WS_Manager.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnImGui_WS_Draw, float);

USTRUCT()
struct IMGUI_WS_API FImGui_WS_Context
{
	GENERATED_BODY()
public:
	FOnImGui_WS_Draw OnDraw;
};

USTRUCT()
struct IMGUI_WS_API FImGui_WS_EditorContext : public FImGui_WS_Context
{
	GENERATED_BODY()
public:
	bool bAlwaysDrawDefaultLayout = false;
};

UCLASS()
class UImGui_WS_WorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;
	bool DoesSupportWorldType(const EWorldType::Type WorldType) const override { return WorldType == EWorldType::Game || WorldType == EWorldType::PIE; }
	
	UPROPERTY()
	FImGui_WS_Context Context;
};

UCLASS()
class IMGUI_WS_API UImGui_WS_Manager : public UEngineSubsystem
{
	GENERATED_BODY()
public:
	static UImGui_WS_Manager* GetChecked();
	static FImGui_WS_Context* GetImGuiContext(const UWorld* World);
	static FImGui_WS_EditorContext* GetImGuiEditorContext();

	static bool IsSettingsEnable();
	bool IsEnable() const { return Impl.IsValid(); }
	void Enable();
	void Disable();
	
	int32 GetPort() const;
	int32 GetConnectionCount() const;

	bool IsRecording() const;
	void StartRecord();
	void StopRecord();

	void ImGuiDrawViewport(float DeltaSeconds);
protected:
	static constexpr int32 EditorIndex = INDEX_NONE;
	int32 DrawContextIndex = 0;
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;
private:
	friend class UImGui_WS_WorldSubsystem;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	FImGui_WS_EditorContext EditorContext;
#endif
	
	UPROPERTY(Transient)
	TArray<UImGui_WS_WorldSubsystem*> WorldSubsystems;

	class FImpl;
	TUniquePtr<FImpl> Impl;
};
