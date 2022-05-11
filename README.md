# ImGui_WS

[[toc]]

## 引用仓库地址

* [ImGui](https://github.com/ocornut/imgui)  
  ImGui的仓库，里面有ImGui的Wiki
* [imgui-ws](https://github.com/ggerganov/imgui-ws)  
  实现了ImGui网页绘制

## 特性

* 网页绘制
* 虚幻世界俯视图绘制
* 虚幻Object的Detials面板
* 接入ImPlot数据可视化库
* 中文字体
* 编辑器布局系统

## 学习如何使用ImGui

在ImGui网页选择ImGui_WS，勾选ImGuiDemo打开Demo面板
代码查看**ImGuiDemo.cpp**可参考Demo面板复制所需的控件绘制方法

## 网页绘制

点击右下角的ImGui按钮开启对应的网页，或者控制台输入**ImGui.WS.LaunchWeb**打开网页

### 端口号配置

可以通过Config或者命令行来设置端口号

1. ProjectSettings - Plugins - ImGui_WS 中可配置端口号
2. ImGui_WS.ini配置文件
    > [/Script/ImGui_WS.ImGui_WS_Settings]  
    > GamePort=8890
    > ServerPort=8891
    > ServerPort=8892

3. 启动参数配置 -ExecCmds="ImGui.WS.Port 8890"

## 虚幻俯视图

![俯视图](Docs/Viewport.png)  

### 根据类型筛选Actor

FilterActor搜索框中输入**type:ClassPath**后在地图中只会显示当前类型的Actor

### 添加需要可视化的Actor类型

创建继承于UImGuiWorldDebuggerDrawerBase的类型

* 添加构造函数

``` cpp
UShootWeaponBulletDrawer::UShootWeaponBulletDrawer()
{
	// 标识该Drawer支持的Actor类型
	DrawActor = AShootWeaponBullet::StaticClass();
	// 绘制的实体半径
	Radius = 10.f;
	// 绘制的颜色
	Color = FLinearColor::Red;
	// 是否会因为视距太远而被剔除
	bAlwaysDebuggerDraw = true;
}
```

* 重写DrawImGuiDebuggerExtendInfo等函数添加额外的调试信息绘制

``` cpp
void UShootWeaponBulletDrawer::DrawImGuiDebuggerExtendInfo(const AActor* Actor, const FImGuiWorldDebuggerContext& DebuggerContext) const
{
	const AShootWeaponBullet* Bullet = CastChecked<AShootWeaponBullet>(Actor);
	const FVector EndLocation = Bullet->GetActorLocation();
	const FVector StartLocation = EndLocation - Actor->GetVelocity() * DebuggerContext.DeltaSeconds;
	DebuggerContext.DrawLine(FVector2D{ StartLocation }, FVector2D{ EndLocation }, Color);
}
```

### 添加额外的世界信息绘制

在UImGuiWorldDebuggerViewportPanel中重写以下虚函数

* DrawDebugInfoUnderActors 在Actor的下层绘制额外的调试信息
* DrawDebugInfoUpperActors 在Actor的上层绘制额外的调试信息

建议每个世界调试信息添加**开关**，避免调试世界同时显示过多的元素

``` cpp
// 声明开关
UPROPERTY(Config)
uint8 bExampleToggle : 1;

// 实现中添加是否开启开关的菜单选项
if (ImGui::BeginMenu("Example Menu"))
{
	{
		bool Value = bExampleToggle;
		if (ImGui::Checkbox("Example Toggle", &Value))
		{
			bShowGlobalLifeTime = Value;
			DebuggerContext.MarkConfigDirty();
		}
	}
	ImGui::EndMenu();
}

// 逻辑中判断开关，开启的情况再进行调试信息的绘制
```

## 虚幻细节面板

![细节面板](Docs/Details.png)  

### 添加自定义类型的绘制方式

查看**FStructCustomizerScoped**的使用方式

## 编辑器面板布局

![默认布局效果](Docs/DefaultLayout.png)  

### 概述

* 继承UImGuiWorldDebuggerPanelBase给ImGuiWorldDebugger新增面板
* 继承UImGuiWorldDebuggerLayoutBase给ImGuiWorldDebugger新增布局描述

### UnrealImGuiPanelBuilder

FUnrealImGuiPanelBuilder用于构建所属窗口的布局，需要配置以下参数

| 属性名 | 描述 |
|  ----  | ---- |
| DockSpaceName | 该布局系统的名称 |
| SupportLayoutType | 支持的布局类型，该布局的子类都会被搜集至该布局系统 |
| SupportPanelType | 支持的面板类型，该面板的子类都会被搜集至该布局系统 |

配置完布局系统的描述信息后调用一下方法进行面板的绘制

| 方法名 | 描述 |
|  ----  | ---- |
| Register | 注册该布局系统，创建时调用 |
| Unregister | 注销该布局系统，销毁时调用 |
| DrawPanels | 绘制该布局系统下的面板 |
| LoadDefaultLayout | 重新加在激活的布局 |

### 新增布局

继承FUnrealImGuiPanelBuilder下支持的布局基类类型，例如ImGuiWorldDebugger拓展布局就继承UImGuiWorldDebuggerLayoutBase  

* 配置LayoutName，没配置布局名的布局不会显示
* 重写LoadDefaultLayout，实现改布局的描述

#### ImGuiWorldDebugger默认布局例子

默认布局将视口划分了四个区域

``` cpp
UCLASS()
class UImGuiWorldDebuggerDefaultLayout : public UImGuiWorldDebuggerLayoutBase
{
	GENERATED_BODY()
public:
	// 声明DockId，作为面板注册Dock时的配置
	enum EDockId
	{
		Viewport,
		Outliner,
		Details,
		Utils,
	};
	UImGuiWorldDebuggerDefaultLayout();
	void LoadDefaultLayout(UObject* Owner, const FUnrealImGuiPanelBuilder& LayoutBuilder) override;
};
```

``` cpp
UImGuiWorldDebuggerDefaultLayout::UImGuiWorldDebuggerDefaultLayout()
{
	// 设置布局名
	LayoutName = LOCTEXT("Default", "Default");
}

void UImGuiWorldDebuggerDefaultLayout::LoadDefaultLayout(UObject* Owner, const FUnrealImGuiPanelBuilder& LayoutBuilder)
{
	const ImGuiID DockId = ImGui::DockBuilderAddNode(DockSpaceId, ImGuiDockNodeFlags_None);

	// 调用DockBuilderSplitNode划分布局
	ImGuiID ViewportId = ImGui::DockBuilderSplitNode(DockSpaceId, ImGuiDir_Left, 0.7f, nullptr, &DockSpaceId);
	const ImGuiID UtilsId = ImGui::DockBuilderSplitNode(ViewportId, ImGuiDir_Down, 0.3f, nullptr, &ViewportId);
	const ImGuiID OutlinerId = ImGui::DockBuilderSplitNode(DockSpaceId, ImGuiDir_Up, 0.3f, nullptr, &DockSpaceId);
	const ImGuiID DetailsId = ImGui::DockBuilderSplitNode(DockSpaceId, ImGuiDir_Down, 0.7f, nullptr, &DockSpaceId);

	// 将声明的DockId与ImGui的实际ID添加映射关系
	const TMap<int32, ImGuiID> DockIdMap
	{
		{ Viewport, ViewportId },
		{ Outliner, OutlinerId },
		{ Details, DetailsId },
		{ Utils, UtilsId },
	};
	// 子面板应用布局信息
	ApplyPanelDockSettings(LayoutBuilder, DockIdMap, EDockId::Utils);

	ImGui::DockBuilderFinish(DockId);
}
```

### 新增面板

继承FUnrealImGuiPanelBuilder下支持的面板基类类型，例如ImGuiWorldDebugger拓展面板就继承UImGuiWorldDebuggerPanelBase

* 配置Title，无命名的面板不会被注册
* 配置DefaultDockSpace，添加面板在布局中的位置
* 重写Draw，实现面板的绘制

#### ImGuiWorldDebuggerViewportPanel面板例子

``` cpp
UImGuiWorldDebuggerViewportPanel::UImGuiWorldDebuggerViewportPanel()
{
	// 声明需要显示菜单栏
	ImGuiWindowFlags = ImGuiWindowFlags_MenuBar;
	// 面板命名
	Title = LOCTEXT("Viewport", "Viewport");
	// 默认在ImGuiWorldDebuggerDefaultLayout布局中的位置为Viewport
	DefaultDockSpace =
	{
		{ UImGuiWorldDebuggerDefaultLayout::StaticClass()->GetFName(), UImGuiWorldDebuggerDefaultLayout::EDockId::Viewport }
	};
}
```