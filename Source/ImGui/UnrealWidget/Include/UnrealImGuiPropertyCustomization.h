// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnrealImGuiPropertyDetails.h"

class AActor;
struct FAssetData;

namespace UnrealImGui
{
	struct IMGUI_API FPropertyDisableScope
	{
		FPropertyDisableScope(const FProperty* Property);
		~FPropertyDisableScope();
	private:
		bool Disable = false;
	};

	struct IMGUI_API FPropertyEnableScope
	{
		FPropertyEnableScope();
		~FPropertyEnableScope();
	private:
		bool Enable = false;
	};

	struct IMGUI_API FBoolPropertyCustomization : public IUnrealPropertyCustomization
	{
		void CreateValueWidget(const FProperty* Property, const FPtrArray& Containers, int32 Offset, bool IsIdentical) const override;
	};

	struct IMGUI_API FNumericPropertyCustomization : public IUnrealPropertyCustomization
	{
		void CreateValueWidget(const FProperty* Property, const FPtrArray& Containers, int32 Offset, bool IsIdentical) const override;
	};

	struct IMGUI_API FObjectPropertyCustomization : public IUnrealPropertyCustomization
	{
		FObjectPropertyCustomization()
		{
			bOverrideHasChildProperties = true;
		}

		bool IsVisible(const FDetailsFilter& Filter, const FProperty* Property, const FPtrArray& Containers, int32 Offset, bool IsIdentical) const override;
		bool HasChildPropertiesOverride(const FProperty* Property, const FPtrArray& Containers, int32 Offset, bool IsIdentical) const override;
		void CreateValueWidget(const FProperty* Property, const FPtrArray& Containers, int32 Offset, bool IsIdentical) const override;
		void CreateChildrenWidget(const FProperty* Property, const FPtrArray& Containers, int32 Offset, bool IsIdentical) const override;
	private:
		mutable TWeakObjectPtr<UClass> CachedAssetClass;
		mutable TArray<FAssetData> CachedAssetList;

		mutable TWeakObjectPtr<UClass> CachedInstancedClass;
		mutable TArray<TWeakObjectPtr<UClass>> CachedClassList;
	};

	struct IMGUI_API FSoftObjectPropertyCustomization : public IUnrealPropertyCustomization
	{
		void CreateValueWidget(const FProperty* Property, const FPtrArray& Containers, int32 Offset, bool IsIdentical) const override;
	private:
		mutable TWeakObjectPtr<UClass> CachedAssetClass;
		mutable TArray<FAssetData> CachedAssetList;

		mutable TWeakObjectPtr<UClass> CachedActorClass;
		mutable TArray<TWeakObjectPtr<AActor>> CachedActorList;
	};

	struct IMGUI_API FClassPropertyCustomization : public IUnrealPropertyCustomization
	{
		void CreateValueWidget(const FProperty* Property, const FPtrArray& Containers, int32 Offset, bool IsIdentical) const override;
	private:
		mutable TWeakObjectPtr<UClass> CachedClass;
		mutable TArray<TWeakObjectPtr<UClass>> CachedClassList;
	};

	struct IMGUI_API FSoftClassPropertyCustomization : public IUnrealPropertyCustomization
	{
		void CreateValueWidget(const FProperty* Property, const FPtrArray& Containers, int32 Offset, bool IsIdentical) const override;
	private:
		mutable TWeakObjectPtr<UClass> CachedClass;
		mutable TArray<TWeakObjectPtr<UClass>> CachedClassList;
	};

	struct IMGUI_API FStringPropertyCustomization : public IUnrealPropertyCustomization
	{
		void CreateValueWidget(const FProperty* Property, const FPtrArray& Containers, int32 Offset, bool IsIdentical) const override;
	};

	struct IMGUI_API FNamePropertyCustomization : public IUnrealPropertyCustomization
	{
		void CreateValueWidget(const FProperty* Property, const FPtrArray& Containers, int32 Offset, bool IsIdentical) const override;
	};

	struct IMGUI_API FTextPropertyCustomization : public IUnrealPropertyCustomization
	{
		void CreateValueWidget(const FProperty* Property, const FPtrArray& Containers, int32 Offset, bool IsIdentical) const override;
	};

	struct IMGUI_API FEnumPropertyCustomization : public IUnrealPropertyCustomization
	{
		void CreateValueWidget(const FProperty* Property, const FPtrArray& Containers, int32 Offset, bool IsIdentical) const override;
	};

	struct IMGUI_API FArrayPropertyCustomization : public IUnrealPropertyCustomization
	{
		FArrayPropertyCustomization()
		{
			bOverrideHasChildProperties = true;
		}

		bool IsVisible(const FDetailsFilter& Filter, const FProperty* Property, const FPtrArray& Containers, int32 Offset, bool IsIdentical) const override;
		bool HasChildPropertiesOverride(const FProperty* Property, const FPtrArray& Containers, int32 Offset, bool IsIdentical) const override;
		void CreateValueWidget(const FProperty* Property, const FPtrArray& Containers, int32 Offset, bool IsIdentical) const override;
		void CreateChildrenWidget(const FProperty* Property, const FPtrArray& Containers, int32 Offset, bool IsIdentical) const override;
	};

	struct IMGUI_API FSetPropertyCustomization : public IUnrealPropertyCustomization
	{
		FSetPropertyCustomization()
		{
			bOverrideHasChildProperties = true;
		}

		bool IsVisible(const FDetailsFilter& Filter, const FProperty* Property, const FPtrArray& Containers, int32 Offset, bool IsIdentical) const override;
		bool HasChildPropertiesOverride(const FProperty* Property, const FPtrArray& Containers, int32 Offset, bool IsIdentical) const override;
		void CreateValueWidget(const FProperty* Property, const FPtrArray& Containers, int32 Offset, bool IsIdentical) const override;
		void CreateChildrenWidget(const FProperty* Property, const FPtrArray& Containers, int32 Offset, bool IsIdentical) const override;
	};

	struct IMGUI_API FMapPropertyCustomization : public IUnrealPropertyCustomization
	{
		FMapPropertyCustomization()
		{
			bOverrideHasChildProperties = true;
		}

		bool IsVisible(const FDetailsFilter& Filter, const FProperty* Property, const FPtrArray& Containers, int32 Offset, bool IsIdentical) const override;
		bool HasChildPropertiesOverride(const FProperty* Property, const FPtrArray& Containers, int32 Offset, bool IsIdentical) const override;
		void CreateValueWidget(const FProperty* Property, const FPtrArray& Containers, int32 Offset, bool IsIdentical) const override;
		void CreateChildrenWidget(const FProperty* Property, const FPtrArray& Containers, int32 Offset, bool IsIdentical) const override;
	};

	struct IMGUI_API FStructPropertyCustomization : public IUnrealPropertyCustomization
	{
		FStructPropertyCustomization()
		{
			bHasChildProperties = true;
		}

		bool IsVisible(const FDetailsFilter& Filter, const FProperty* Property, const FPtrArray& Containers, int32 Offset, bool IsIdentical) const override;
		void CreateValueWidget(const FProperty* Property, const FPtrArray& Containers, int32 Offset, bool IsIdentical) const override;
		void CreateChildrenWidget(const FProperty* Property, const FPtrArray& Containers, int32 Offset, bool IsIdentical) const override;
	};
}