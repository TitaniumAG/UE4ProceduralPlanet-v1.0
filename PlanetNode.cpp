// Fill out your copyright notice in the Description page of Project Settings.

#include "PlanetNode.h"

// Sets default values
APlanetNode::APlanetNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Add root component
	RootComponent = CreateAbstractDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Add procedural mesh component 
	PlanetMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("TerrainMesh"));

	// Attach the procedural mesh component to the root
	PlanetMesh->AttachTo(RootComponent);

	// Box node 1
	boxNode1 = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxNode1"));
	boxNode1->AttachTo(RootComponent);

	// Set the terrain scale value
	PlanetScale = 500;

	// Set the search index values
	IndexA = 0;
	IndexB = 1;
	IndexC = 2;

	Ia = 0;
	Ib = 1;
	Ic = 2;


}


// Called in editor
void APlanetNode::PostActorCreated()
{
	Super::PostActorCreated();

	HandleSubdivision();

	PlanetMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);
}



//Called when a property is changed
void APlanetNode::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	HandleSubdivision();

	PlanetMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);


}

// Called when the game starts or when spawned
void APlanetNode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlanetNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void APlanetNode::GenerateMesh()
{

}

void APlanetNode::Subdivide(int32 a, int32 b, int32 c)
{
	FVector va, vb, vc, vab, vbc, vca;


	va = Vertices[a];
	vb = Vertices[b];
	vc = Vertices[c];

	vab = FMath::Lerp(va, vb, 0.5);
	vbc = FMath::Lerp(vb, vc, 0.5);
	vca = FMath::Lerp(vc, va, 0.5);

	// Normalise vectors
	va.Normalize();
	vb.Normalize();
	vc.Normalize();

	vab.Normalize();
	vbc.Normalize();
	vca.Normalize();

	// Scale vectors back up
	va = va * PlanetScale / 4;
	vb = vb * PlanetScale / 4;
	vc = vc * PlanetScale / 4;

	vab = vab * PlanetScale / 4;
	vbc = vbc * PlanetScale / 4;
	vca = vca * PlanetScale / 4;


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

void APlanetNode::BuildTriangleList()
{

	for (int i = 0; i < Vertices.Num(); i++)
	{
		// Build vertecx index list
		Triangles.Add(i);
	}
}

void APlanetNode::HandleSubdivision()
{
	// Start from base
	Vertices = {
	FVector(-100,	-100,	-100),
	FVector(-100,	-100,	100),
	FVector(-100,	100,	-100),
	FVector(-100,	100,	100),
	FVector(100,	-100,	-100),
	FVector(100,	-100,	100),
	FVector(100,	100,	-100),
	FVector(100,	100,	100) };

	Triangles = {
		0	,	2	,	1	,
		2	,	6	,	3	,
		6	,	4	,	7	,
		4	,	0	,	5	,
		2	,	0	,	6	,
		7	,	5	,	3	,
		2	,	3	,	1	,
		6	,	7	,	3	,
		4	,	5	,	7	,
		0	,	1	,	5	,
		0	,	4	,	6	,
		5	,	1	,	3 };


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


}


void APlanetNode::SeekInternalTris()
{
	Ia = 0;
	Ib = 1;
	Ic = 2;

	for (int i = 0; i < TerrainTriangles.Num() / 3; i++)
	{
		// Get average point of current triangle
		FVector avg;
		avg = (Vertices[Ia] + Vertices[Ib] + Vertices[Ic]) / 3;

		// Check whether the average point is within the bounds of this node

		if(avg.X > )
	}
}
