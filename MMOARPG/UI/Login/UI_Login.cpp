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
	//�󶨰�������
	SignInButton->OnReleased.AddDynamic(this, &UUI_Login::SignIn);//�ɿ�����
	RegisterButton->OnReleased.AddDynamic(this, &UUI_Login::Register);//�ɿ�����
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
	//·������
	FString InCypPaths = InPaths;
	FPaths::NormalizeFilename(InCypPaths);// ��\\תΪ/
	InCypPaths.RemoveFromEnd(TEXT("/"));

	if (!AccountString.IsEmpty() && !PassWordString.IsEmpty() && !InPaths.IsEmpty())
	{
		TArray<uint8> PasswordBuff;
		FSimpleIOStream Stream(PasswordBuff);

		Stream << PassWordString;

		FString HashKey = FMD5::HashAnsiString(*AccountString);
		//����32��
		for (int32 i = 0; i < 32; i++)
		{
			HashKey = FMD5::HashAnsiString(*HashKey);
		}
		if (!HashKey.IsEmpty())
		{
			//�����벹��16����
			int32 Remain = 0;
			int32 Total = PasswordBuff.Num();
			if (Total%16)
			{
				float Value = (float)Total / 16.f;
				int32 Ceil = FMath::CeilToInt(Value);//ȡ����
				int32 CeilTotal = Ceil * 16;//��������λ
				Remain = CeilTotal - Total;
			}
			for (int32 i =0; i<Remain; i++)
			{
				uint8 Tmp = 0;
				Stream << Tmp;
			}
			ANSICHAR* Inkey = TCHAR_TO_UTF8(*HashKey);
			FAES::EncryptData(PasswordBuff.GetData(), PasswordBuff.Num(), Inkey);

			//ɾ��ԭ�Ⱦɰ汾���˻�������
			IFileManager::Get().DeleteDirectory(*InCypPaths);

			FFileHelper::SaveArrayToFile(PasswordBuff, *(InCypPaths / AccountString + TEXT(".arpg")));

			return true;
		}
	}
	return false;
}

bool UUI_Login::DecryptionFromLocal(const FString& InPaths)
{
	//·������
	FString InCypPaths = InPaths;
	FPaths::NormalizeFilename(InCypPaths);// ��\\תΪ/
	InCypPaths.RemoveFromEnd(TEXT("/"));

	TArray<FString> Filenames;
	//�ҵ�·�����.���ļ���true�ж��������ļ� falseֻҪ�ļ�����Ҫ·��
	IFileManager::Get().FindFilesRecursive(Filenames, *InPaths, TEXT("*.*"), true, false);

	//����ȥͷ��
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
				//����32��
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
