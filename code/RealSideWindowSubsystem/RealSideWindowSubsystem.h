#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RealSideWindowSubsystem.generated.h"

class UUserWidget;
class SWindow;

UCLASS()
class PLUSE0609_API URealSideWindowSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, Category = "Real Side Window")
    void OpenRealSideWindow(TSubclassOf<UUserWidget> WidgetClass);

    // VR Preview / Play終了時に呼ばれる
    virtual void Deinitialize() override;

private:

    // ユーザーがウィンドウを手動で閉じた時
    void HandleRealSideWindowClosed(
        const TSharedRef<SWindow>& ClosedWindow
    );

    // Real Side用の別ウィンドウ
    TSharedPtr<SWindow> RealSideWindow;

    // Real Sideに表示するWidget
    UPROPERTY()
    TObjectPtr<UUserWidget> RealSideWidget;
};