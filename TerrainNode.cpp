// Fill out your copyright notice in the Description page of Project Settings.

#include "TerrainNode.h"


// Sets default values
ATerrainNode::ATerrainNode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Add root component
	RootComponent = CreateAbstractDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Add procedural mesh component 
	TerrainMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("TerrainMesh"));

	// Attach the procedural mesh component to the root
	TerrainMesh->AttachTo(RootComponent);

	// Box node 1
	BoxVisual = CreateDefaultSubobject<UBoxComponent>(TEXT("boxNode1"));
	BoxVisual->AttachTo(RootComponent);


}

// Called in editor
void ATerrainNode::PostActorCreated()
{
	Super::PostActorCreated();


}



//Called when a property is changed
void ATerrainNode::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	BoxVisual->SetBoxExtent(FVector(TerrainNodeScale, TerrainNodeScale, TerrainNodeScaleZ));

}

// Called when the game starts or when spawned
void ATerrainNode::BeginPlay()
{
	Super::BeginPlay();

	BoxVisual->SetBoxExtent(FVector(TerrainNodeScale, TerrainNodeScale, TerrainNodeScaleZ));

}

// Called every frame
void ATerrainNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckPlayerInBounds();

}

void ATerrainNode::SeekInternalTris()
{
	ia = 0;
	ib = 1;
	ic = 2;
/*
	// Take the terrain location into account...
	for (int i = 0; i < TerrainVertices.Num(); i++)
	{
		TerrainVertices[i] = TerrainVertices[i] + TerrainLocation;
	}
	*/
	

	// look for and add internal triangles
	for (int i = 0; i < TerrainTriangles.Num() / 3; i++)
	{
		FVector a, b, c, avg;

		a = TerrainVertices[TerrainTriangles[ia]];
		b = TerrainVertices[TerrainTriangles[ib]];
		c = TerrainVertices[TerrainTriangles[ic]];

		avg = (a + b + c) / 3;
		avg = avg + TerrainLocation;

		if (avg.X < GetActorLocation().X + TerrainNodeScale &&
			(avg.X > GetActorLocation().X - TerrainNodeScale) &&
			(avg.Y < GetActorLocation().Y + TerrainNodeScale) &&
			(avg.Y > GetActorLocation().Y - TerrainNodeScale))
		{
			Vertices.Add(a);
			Vertices.Add(b);
			Vertices.Add(c);
		}

		ia = ia + 3;
		ib = ib + 3;
		ic = ic + 3;


	}
	Triangles.Empty();

	for (int i = 0; i < Vertices.Num(); i++)
	{
		Triangles.Add(i);
	}
	
	TerrainMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);
	TerrainMesh->SetWorldLocation(TerrainLocation);
}



void ATerrainNode::CheckPlayerInBounds()
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

	if (FVector(PlayerPos - BoxVisual->GetComponentLocation()).Size() < DistFromNode)
	{
		InBounds = true;
	}
	else
	{
		InBounds = false;
	}

}


void ATerrainNode::Subdivide(int32 a, int32 b, int32 c)
{
	FVector va, vb, vc, vab, vbc, vca;


	va = Vertices[a];
	vb = Vertices[b];
	vc = Vertices[c];

	vab = FMath::Lerp(va, vb, 0.5);
	vbc = FMath::Lerp(vb, vc, 0.5);
	vca = FMath::Lerp(vc, va, 0.5);
	

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

void ATerrainNode::BuildTriangleList()
{

	for (int i = 0; i < Vertices.Num(); i++)
	{
		// Build vertecx index list
		Triangles.Add(i);
	}
}

void ATerrainNode::HandleSubdivision()
{
	IndexA = 0;
	IndexB = 1;
	IndexC = 2;
	// Keep subdivisions at a safe level! 
	if (Recursions > 5)
	{
		Recursions = 5;
	}	


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
