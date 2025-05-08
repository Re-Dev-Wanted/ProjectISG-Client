// Fill out your copyright notice in the Description page of Project Settings.


#include "UIC_Time.h"

#include "UIV_Time.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UUIC_Time::UpdateTimeText(int32 ChangeHour, int32 ChangeMinute)
{
	UUIV_Time* TimeView = Cast<UUIV_Time>(GetView());
	if (TimeView)
	{
		FString Str;
		if (ChangeHour < 10)
		{
			if (ChangeMinute < 10)
			{
				Str = FString::Printf(
					TEXT("0%d:0%d"), ChangeHour, ChangeMinute);
			}
			else
			{
				Str = FString::Printf(TEXT("0%d:%d"), ChangeHour, ChangeMinute);
			}
		}
		else
		{
			if (ChangeMinute < 10)
			{
				Str = FString::Printf(TEXT("%d:0%d"), ChangeHour, ChangeMinute);
			}
			else
			{
				Str = FString::Printf(TEXT("%d:%d"), ChangeHour, ChangeMinute);
			}
		}
		TimeView->GetTimeText()->SetText(FText::FromString(Str));
	}
}

void UUIC_Time::UpdateDayText(int32 ChangeDay, int32 ChangeMonth)
{
	UUIV_Time* TimeView = Cast<UUIV_Time>(GetView());
	if (TimeView)
	{
		FString Str;
		if (ChangeMonth < 10)
		{
			if (ChangeDay < 10)
			{
				Str = FString::Printf(
					TEXT("0%d월 0%d일"), ChangeMonth, ChangeDay);
			}
			else
			{
				Str = FString::Printf(TEXT("0%d월 %d일"), ChangeMonth, ChangeDay);
			}
		}
		else
		{
			if (ChangeDay < 10)
			{
				Str = FString::Printf(TEXT("%d월 0%d일"), ChangeMonth, ChangeDay);
			}
			else
			{
				Str = FString::Printf(TEXT("%d월 %d일"), ChangeMonth, ChangeDay);
			}
		}
		TimeView->GetDayText()->SetText(FText::FromString(Str));
	}
}

void UUIC_Time::UpdateTimeImage(class UTexture2D* ChangeImage)
{
	UUIV_Time* TimeView = Cast<UUIV_Time>(GetView());

	if (TimeView)
	{
		TimeView->GetTimeImage()->SetBrushFromTexture(ChangeImage);
	}
}
