/* Created by Luke Jeffers
* This file is subject to the terms and conditions mentioned in the license file
*/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Planet.generated.h"

UCLASS()
class PROCEDURALPLANETLOD_API APlanet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlanet();

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		FVector PlayerPos;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		float PlanetScale;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		float DistFromPlanetVertex;

	// Create our procedural mesh component
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		UProceduralMeshComponent * PlanetMesh;

	// Variables for the component
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FLinearColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	// Variables for subdivision
	TArray<FVector> Vertices_New;
	TArray<int32> Triangles_New;
	int32 IndexA, IndexB, IndexC;

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

};
