// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Components/BoxComponent.h"
#include "TerrainNode.generated.h"

UCLASS()
class PROCEDURALPLANETLOD_API ATerrainNode : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATerrainNode();

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

	// Node box component
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuadtreeData")
		UBoxComponent * BoxVisual;

	// Create our procedural mesh component
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		UProceduralMeshComponent * TerrainMesh;

	// The player's current position
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		FVector PlayerPos;

	// Within bounds boolean
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		bool InBounds;

	// Amount of times to subdivide
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults", meta = (ExposeOnSpawn = true))
		int32 Recursions;

	// The player's distance from this terrain node
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults", meta = (ExposeOnSpawn = true))
		float DistFromNode;


	// The size of this terrain node
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults", meta = (ExposeOnSpawn = true))
		float TerrainNodeScale;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		float TerrainNodeScaleZ;

	// Variables from the TerrainClass

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults", meta = (ExposeOnSpawn = true))
		TArray<FVector> TerrainVertices;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults", meta = (ExposeOnSpawn = true))
		TArray<int32> TerrainTriangles;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults", meta = (ExposeOnSpawn = true))
		FVector TerrainLocation;


	// Variables for the terrain node mesh component
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		TArray<FVector> Vertices;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FLinearColor> VertexColors;
	TArray<FColor> UpVertexColors;
	TArray<FProcMeshTangent> Tangents;

	// Variables for subdivision
	TArray<FVector> Vertices_New;
	TArray<int32> Triangles_New;
	int32 IndexA, IndexB, IndexC;

	// Indexes to use when seeking internal triangles
	int32 ia, ib, ic;

	UFUNCTION(BlueprintCallable, Category = "Defaults")
		void SeekInternalTris();


	//Check within bounds
	void CheckPlayerInBounds();


	// Handle Subdivision count
	UFUNCTION(BlueprintCallable, Category = "Defaults")
		void HandleSubdivision();

	//Rebuild triangle list
	void BuildTriangleList();

	//Do the actual subdivision
	void Subdivide(int32 a, int32 b, int32 c);


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		float NoiseScale;

	// Displacement points
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults", meta = (ExposeOnSpawn = true))
		TArray<FVector> DisplacePoints;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		float DistFromNoisePoint;

};
