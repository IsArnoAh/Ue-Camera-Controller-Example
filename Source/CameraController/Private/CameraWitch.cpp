// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraWitch.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ACameraWitch::ACameraWitch()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//创建组件
	RootComponent=CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponet"));
	StaticMeshComponent=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StacicMeshComponet"));
	SpringArmComponent=CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraComponent=CreateDefaultSubobject<UCameraComponent>(TEXT("CrameraComponet"));

	//绑定组件
	StaticMeshComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetupAttachment(StaticMeshComponent);
	CameraComponent->SetupAttachment(SpringArmComponent,USpringArmComponent::SocketName);
	//为弹簧臂赋值
	SpringArmComponent->SetRelativeLocationAndRotation(FVector(0.0f,0.0f,50.0f),FRotator(-60.0f,0.0f,0.0f));
	SpringArmComponent->TargetArmLength=400.f;
	SpringArmComponent->bEnableCameraLag=true;
	SpringArmComponent->CameraLagSpeed=3.0f;
	//默认控制玩家
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ACameraWitch::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraWitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//实现放大镜头功能更新
	{
		if (bZoomingIn)
		{
			ZoomFactor+=DeltaTime/0.5f;
		}
		else
		{
			ZoomFactor-=DeltaTime/0.25f;
		}
		ZoomFactor=FMath::Clamp<float>(ZoomFactor,0.0f,1.0f);
		//根据ZoomFactor来设置摄像机视野和弹簧臂长度，用lerp插值函数对其进行赋值
		CameraComponent->FieldOfView=FMath::Lerp<float>(90.0f,60.0f,ZoomFactor);
		SpringArmComponent->TargetArmLength=FMath::Lerp<float>(400.0f,300.0f,ZoomFactor);
	}
//旋转Actor偏转角度，摄像机也能旋转，
	{
		FRotator NewRotation=GetActorRotation();
		NewRotation.Yaw+=CameraInput.X;
		SetActorRotation(NewRotation);
	}
	//旋转摄像机的俯视角，但对其进行限制，确保我们一直看向下边
	{
		FRotator NewRotation=SpringArmComponent->GetComponentRotation();
		NewRotation.Pitch=FMath::Clamp(NewRotation.Pitch+CameraInput.Y,-80.0f,-15.0f);
		SpringArmComponent->SetWorldRotation(NewRotation);
	}
	//基于MoveX和MoveY坐标轴处理移动
	{
		if (!MovementInput.IsZero())
		{
			//把移动数值放大原来的100倍
			MovementInput=MovementInput.GetSafeNormal()*100.0f;
			FVector NewLocation=GetActorLocation();
			NewLocation+=GetActorForwardVector()*MovementInput.X*DeltaTime;
			NewLocation+=GetActorRightVector()*MovementInput.Y*DeltaTime;
			SetActorLocation(NewLocation);
		}
	}

}


// Called to bind functionality to input
void ACameraWitch::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//绑定操作事件Focus(ZoomIn)
	InputComponent->BindAction("Focus",IE_Pressed,this,&ACameraWitch::ZoomIn);
	InputComponent->BindAction("Focus",IE_Released,this,&ACameraWitch::ZoomOut);
	//绑定轴输入（）
	InputComponent->BindAxis("Forward",this,&ACameraWitch::Forward);
	InputComponent->BindAxis("Right",this,&ACameraWitch::Right);
	InputComponent->BindAxis("Offset",this,&ACameraWitch::Offset);
	InputComponent->BindAxis("Turn",this,&ACameraWitch::Turn);
}
//自定义函数实现
void ACameraWitch::Forward(float AxisValue)//前后
{
	MovementInput.X=FMath::Clamp<float>(AxisValue,-1.0f,1.0f);
}
void ACameraWitch::Right(float AxisValue)//左右
{
	MovementInput.Y=FMath::Clamp<float>(AxisValue,-1.0f,1.0f);
}
void ACameraWitch::Offset(float AxisValue)//偏移
{
	CameraInput.Y=AxisValue;
}
void ACameraWitch::Turn(float AxisValue)//旋转
{
	CameraInput.X=AxisValue;
}
void ACameraWitch::ZoomIn()//放大
{
	bZoomingIn=true;
}
void ACameraWitch::ZoomOut()//缩小
{
	bZoomingIn=false;
}






