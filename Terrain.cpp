// Fill out your copyright notice in the Description page of Project Settings.

#include "Terrain.h"


// Sets default values
ATerrain::ATerrain()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Add root component
	RootComponent = CreateAbstractDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Add procedural mesh component 
	TerrainMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("TerrainMesh"));

	// Attach the procedural mesh component to the root
	TerrainMesh->AttachTo(RootComponent);

	// Set the terrain scale value
	TerrainScale = 500;

	// Set the search index values
	IndexA = 0;
	IndexB = 1;
	IndexC = 2;




}


// Called in editor
void ATerrain::PostActorCreated()
{
	Super::PostActorCreated();

	// Add vertices
	Vertices.Add(FVector(-TerrainScale, -TerrainScale, 0));
	Vertices.Add(FVector(-TerrainScale, TerrainScale, 0));
	Vertices.Add(FVector(TerrainScale, -TerrainScale, 0));
	Vertices.Add(FVector(TerrainScale, TerrainScale, 0));

	Triangles.Add(0);
	Triangles.Add(1);
	Triangles.Add(2);

	Triangles.Add(2);
	Triangles.Add(1);
	Triangles.Add(3);

	TerrainMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, false);

}



//Called when a property is changed
void ATerrain::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	HandleSubdivision();

	//TerrainMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);

	// Add vertices


}

// Called when the game starts or when spawned
void ATerrain::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATerrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckPlayerInBounds();

}

void ATerrain::GenerateMesh()
{
	// Add vertices
	Vertices.Add(FVector(-TerrainScale, -TerrainScale, 0));
	Vertices.Add(FVector(-TerrainScale, TerrainScale, 0));
	Vertices.Add(FVector(TerrainScale, -TerrainScale, 0));
	Vertices.Add(FVector(TerrainScale, TerrainScale, 0));

	Triangles.Add(0);
	Triangles.Add(1);
	Triangles.Add(2);

	Triangles.Add(2);
	Triangles.Add(1);
	Triangles.Add(3);

	TerrainMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);
}

void ATerrain::Subdivide(int32 a, int32 b, int32 c)
{
	FVector va, vb, vc, vab, vbc, vca;


	va = Vertices[a];
	vb = Vertices[b];
	vc = Vertices[c];

	vab = FMath::Lerp(va, vb, 0.5);
	vbc = FMath::Lerp(vb, vc, 0.5);
	vca = FMath::Lerp(vc, va, 0.5);
	
	for (int i = 0; i < DisplacePoints.Num(); i++)
	{
		if (FVector(va + GetActorLocation() - DisplacePoints[i]).Size() < DistFromPoint)
		{
			va = va + FVector(0,0,NoiseScale);
		}
		if (FVector(vb + GetActorLocation() - DisplacePoints[i]).Size() < DistFromPoint)
		{
			vb = vb + FVector(0, 0, NoiseScale);
		}
		if (FVector(vc + GetActorLocation() - DisplacePoints[i]).Size() < DistFromPoint)
		{
			vc = vc + FVector(0, 0, NoiseScale);
		}

		if (FVector(vab + GetActorLocation() - DisplacePoints[i]).Size() < DistFromPoint)
		{
			vab = vab + FVector(0, 0, NoiseScale);
		}
		if (FVector(vbc + GetActorLocation() - DisplacePoints[i]).Size() < DistFromPoint)
		{
			vbc = vbc + FVector(0, 0, NoiseScale);
		}
		if (FVector(vca + GetActorLocation() - DisplacePoints[i]).Size() < DistFromPoint)
		{
			vca = vca + FVector(0, 0, NoiseScale);
		}
	}

	//Push up midpoints
	vab = vab + FVector(0,0,TerrainScale/100000);
	vbc = vbc + FVector(0, 0, TerrainScale / 100000);
	vca = vca + FVector(0, 0, TerrainScale / 100000);

	Vertices_New.Add(va);
	Vertices_New.Add(vab);
	Vertices_New.Add(vca);

	Vertices_New.Add(vca);
	Vertices_New.Add(vbc);
	Vertices_New.Add(vc);

	Vertices_New.Add(vab);
	Vertices_New.Add(vb);
	Vertices_New.Add(vbc);

	Vertices_New.Add(vab);
	Vertices_New.Add(vbc);
	Vertices_New.Add(vca);


	IndexA = IndexA + 3;
	IndexB = IndexB + 3;
	IndexC = IndexC + 3;

}

void ATerrain::BuildTriangleList()
{

	for (int i = 0; i < Vertices.Num(); i++)
	{
		// Build vertecx index list
		Triangles.Add(i);
	}
}

void ATerrain::HandleSubdivision()
{
	// Keep subdivisions at a safe level! 
	if (Recursions > 5)
	{
		Recursions = 5;
	}
	// Start from base
	Vertices = { FVector(-TerrainScale, -TerrainScale, 0),
		FVector(-TerrainScale, TerrainScale, 0),
		FVector(TerrainScale, -TerrainScale, 0),
		FVector(TerrainScale, TerrainScale, 0) };

	Triangles = { 0,1,2,2,1,3 };


	// Handle the subdivisions
	for (int i = 0; i < Recursions; i++)
	{

		for (int j = 0; j < Triangles.Num() / 3; j++)
		{

			Subdivide(Triangles[IndexA], Triangles[IndexB], Triangles[IndexC]);
		}

		// Empty
		Vertices.Empty();
		Triangles.Empty();



		//Assign new to current
		Vertices = Vertices_New;


		//New empty 
		Vertices_New.Empty();
		VertexColors.Empty();

		//Build tri indices
		BuildTriangleList();


		//Reset index counters 
		IndexA = 0;
		IndexB = 1;
		IndexC = 2;


	}

	TerrainMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);
}


void ATerrain::CheckPlayerInBounds()
{
	/*
	if (PlayerPos.X < TerrainMesh->GetComponentLocation().X + TerrainScale&&
		PlayerPos.X > TerrainMesh->GetComponentLocation().X - TerrainScale &&
		PlayerPos.Y < TerrainMesh->GetComponentLocation().Y + TerrainScale &&
		PlayerPos.Y > TerrainMesh->GetComponentLocation().Y - TerrainScale)
	{
		InBounds = true;
	}
	else
	{
		InBounds = false;
	}
	*/

	if (FVector(PlayerPos - TerrainMesh->GetComponentLocation()).Size() < DistFromTerrain)
	{
		InBounds = true;
	}
	else
	{
		InBounds = false;
	}

}
