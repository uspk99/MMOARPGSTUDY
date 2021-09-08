// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Login.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "UI_LoginMain.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Stream/SimpleIOStream.h"

void UUI_Login::NativeConstruct()
{
	Super::NativeConstruct();
	//绑定按键动作
	SignInButton->OnReleased.AddDynamic(this, &UUI_Login::SignIn);//松开按键
	RegisterButton->OnReleased.AddDynamic(this, &UUI_Login::Register);//松开按键
	UKismetSystemLibrary::PrintString(this, TEXT("UUI_Login"));
}

void UUI_Login::NativeDestruct()
{
	Super::NativeDestruct();
}

bool UUI_Login::EncryptionToLocal(const FString& InPaths)
{
	FString AccountString = Account->GetText().ToString();
	FString PassWordString = PassWord->GetText().ToString();
	//路径纠正
	FString InCypPaths = InPaths;
	FPaths::NormalizeFilename(InCypPaths);// 把\\转为/
	InCypPaths.RemoveFromEnd(TEXT("/"));

	if (!AccountString.IsEmpty() && !PassWordString.IsEmpty() && !InPaths.IsEmpty())
	{
		TArray<uint8> PasswordBuff;
		FSimpleIOStream Stream(PasswordBuff);

		Stream << PassWordString;

		FString HashKey = FMD5::HashAnsiString(*AccountString);
		//加密32次
		for (int32 i = 0; i < 32; i++)
		{
			HashKey = FMD5::HashAnsiString(*HashKey);
		}
		if (!HashKey.IsEmpty())
		{
			//把密码补到16倍数
			int32 Remain = 0;
			int32 Total = PasswordBuff.Num();
			if (Total%16)
			{
				float Value = (float)Total / 16.f;
				int32 Ceil = FMath::CeilToInt(Value);//取整数
				int32 CeilTotal = Ceil * 16;//补至多少位
				Remain = CeilTotal - Total;
			}
			for (int32 i =0; i<Remain; i++)
			{
				uint8 Tmp = 0;
				Stream << Tmp;
			}
			ANSICHAR* Inkey = TCHAR_TO_UTF8(*HashKey);
			FAES::EncryptData(PasswordBuff.GetData(), PasswordBuff.Num(), Inkey);

			//删除原先旧版本的账户和密码
			IFileManager::Get().DeleteDirectory(*InCypPaths);

			FFileHelper::SaveArrayToFile(PasswordBuff, *(InCypPaths / AccountString + TEXT(".arpg")));

			return true;
		}
	}
	return false;
}

bool UUI_Login::DecryptionFromLocal(const FString& InPaths)
{
	//路径纠正
	FString InCypPaths = InPaths;
	FPaths::NormalizeFilename(InCypPaths);// 把\\转为/
	InCypPaths.RemoveFromEnd(TEXT("/"));

	TArray<FString> Filenames;
	//找到路径里带.的文件，true判断是这种文件 false只要文件名不要路径
	IFileManager::Get().FindFilesRecursive(Filenames, *InPaths, TEXT("*.*"), true, false);

	//遍历去头部
	//D:\new_new_vide2\111.arpg
	for (auto& Tmp : Filenames)
	{
		FString FilenameHeard = FPaths::GetCleanFilename(Tmp);
		if (FilenameHeard.Contains(TEXT(".arpg")))
		{
			TArray<uint8> InByte;
			FFileHelper::LoadFileToArray(InByte, *Tmp);

			FilenameHeard.RemoveFromEnd(TEXT(".arpg"));

			if (InByte.Num()>0)
			{

				FString HashKey = FMD5::HashAnsiString(*FilenameHeard);
				//加密32次
				for (int32 i = 0; i < 32; i++)
				{
					HashKey = FMD5::HashAnsiString(*HashKey);
				}
				if (!HashKey.IsEmpty())
				{
					FSimpleIOStream Stream(InByte);

					ANSICHAR* Inkey = TCHAR_TO_UTF8(*HashKey);
					FAES::DecryptData(InByte.GetData(), InByte.Num(), Inkey);

					FString PasswordString;
					Stream >> PasswordString;

					Account->SetText(FText::FromString(FilenameHeard));
					PassWord->SetText(FText::FromString(PasswordString));

					return true;

				}
			}
		}
	}
	return false;
}

void UUI_Login::SignIn()
{
	UKismetSystemLibrary::PrintString(this, TEXT("SignIn"));
	if (UUI_LoginMain* InLoginMain = GetParents<UUI_LoginMain>())
	{
		FString AccountString = Account->GetText().ToString();
		FString PassWordString = PassWord->GetText().ToString();
		InLoginMain->SignIn(AccountString, PassWordString);
	}
}

void UUI_Login::Register()
{
	if (UUI_LoginMain* InLoginMain = GetParents<UUI_LoginMain>())
	{
		InLoginMain->Register();
	}
}
