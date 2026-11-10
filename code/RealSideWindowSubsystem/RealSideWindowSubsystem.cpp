#include "RealSideWindowSubsystem.h"

#include "Blueprint/UserWidget.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/SWindow.h"
#include "GenericPlatform/GenericApplication.h"


void URealSideWindowSubsystem::OpenRealSideWindow(
	TSubclassOf<UUserWidget> WidgetClass)
{
	// Widgetが指定されていなければ終了
	if (!WidgetClass)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("RealSideWindow: WidgetClass is not set.")
		);

		return;
	}


	// すでにWindowが開いている場合は新しく作らない
	if (RealSideWindow.IsValid())
	{
		RealSideWindow->BringToFront();
		return;
	}


	// Widgetを生成
	RealSideWidget = CreateWidget<UUserWidget>(
		GetGameInstance(),
		WidgetClass
	);

	if (!RealSideWidget)
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("RealSideWindow: Failed to create widget.")
		);

		return;
	}


	// PCに接続されているモニター情報を取得
	FDisplayMetrics DisplayMetrics;
	FDisplayMetrics::RebuildDisplayMetrics(DisplayMetrics);


	// サブモニターが見つからなかった場合の初期位置
	FVector2D WindowPosition(100.0f, 100.0f);

	// サブモニターが見つからなかった場合の初期サイズ
	FVector2D WindowSize(1280.0f, 720.0f);


	// メインモニターではないモニターを探す
	for (const FMonitorInfo& Monitor : DisplayMetrics.MonitorInfo)
	{
		if (!Monitor.bIsPrimary)
		{
			// サブモニターの左上座標
			WindowPosition = FVector2D(
				Monitor.WorkArea.Left,
				Monitor.WorkArea.Top
			);

			// サブモニターの表示領域サイズ
			WindowSize = FVector2D(
				Monitor.WorkArea.Right - Monitor.WorkArea.Left,
				Monitor.WorkArea.Bottom - Monitor.WorkArea.Top
			);

			UE_LOG(
				LogTemp,
				Log,
				TEXT("RealSideWindow: Secondary monitor found.")
			);

			break;
		}
	}


	// Real Side用Windowを作成
	RealSideWindow =
		SNew(SWindow)

		.Title(
			FText::FromString(
				TEXT("PULSE - Real Side")
			)
		)

		// 初期サイズ
		.ClientSize(WindowSize)

		// 初期位置
		.ScreenPosition(WindowPosition)

		// 自動中央配置を無効化
		.AutoCenter(EAutoCenter::None)

		// ユーザーによるサイズ変更を許可
		.SizingRule(ESizingRule::UserSized)

		.SupportsMaximize(true)
		.SupportsMinimize(true)

		[
			RealSideWidget->TakeWidget()
		];


	// ユーザーが×ボタンでWindowを閉じた時の処理
	RealSideWindow->SetOnWindowClosed(
		FOnWindowClosed::CreateUObject(
			this,
			&URealSideWindowSubsystem::HandleRealSideWindowClosed
		)
	);


	// Native Windowを作る
	// ここではまだ表示しない
	FSlateApplication::Get().AddWindow(
		RealSideWindow.ToSharedRef(),
		false
	);


	// Window作成後に
	// サブモニターの位置とサイズを明示的に設定
	RealSideWindow->ReshapeWindow(
		WindowPosition,
		WindowSize
	);


	// 位置を決めてから表示
	RealSideWindow->ShowWindow();
}


void URealSideWindowSubsystem::HandleRealSideWindowClosed(
	const TSharedRef<SWindow>& ClosedWindow)
{
	// Windowが閉じられたので参照を解除
	RealSideWindow.Reset();

	// Widgetの参照も解除
	RealSideWidget = nullptr;
}


void URealSideWindowSubsystem::Deinitialize()
{
	// Play終了時にReal Side Windowが残っていれば閉じる
	if (RealSideWindow.IsValid())
	{
		// Deinitialize中に
		// HandleRealSideWindowClosedを呼ばないよう解除
		RealSideWindow->SetOnWindowClosed(
			FOnWindowClosed()
		);


		// Slate側にWindowの破棄を依頼
		RealSideWindow->RequestDestroyWindow();


		// Window参照を解除
		RealSideWindow.Reset();
	}


	// Widget参照を解除
	RealSideWidget = nullptr;


	// 親クラスの終了処理
	Super::Deinitialize();
}