// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Components/BoxComponent.h"
#include "PlanetNode.generated.h"

UCLASS()
class PROCEDURALPLANETLOD_API APlanetNode : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlanetNode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called in editor
	virtual void PostActorCreated() override;

	//Called when a property is changed
	virtual void PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent);

	// Subdivision count
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		int32 Recursions;

	// Is player within bounderies of this node?
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		bool InBounds;

	// Node Scale
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults", meta = (ExposeOnSpawn = "true"))
		float NodeScale;

	// Must match node scale! 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults", meta = (ExposeOnSpawn = "true"))
		float DefaultNodeScale;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		FVector PlayerPos;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults", meta = (ExposeOnSpawn = "true"))
		FVector PlanetLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		float PlanetScale;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults", meta = (ExposeOnSpawn = "true"))
		float DistFromPlanetNode;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults", meta = (ExposeOnSpawn = "true"))
		float DistFromPlanetVertex;

	// Create our procedural mesh component
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		UProceduralMeshComponent * PlanetMesh;

	// Node box component
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		UBoxComponent * BoxFrame;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults", meta = (ExposeOnSpawn = "true"))
		TArray<FVector>PlanetVertices;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults", meta = (ExposeOnSpawn = "true"))
		TArray<int32> PlanetTriangles;

	// Variables for the component
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
	TArray<FVector> Vertices;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FLinearColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	// Variables for subdivision
	TArray<FVector> Vertices_New;
	TArray<int32> Triangles_New;
	int32 IndexA, IndexB, IndexC, Ia, Ib, Ic;

	// Function to generate mesh section
	UFUNCTION(BlueprintCallable, Category = "Defaults")
		void GenerateMesh();

	// Handle Subdivision count
	UFUNCTION(BlueprintCallable, Category = "Defaults")
		void HandleSubdivision();

	//Do the actual subdivision
	void Subdivide(int32 a, int32 b, int32 c);

	//Rebuild triangle list
	void BuildTriangleList();

	//Seek internal triangles
	UFUNCTION(BlueprintCallable, Category = "Defaults")
		void SeekInternalTris();

	void CheckPlayerInBounds();


	

};
