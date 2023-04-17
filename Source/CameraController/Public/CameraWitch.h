// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CameraWitch.generated.h"

UCLASS()
class CAMERACONTROLLER_API ACameraWitch : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACameraWitch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//配置弹簧臂
	UPROPERTY(EditAnywhere)
	class  USpringArmComponent* SpringArmComponent;
	//配置相机组件
	UPROPERTY(EditAnywhere)
	class UCameraComponent* CameraComponent;
	//配置静态物体组件
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;
	//参数配置
	FVector2D MovementInput;
	FVector2D CameraInput;
	float ZoomFactor;
	bool bZoomingIn;
	//声明函数参数跟踪
	void Forward(float AxisValue);
	void Right(float AxisValue);
	void Offset(float AxisValue);
	void Turn(float AxisValue);
	void ZoomIn();
	void ZoomOut();
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
