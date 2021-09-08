// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Register.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "UI_LoginMain.h"

#define LOCTEXT_NAMESPACE "UUI_Register"

void UUI_Register::NativeConstruct()
{
	Super::NativeConstruct();
	SubmitButton->OnReleased.AddDynamic(this, &UUI_Register::Submit);
	CancelButton->OnReleased.AddDynamic(this, &UUI_Register::Cancel);

	ResetColor();
	//最初UI塌陷
	SetVisibility(ESlateVisibility::Collapsed);
}

void UUI_Register::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_Register::Submit()
{
	if (UUI_LoginMain* InLoginMain=GetParents<UUI_LoginMain>())
	{
		ResetColor();
		FString NiceNameString = NiceName->GetText().ToString();
		FString EmailString = Email->GetText().ToString();
		FString PlayerAccountString = PlayerAccount->GetText().ToString();
		FString PlayerPasswordString = PlayerPassword->GetText().ToString();
		FString ConfirmPasswordString = ConfirmPassword->GetText().ToString();
		FString AuthorURLString = AuthorURL->GetText().ToString();

		if (NiceNameString.Len() < 3)
		{
			NiceNameText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
			//LoC Text :Key value 可通过key翻译语言
			InLoginMain->PrintLog(LOCTEXT("Register_NiceName", "The name has at least 3 characters."));
		}
		else if (PlayerAccountString.Len() < 6)
		{
			PlayerAccountText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));

			InLoginMain->PrintLog(LOCTEXT("Register_Account", "The account has at least 6 characters."));
		}
		else if (!EmailString.Contains(TEXT(".")) || !EmailString.Contains(TEXT("@")))
		{
			EmailText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));

			InLoginMain->PrintLog(LOCTEXT("Register_Email", "The mailbox format is incorrect."));
		}
		else if (PlayerPasswordString.Len() < 6)
		{
			PlayerPasswordText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));

			InLoginMain->PrintLog(LOCTEXT("Register_Password", "The password has at least 6 characters."));
		}
		else if (PlayerPasswordString != ConfirmPasswordString)
		{
			PlayerPasswordText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
			ConfirmPasswordText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));

			InLoginMain->PrintLog(LOCTEXT("Register_ConfirmPassword", "The password is inconsistent."));
		}
		else
		{
			FString RegisterInfo =
				FString::Printf(TEXT(
					"NiceName=%s&Email=%s&Account=%s&Password=%s&AuthorURL=%s"),
					*NiceNameString,
					*PlayerAccountString,
					*EmailString,
					*PlayerPasswordString,
					*AuthorURLString);

			InLoginMain->Register(RegisterInfo);

			RegisterOut();
		}
	}
}


void UUI_Register::ResetText()
{
	//清空输入框
	NiceName->SetText(FText::GetEmpty());
	Email->SetText(FText::GetEmpty());
	PlayerAccount->SetText(FText::GetEmpty());
	PlayerPassword->SetText(FText::GetEmpty());
	ConfirmPassword->SetText(FText::GetEmpty());
	AuthorURL->SetText(FText::GetEmpty());
}

void UUI_Register::Cancel()
{
	RegisterOut();
}

void UUI_Register::ResetColor()
{
	//重设文本颜色为白色

	//NiceNameText->SetColorAndOpacity((FLinearColor(1,1,1,1)));
	NiceNameText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	PlayerAccountText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	PlayerPasswordText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	ConfirmPasswordText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	EmailText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
}

void UUI_Register::RegisterIn()
{
	PlayWidgetAnim(TEXT("RegisterIn"));
}

void UUI_Register::RegisterOut()
{
	PlayWidgetAnim(TEXT("RegisterOut"));
}

#undef LOCTEXT_NAMESPACE