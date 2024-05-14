#include "RenderTestViewportClient.h"
#include "Runtime/Engine/Public/CanvasTypes.h"
#include "Runtime/Engine/Public/CanvasItem.h"
#include "Engine/TextureRenderTarget2D.h"


FRenderTestViewportClient::FRenderTestViewportClient()
{
	FString FileName = FPaths::Combine(FPaths::GameSourceDir(), TEXT("data\\Result.PNG"));
	renderer.LoadImageToTexture2DEx(FileName, OutRawData, Width, Height);
	renderer.LoadImageToTexture2D(FileName, InTexture, Width, Height);
	RenderTarget = NewObject<UTextureRenderTarget2D>();
	RenderTarget->RenderTargetFormat = RTF_RGBA8;
	//RenderTarget->InitAutoFormat(Width,Height);
	RenderTarget->InitAutoFormat(1024, 1024);
	RenderTarget->ClearColor = FLinearColor::White;
	RenderTarget->bAutoGenerateMips = false;
	//RenderTarget->NumSamples = 4;
	RenderTarget->UpdateResource();
}

static TAutoConsoleVariable CVarMyTest(
	TEXT("r.MyTest"),
	1,
	TEXT("Test My Global Shader, set it to 0 to disable, or to 1, 2 or 3 for fun!"),
	ECVF_RenderThreadSafe
);

void FRenderTestViewportClient::Draw(FViewport* Viewport, FCanvas* Canvas)
{
	int32 MyTestValue = CVarMyTest.GetValueOnAnyThread();
	if (MyTestValue == 0)
	{
		// �������������λ��
		FVector2D V0 = FVector2D(100, 100);  // ����1
		FVector2D V1 = FVector2D(Viewport->GetSizeXY().X - 100, 100);  // ����2
		FVector2D V2 = FVector2D(Viewport->GetSizeXY().X / 2, Viewport->GetSizeXY().Y - 100);  // ����3

		// ����ÿ���������ɫ
		FLinearColor Color0 = FLinearColor::Red;
		FLinearColor Color1 = FLinearColor::Green;
		FLinearColor Color2 = FLinearColor::Blue;

		// ������������Ŀ
		UTexture* WhiteTexture = LoadObject<UTexture>(NULL, TEXT("/Engine/EngineMaterials/DefaultWhiteGrid.DefaultWhiteGrid"), NULL, LOAD_None, NULL);

		FCanvasTriangleItem Triangle(V0, V1, V2, WhiteTexture->GetResource());
		Triangle.SetColor(FLinearColor::White); // ������ɫ�������ǰ�ɫ������ʾ�������ʵ��ɫ

		// ���ö�����ɫ
		Triangle.TriangleList[0].V0_Color = Color0;
		Triangle.TriangleList[0].V1_Color = Color1;
		Triangle.TriangleList[0].V2_Color = Color2;

		// ����������
		Canvas->DrawItem(Triangle);

	}
	else if (MyTestValue == 1)
	{
		FSimpleRenderer tt;
		tt.Render2(RenderTarget, FVector4(1, 0, 0, 1), 1);// FVector2D(0, 0));
		FCanvasTileItem Tile(FVector2D::ZeroVector, RenderTarget->GetResource(), Viewport->GetSizeXY(), FLinearColor(1, 1, 1, 1));
		Canvas->DrawItem(Tile);
	}
	else if (MyTestValue == 2)
	{
		FCanvasTileItem Tile(FVector2D::ZeroVector, InTexture->GetResource(), Viewport->GetSizeXY(), FLinearColor::White);
		Canvas->DrawItem(Tile);
	}

	//FCanvasLineItem Line(FVector2D(100, 100), FVector2D(Viewport->GetSizeXY().X - 100, Viewport->GetSizeXY().Y - 100));
	//Line.SetColor(FLinearColor::Blue);
	//Canvas->DrawItem(Line);



	////Canvas->Clear(FLinearColor::Black);

	//if (RenderTarget)
	//{
	//	FCanvasTileItem Tile(FVector2D::ZeroVector, RenderTarget->GetResource(), Viewport->GetSizeXY(), FLinearColor::White);
	//	Canvas->DrawItem(Tile);

	//	//FCanvasLineItem tLine(FVector2D(100, 100), FVector2D(Viewport->GetSizeXY().X-100, Viewport->GetSizeXY().Y-100));
	//	//tLine.SetColor(FLinearColor::Red);
	//	//Canvas->DrawItem(tLine);
	//}
	
	
	//auto region = FUpdateTextureRegion2D(0, 0, 0, 0, Width, Height);
	//renderer.UpdateTextureRegion(RenderTarget, 0, 1, region, Width * 4, 0, OutRawData.GetData());
	//FString FileName= FPaths::Combine(FPaths::GameSourceDir(),TEXT("data\\Result.PNG"));
	////renderer.SaveRenderTargetToFile(RenderTarget, FileName);
	//renderer.Render(RenderTarget);
	//FCanvasTileItem Tile(FVector2D::ZeroVector, RenderTarget->Resource, Viewport->GetSizeXY(), FLinearColor::White);
	//Canvas->DrawItem(Tile);
}