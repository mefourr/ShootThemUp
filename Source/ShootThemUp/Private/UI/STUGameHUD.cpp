// Shoot Them Uo Game. All Right Reserved

#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"

void ASTUGameHUD::DrawHUD()
{
    Super::DrawHUD();

    DrawCrossHair();
}

void ASTUGameHUD::DrawCrossHair()
{
    const TInterval<float> DispalyCenter(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

    const float HalfLineSize = 20.0f;
    const float LineThickness = 2.0f;
    const FLinearColor LineColor = FLinearColor::Green;

   

    DrawLine(
        DispalyCenter.Min - HalfLineSize, DispalyCenter.Max, DispalyCenter.Min + HalfLineSize, DispalyCenter.Max, LineColor, LineThickness);
    DrawLine(
        DispalyCenter.Min, DispalyCenter.Max - HalfLineSize, DispalyCenter.Min, DispalyCenter.Max + HalfLineSize, LineColor, LineThickness);
}